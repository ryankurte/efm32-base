/***************************************************************************//**
 * @file
 * @brief Power and Interrupt Controller Register Definitions
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef BOARD_PIC_REGS_H
#define BOARD_PIC_REGS_H
/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup BOARD_4160
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @defgroup PIC_defs Power and Interrupt Controller Definitions
 * @{
 * @brief Thunderboard Sense 1 Power and Interrupt Controller register and
 *        bitfield definitions
 ******************************************************************************/

/* Application registers */
#define BOARD_PIC_REG_IMU_CTRL              0x00   /**< IMU control register                        */
#define BOARD_PIC_REG_ENV_SENSOR_CTRL       0x01   /**< Environmental sensor group control register */
#define BOARD_PIC_REG_MIC_CTRL              0x02   /**< Microphone control register                 */
#define BOARD_PIC_REG_CCS_CTRL              0x03   /**< CCS811 control register                     */
#define BOARD_PIC_REG_LED_CTRL              0x04   /**< LED control register                        */
#define BOARD_PIC_REG_INT_ENABLE            0x05   /**< Interrupt enable register                   */
#define BOARD_PIC_REG_INT_CLEAR             0x06   /**< Interrupt clear register                    */
#define BOARD_PIC_REG_INT_FLAG              0x07   /**< Interrupt flag register                     */
#define BOARD_PIC_REG_INT_CTRL              0x08   /**< Interrupt options register                  */

#define BOARD_PIC_NUM_APP_REGS              9      /**< Number of application registers             */

/* System registers */
#define BOARD_PIC_REG_SYS_CMD               0xF0   /**< System command register           */
#define BOARD_PIC_REG_VERSION_MAJOR         0xF1   /**< Major version register            */
#define BOARD_PIC_REG_VERSION_MINOR         0xF2   /**< Minor version register            */
#define BOARD_PIC_REG_VERSION_PATCH         0xF3   /**< Patch version register            */

#define BOARD_PIC_REG_SCRATCH0              0xF4   /**< Scratchpad0 register              */
#define BOARD_PIC_REG_SCRATCH1              0xF5   /**< Scratchpad1 register              */
#define BOARD_PIC_REG_SCRATCH2              0xF6   /**< Scratchpad2 register              */
#define BOARD_PIC_REG_SCRATCH3              0xF7   /**< Scratchpad3 register              */

#define BOARD_PIC_REG_DEVICE_ID0            0xF8   /**< Device ID register, byte 0        */
#define BOARD_PIC_REG_DEVICE_ID1            0xF9   /**< Device ID register, byte 1        */
#define BOARD_PIC_REG_DEVICE_ID2            0xFA   /**< Device ID register, byte 2        */
#define BOARD_PIC_REG_DEVICE_ID3            0xFB   /**< Device ID register, byte 3        */

#define BOARD_PIC_REG_BOARD_REV             0xFC   /**< Board revision ID register        */

#define BOARD_PIC_SYS_REG_START             0xF0   /**< Start address of system register  */
#define BOARD_PIC_NUM_SYS_REGS              13     /**< Number of system registers        */

/* Bit fields for PIC_REG_IMU_CTRL */
#define BOARD_PIC_REG_IMU_CTRL_EN           0x01   /**< IMU control register, Enable bit  */

/* Bit fields for PIC_REG_ENV_SENSOR_CTRL */
#define BOARD_PIC_REG_ENV_SENSOR_CTRL_EN    0x01   /**< Environmental sensor group control register, Enable bit */

/* Bit fields for PIC_REG_MIC_CTRL */
#define BOARD_PIC_REG_MIC_CTRL_EN           0x01   /**< Microphone control register, Enable bit */

/* Bit fields for PIC_REG_CCS_CTRL */
#define BOARD_PIC_REG_CCS_CTRL_EN           0x01   /**< CCS811 control register, Enable bit  */
#define BOARD_PIC_REG_CCS_CTRL_WAKE         0x02   /**< CCS811 control register, Wake-up bit */

/* Bit fields for PIC_REG_LED_CTRL */
#define BOARD_PIC_REG_LED_CTRL_PWR_EN       0x01   /**< LED control register, Power enable bit        */
#define BOARD_PIC_REG_LED_CTRL_LED0         0x10   /**< LED control register, LED0 control bit        */
#define BOARD_PIC_REG_LED_CTRL_LED1         0x20   /**< LED control register, LED1 control bit        */
#define BOARD_PIC_REG_LED_CTRL_LED2         0x40   /**< LED control register, LED2 control bit        */
#define BOARD_PIC_REG_LED_CTRL_LED3         0x80   /**< LED control register, LED3 control bit        */
#define BOARD_PIC_REG_LED_CTRL_LED_MASK     0xf0   /**< LED control register, LED control mask        */
#define BOARD_PIC_REG_LED_CTRL_LED_SHIFT    4      /**< LED control register, LED control shift value */

/* Bit fields for PIC_REG_INT_ENABLE */
#define BOARD_PIC_REG_INT_ENABLE_CCS811     0x01   /**< Interrupt enable register, CSS811 INT enable bit */
#define BOARD_PIC_REG_INT_ENABLE_IMU        0x02   /**< Interrupt enable register, IMU INT enable bit    */
#define BOARD_PIC_REG_INT_ENABLE_UV_ALS     0x04   /**< Interrupt enable register, UV/ALS INT enable bit */

/* Bit fields for PIC_REG_INT_FLAG */
#define BOARD_PIC_REG_INT_FLAG_CCS811       0x01   /**< Interrupt flag register, CSS811 INT flag bit */
#define BOARD_PIC_REG_INT_FLAG_IMU          0x02   /**< Interrupt flag register, IMU INT flag bit    */
#define BOARD_PIC_REG_INT_FLAG_UV_ALS       0x04   /**< Interrupt flag register, UV/ALS INT flag bit */

/* Bit fields for PIC_REG_INT_CLEAR */
#define BOARD_PIC_REG_INT_CLEAR_CCS811      0x01   /**< Interrupt clear register, CSS811 INT clear bit */
#define BOARD_PIC_REG_INT_CLEAR_IMU         0x02   /**< Interrupt clear register, IMU INT clear bit    */
#define BOARD_PIC_REG_INT_CLEAR_UV_ALS      0x04   /**< Interrupt clear register, UV/ALS INT clear bit */

/* Bit fields for PIC_REG_INT_CTRL */
#define BOARD_PIC_REG_INT_CTRL_TIMER_MASK   0x0F   /**< Interrupt control register, TIMER period mask       */
#define BOARD_PIC_REG_INT_CTRL_TIMER_SHIFT  0      /**< Interrupt control register, TIMER period shift      */
#define BOARD_PIC_REG_INT_CTRL_TIMER_ENABLE 0x10   /**< Interrupt control register, Periodic timer enable   */
#define BOARD_PIC_REG_INT_CTRL_LEGACY_MODE  0x20   /**< Interrupt control register, legacy level interrupts */

#define BOARD_PIC_REG_SYS_CMD_BOOTMODE      0xA5   /**< System command register, Bootmode command  */

/** @} {end defgroup PIC_defs} */

/** @} {end addtogroup BOARD_4166} */
/** @} {end addtogroup TBSense_BSP} */

#endif // BOARD_PIC_REGS_H
