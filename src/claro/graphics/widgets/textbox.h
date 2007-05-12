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


#ifndef _CLARO_GRAPHICS_WIDGETS_TEXTBOX_H
#define _CLARO_GRAPHICS_WIDGETS_TEXTBOX_H
/** \defgroup textbox_widget_t TextBox Widget
 * \brief The TextBox Widget
 * \warning The API of this widget has \b NOT been finalised!
 * @{
 */

#define assert_only_textbox_widget(o,n)	assert_type_check( o, n, "textbox_widget" )
#define assert_valid_textbox_widget(o,n)	assert_not_null( o, n, "TextBox" ); \
						assert_only_textbox_widget( o, n )

typedef struct textbox_widget_ textbox_widget_t;

#define CLARO_TEXTBOX_MAXIMUM 8192

declare_class( textbox_widget );
#define textbox_widget_type textbox_widget_get_type()

struct textbox_widget_
{
	widget_t widget;
	char text[CLARO_TEXTBOX_MAXIMUM];
};

enum
{
	cTextBoxTypePassword = 1,
};

/* functions */

/**
 * \brief Creates a TextBox widget
 * 
 * \param parent The parent widget of this widget, NOT NULL.
 * \param bounds The initial bounds of this widget, or NO_BOUNDS.
 * \param flags Widget flags.
 * \return A new TextBox widget object.
 */
CLFEXP object_t *textbox_widget_create( object_t *parent, bounds_t *bounds, int flags );

/**
 * \brief Sets the text of a textbox
 * 
 * \param obj A valid TextBox widget
 * \param text The new text
 */
CLFEXP void textbox_set_text( object_t *obj, char *text );

/**
 * \brief Retrieve the text of a textbox
 * 
 * \param obj A valid TextBox widget
 * \return Pointer to an internal reference of the text. Should not be changed.
 */
CLFEXP char *textbox_get_text( object_t *obj );

/**
 * \brief Retrieve the cursor position inside a textbox
 * 
 * \param obj A valid TextBox widget
 * \return Cursor position inside TextBox
 */
CLFEXP int textbox_get_pos( object_t *obj );

/**
 * \brief Sets the cursor position inside a textbox
 * 
 * \param obj A valid TextBox widget
 * \param pos New cursor position inside TextBox
 */
CLFEXP void textbox_set_pos( object_t *obj, int pos );

/*\@}*/
#endif
