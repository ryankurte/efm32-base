#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_core.h"
#include "em_msc.h"

#include "radio/radio.h"
#include "radio/token.h"

#include "retargetserial.h"

#include "bsp/thunderboard/icm20648.h"
#include "bsp/thunderboard/util.h"
#include "bsp/thunderboard/board.h"

typedef struct __RadioPacket {
  uint8_t packetType;
  uint8_t payload[15];
} RadioPacket;

/* Local variables */
static uint8_t    red, green, blue;
static uint8_t    nodeID;
static uint8_t    nodeRole;
static uint32_t   nodeColour;
static uint8_t    accelState;
static uint8_t    nodeCount;
static uint16_t   throwForce;

static uint16_t   RADIO_xoTune;

#define ACCEL_FILTER_LENGTH 32
static uint32_t accMagnitudeSum;
static uint16_t accMagnitudes[ACCEL_FILTER_LENGTH];
static uint16_t accMagnitudeValueCount;
static uint16_t accMagnitudeIndex;

static uint8_t MAIN_rxBuffer[256];
static RAIL_RxPacketInfo_t *MAIN_rxPacket;
static RadioPacket txPacket;

static uint32_t loopCount             = 0;
static uint32_t dontDimIterations     = 0;
static uint32_t dimIterations         = 0;
static uint32_t lastActiveIteration   = 0;
static uint32_t dontReceiveIterations = 0;
static uint8_t  dimIndex              = 0;

static uint8_t pageBuffer[FLASH_PAGE_SIZE];

static const uint32_t defaultColors[3] = {
  0xFF0000,
  0x600090,
  0x609000
};

#define ACCEL_GRAVITY_LEVEL       1000
#define ACCEL_NOISE_LIMIT         100
#define ACCEL_FREEFALL_LIMIT      300
#define ACCEL_STATE_UNKNOWN       0
#define ACCEL_STATE_FREEFALL      1
#define ACCEL_STATE_MOVEMENT      2
#define ACCEL_STATE_STATIONARY    3
#define ACCEL_STATE_FILTER_LENGTH 4

/* Throws harder than max or weaker than min will have the same color */
#define THROW_FORCE_MAX 3500
#define THROW_FORCE_MIN 1800

#define MOVEMENT_TIMEOUT       100
#define MOVEMENT_FADE_LENGTH   60

/* Approx. 6000 iterations per minute, sleep after 15 minutes */
#define INACTIVE_SLEEP_TIMEOUT 6000 * 15

/* Color used when dimming : 0 - 255 */
#define DIM_RED   0x00
#define DIM_GREEN 0xD0
#define DIM_BLUE  0x00

/* These must be multiples of 10! */
#define DIM_ITERATIONS     400
#define DIM_FADE_LENGTH    200

#define PACKET_TYPE_DIM_NODE       0x01
#define PACKET_TYPE_FREEFALL       0x02
#define PACKET_TYPE_MOVEMENT       0x03

#define NODE_ROLE_REGULAR            0
#define NODE_ROLE_MASTER             1

void SysTick_Handler(void);

static void     init                (bool radio);
static void     staticInit          (void);
static void     configureNode       (void);
static void     blinkRGB            (uint8_t r, uint8_t g, uint8_t b, uint8_t count);
static void     readTokens          (void);
static void     writeTokens         (void);
static void     mainLoop            (void);
static void     handleReceivedPacket(void);
static void     showMovementColor   (void);
static void     showDimColor        (void);
static int      sendDimPacket       (void);
static void     handleMovementState (void);
static void     handleFreeFallState (void);
static void     initIMU             (void);
static void     configAccel         (void);
static uint16_t readAccVectorLength (void);
static void     updateAccelState    (void);
static void     sendFreefallPacket  (void);
static void     sendMovementPacket  (void);
static void     updateAvgAccel      (uint16_t vectorLength);
static uint16_t getAvgAccel         (void);

int main(void)
{
  /**************************************************************************/
  /* Device errata init                                                     */
  /**************************************************************************/
  CHIP_Init();

  /**************************************************************************/
  /* Init static variables                                                  */
  /**************************************************************************/
  staticInit();

  /**************************************************************************/
  /* Read tokens and store settings                                         */
  /**************************************************************************/
  readTokens();

  /**************************************************************************/
  /* Board init                                                             */
  /**************************************************************************/
  init(true);

  /**************************************************************************/
  /* If both buttons are pressed, allow the user to configure the node      */
  /**************************************************************************/
  configureNode();

  /**************************************************************************/
  /* Application loop                                                       */
  /**************************************************************************/
  mainLoop();
}

void mainLoop(void)
{
  int status;

  while ( 1 ) {
    loopCount++;

    /* Wait for incoming packet with timeout - Sets up basic timing */
    status = RADIO_receivePacket(MAIN_rxBuffer, sizeof(MAIN_rxBuffer), 10, &MAIN_rxPacket);

    if ( dontReceiveIterations > 0 ) {
      /* Ignore received packets while moving */
      --dontReceiveIterations;
    } else if ( status == 1 ) {
      handleReceivedPacket();
    } else {
      /* Enter low power state */
      if ( dimIterations == 0 && loopCount - lastActiveIteration > INACTIVE_SLEEP_TIMEOUT ) {
        BOARD_rgbledPowerEnable(false);
        UTIL_sleep(200);
        BOARD_rgbledPowerEnable(true);
      }
    }

    if ( dontDimIterations > 0 ) {
      showMovementColor();
      dontDimIterations--;
    } else if ( dimIterations > 0 ) {
      showDimColor();
      dimIterations--;
    } else {
      BOARD_rgbledSetColor(0, 0, 0);
    }

    if ( nodeRole == NODE_ROLE_MASTER
         && dontDimIterations == 0
         && (loopCount % 10) == 0
         && (loopCount - lastActiveIteration) < INACTIVE_SLEEP_TIMEOUT ) {
      sendDimPacket();

      if ( loopCount % (DIM_ITERATIONS - DIM_FADE_LENGTH) == 0) {
        /* Change to the next node */
        if ( ++dimIndex >= nodeCount ) {
          dimIndex = 0;
        }
        if ( dimIndex == nodeID ) {
          dimIterations = DIM_ITERATIONS;
        }
      }
    }

    updateAccelState();

    if ( accelState == ACCEL_STATE_FREEFALL ) {
      handleFreeFallState();
    } else if ( accelState == ACCEL_STATE_MOVEMENT ) {
      handleMovementState();
    }
  }
}

void handleReceivedPacket()
{
#if 0
  /* Dump packet */
  printf("\r\n");
  printf("-------------------- RX packet --------------------\r\n");
  printf("Length      : %d\r\n", MAIN_rxPacket->dataLength);
  printf("RSSI        : %d [dBm]\r\n", RAIL_RxGetRSSI() );
  printf(".time       : %ld [us]\r\n", MAIN_rxPacket->appendedInfo.timeUs);
  printf(".crcStatus  : %s\r\n", (MAIN_rxPacket->appendedInfo.crcStatus == 1) ? "OK" : "Invalid");
  printf(".lqi        : %d\r\n", MAIN_rxPacket->appendedInfo.lqi);
  printf(".syncWordId : %d\r\n", MAIN_rxPacket->appendedInfo.syncWordId);
  printf(".rssiLatch  : %d [dBm]\r\n", (int8_t)MAIN_rxPacket->appendedInfo.rssiLatch);
  printf("Data   : %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
         MAIN_rxPacket->dataPtr[0], MAIN_rxPacket->dataPtr[1],
         MAIN_rxPacket->dataPtr[2], MAIN_rxPacket->dataPtr[3],
         MAIN_rxPacket->dataPtr[4], MAIN_rxPacket->dataPtr[5],
         MAIN_rxPacket->dataPtr[6], MAIN_rxPacket->dataPtr[7]);
#endif

  if ( MAIN_rxPacket->dataPtr[0] == PACKET_TYPE_FREEFALL
       || MAIN_rxPacket->dataPtr[0] == PACKET_TYPE_MOVEMENT ) {
    /* One of the other nodes is moving, use their color */

    red   = MAIN_rxPacket->dataPtr[2];
    green = MAIN_rxPacket->dataPtr[3];
    blue  = MAIN_rxPacket->dataPtr[4];
    BOARD_rgbledSetColor(red, green, blue);
    dontDimIterations = MOVEMENT_TIMEOUT;
    lastActiveIteration = loopCount;
    dimIterations = 0;
  } else if ( MAIN_rxPacket->dataPtr[0] == PACKET_TYPE_DIM_NODE
              && nodeRole == NODE_ROLE_REGULAR
              && dontDimIterations == 0 ) {
    /* All nodes are stationary and the master node is broadcasting DIM packets */

    if ( (MAIN_rxPacket->dataPtr[1] == nodeID) ) {
      if (dimIndex != nodeID) {
        /* Start dimming now */
        dimIterations = DIM_ITERATIONS;
        dimIndex = nodeID;
      }
    } else {
      dimIndex = MAIN_rxPacket->dataPtr[1];
    }
  }
}

void showMovementColor(void)
{
  uint16_t factor;
  if ( dontDimIterations > MOVEMENT_FADE_LENGTH ) {
    /* Don't start fading yet */
    factor = 100;
  } else {
    /* Fade out */
    factor = (100 * dontDimIterations) / MOVEMENT_FADE_LENGTH;
  }

  BOARD_rgbledSetColor( (red * factor) / 100, (green * factor) / 100, (blue * factor) / 100);

  return;
}

void showDimColor(void)
{
  uint16_t factor;
  uint16_t dimProgress = DIM_ITERATIONS  - dimIterations;

  if ( dimProgress < DIM_FADE_LENGTH ) {
    /* Fade in */
    factor = (100 * dimProgress) / DIM_FADE_LENGTH;
  } else if ( dimIterations > DIM_FADE_LENGTH ) {
    factor = 100;
  } else {
    /* Fade out */
    factor = (100 * dimIterations) / DIM_FADE_LENGTH;
  }

  BOARD_rgbledSetColor( (DIM_RED * factor) / 100, (DIM_GREEN * factor) / 100, (DIM_BLUE * factor) / 100);

  return;
}

int sendDimPacket()
{
  RAIL_TxPacketInfo_t *txPacketInfo;
  int status;

  memset(&txPacket, 0x0, sizeof(txPacket) );
  txPacket.packetType = PACKET_TYPE_DIM_NODE;
  txPacket.payload[0] = dimIndex;

  status = 0;
  for ( int i = 0; i < 3; i++ ) {
    status |= RADIO_sendPacket( (uint8_t*)&txPacket, sizeof(txPacket), sizeof(txPacket), 100, &txPacketInfo);
  }

  return status;
}

void handleMovementState(void)
{
  red   = (nodeColour >> 0);
  green = (nodeColour >> 8);
  blue  = (nodeColour >> 16);

  sendMovementPacket();
  dontDimIterations = MOVEMENT_TIMEOUT;
  dontReceiveIterations = MOVEMENT_TIMEOUT / 2;
  lastActiveIteration = loopCount;
  dimIterations = 0;

  return;
}

void handleFreeFallState(void)
{
  /* Select color based on the force of the throw */
  uint16_t normalizedForce;

  if (throwForce > THROW_FORCE_MIN) {
    normalizedForce = 256 * (throwForce - THROW_FORCE_MIN) / (THROW_FORCE_MAX - THROW_FORCE_MIN);
  } else {
    normalizedForce  = 0;
  }

  if (normalizedForce > 255) {
    normalizedForce = 255;
  }
  /* printf("Force: %d\r\n", normalizedForce); */

  if (normalizedForce < 128) {
    red   = 255 - normalizedForce * 2;
    green = normalizedForce * 2 + 1;
    blue  = 0;
  } else {
    red  = 0;
    green  = 255 - (normalizedForce - 128) * 2;
    blue = (normalizedForce - 128) * 2 + 1;
  }

  sendFreefallPacket();
  dontDimIterations = MOVEMENT_TIMEOUT;
  dontReceiveIterations = MOVEMENT_TIMEOUT / 2;
  lastActiveIteration = loopCount;
  dimIterations = 0;

  return;
}

void sendFreefallPacket(void)
{
  int ret;

  RAIL_TxPacketInfo_t *txPacketInfo;

  memset(&txPacket, 0x0, sizeof(txPacket) );
  txPacket.packetType = PACKET_TYPE_FREEFALL;
  txPacket.payload[0] = nodeID;
  txPacket.payload[1] = red;
  txPacket.payload[2] = green;
  txPacket.payload[3] = blue;
  txPacket.payload[4] = 0;
  ret = RADIO_sendPacket( (uint8_t*)&txPacket, sizeof(txPacket), sizeof(txPacket), 100, &txPacketInfo);
  if ( ret != 1 ) {
    printf("Packet send failed!\r\n");
  }

  return;
}

void sendMovementPacket(void)
{
  int ret;

  RAIL_TxPacketInfo_t *txPacketInfo;

  memset(&txPacket, 0x0, sizeof(txPacket) );
  txPacket.packetType = PACKET_TYPE_MOVEMENT;
  txPacket.payload[0] = nodeID;
  txPacket.payload[1] = (uint8_t) (nodeColour >> 0);  /* R */
  txPacket.payload[2] = (uint8_t) (nodeColour >> 8);  /* G */
  txPacket.payload[3] = (uint8_t) (nodeColour >> 16);  /* B */
  txPacket.payload[4] = 0;
  ret = RADIO_sendPacket( (uint8_t*)&txPacket, sizeof(txPacket), sizeof(txPacket), 100, &txPacketInfo);
  if ( ret != 1 ) {
    printf("Packet send failed!\r\n");
  }

  return;
}

void updateAccelState(void)
{
  uint16_t vlen;
  static uint16_t fCount = 0;
  static uint8_t currentState = 0, lastState = 0;

  vlen = readAccVectorLength();
  updateAvgAccel(vlen);
  /* printf("Accel: %5d  -  %5d\r\n", vlen, getAvgAccel()); */
  if ( vlen < ACCEL_FREEFALL_LIMIT ) {
    /* Free fall */
    if (lastState != ACCEL_STATE_FREEFALL) {
      /* Approximate the force of the throw by averaging the acceleration in a window before */
      /* we entered free fall */
      throwForce = getAvgAccel();
      /* printf( "FreeFall : %5d - %5d (%d)\r\n", vlen, throwForce, fCount ); */
    }
    currentState = ACCEL_STATE_FREEFALL;
  } else if ( ((vlen - ACCEL_GRAVITY_LEVEL) < -ACCEL_NOISE_LIMIT)
              || ((vlen - ACCEL_GRAVITY_LEVEL) >  ACCEL_NOISE_LIMIT) ) {
    /* Movement */
    if (lastState != ACCEL_STATE_MOVEMENT) {
      /* printf( "Movement : %d (%d)\r\n", vlen, fCount ); */
    }
    currentState = ACCEL_STATE_MOVEMENT;
  } else {
    /* Stationary */
    if (lastState != ACCEL_STATE_STATIONARY) {
      /* printf( "Stationary : %d\r\n", fCount ); */
    }
    currentState = ACCEL_STATE_STATIONARY;
  }

  if ( currentState == lastState ) {
    fCount++;
  }

  if ( fCount > ACCEL_STATE_FILTER_LENGTH ) {
    accelState = currentState;
    fCount = 0;
    /*printf( "Node State : %d\r\n", accelState );*/
  }

  lastState = currentState;

  return;
}

void updateAvgAccel(uint16_t vectorLength)
{
  if (accMagnitudeValueCount == ACCEL_FILTER_LENGTH) {
    accMagnitudeSum -= accMagnitudes[accMagnitudeIndex];
  } else {
    accMagnitudeValueCount++;
  }

  accMagnitudeSum += vectorLength;
  accMagnitudes[accMagnitudeIndex] = vectorLength;
  accMagnitudeIndex++;
  accMagnitudeIndex %= ACCEL_FILTER_LENGTH;

  return;
}

uint16_t getAvgAccel(void)
{
  if (accMagnitudeValueCount == 0) {
    return 0;
  }

  return accMagnitudeSum / accMagnitudeValueCount;
}

void staticInit(void)
{
  nodeRole   = NODE_ROLE_REGULAR;
  nodeID     = 1;
  nodeCount  = 3;
  nodeColour = defaultColors[nodeID];
  RADIO_xoTune = 344;

  return;
}

void init(bool radio)
{
  /**************************************************************************/
  /* System clock and timer init                                            */
  /**************************************************************************/
  if ( radio ) {
    RADIO_init(RADIO_xoTune);
  } else {
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  }

  /**************************************************************************/
  /* Re-Initialize serial port for stdin/stdout                                */
  /**************************************************************************/
  RETARGET_SerialInit();

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
    while (1) ;
  }

  UTIL_init();
  UTIL_sleepInit();
  BOARD_init();

  /* Wait for PWRCTRL to come online */
  UTIL_delay(50);
  initIMU();

  BOARD_rgbledPowerEnable(1);
  BOARD_rgbledEnable(true, 0xf);

  return;
}

static void configureNode(void)
{
  uint8_t pbState;
  pbState = BOARD_pushButtonGetState();

  if ( pbState != 3 ) {
    return;
  }

  blinkRGB(0xFF, 0xFF, 0xFF, 3);

  if ( nodeColour == 0xFFFFFFFF ) {
    nodeColour = defaultColors[nodeID];
  }

  /* Wait for buttons to be released to prevent accidental selection */
  while ( BOARD_pushButtonGetState() != 0 ) ;

  nodeCount = 3;

  while ( 1 ) {
    UTIL_delay(10);
    pbState = BOARD_pushButtonGetState();

    if ( pbState == 1 ) {
      /* Store current configuration */
      break;
    }
    if ( pbState == 2 ) {
      /* Switch to next configuration */
      nodeID++;
      nodeID %= 3;
      if ( nodeID == 0 ) {
        nodeRole = NODE_ROLE_MASTER;
      } else {
        nodeRole = NODE_ROLE_REGULAR;
      }
      nodeColour = defaultColors[nodeID];
    }

    /* Preview the selected color */
    red   = (nodeColour >> 0);
    green = (nodeColour >> 8);
    blue  = (nodeColour >> 16);
    BOARD_rgbledSetColor(red, green, blue);

    /* Wait for buttons to be released */
    while ( BOARD_pushButtonGetState() != 0 ) ;
  }

  /* Store the configuration to flash */
  writeTokens();

  /* Blink to confirm the selection */
  red   = (nodeColour >> 0);
  green = (nodeColour >> 8);
  blue  = (nodeColour >> 16);
  blinkRGB(red, green, blue, 3);

  return;
}

static void blinkRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t count)
{
  for ( uint8_t i = 0; i < count; i++ ) {
    BOARD_rgbledSetColor(r, g, b);
    UTIL_delay(5);
    BOARD_rgbledSetColor(0, 0, 0);
    UTIL_delay(70);
  }
}

void initIMU(void)
{
  uint32_t status;
  uint8_t  devid;

  BOARD_imuEnable(true);

  UTIL_delay(100);

  /* Initialize driver */
  printf("IMU init...");
  status = ICM20648_init();
  if ( status != ICM20648_OK ) {
    printf("Failed! status = %08Xh\r\n", status);
    return;
  }
  printf("OK\r\n");

  status = ICM20648_getDeviceID(&devid);
  if ( status != ICM20648_OK ) {
    printf("Could not get IMU device id! status = %08Xh\r\n", status);
    return;
  }
  printf("IMU device ID = %02Xh\r\n", devid);

  if ( status == ICM20648_OK ) {
    configAccel();
  }

  return;
}

uint16_t readAccVectorLength(void)
{
  float accData[3];
  int16_t vlen;

  ICM20648_accelDataRead(accData);
  vlen = (int16_t)(sqrtf(accData[0] * accData[0]
                         + accData[1] * accData[1]
                         + accData[2] * accData[2]
                         ) * 1000.0f);

  return vlen;
}

static void configAccel(void)
{
  float accelSampleRate;

  /* Enable accelerometer */
  ICM20648_sensorEnable(true, false, false);

  /* Set sample rate */
  accelSampleRate = ICM20648_accelSampleRateSet(225.0f);

  printf("IMU sample rate set to %f Hz\r\n", accelSampleRate);

  /* Filter bandwidth: 12kHz */
  ICM20648_accelBandwidthSet(ICM20648_ACCEL_BW_1210HZ);

  /* Accel: 16 G full scale */
  ICM20648_accelFullscaleSet(ICM20648_ACCEL_FULLSCALE_16G);

  UTIL_delay(50);

  /* Enter low power mode */
//   ICM20648_lowPowerModeEnter( true, false, false );

  return;
}

void readTokens(void)
{
  uint8_t t8;
  uint16_t t16;
  uint32_t t32;

  /* Dump tokens */
  t16 = TOKEN_getU16(SB_RADIO_CTUNE);
  if ( t16 != 0xFFFF ) {
    RADIO_xoTune = t16;
    printf("\r\nSB_RADIO_CTUNE = %d\r\n", t16);
  }
  t16 = TOKEN_getU16(SB_RADIO_CHANNEL);
  if ( t16 != 0xFFFF ) {
    printf("SB_RADIO_CHANNEL = %d\r\n", t16);
  }
  t16 = TOKEN_getU16(SB_RADIO_OUTPUT_POWER);
  if ( t16 != 0xFFFF ) {
    printf("SB_RADIO_OUTPUT_POWER = %d\r\n", t16);
  }
  t8 = TOKEN_getU16(SB_RADIO_NODE_ID);
  if ( t8 != 0xFF ) {
    printf("SB_RADIO_NODE_ID = %d\r\n", t8);
    nodeID = t8;
  }
  t8 = TOKEN_getU16(SB_RADIO_NODE_ROLE);
  if ( t8 != 0xFF ) {
    printf("SB_RADIO_NODE_ROLE = %d\r\n", t8);
    nodeRole = t8;
  }
  t16 = TOKEN_getU16(SB_RADIO_BEACON_PERIOD);
  if ( t16 != 0xFFFF ) {
    printf("SB_RADIO_BEACON_PERIOD = %d\r\n", t16);
  }
  t32 = TOKEN_getU32(SB_NODE_COLOUR);
  if ( t32 != 0xFFFFFFFF ) {
    printf("SB_NODE_COLOUR = %06X\r\n", t32);
    nodeColour = t32 & 0xFFFFFF;
  }
  t8 = TOKEN_getU8(SB_NODE_COUNT);
  if ( t8 != 0xFF ) {
    printf("SB_NODE_COUNT = %d\r\n", t8);
    nodeCount = t8;
  }
  printf("\r\n");

  return;
}

static void writeTokens(void)
{
  uint32_t *userdataPtr = (uint32_t*)(USERDATA_BASE);
  memcpy(pageBuffer, userdataPtr, sizeof(pageBuffer));

  memcpy(&pageBuffer[SB_RADIO_NODE_ID_OFFSET], &nodeID, sizeof(nodeID));
  memcpy(&pageBuffer[SB_RADIO_NODE_ROLE_OFFSET], &nodeRole, sizeof(nodeRole));
  memcpy(&pageBuffer[SB_NODE_COLOUR_OFFSET], &nodeColour, sizeof(nodeColour));
  memcpy(&pageBuffer[SB_NODE_COUNT_OFFSET], &nodeCount, sizeof(nodeCount));

  MSC_Init();
  MSC_ErasePage(userdataPtr);
  MSC_WriteWordFast(userdataPtr, pageBuffer, sizeof(pageBuffer));
  MSC_Deinit();
}
