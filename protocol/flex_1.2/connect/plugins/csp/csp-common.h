// File: csp-common.h
//
// Description: Common functions for Connect Serial Protocol.
//
// Copyright 2015 by Ember Corporation. All rights reserved.                *80*

#ifndef __CSP_COMMON_H__
#define __CSP_COMMON_H__

/** @brief
 *  Gets the CSP version.
 */
uint16_t emberGetCspVersion(void);

/** @brief
 * Detects if the standalone bootloder is installed, and if so
 * returns the installed version and info about the platform,
 * micro and phy. If not version will be set to 0xffff. A
 * returned version of 0x1234 would indicate version 1.2 build 34.
 */
void emberGetStandaloneBootloaderInfo(void);

/** @brief
 * Provides the result of a call to ::emberGetStandaloneBootloaderInfo.
 *
 * @param version   BOOTLOADER_INVALID_VERSION if the standalone
 *                  bootloader is not present, or the version of
 *                  the installed standalone bootloader.
 * @param nodePlat  The value of PLAT on the node.
 * @param nodeMicro The value of MICRO on the node.
 * @param nodePhy   The value of PHY on the node.
 */
void emberGetStandaloneBootloaderInfoReturn(uint16_t version,
                                            uint8_t platformId,
                                            uint8_t microId,
                                            uint8_t phyId);

/** @brief
 * Launch the standalone bootloader (if installed).
 * The function returns an error if the standalone bootloader
 * is not present
 *
 * @param mode Controls the mode in which the standalone
 *             bootloader will run. See the app. note for full
 *             details. Options are:
 *             STANDALONE_BOOTLOADER_NORMAL_MODE: Will listen for
 *             an over-the-air image transfer on the current
 *             channel with current power settings.
 *             STANDALONE_BOOTLOADER_RECOVERY_MODE: Will listen for
 *             an over-the-air image transfer on the default
 *             channel with default power settings. Both modes
 *             also allow an image transfer to begin with
 *             XMODEM over the serial protocol's Bootloader
 *             Frame.
 */
void emberLaunchStandaloneBootloader(uint8_t mode);

/** @brief
 * Provides the result of a call to ::emberLaunchStandaloneBootloader.
 *
 * @param status An EmberStatus value indicating success or the
 *               reason for failure.
 */
void emberLaunchStandaloneBootloaderReturn(EmberStatus status);

#endif // __CSP_COMMON_H__
