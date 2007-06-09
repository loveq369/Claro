/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#ifndef _CLARO_BASE_H
#define _CLARO_BASE_H

#ifdef _NIX
typedef unsigned short bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true !(false)
#endif

#ifdef REAL_GLIB
#	include <glib.h>
#else
#	ifdef NEEDS_GLIB
#		include <claro/base/eglib/src/glib.h>
#	else
// some defs to compile without the glib header..
#		define GPtrArray void
#		define GHashTable void
#		define gboolean int
#	endif
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !(FALSE)
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "base/stdinc.h"
#include "base/list.h"
#include "base/block.h"
#include "base/object.h"
#include "base/classes.h"
#include "base/events.h"
#include "base/claro.h"
#include "base/log.h"
#include "base/memory.h"
#include "base/store.h"
#include "base/object_map.h"
#include "base/string.h"
//added by Cody, 2/8/06
//this crap can be removed now
#include "base/hashtable.h"
//added by Cody, 5/8/07
#include "base/atomic.h"

#ifdef __cplusplus
}
#endif

#endif
