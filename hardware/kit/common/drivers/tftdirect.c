/***************************************************************************//**
 * @file
 * @brief SSD2119 TFT initialization and setup for Direct Drive mode
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

#include <stdbool.h>
#include "em_device.h"
#include "em_gpio.h"
#include "em_ebi.h"
#include "bsp.h"
#include "tftdirect.h"
/* DMD init from dmd_ssd2119_direct.c */
#include "glib/glib.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Tft
 * @{
 ******************************************************************************/

/** Flag to indicate that we need to initialize once if starting up in */
static bool runOnce = true;

/**************************************************************************//**
 * @brief  GPIO configuration for TFT direct drive
 *****************************************************************************/
void TFT_DirectGPIOConfig(void)
{
  /* EBI TFT DCLK/Dot Clock */
  GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull, 0);
  /* EBI TFT DATAEN */
  GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 0);
  /* EBI TFT VSYNC  */
  GPIO_PinModeSet(gpioPortA, 10, gpioModePushPull, 0);
  /* EBI TFT HSYNC */
  GPIO_PinModeSet(gpioPortA, 11, gpioModePushPull, 0);
}

/**************************************************************************//**
 * @brief  TFT initialization funxction. Assumes EBI has been configured
 *         correctly in @ref BSP_Init(BSP_INIT_DK_EBI)
 *
 * @param[in] tftInit Pointer to EBI TFT initialization structure
 *
 * @return true if we should redraw into buffer, false if BC has control
 *         over display
 *****************************************************************************/
bool TFT_DirectInit(const EBI_TFTInit_TypeDef *tftInit)
{
  bool     ret;
  uint32_t i, freq;
  EMSTATUS stat;

  /* If we are in BC_UIF_AEM_EFM state, we can redraw graphics */
  if (BSP_RegisterRead(&BC_REGISTER->UIF_AEM) == BC_UIF_AEM_EFM) {
    /* If we're not BC_ARB_CTRL_EBI state, we need to reconfigure display controller */
    if ((BSP_RegisterRead(&BC_REGISTER->ARB_CTRL) != BC_ARB_CTRL_EBI) || runOnce) {
      /* Enable SSD2119 Serial Port Interface */
      BSP_PeripheralAccess(BSP_TFT, true);

      /* Enable EBI mode of operation on SSD2119 controller */
      BSP_DisplayControl(BSP_Display_EBI);
      BSP_DisplayControl(BSP_Display_ResetAssert);
      BSP_DisplayControl(BSP_Display_PowerDisable);
      freq = SystemCoreClockGet();
      for (i = 0; i < (freq / 100); i++) {
        __NOP();
      }
      /* Configure display for Direct Drive "Mode Generic" + 3-wire SPI mode */
      BSP_DisplayControl(BSP_Display_ModeGeneric);
      BSP_DisplayControl(BSP_Display_PowerEnable);
      BSP_DisplayControl(BSP_Display_ResetRelease);

      /* Configure GPIO for EBI and TFT */
      TFT_DirectGPIOConfig();

      /* Initialize display */
      stat = DMD_init(0);
      if (DMD_OK == stat) {
        stat = DMD_selectFramebuffer((void*)EBI_BankAddress(EBI_BANK2));
      }
      if (DMD_OK != stat) {
        while (1) ;
      }

      /* Configure EBI TFT direct drive */
      EBI_TFTInit(tftInit);

      runOnce = false;
    }
    ret = true;
  } else {
    ret = false;
  }

  return ret;
}

/** @} (end group Tft) */
/** @} (end group kitdrv) */
