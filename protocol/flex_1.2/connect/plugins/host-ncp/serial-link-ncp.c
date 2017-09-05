// File: serial-link.c
//
// Description: Serial link (UART or SPI).
//
// Copyright 2010 by Ember Corporation. All rights reserved.                *80*

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"
#include "host-ncp/ncp-link.h"
#include "hal/micro/generic/ash-v3.h"
#include "hal/micro/uart-link.h"
#include "stack/framework/buffer-management.h"
#include "host-ncp/generic-list.h"

#include "uart-link-protocol.h"
#include "ncp-link.h"
#include "serial-link.h"
#include "hal/micro/serial.h"

#define SERIAL_DEBUG(x)

#if defined(EMBER_SCRIPTED_TEST)
  #define DEBUG_PRINTF(...) emLogLine(UNIT_TEST, " > " __VA_ARGS__)
#elif defined(EMBER_STACK_COBRA)
void DEBUG_PRINTF(const char* formatString, ...);
#else
  #define DEBUG_PRINTF(...) emberDebugPrintf(__VA_ARGS__)
#endif

//------------------------------------------------------------------------------
// Data plane interface for transmitting and receiving buffers.

typedef struct {
  Buffer data;
  SerialLinkMessageType type;
  ManagementType managementType;
} SerialMessage;

static void markSerialMessage(ListElement *element, Marker mark)
{
  SerialMessage *message = (SerialMessage*)element;
  mark(&message->data);
}

static const ListType serialListType = {
  sizeof(SerialMessage),
  0xFFFF,
  LIST_REMOVE_HEAD_WHEN_FULL,
  NULL, // sort
  NULL, // compare
  &markSerialMessage
};

static Buffer serialTxMessages = NULL_BUFFER;
static Buffer rxBuffer = { 0 };

void ncpLinkMarkBuffers(void)
{
  emListMark(&serialListType, &serialTxMessages, emMarkBuffer);
  emMarkBuffer(&rxBuffer);
  markUartLinkNcpBuffers();
}

//------------------------------------------------------------------------------
// UART link interface for transmitting and receiving bytes.

bool serialLinkReset = false;

typedef struct {
  uint16_t byteIndex;             // index of next byte to be sent
  uint16_t byteCount;             // total number of bytes in the packet
  uint16_t start;
  uint8_t header[UART_LINK_HEADER_SIZE];
  uint8_t headerIndex;
  SerialLinkMessageType type;
} TxState;

typedef struct {
  uint8_t header[UART_LINK_HEADER_SIZE];  // incoming header
  uint16_t index;                         // index of next byte to be read
} RxState;

static TxState txState = { 0 };
static RxState rxState = { 0 };

// index        = number of bytes already received
// neededBytes  = total number of bytes needed (== UART_LINK_HEADER_SIZE)
// rxBytes      = new bytes
// rxBytesCount = number of new bytes
//
// If the entire header has arrived we do some error checking.

static uint16_t headerRx(uint16_t index,
                         uint16_t neededBytes,
                         const uint8_t *rxBytes,
                         uint16_t rxBytesCount)
{
  MEMCOPY(rxState.header + index, rxBytes, rxBytesCount);

  if (index + rxBytesCount == neededBytes) {
    uint16_t payloadLength = emberFetchHighLowInt16u(rxState.header + 2);

    if (rxState.header[0] != '['
        || !(rxState.header[1] == UART_LINK_TYPE_MANAGEMENT
             || rxState.header[1] == UART_LINK_TYPE_NON_MANAGEMENT)
        || payloadLength == 0) {
      // this should not happen
      DEBUG_PRINTF("frame error (%x %u %u)",
                   rxState.header[0], rxState.header[1], payloadLength);
      assert(false);
      return 0;
    } else {
      DEBUG_PRINTF("rx start (type %u, length %u)",
                   rxState.header[1], payloadLength);
    }
  }

  return rxBytesCount;
}

// index        = number of bytes already received
// count        = total number of bytes needed
// rxBytes      = new bytes
// rxBytesCount = number of new bytes

static uint16_t dataRx(uint16_t index,
                       uint16_t count,
                       const uint8_t *rxBytes,
                       uint16_t rxBytesCount)
{
  if (count == 0) {
    rxBuffer = NULL_BUFFER;
    return 0;
  }

  if (rxBuffer == NULL_BUFFER) {
    assert(index == 0);
    rxBuffer = emAllocateBuffer(count);
    if (rxBuffer == NULL_BUFFER) {
      DEBUG_PRINTF("failed to allocate buffer");
      return 0;
    }
  }

  assert(emGetBufferLength(rxBuffer) == count);
  MEMCOPY(emGetBufferPointer(rxBuffer) + index,
          rxBytes,
          rxBytesCount);

  if (index + rxBytesCount == count) {
    // if (! ncpLinkIpPacketHandler((SerialLinkMessageType) rxState.header[1],
    //                                  rxBuffer)) {
    //   DEBUG_PRINTF("failed to deliver buffer");
    //   // TODO: if delivery failure was a result of no route or some other
    //   // reportable condition, we should send back an ICMP message.
    // }
    rxBuffer = NULL_BUFFER;
  }

  return rxBytesCount;
}

bool ncpLinkSendManagementCommand(ManagementType managementType,
                                  const uint8_t *data,
                                  uint8_t dataLength)
{
  SerialMessage message = { 0 };
  message.type = UART_LINK_TYPE_MANAGEMENT;
  message.managementType = managementType;
  message.data = emAllocateBuffer(dataLength + 1);

  if (message.data == NULL_BUFFER) {
    return false;
  }

  uint8_t *bytes = emGetBufferPointer(message.data);
  bytes[0] = managementType;
  MEMCOPY(bytes + 1, data, dataLength);

  emListPush(&serialListType, &serialTxMessages, (ListElement*)&message);
  emLoadSerialTx();

  return true;
}

static Buffer managementRxBuffer = NULL_BUFFER;
static uint16_t managementBytesTaken = 0;

void markUartLinkNcpBuffers(void)
{
  emMarkBuffer(&managementRxBuffer);
}

//
// The data arrives piecemeal-like, and we must store it until it's all arrived
//
static uint16_t managementRx(uint16_t dataIndex,
                             uint16_t frameLength,
                             const uint8_t *data,
                             uint16_t dataLength)
{
  if (managementRxBuffer == NULL_BUFFER) {
    managementRxBuffer = emAllocateBuffer(frameLength);

    if (managementRxBuffer == NULL_BUFFER) {
      // out of memory, try again by returning 0 (??)
    }
  }

  assert(dataIndex + dataLength <= frameLength);

  if (dataIndex == 0) {
    assert((ManagementType) data[0] == MANAGEMENT_COMMAND);
  }

  uint8_t *managementRxBufferBytes = emGetBufferPointer(managementRxBuffer);
  MEMCOPY(managementRxBufferBytes + dataIndex, data, dataLength);
  managementBytesTaken += dataLength;

  assert(managementBytesTaken <= frameLength);

  if (managementBytesTaken == frameLength) {
    // Skip over the first byte which should always be MANAGEMENT_COMMAND
    // since we assert above if it's not
    ncpLinkManagementPacketHandler(MANAGEMENT_COMMAND,
                                   managementRxBufferBytes + 1,
                                   frameLength - 1);
    // we're done with the buffer
    managementRxBuffer = NULL_BUFFER;
    managementBytesTaken = 0;
  }

  return dataLength;
}

uint16_t serialCopyFromRx(const uint8_t *data, uint16_t length)
{
  uint16_t remaining = length;
  serialLinkReset = false;

  while (0 < remaining) {
    // Block to be read is either the header or the payload.
    uint16_t startIndex;  // 0 or UART_LINK_HEADER_SIZE
    uint16_t endIndex;    // UART_LINK_HEADER_SIZE or end of message
    uint16_t (*action)(uint16_t index,
                       uint16_t count,
                       const uint8_t *data,
                       uint16_t length);

    if (rxState.index < UART_LINK_HEADER_SIZE) {
      startIndex = 0;
      endIndex = UART_LINK_HEADER_SIZE;
      action = headerRx;
    } else {
      startIndex = UART_LINK_HEADER_SIZE;
      endIndex = startIndex + emberFetchHighLowInt16u(rxState.header + 2);

      if (rxState.header[1] == UART_LINK_TYPE_MANAGEMENT) {
        action = managementRx;
      } else {
        action = dataRx;
      }
    }

    uint16_t rxCount = remaining;

    if (endIndex < rxState.index + rxCount) {
      rxCount = endIndex - rxState.index;
    }

    uint16_t consumed = action(rxState.index - startIndex,
                               endIndex - startIndex,
                               data,
                               rxCount);

    if (consumed == 0
        || serialLinkReset) {
      serialLinkReset = false;
      // an error occured
      break;
    }

    assert(consumed <= rxCount);
    rxState.index += consumed;
    data += consumed;
    remaining -= consumed;

    if (rxState.index == endIndex
        && startIndex != 0) {
      DEBUG_PRINTF("rx complete");
      emApiCounterHandler((rxState.header[1] == UART_LINK_TYPE_MANAGEMENT
                           ? EMBER_COUNTER_UART_IN_MANAGEMENT
                           : EMBER_COUNTER_UART_IN_DATA), 1);
      MEMSET(&rxState, 0, sizeof(rxState));
    }
  }

  return length;
}

static bool loadingTx = false;
extern const char *emAppName;

SERIAL_DEBUG(uint16_t transmitCount = 0;
             uint16_t headerCount = 0; );

void emLoadSerialTx(void)
{
  // no loops please
  assert(!loadingTx);
  loadingTx = true;

  // check if we have data to send
  SerialMessage *message = (SerialMessage*)emListGetHead(serialTxMessages);

  if (message != NULL) {
    // we do!

    if (txState.byteIndex == 0) {
      txState.type = message->type;

      if (message->type == UART_LINK_TYPE_MANAGEMENT) {
        DEBUG_PRINTF("serial start management TX");
        txState.byteCount = emGetBufferLength(message->data);
        txState.start = 0;
      } else {
        DEBUG_PRINTF("serial start data TX");
        // txState.start =
        //   emMacPayloadIndex(message->data) + INTERNAL_IP_OVERHEAD;
        // txState.byteCount =
        //   emInternalDatagramLength(emMacPayloadPointer(message->data));
      }
    }

    if (uartTxSpaceAvailable()) {
      if (txState.headerIndex < UART_LINK_HEADER_SIZE) {
        txState.header[0] = '[';
        txState.header[1] = txState.type;
        emberStoreHighLowInt16u(txState.header + 2, txState.byteCount);
        uint8_t count =
          halHostEnqueueTx(txState.header + txState.headerIndex,
                           sizeof(txState.header) - txState.headerIndex);
        SERIAL_DEBUG(fprintf(stderr,
                             "[[%s (%u): serial-link.c wrote %u header bytes to ASHv3: ",
                             emAppName,
                             headerCount,
                             count);
                     emPrintBytes(txState.header + txState.headerIndex, count, 0);
                     fprintf(stderr, "]]\n");
                     headerCount++; );
        txState.headerIndex += count;
      }

      if (txState.headerIndex >= UART_LINK_HEADER_SIZE) {
        uint16_t count =
          halHostEnqueueTx(emGetBufferPointer(message->data)
                           + txState.start
                           + txState.byteIndex,
                           txState.byteCount - txState.byteIndex);
        SERIAL_DEBUG(fprintf(stderr,
                             "[[%s (%u): serial-link.c wrote %u bytes to ASHV3: ",
                             emAppName,
                             transmitCount,
                             count);
                     emPrintBytes(emGetBufferPointer(message->data)
                                  + txState.start
                                  + txState.byteIndex,
                                  count,
                                  0);
                     fprintf(stderr, "]]\n");
                     transmitCount++; );

        txState.byteIndex += count;

        // TODO: fix this discrepancy
        if (txState.type == UART_LINK_TYPE_MANAGEMENT) {
          assert(txState.byteIndex <= txState.byteCount);
        } else {
          //assert(txState.byteIndex <= emGetBufferLength(message->data));
        }

        if (txState.byteIndex == txState.byteCount) {
          // we're done sending this packet, move to the next
          uartFlushTx();
          emListPop(&serialListType, &serialTxMessages);
          MEMSET(&txState, 0, sizeof(TxState));
        }
      }
    }
  }

  loadingTx = false;
}

bool ncpLinkPreparingForPowerDown(void)
{
  emLoadSerialTx();
  uartFlushTx();

#ifndef EMBER_STACK_COBRA
  // wait until there is no more data to send
  return (emListGetSize(serialTxMessages) > 0) || emAshPreparingForPowerDown();
#else
  halHostUartForceRTS();
  return (emListGetSize(serialTxMessages) > 0) || !halHostUartTxEmpty() || emAshPreparingForPowerDown();
#endif
}

//------------------------------------------------------------------------------
// In addition to the normal serial I/O, we copy outgoing commands to the
// debug port so that they will show up in the trace.
void ncpLinkIpPacketReceived(SerialLinkMessageType type,
                             Buffer header)
{
  SerialMessage message = { 0 };
  message.type = type;
  message.data = header;

  emListPush(&serialListType, &serialTxMessages, (ListElement*)&message);
  emLoadSerialTx();
}

void emResetSerialState(bool external)
{
  // reset data in the txState
  MEMSET(&rxState, 0, sizeof(rxState));
  MEMSET(&txState, 0, sizeof(txState));
  managementRxBuffer = NULL_BUFFER;
  managementBytesTaken = 0;
  serialLinkReset = true;
}

void ncpLinkInit(void)
{
  DEBUG_PRINTF("init");
  halHostSerialInit();
  uartLinkReset();
}

void ncpLinkProcessIncoming(void)
{
  halHostSerialTick();
}

// The serial link doesn't really support the wait for incoming API. The
// timeout paramenter is ignored and we can only tell you whether we are
// in the middle of receiving a packet.
bool ncpLinkWaitForIncoming(uint32_t timeoutMs)
{
  // If we're receiving either data or management packets then return
  // true since we're in the middle of receiving.
  if (rxBuffer || managementRxBuffer) {
    return true;
  }
  return false;
}

void ncpLinkAbortWaitForIncoming(void)
{
  // Do nothing since this driver does not actually block to wait for an
  // incoming message above.
}

void ncpLinkReset(void)
{
  uartLinkReset();
}

// This function is called if we are unable to send a management command
void ncpLinkManagementErrorHandler(const uint8_t *data,
                                   uint8_t len,
                                   bool hostToNcp)
{
  // This can happen if we run out of ember buffers to hold management data.
  // It's unclear what we should do to handle this. For now we're doing nothing
  // because that's what the code has always done.

  // @TODO: Better handle this error condition. By the time we get here we
  // should see the EMBER_COUNTER_BUFFER_ALLOCATION_FAIL counter incremented.
  // It would probably make sense to add another counter for this specific
  // condition so that the host could be sure of what the problem was.
  DEBUG_PRINTF("Failed to send management response");
}
