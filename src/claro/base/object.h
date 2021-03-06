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


#ifndef _CLARO_BASE_OBJECT_H
#define _CLARO_BASE_OBJECT_H

#define return_if_pending_destroy(o) if (OBJECT(o)->destroy_pending==1) return;
#define object_pending_destroy(o) (OBJECT(o)->destroy_pending==1)

CLBVEXP struct class_info_ object_class_info;

typedef struct object_
{
    // this can be removed soon..
	char * type;
	
	const struct class_type_ * class_type;
	
    volatile int ref_count;
    
    int realized;
    // this one too 	
	int destroy_pending;
	
	claro_list_t * event_handlers;

    claro_list_t * children;

	struct object_ * parent;
	
	void * appdata; /* !! this is for APPLICATION USE ONLY !! */
} object_t;

CLFEXP void object_init( );

CLFEXP void object_set_parent( object_t *obj, object_t *parent );

CLFEXP struct class_type_ *object_get_type( );

void object_queue_destruction( object_t *object );

CLFEXP int object_is_realized( object_t *object );

#define object_parent(o) (((object_t *)o)->parent)
#define object_cmptype(o,t) (!strcmp(((object_t *)o)->type,t))
#define OBJECT(o) ((object_t*)o)

#endif
