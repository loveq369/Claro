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


#include <claro/graphics.h>
#include <assert.h>
#include <claro/graphics/platform.h>

/* TextBox */

static int cgraphics_textbox_mod_flag = 0;

static gint cgraphics_textbox_changed_handler( GtkWidget *widget, widget_t *w )
{
	const char *tmp = gtk_entry_get_text( GTK_ENTRY(w->native) );
	textbox_widget_t *tw = (textbox_widget_t *)w;
	
	/* hack, gtk gives us an empty string the first time on set text */
	if ( cgraphics_textbox_mod_flag > 0 )
	{
		cgraphics_textbox_mod_flag--;
		return 0;
	}
	
	strncpy( tw->text, tmp, CLARO_TEXTBOX_MAXIMUM-1 );
	
	event_send( OBJECT(w), "changed", "" );
		
	return 1;
}

static gint cgraphics_textbox_keypress_handler( GtkWidget *widget, GdkEventKey *e, widget_t *w )
{
	int ucs4 = gdk_keyval_to_unicode(e->keyval);
	gchar * utf8 = g_ucs4_to_utf8(&ucs4, 1, NULL, NULL, NULL);
	int r = event_send( w, "key_down", "i", "key", (int)utf8[0] );
	g_free(utf8);
	return r;
}

//FIXME: add "activate" signal, must be implemented on other platforms
void cgraphics_textbox_widget_create( widget_t *widget )
{
	widget->native = gtk_entry_new( );
	
	cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "changed", G_CALLBACK(cgraphics_textbox_changed_handler), widget );
	g_signal_connect( G_OBJECT(widget->native), "key_press_event", G_CALLBACK(cgraphics_textbox_keypress_handler), widget );
}

void cgraphics_textbox_update_text( textbox_widget_t *widget )
{
	/* hack: see above */
	cgraphics_textbox_mod_flag = 1;
	
	gtk_entry_set_text( GTK_ENTRY(widget->widget.native), widget->text );
}

void cgraphics_textbox_set_pos( widget_t *widget, int pos )
{
	
}

int cgraphics_textbox_get_pos( widget_t *widget )
{
	
}
