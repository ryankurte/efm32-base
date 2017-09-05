/**
 * @file ember-debug.h
 * See @ref debug for documentation.
 *
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

#ifndef __EMBER_DEBUG_H__
#define __EMBER_DEBUG_H__

// We currently don't have debug support in Connect.

// Define the values for DEBUG_LEVEL
#define NO_DEBUG    0
#define BASIC_DEBUG 1
#define FULL_DEBUG  2

#define emberDebugAssert(filename, linenumber) do {} while (FALSE)
#define emberDebugMemoryDump(start, end) do {} while (FALSE)
#define emberDebugBinaryPrintf(formatstring, ...) do {} while (FALSE)
#define emDebugSendVuartMessage(buff, len) do {} while (FALSE)
#define emberDebugError(code) do {} while (FALSE)
#define emberDebugReportOff() (FALSE)
#define emberDebugReportRestore(state) do {} while (FALSE)
#define emberDebugPrintf(...) do {} while (FALSE)

#define emLog(type, ...)
#define emLogLine(type, ...)
#define emLogBytes(type, format, bytes, count, ...)

#endif // __EMBER_DEBUG_H__
