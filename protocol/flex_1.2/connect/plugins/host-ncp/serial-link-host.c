#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <signal.h>

#ifdef __linux__
  #include <linux/if_tun.h>
#endif

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "phy/phy.h"
#include "serial/serial.h"
#include "host-ncp/uart-link-protocol.h"
#include "host-ncp/ncp-link.h"
#include "hal/micro/generic/ash-v3.h"
#include "csp/csp-enum.h"
#include "host-ncp/host-stream.h"
#include "host-ncp/host-driver.h"
#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"
#include "host-ncp/serial-link-host.h"

bool ncpUartUseAsh = true;
int ncpFileDescriptor;

static Stream myManagementStream = { 0 };
static Stream outgoingDataStream = { 0 };
static bool loadingSerialTx = false;
static uint16_t ashIndex = 0;
Stream ncpStream = { { 0 } };

#if defined(EMBER_NCP_RESET_ENABLE) && !defined(UNIX_HOST_SIM)
#if defined(UNIX_HOST)
static int fdDirection, fdValue;
#ifdef EMBER_NCP_RESET_REQUIRE_ENABLE
static bool resetNcpEnabled = false;
#else
static bool resetNcpEnabled = true;
#endif
#endif

// Define gpio files. The usual filenames for gpio xx are:
// GPIO_EXPORT_FILE          /sys/class/gpio/export
// GPIO_DIRECTION_FILE       /sys/class/gpio/gpioxx/direction
// GPIO_ACTIVE_LOW_FILE      /sys/class/gpio/gpioxx/active_low
// GPIO_VALUE_FILE           /sys/class/gpio/gpioxx/value
#define GPIO_EXPORT_FILE     EMBER_GPIO_PATH "export"
#define GPIO_DIRECTORY       EMBER_GPIO_PATH "gpio" EMBER_NCP_RESET_GPIO "/"
#define GPIO_DIRECTION_FILE  GPIO_DIRECTORY "direction"
#define GPIO_ACTIVE_LOW_FILE GPIO_DIRECTORY "active_low"
#define GPIO_VALUE_FILE      GPIO_DIRECTORY "value"
#define OPEN_MODE            (S_IWUSR | S_IWOTH | S_IRUSR | S_IROTH)
#define RESET_TIME_US        50
#ifdef EMBER_NCP_RESET_ASSERT_LOW
#define ASSERT_VALUE    "0"
#define DEASSERT_VALUE  "1"
#else
#define ASSERT_VALUE    "1"
#define DEASSERT_VALUE  "0"
#endif
#ifdef EMBER_NCP_RESET_DEASSERT_TRISTATED
#define TRISTATE_GPIO_IF_ENABLED()  write(fdDirection, "in", 2)
#else
#define TRISTATE_GPIO_IF_ENABLED()
#endif

#endif // #if defined(EMBER_NCP_RESET_ENABLE) && !defined(UNIX_HOST_SIM)

void emResetSerialState(bool external)
{
  MEMSET(&myManagementStream, 0, sizeof(Stream));
  MEMSET(&outgoingDataStream, 0, sizeof(Stream));

  if (!external) {
    MEMSET(&ncpStream, 0, sizeof(Stream));
  }

  ashIndex = 0;
}

#define UART_BAUD B115200

#ifdef UNIX_HOST
  #define EMBER_READ   read
  #define EMBER_WRITE  write
  #define EMBER_SELECT select
  #define LOG(x) x
#else
  #error "Only unix Host supported for now."
#endif

void emOpenNcpUart(const char *uartDevice)
{
  struct termios tios;
  ncpFileDescriptor = open(uartDevice, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (ncpFileDescriptor < 0) {
    perror("uart open failed");
    exit(1);
  }

  tcflush(ncpFileDescriptor, TCIOFLUSH);
  fcntl(ncpFileDescriptor, F_SETFL, FNDELAY);
  tcgetattr(ncpFileDescriptor, &tios);
  cfsetispeed(&tios, UART_BAUD);
  cfsetospeed(&tios, UART_BAUD);
  tios.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
  tios.c_cflag |= (CLOCAL | CREAD | CS8 | CRTSCTS);
  tios.c_iflag &= ~(BRKINT | INLCR | IGNCR | ICRNL | INPCK
                    | ISTRIP | IMAXBEL | IXON | IXOFF | IXANY);
  tios.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
  tios.c_oflag &= ~OPOST;
  memset(tios.c_cc, _POSIX_VDISABLE, NCCS);
  tios.c_cc[VSTART] = CSTART;
  tios.c_cc[VSTOP] = CSTOP;
  tios.c_cc[VMIN] = 1;
  tios.c_cc[VTIME] = 0;
  tcsetattr(ncpFileDescriptor, TCSAFLUSH, &tios);
}

uint16_t ncpWrite(int fd,
                  SerialLinkMessageType type,
                  const uint8_t *buffer,
                  uint16_t bufferLength)
{
  uint16_t result = 0;

  if (fd < 0) {
    return result;
  }

  uint16_t bigBufferLength = bufferLength + UART_LINK_HEADER_SIZE;
  uint8_t *bigBuffer = malloc(bufferLength + UART_LINK_HEADER_SIZE);

  bigBuffer[0] = '[';
  bigBuffer[1] = type;
  emberStoreHighLowInt16u(bigBuffer + 2, bufferLength);
  MEMCOPY(bigBuffer + UART_LINK_HEADER_SIZE, buffer, bufferLength);

  // if we're sending to the NCP, work with ASH
  if (ncpUartUseAsh
      && fd == ncpFileDescriptor) {
    if (outgoingDataStream.index + bigBufferLength
        < sizeof(outgoingDataStream.buffer)) {
      MEMCOPY(outgoingDataStream.buffer + outgoingDataStream.index,
              bigBuffer,
              bigBufferLength);
      outgoingDataStream.index += bigBufferLength;
      assert(outgoingDataStream.index < sizeof(outgoingDataStream.buffer));
      result = bufferLength;
      emLoadSerialTx();
    } else {
      txBufferFullHandler(buffer,
                          bufferLength,
                          sizeof(outgoingDataStream.buffer)
                          - outgoingDataStream.index);
    }
  } else {
    result = EMBER_WRITE(fd, bigBuffer, bigBufferLength);

    if (result != bigBufferLength) {
      txFailedHandler(fd, bigBuffer, bigBufferLength, result);
    }
  }

  free(bigBuffer);
  return result;
}

// Data messages from the NCP are forwarded to the IP stack; all
// others are processed by host-driver.c.

void ncpMessageHandler(SerialLinkMessageType type,
                       const uint8_t *message,
                       uint16_t length)
{
  bool isDataForHost = (type == UART_LINK_TYPE_MANAGEMENT);

  if (isDataForHost) {
    ncpIncomingMessageHandler(type, message, length);
  } else {
    // Here we can process messages to multiple destinations in the future, if needed
    perror("Message header incorrect. Expected: UART_LINK_TYPE_MANAGEMENT");
  }
}

void emTestNcpReadStatusResult(ncpReadStatus status, Stream *stream)
{
  if (status == NCP_READ_IO_ERROR) {
    perror("IO error (NCP_READ_IO_ERROR)");
  } else if (status == NCP_READ_FORMAT_ERROR) {
    uint16_t i;
#if 0
    // For ASHv3 testing only
    fprintf(stderr, "\nashRxTestState.rawData: [");
    for (i = 0; i < ashRxTestState.rawDataIndex; i++) {
      fprintf(stderr, "%X ", ashRxTestState.rawData[i]);
    }
    fprintf(stderr,
            "] | CRC should be: 0x%4X\n",
            emGetAshCrc(ashRxTestState.rawData, MAX_ASH_PACKET_SIZE - 3));
#endif
    fprintf(stderr,
            "Management formatting error (NCP_READ_FORMAT_ERROR), "
            "%u bytes: ",
            stream->index);

    for (i = 0; i < stream->index; i++) {
      fprintf(stderr, "%X ", stream->buffer[i]);
    }

    fprintf(stderr, "\n");
    assert(false);
  } else {
    if (!(status == NCP_READ_DONE
          || status == NCP_READ_PROGRESS
          || status == NCP_READ_PENDING)) {
      fprintf(stderr, "failed status: %u\n", status);
      assert(false);
    }
  }
}

#ifdef EMBER_TEST
uint32_t copyCounter = 0;
#endif

uint16_t serialCopyFromRx(const uint8_t *data, uint16_t length)
{
#ifdef EMBER_TEST
  copyCounter += 1;
#endif

  assert(length > 0);
  MEMCOPY(myManagementStream.buffer + myManagementStream.index, data, length);
  myManagementStream.index += length;
  uint16_t index;

  do {
    index = myManagementStream.index;
    ncpReadStatus result =
      processNcpInput(&myManagementStream, ncpMessageHandler);
    emTestNcpReadStatusResult(result, &myManagementStream);

    // do while we have eaten a little
  } while (myManagementStream.index >= UART_LINK_HEADER_SIZE
           && myManagementStream.index < index);

  return length;
}

ncpReadStatus readNcpAshInput(int fd,
                              ncpMsgHandler ncpMessageHandler)
{
  assert(ncpUartUseAsh);

  int got = EMBER_READ(fd,
                       ncpStream.buffer + ncpStream.index,
                       sizeof(ncpStream.buffer) - ncpStream.index);
  ncpReadStatus result = NCP_READ_DONE;

  if (got < 0) {
    result = NCP_READ_IO_ERROR;
  } else if (got > 0) {
    // eat ASH input

    // ashIndex is in the range: [ncpStream.index, ncpStream.index + got]
    // it corresponds to the current ash input index
    // it may be reset
    ashIndex = ncpStream.index;
    ncpStream.index += got;

    // totalEaten monotonically increases and is in the range: [0, got]
    // it corresponds to how much data has been eaten from ncpStream.buffer
    uint16_t totalEaten = 0;

    do {
      uint16_t increment = emProcessAshRxInput(ncpStream.buffer + ashIndex,
                                               got - totalEaten);

      if (increment == 0) {
        break;
      }

      totalEaten += increment;
      ashIndex += increment;

      if (isAshActive()) {
        ashIndex += increment;
      } else if (ncpStream.index > 0) {
        // ^^ reset from ASH was a possibility, check that ncpStream
        // still has valid data before removing bytes from it below
        emRemoveStreamBytes(&ncpStream, ashIndex);
        ashIndex = 0;
      }
    } while (totalEaten < got);
  } else if (myManagementStream.index > 0) {
    processNcpInput(&myManagementStream, ncpMessageHandler);
  }

  return (ncpStream.index == 0
          ? NCP_READ_DONE
          : NCP_READ_PROGRESS);
}

void emLoadSerialTx(void)
{
  assert(ncpUartUseAsh);

  if (loadingSerialTx) {
    // no loops
    return;
  }

  loadingSerialTx = true;

  // enqueue as much data as possible
  if (outgoingDataStream.index > 0) {
    uint16_t eaten =
      halHostEnqueueTx(outgoingDataStream.buffer,
                       outgoingDataStream.index >= MAX_ASH_PAYLOAD_SIZE
                       ? MAX_ASH_PAYLOAD_SIZE
                       : outgoingDataStream.index);

    if (eaten > 0) {
      emRemoveStreamBytes(&outgoingDataStream, eaten);
      uartFlushTx();
    }
  }

  loadingSerialTx = false;
}

void emCheckNcpUartInput(fd_set *input)
{
  if (FD_ISSET(ncpFileDescriptor, input)) {
    ncpReadStatus status;

    if (ncpUartUseAsh) {
      status = readNcpAshInput(ncpFileDescriptor, ncpMessageHandler);
    } else {
      // status = readNcpInput(ncpFileDescriptor, &ncpStream, ncpMessageHandler);
    }

    switch (status) {
      case NCP_READ_FORMAT_ERROR:
        assert(false);
        break;
      case NCP_READ_IO_ERROR:
        // what to do?
        break;
      default:
        break;
    }
  }
}

void emReallySendManagementCommand(const uint8_t *command, uint16_t length)
{
  ncpWrite(ncpFileDescriptor,
           UART_LINK_TYPE_MANAGEMENT,
           command + UART_LINK_HEADER_SIZE,
           length - UART_LINK_HEADER_SIZE);
}

void ncpIncomingMessageHandler(SerialLinkMessageType type,
                               const uint8_t *message,
                               uint16_t length)
{
  emCommandState.defaultBase = 0xFF;

  switch (type) {
    case UART_LINK_TYPE_NON_MANAGEMENT:
      assert(false);    // should have been handled by caller
      break;
    case UART_LINK_TYPE_MANAGEMENT: {
      emProcessNcpManagementCommand(type, message, length);
      break;
    }
    default:
      break;
  }
  emCommandState.defaultBase = 10;
}

extern EmberCommandEntry managementCallbackCommandTable[];
static EmberCommandState callbackCommandState = { 0 };

void emProcessNcpManagementCommand(SerialLinkMessageType type,
                                   const uint8_t *message,
                                   uint16_t length)
{
  emberRunCommandInterpreter(true,
                             &callbackCommandState,
                             managementCallbackCommandTable,
                             emberCommandErrorHandler,
                             message + 1,
                             length - 1);
}

#if defined(EMBER_NCP_RESET_ENABLE) && !defined(UNIX_HOST_SIM) && defined(UNIX_HOST)

// Open the gpio used to reset the ncp and leave it deasserted
void openNcpReset(void)
{
  int fdExport, fdActiveLow;

  fdExport = open(GPIO_EXPORT_FILE, O_WRONLY, OPEN_MODE);
  write(fdExport, EMBER_NCP_RESET_GPIO, strlen(EMBER_NCP_RESET_GPIO));
  close(fdExport);

  fdDirection = open(GPIO_DIRECTION_FILE, O_WRONLY, OPEN_MODE);
  if (fdDirection < 0) {
    perror("ncp reset gpio open failed");
    exit(1);
  }
  write(fdDirection, "in", 2);

  fdActiveLow = open(GPIO_ACTIVE_LOW_FILE, O_WRONLY, OPEN_MODE);
  write(fdActiveLow, "0", 1);
  close(fdActiveLow);

  fdValue = open(GPIO_VALUE_FILE, O_WRONLY, OPEN_MODE);
  write(fdDirection, "out", 3);
  write(fdValue, DEASSERT_VALUE, 1);
  TRISTATE_GPIO_IF_ENABLED();
}

// If enabled, briefly assert the reset output to cause a pin reset on the ncp
void resetNcp(void)
{
  assert( (fdDirection >= 0) && (fdValue >= 0) );
  if (resetNcpEnabled) {
    write(fdDirection, "out", 3);
    write(fdValue, ASSERT_VALUE, 1);
    usleep(RESET_TIME_US);
    write(fdValue, DEASSERT_VALUE, 1);
    TRISTATE_GPIO_IF_ENABLED();
  }
}

void enableResetNcp(uint8_t enable)
{
  resetNcpEnabled = enable ? true : false;
}

#else
void openNcpReset(void)
{
}
void resetNcp(void)
{
}
void enableResetNcp(uint8_t enable)
{
}
#endif
