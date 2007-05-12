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
#include <claro/graphics/platform.h>

void menu_new_item_handle( object_t *obj, event_t *event );
void menu_remove_item_handle( object_t *obj, event_t *event );

void menu_widget_inst_create( object_t *object );
void menu_widget_inst_realize( object_t *object );

claro_define_type_partial( menu_widget, list_widget, menu_widget_inst_create, menu_widget_inst_realize, NULL, NULL );

void menu_widget_inst_create( object_t *object )
{
	list_widget_init( object, 2, CLIST_TYPE_PTR, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( object, "new_row", menu_new_item_handle );
	object_addhandler( object, "remove_row", menu_remove_item_handle );
}

void menu_widget_inst_realize( object_t *object )
{
	cgraphics_menu_widget_create( WIDGET(object) );
}

object_t *menu_widget_create( object_t *parent, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( menu_widget_type, parent );
	
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

list_item_t *menu_append_item( object_t *menu, list_item_t *parent, image_t *image, char *title )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_only_image( image, "image" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_append( menu, parent, image, title );
}

list_item_t *menu_insert_item( object_t *menu, list_item_t *parent, int pos, image_t *image, char *title )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_only_image( image, "image" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_insert( menu, parent, pos, image, title );
}

list_item_t *menu_append_separator( object_t *menu, list_item_t *parent )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_append( menu, parent, 0, 0 );
}

list_item_t *menu_insert_separator( object_t *menu, list_item_t *parent, int pos )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_only_list_item( parent, "parent" );
	
	return list_widget_row_insert( menu, parent, pos, 0, 0 );
}

void menu_new_item_handle( object_t *obj, event_t *event )
{
	//list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_menu_new_item( WIDGET(obj), item );
}

void menu_move_item( object_t *menu, list_item_t *item, int row )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_move( menu, item, row );
}

void menu_remove_item( object_t *menu, list_item_t *item )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_remove( menu, item );
}

void menu_remove_item_handle( object_t *obj, event_t *event )
{
	//list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_arg_ptr( event, 0 );
	
	cgraphics_menu_remove_item( WIDGET(obj), item );
}

int menu_item_count( object_t *obj, list_item_t *parent )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_menu_widget( obj, "obj" );
	assert_only_list_item( parent, "parent" );
	
	return LIST_LENGTH( &lw->items );
}

void menu_disable_item( object_t *menu, list_item_t *item )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_menu_disable_item( WIDGET(menu), item );
}

void menu_enable_item( object_t *menu, list_item_t *item )
{
	assert_valid_menu_widget( menu, "menu" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_menu_enable_item( WIDGET(menu), item );
}

void menu_popup( object_t *menu, int x, int y, int flags )
{
	assert_valid_menu_widget( menu, "menu" );
	
	cgraphics_menu_popup( WIDGET(menu), x, y, flags );
}
