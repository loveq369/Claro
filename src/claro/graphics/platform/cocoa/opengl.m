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


#define _PLATFORM_INC
#include <claro/graphics.h>
//#include <cairo-quartz.h>
#include "../macosx_cocoa.h"

#ifdef CLARO_OPENGL

int cgraphics_unikeycode_to_claro( int code );

@interface ClaroOpenGL : NSOpenGLView
{
	object_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

- (void)mouseDown:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)otherMouseDown:(NSEvent *)event;
- (void)otherMouseUp:(NSEvent *)event;
- (void)otherMouseDragged:(NSEvent *)event;
- (void)mouseMoved:(NSEvent *)event;

- (NSPoint)getLocalMousePosition:(NSEvent *)event;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroMove:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

/* event responders */
- (BOOL) acceptsFirstResponder;
- (void) keyUp:(NSEvent *)theEvent;
- (void) keyDown:(NSEvent *)theEvent;

- (int) claroModifiers:(NSEvent *)theEvent;
@end

@implementation ClaroOpenGL

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

#define macroMouseEvent(e) NSPoint pt = [self getLocalMousePosition: event]; \
event_send( OBJECT(cw), e, "iii", "x", (int)pt.x, "y", (int)pt.y, "modifiers", [self claroModifiers: event] );

- (void)rightMouseDown:(NSEvent *)event
{
	macroMouseEvent("right_clicked");
}

- (void)rightMouseUp:(NSEvent *)event
{
	macroMouseEvent("right_released");
}

- (void)otherMouseDown:(NSEvent *)event
{
	macroMouseEvent("middle_clicked");
}

- (void)otherMouseUp:(NSEvent *)event
{
	macroMouseEvent("middle_released");
}

- (void)mouseDown:(NSEvent *)event
{
	macroMouseEvent("clicked");
}

- (void)mouseUp:(NSEvent *)event
{
	macroMouseEvent("released");
}

- (NSPoint)getLocalMousePosition:(NSEvent *)event
{
	NSPoint event_location = [event locationInWindow];
	NSPoint local_point = [self convertPoint:event_location fromView:nil];
	NSRect frame = [self frame];
	
	local_point.y = frame.size.height - local_point.y;
	
	return local_point;
}


- (BOOL) acceptsFirstResponder
{
	return (WIDGET(cw)->notify_flags & cNotifyKey);
}

- (int) claroModifiers:(NSEvent *)theEvent
{
	int mods = 0, lmods = 0;
	
	lmods = [theEvent modifierFlags];
	
	if ( lmods & NSAlternateKeyMask ) mods |= cModifierKeyAlternate;
	if ( lmods & NSControlKeyMask ) mods |= cModifierKeyControl;
	if ( lmods & NSCommandKeyMask ) mods |= cModifierKeyCommand;
	if ( lmods & NSShiftKeyMask ) mods |= cModifierKeyShift;
	
	return mods;
}

- (void) keyUp:(NSEvent *)theEvent
{
	int key = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
	int mods = 0;
	
	if ( !( WIDGET(cw)->notify_flags & cNotifyKey ) )
		return;
	
	mods = [self claroModifiers: theEvent];
	
	event_send( OBJECT(cw), "key_up", "ii", "key", key, "modifiers", mods );
}

- (void) keyDown:(NSEvent *)theEvent
{
	int key = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
	int mods = 0;
	
	if ( !( WIDGET(cw)->notify_flags & cNotifyKey ) )
		return;
	
	mods = [self claroModifiers: theEvent];
	
	event_send( OBJECT(cw), "key_down", "ii", "key", key, "modifiers", mods );
}

- (void)mouseMoved:(NSEvent *)event
{
	macroMouseEvent("mouse_moved");
	
	[super mouseMoved: event];
}

- (void)mouseDragged:(NSEvent *)event
{
	macroMouseEvent("mouse_moved");
	
	[super mouseDragged: event];
}

- (void)otherMouseDragged:(NSEvent *)event
{
	macroMouseEvent("mouse_moved");
	
	[super otherMouseDragged: event];
}

- (void)scrollWheel:(NSEvent *)event
{
	NSPoint pt = [self getLocalMousePosition: event];
	double dx=[event deltaX], dy=[event deltaY], dz=[event deltaZ];
	
	event_send( OBJECT(cw), "scroll_wheel", "iidddi",
			"x", (int)pt.x, "y", (int)pt.y,
			"deltaX", dx, "deltaY", dy, "deltaZ", dz,
			"modifiers", [self claroModifiers: event]
	);
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = OBJECT(widget);
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSWindowDidMoveNotification
		object:self];
	
	[self openGLContext];
}

@end

void cgraphics_opengl_widget_create( widget_t *widget )
{
	ClaroOpenGL *v;
	NSView *parent = cgraphics_get_native_parent( widget );
	NSOpenGLPixelFormat *pf;
	NSOpenGLPixelFormatAttribute attrs[] =
	{
	    NSOpenGLPFADoubleBuffer,
	    NSOpenGLPFADepthSize, 32,
	    0
	};
	
	pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	
	v = [[ClaroOpenGL alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		pixelFormat: pf
		];
	
	[v setClaroWidget:widget];
	[parent addSubview: v];
	
	[[v window] makeFirstResponder:v];
	[[v window] setAcceptsMouseMovedEvents:YES];
	
	widget->native = (NSControl *)v;
}

void cgraphics_opengl_flip( widget_t *widget )
{
	ClaroOpenGL *v = (ClaroOpenGL *)widget->native;
	[[v openGLContext] flushBuffer];
}

void cgraphics_opengl_activate( widget_t *widget )
{
	ClaroOpenGL *v = (ClaroOpenGL *)widget->native;
	[[v openGLContext] makeCurrentContext];
}

#endif
