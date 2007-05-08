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

void cgraphics_workspace_widget_create( widget_t *widget )
{
	widget->native = gtk_notebook_new( );

	//gtk_notebook_set_show_tabs( widget->native, TRUE );	
	//g_object_set(G_OBJECT(widget->native), "enable-popup", TRUE, NULL);
	
	cgraphics_widget_create( widget );
}

void cgraphics_workspace_set_active( widget_t *widget, object_t *child )
{
	int pid = -1;
	
	pid = gtk_notebook_page_num( widget->native, WIDGET(child)->native );
	
	gtk_notebook_set_current_page( widget->native, pid );
}

object_t *cgraphics_workspace_get_active( widget_t *widget )
{
	int active;
	GtkWidget *gwidget;
	widget_t *cw;
	
	active = gtk_notebook_get_current_page( widget->native );
	
	if ( active == -1 )
		return NULL;
	
	gwidget = gtk_notebook_get_nth_page( widget->native, active );
	cw = cgraphics_gtk_to_claro_widget( gwidget );
	
	return (object_t*)cw;
}

void cgraphics_workspace_cascade( widget_t *widget )
{
	
}

void cgraphics_workspace_tile( widget_t *widget, int dir )
{
	
}


void cgraphics_workspace_window_widget_create( widget_t *widget )
{
	widget_t *parent = WIDGET(OBJECT(widget)->parent);
		
	widget->native = gtk_layout_new( NULL, NULL );
		
	cgraphics_widget_create( widget );
    
    gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(parent->native), GTK_WIDGET(widget->native), TRUE);
	gtk_notebook_set_tab_detachable(GTK_NOTEBOOK(parent->native), GTK_WIDGET(widget->native), TRUE);
}

void cgraphics_workspace_window_update_title( widget_t *widget )
{
	workspace_window_widget_t * window = (workspace_window_widget_t *)widget;
	GtkNotebook * notebook = GTK_NOTEBOOK(WIDGET(OBJECT(window)->parent)->native);
	GtkWidget * tab_label = gtk_notebook_get_tab_label(notebook, GTK_WIDGET(widget->native));
	
	//should create a custom widget here that contains a label + image
	/*     gtk_notebook_set_tab_label_text(
	        GTK_NOTEBOOK(WIDGET(OBJECT(window)->parent)->native), 
	        GTK_WIDGET(widget->native), window->title);    
	*/
	if(window->title) 
	    gtk_label_set_text(GTK_LABEL(g_object_get_data(G_OBJECT(tab_label), "label")), window->title);    
}

void cgraphics_workspace_window_show( widget_t *widget )
{
	gtk_widget_show( widget->native );
}

void cgraphics_workspace_window_hide( widget_t *widget )
{
	gtk_widget_hide( widget->native );
}

void cgraphics_workspace_window_restore( widget_t *widget )
{
	gtk_widget_show( widget->native );
}

void cgraphics_workspace_window_minimise( widget_t *widget )
{
	gtk_widget_hide( widget->native );
}

void cgraphics_workspace_window_maximise( widget_t *widget )
{
	gtk_widget_show( widget->native );
}

void cgraphics_workspace_window_update_icon( widget_t *widget )
{
    workspace_window_widget_t * window = (workspace_window_widget_t *)widget;
    GtkNotebook * notebook = GTK_NOTEBOOK(WIDGET(OBJECT(window)->parent)->native);
	GtkWidget * tab_label = gtk_notebook_get_tab_label(notebook, GTK_WIDGET(widget->native));	
	GtkImage * icon = GTK_IMAGE(g_object_get_data(G_OBJECT(tab_label), "icon"));
	if(window->icon)
	    gtk_image_set_from_pixbuf(icon, GDK_PIXBUF(window->icon->native));
}
