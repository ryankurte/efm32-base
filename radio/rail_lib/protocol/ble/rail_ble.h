/***************************************************************************//**
 * @file
 * @brief The BLE specific header file for the RAIL library.
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

#ifndef __RAIL_BLE_H__
#define __RAIL_BLE_H__

// Get the standard include types
#include <stdint.h>
#include <stdbool.h>

// Get the RAIL specific structures and types
#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup BLE
/// @ingroup Protocol_Specific
/// Accelerator routines for Bluetooth Low Energy (BLE).
///
/// The APIs in this module configure the radio for BLE
/// operation and provide additional helper routines necessary for
/// normal BLE send/receive that aren't available directly in RAIL.
/// RAIL APIs should be used to set up the application. However,
/// RAIL_ConfigChannels() and RAIL_ConfigRadio() should not be called to set up
/// the PHY. Instead, RAIL_BLE_Config* APIs should be used to set up the
/// 1 Mbps, 2 Mbps, or Coded PHY configurations needed by the application. These
/// APIs will configure the hardware and also configure the set of valid BLE
/// channels.
///
/// To implement a standard BLE link layer, you will also need to handle tight
/// turnaround times and send packets at specific instants. This can all be
/// managed through general RAIL functions, such as RAIL_ScheduleTx(),
/// RAIL_ScheduleRx(), and RAIL_SetStateTiming(). See RAIL APIs for more
/// useful functions.
///
/// A simple example to set up the application to be in BLE mode is shown
/// below. Note that this will put the radio on the first advertising channel
/// with the advertising Access Address. In any full-featured BLE application you
/// will need to use the RAIL_BLE_ConfigChannelRadioParams() function to change
/// the sync word and other parameters as needed based on your connection.
///
/// @code{.c}
///
/// // RAIL Handle set at initialization time.
/// static RAIL_Handle_t gRailHandle = NULL;
///
/// static void radioEventHandler(RAIL_Handle_t railHandle,
///                               RAIL_Events_t events)
/// {
///   // ... handle RAIL events, e.g., receive and transmit completion
/// }
///
/// #if MULTIPROTOCOL
/// // Allocate memory for RAIL to hold BLE-specific state information
/// static RAIL_BLE_State_t bleState; // Must never be const
/// static RAILSched_Config_t schedCfg; // Must never be const
/// static RAIL_Config_t railCfg = {  // Must never be const
///   .eventsCallback = &radioEventHandler,
///   .protocol = &bleState, // For BLE, RAIL needs additional state memory
///   .scheduler = &schedCfg, // For MultiProtocol, additional scheduler memory
/// };
/// #else
/// static RAIL_Config_t railCfg = {  // Must never be const
///   .eventsCallback = &radioEventHandler,
///   .protocol = NULL,
///   .scheduler = NULL,
/// };
/// #endif
///
/// // Set the radio to receive on the first BLE advertising channel.
/// int bleAdvertiseEnable(void)
/// {
///   // Initializes the RAIL library and any internal state it requires.
///   gRailHandle = RAIL_Init(&railCfg, NULL);
///
///   // Calls the BLE initialization function to load the right radio configuration.
///   RAIL_BLE_Init(gRailHandle);
///
///   // Always choose the Viterbi PHY configuration if available on your chip
///   // for performance reasons.
///   RAIL_BLE_ConfigPhy1MbpsViterbi(gRailHandle);
///
///   // Configures us for the first advertising channel (Physical: 0, Logical: 37).
///   // The CRC init value and Access Address come from the BLE specification.
///   RAIL_BLE_ConfigChannelRadioParams(gRailHandle,
///                                     0x555555,
///                                     0x8E89BED6,
///                                     37,
///                                     false);
///
///   // Starts receiving on physical channel 0 (logical channel 37).
///   RAIL_StartRx(gRailHandle, 0, NULL);
///  }
/// @endcode
///
/// @{

/**
 * @enum RAIL_BLE_Coding_t
 * @brief The variant of the BLE Coded PHY.
 */
RAIL_ENUM(RAIL_BLE_Coding_t) {
  /** Enables the 125 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coding_125kbps = 0,
  /** \deprecated Will be removed in a future version of RAIL */
  RAIL_BLE_Coding_125kbps_DSA = 1,
  /** Enables the 500 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coding_500kbps = 2,
  /** \deprecated Will be removed in a future version of RAIL */
  RAIL_BLE_Coding_500kbps_DSA = 3,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_BLE_Coding_125kbps     ((RAIL_BLE_Coding_t) RAIL_BLE_Coding_125kbps)
#define RAIL_BLE_Coding_125kbps_DSA ((RAIL_BLE_Coding_t) RAIL_BLE_Coding_125kbps_DSA)
#define RAIL_BLE_Coding_500kbps     ((RAIL_BLE_Coding_t) RAIL_BLE_Coding_500kbps)
#define RAIL_BLE_Coding_500kbps_DSA ((RAIL_BLE_Coding_t) RAIL_BLE_Coding_500kbps_DSA)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @enum RAIL_BLE_Phy_t
 * @brief The variant of the BLE PHY.
 */
RAIL_ENUM(RAIL_BLE_Phy_t) {
  /** Use the standard BLE 1Mbps PHY */
  RAIL_BLE_1Mbps,
  /** Use the high data rate BLE 2Mbps PHY */
  RAIL_BLE_2Mbps,
  /** Enables the 125 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coded125kbps,
  /** Enables the 500 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coded500kbps,
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Self-referencing defines minimize compiler complaints when using RAIL_ENUM
#define RAIL_BLE_1Mbps        ((RAIL_BLE_Phy_t) RAIL_BLE_1Mbps)
#define RAIL_BLE_2Mbps        ((RAIL_BLE_Phy_t) RAIL_BLE_2Mbps)
#define RAIL_BLE_Coded125kbps ((RAIL_BLE_Phy_t) RAIL_BLE_Coded125kbps)
#define RAIL_BLE_Coded500kbps ((RAIL_BLE_Phy_t) RAIL_BLE_Coded500kbps)
#endif//DOXYGEN_SHOULD_SKIP_THIS

/**
 * @struct RAIL_BLE_State_t
 * @brief A state structure for BLE.
 *
 * This structure must be allocated in application global read-write memory
 * that persists for the duration of BLE usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef struct RAIL_BLE_State {
  uint32_t crcInit; /**< The value used to initialize the CRC algorithm. */
  uint32_t accessAddress; /**< The access address used for the connection. */
  uint16_t channel; /**< The logical channel used. */
  bool disableWhitening; /**< Indicates whether the whitening engine should be off. */
} RAIL_BLE_State_t;

/**
 * Configures RAIL to run in BLE mode.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * This function will change your radio, channel configuration, and other
 * parameters to match what is needed for BLE. To switch back to a
 * default RAIL mode, call RAIL_BLE_Deinit() first. This function
 * will configure the protocol output on PTI to \ref RAIL_PTI_PROTOCOL_BLE.
 */
void RAIL_BLE_Init(RAIL_Handle_t railHandle);

/**
 * Takes RAIL out of BLE mode.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * This function will undo some of the configuration that happens when you call
 * RAIL_BLE_Init(). After this you can safely run your normal radio
 * initialization code to use a non-BLE configuration. This function will \b
 * not change back your radio or channel configurations so you must do this by
 * manually reinitializing. This also resets the protocol output on PTI to \ref
 * RAIL_PTI_PROTOCOL_CUSTOM.
 */
void RAIL_BLE_Deinit(RAIL_Handle_t railHandle);

/**
 * Determines whether BLE mode is enabled or not.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return True if BLE mode is enabled and false otherwise.
 * This function returns the current status of RAIL's BLE mode. It is enabled by
 * a call to RAIL_BLE_Init() and disabled by a call to RAIL_BLE_Deinit().
 */
bool RAIL_BLE_IsEnabled(RAIL_Handle_t railHandle);

/**
 * Switches the Viterbi 1 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to the defualt BLE 1 Mbps PHY if you
 * have switched to the 2 Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 *
 * @note The EFR32XG1 family does not support BLE Viterbi PHYs, however calls
 *   to this function from that family will be silently redirected to the legacy
 *   \ref RAIL_BLE_ConfigPhy1Mbps().
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switches the legacy non-Viterbi 1 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * You can use this function to switch back to the legacy BLE 1 Mbps PHY if you
 * have switched to the 2 Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 *
 * @note The EFR32XG2x family does not support BLE non-Viterbi PHYs.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1Mbps(RAIL_Handle_t railHandle);

/**
 * Switches the Viterbi 2 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * You can use this function to switch back to the BLE 2 Mbps PHY from the
 * default 1 Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note The EFR32XG1 family does not support BLE Viterbi PHYs.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switches the legacy non-Viterbi 2 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to legacy BLE 2Mbps PHY from the
 * default 1 Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note The EFR32XG1 and EFR32XG2x families do not support BLE non-Viterbi
 *   2 Mbps PHY.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2Mbps(RAIL_Handle_t railHandle);

/**
 * Switches to the BLE Coded PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] bleCoding The RAIL_BLE_Coding_t to use
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to BLE Coded PHY from the default
 * 1 Mbps option. You may only call this function after initializing BLE and
 * while the radio is idle. When using a BLE Coded PHY, the subPhy in
 * RAIL_AppendedInfo_t marks the coding of the received packet. A subPhy of 0
 * marks a 500 kbps packet, and a subPhy of 1 marks a 125 kbps packet.
 *
 * @note The EFR32XG1, EFR32XG12, and EFR32XG14 families do not support BLE
 *   Coded PHYs.
 */
RAIL_Status_t RAIL_BLE_ConfigPhyCoded(RAIL_Handle_t railHandle,
                                      RAIL_BLE_Coding_t bleCoding);

/**
 * Helper function to change BLE radio parameters.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] crcInit The value to use for CRC initialization.
 * @param[in] accessAddress The access address to use for the connection.
 * @param[in] channel The logical channel that you're changing to, which
 * initializes the whitener if used.
 * @param[in] disableWhitening This can turn off the whitening engine and is useful
 * for sending BLE test mode packets that don't have this turned on.
 * @return A status code indicating success of the function call.
 *
 * This function can be used to switch radio parameters on every connection
 * and/or channel change. It is BLE-aware and will set the access address,
 * preamble, CRC initialization value, and whitening configuration without
 * requiring you to load a new radio configuration.
 */
RAIL_Status_t RAIL_BLE_ConfigChannelRadioParams(RAIL_Handle_t railHandle,
                                                uint32_t crcInit,
                                                uint32_t accessAddress,
                                                uint16_t channel,
                                                bool disableWhitening);

/**
 * Change the current BLE PHY and go into receive
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] phy Which PHY to receive on
 * @param[in] railChannel Which channel of the given PHY to receive on
 * @param[in] startRxTime When to enter RX
 * @param[in] crcInit The value to use for CRC initialization.
 * @param[in] accessAddress The access address to use for the connection.
 * @param[in] logicalChannel The logical channel that you're changing to, which
 * initializes the whitener if used.
 * @param[in] disableWhitening This can turn off the whitening engine and is useful
 * for sending BLE test mode packets that don't have this turned on.
 * @return A status code indicating success of the function call.
 *
 * This function is used to implement auxillary packet reception, as defined in
 * the BLE specification. The radio will be put into IDLE, the PHY and channel
 * will be changed, and then receive will be entered at the start time given.
 * The new receive will have a timeout of 30 us, which means that this function
 * should only be called if the offset unit is 30 us.
 */
RAIL_Status_t RAIL_BLE_PhySwitchToRx(RAIL_Handle_t railHandle,
                                     RAIL_BLE_Phy_t phy,
                                     uint16_t railChannel,
                                     uint32_t startRxTime,
                                     uint32_t crcInit,
                                     uint32_t accessAddress,
                                     uint16_t logicalChannel,
                                     bool disableWhitening);

/******************************************************************************
 * Angle of Arrival/Departure (AoX)
 *****************************************************************************/
/**
 * @addtogroup AoX Angle of Arrival/Departure
 * @{
 * @brief These APIs are to a stack implementing BLE's angle of arrival and
 * angle of departure functionality.
 *
 * They are designed for use by the Silicon Labs BLE stack only at this time and
 * may cause problems if accessed directly.
 */

/**
 * @enum RAIL_BLE_AoxOptions_t
 * @brief Angle of Arrival/Departure options bit fields
 */
RAIL_ENUM_GENERIC(RAIL_BLE_AoxOptions_t, uint16_t) {
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_DO_SWITCH bit */
  RAIL_BLE_AOX_OPTIONS_DO_SWITCH_SHIFT = 0,
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_RX_ENABLED bit */
  RAIL_BLE_AOX_OPTIONS_RX_ENABLED_SHIFT = 1,
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_SAMPLE_MODE bit */
  RAIL_BLE_AOX_OPTIONS_SAMPLE_MODE_SHIFT = 2,
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_TX_ENABLED bit */
  RAIL_BLE_AOX_OPTIONS_TX_ENABLED_SHIFT = 3,
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_CONNLESS_SHIFT bit */
  RAIL_BLE_AOX_OPTIONS_CONNLESS_SHIFT = 10,
  /** Shift position of \ref RAIL_BLE_AOX_OPTIONS_CONN_SHIFT bit */
  RAIL_BLE_AOX_OPTIONS_CONN_SHIFT = 13,
};

/**
 * Sets one of the two AoX sampling/switching modes: 1us or 2us window.
 */
#define RAIL_BLE_AOX_OPTIONS_SAMPLE_MODE  (1U << RAIL_BLE_AOX_OPTIONS_SAMPLE_MODE_SHIFT)
/**
 * Enables AoX Rx packets. When enabled radio will either receive AoX packet if
 * CTE-present bit in the packet is set or normal packet if this bit isn't set.
 */
#define RAIL_BLE_AOX_OPTIONS_RX_ENABLED   (1U << RAIL_BLE_AOX_OPTIONS_RX_ENABLED_SHIFT)
/**
 * Enables antennae switching during CTE. If disabled antenna 0 will be active.
 * If enabled antennae will switch from 0 to N, wrapping back to 0.
 */
#define RAIL_BLE_AOX_OPTIONS_DO_SWITCH    (1U << RAIL_BLE_AOX_OPTIONS_DO_SWITCH_SHIFT)
/**
 * Enables AoX Tx packets. When set radio will transmit AoX packet, to
 * transmit normal (non-AoX) packet clear this bitfield.
 */
#define RAIL_BLE_AOX_OPTIONS_TX_ENABLED   (1U << RAIL_BLE_AOX_OPTIONS_TX_ENABLED_SHIFT)
/**
 * Enables connectionless AoX Rx packets. This requires \ref
 * RAIL_BLE_AOX_OPTIONS_RX_ENABLED to be set.
 */
#define RAIL_BLE_AOX_OPTIONS_CONNLESS     (1U << RAIL_BLE_AOX_OPTIONS_CONNLESS_SHIFT)
/**
 * Enables connection based AoX Rx packets. This requires \ref
 * RAIL_BLE_AOX_OPTIONS_RX_ENABLED to be set.
 */
#define RAIL_BLE_AOX_OPTIONS_CONN         (1U << RAIL_BLE_AOX_OPTIONS_CONN_SHIFT)

/**
 * @struct RAIL_BLE_AoxConfig_t
 * @brief Contains arguments for \ref RAIL_BLE_ConfigAox function.
 */
typedef struct RAIL_BLE_AoxConfig {
  /**
   * See RAIL_BLE_AOX_OPTIONS_* for bitfield defines for different AoX features.
   */
  RAIL_BLE_AoxOptions_t aoxOptions;
  /**
   * Size of the raw AoX CTE (continuous tone extension) data capture buffer in
   * bytes.
   */
  uint16_t cteBuffSize;
  /**
   * Address to where the received CTE is written.
   */
  uint32_t * cteBuffAddr;
  /**
   * Address to first element of antenna pattern array.
   */
  // @TODO: Can we change this to uint8_t* and make non-word-aligned
  // memory accesses in the sequencer?
  uint32_t * antArrayAddr;
  /**
   * Size of the antenna pattern array.
   */
  uint8_t antArraySize;
} RAIL_BLE_AoxConfig_t;

/**
 * Locks/unlocks the CTE buffer from the application's perspective. The radio
 * will write to the buffer only if the bit is NOT set at the beginning of the
 * sampling period. The radio will set the bit once the sampling period starts
 * to indicate that some CTE data has been collected, which will not be
 * overwritten during the next sampling period, unless the buffer is unlocked by
 * the application.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] lock Lock the CTE buffer if true and unlock it if false.
 * @return True if the CTE buffer is locked after the call, otherwise false.
 */
bool RAIL_BLE_LockCteBuffer(RAIL_Handle_t railHandle, bool lock);

/**
 * Determines whether the CTE buffer is currently locked or not.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return True if CTE buffer is locked and false otherwise.
 */
bool RAIL_BLE_CteBufferIsLocked(RAIL_Handle_t railHandle);

/**
 * Configures Angle of Arrival/Departure (AoX) functionality. AoX is a method
 * of radio localization which infers angle of arrival/departure of the signal
 * based on different phases of the raw I/Q signal from different antennas by
 * controlling external RF switch during the continuous tone extension (CTE).
 * Connection based AoX packets are different than normal BLE packets in that
 * they have 3 header bytes instead of 2, and they have CTE appended after the
 * payload's CRC. 3rd byte or CTE info contains CTE length. Connectionless AoX
 * packets have 2 header bytes and CTE info is part of the paylod. AoX is
 * supported on efr32xg12/13/14 only on legacy 1Mbps BLE PHY. Note that
 * \ref RAIL_TX_OPTION_REMOVE_CRC_SHIFT option in \ref RAIL_TxOptions_t will
 * not work when \ref RAIL_BLE_AOX_OPTIONS_TX_ENABLED is enabled. Also
 * note that calling \ref RAIL_GetRadioEntropy during AoX reception may break
 * receiving packets.
 *
 * @param[in] railHandle A RAIL instance handle.
 * @param[in] aoxConfig Configuration options for AoX
 * @return RAIL_Status_t indicating success or failure of the call.
 */
RAIL_Status_t RAIL_BLE_ConfigAox(RAIL_Handle_t railHandle,
                                 const RAIL_BLE_AoxConfig_t *aoxConfig);

/** @} */  // end of group AoX

/** @} */ // end of BLE

#ifdef __cplusplus
}
#endif

#endif // __RAIL_BLE_H__
