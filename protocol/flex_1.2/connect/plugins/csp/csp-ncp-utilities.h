/** @file csp-ncp-utilities.h
 *
 *  Utilities to support NCP-side processing of CSP commands and returns.
 *
 * <!--Copyright 2016 by Silicon Labs. All rights reserved.              *80*-->
 */

#ifndef __CSP_NCP_UTILITIES_H__
#define __CSP_NCP_UTILITIES_H__

// Pre/post hooks allow for special handling before/after the default behavior
// of generated NCP command handler/return code.

bool cspNcpLaunchStandaloneBootloaderPreHook(uint8_t mode);

#endif //__CSP_NCP_UTILITIES_H__
