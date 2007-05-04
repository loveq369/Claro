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


#ifndef _CLARO_GRAPHICS_GRAPHICS_H
#define _CLARO_GRAPHICS_GRAPHICS_H

#ifdef _MAC
#	include "platform/macosx_cocoa.h"
#endif

#ifdef _NIX
#	include "platform/gtk.h"
#endif

#ifdef _WIN32
#	include "platform/win32.h"
#endif

#ifdef CLARO_SOURCE
#undef CLARO_APPLICATION
#define CLARO_APPLICATION "claro.graphics"
#endif

#endif
