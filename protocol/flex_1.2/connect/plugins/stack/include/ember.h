/**
 * @file ember.h
 * @brief The master include file for the Ember Connect API.
 *
 *  See @ref ember for documentation.
 *
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

#ifndef __EMBER_H__
#define __EMBER_H__

#if !defined(UNIX_HOST) && !defined(CORTEXM3_EMBER_MICRO)
#include "em_device.h"
#endif

#include "ember-types.h"
#include "error.h"
#include "memory-buffer.h"
#include "event.h"
#include "network-management.h"
#include "stack-info.h"
#include "message.h"
#include "ember-debug.h"
#include "byte-utilities.h"

#endif // __EMBER_H__
