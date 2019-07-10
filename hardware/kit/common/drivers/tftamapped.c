/***************************************************************************//**
 * @file
 * @brief SSD2119 TFT initialization and setup for Adress Mapped mode
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
#include "tftamapped.h"
/* DMD init from dmd_ssd2119_direct.c */
#include "dmd/ssd2119/dmd_ssd2119.h"
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
 * @brief  TFT initialize or reinitialize to Address Mapped Mode
 *         Assumes EBI has been configured correctly in @ref BSP_Init(BSP_INIT_DK_EBI)
 *
 * @return true if we should redraw into buffer, false if BC has control
 *         over display
 *****************************************************************************/
bool TFT_AddressMappedInit(void)
{
  bool     ret;
  EMSTATUS status;
  uint32_t i, freq;

  /* If we are in BC_UIF_AEM_EFM state, we can redraw graphics */
  if (BSP_RegisterRead(&BC_REGISTER->UIF_AEM) == BC_UIF_AEM_EFM) {
    /* If we're not BC_ARB_CTRL_EBI state, we need to reconfigure display controller */
    if ((BSP_RegisterRead(&BC_REGISTER->ARB_CTRL) != BC_ARB_CTRL_EBI) || runOnce) {
      /* Resetting display while SPI_DEMUX is set to display does not work */
      if (BSP_RegisterRead(&BC_REGISTER->SPI_DEMUX) == BC_SPI_DEMUX_SLAVE_DISPLAY) {
        BSP_RegisterWrite(&BC_REGISTER->SPI_DEMUX, BC_SPI_DEMUX_SLAVE_AUDIO);
      }
      /* Configure for EBI mode and reset display */
      BSP_DisplayControl(BSP_Display_EBI);
      BSP_DisplayControl(BSP_Display_ResetAssert);
      BSP_DisplayControl(BSP_Display_PowerDisable);
      /* Short reset delay */
      freq = SystemCoreClockGet();
      for (i = 0; i < (freq / 100); i++) {
        __NOP();
      }
      /* Configure display for Direct Drive + SPI mode */
      BSP_DisplayControl(BSP_Display_Mode8080);
      BSP_DisplayControl(BSP_Display_PowerEnable);
      BSP_DisplayControl(BSP_Display_ResetRelease);

      /* Initialize graphics - abort on failure */
      status = DMDIF_init(BC_SSD2119_BASE, BC_SSD2119_BASE + 2);
      if (status == DMD_OK) {
        status = DMD_init(0);
      }
      if ((status != DMD_OK) && (status != DMD_ERROR_DRIVER_ALREADY_INITIALIZED)) {
        while (1) ;
      }
      /* Make sure display is configured with correct rotation */
      if ((status == DMD_OK)) {
        DMD_flipDisplay(1, 1);
      }

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
