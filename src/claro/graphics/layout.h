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


#ifndef _CLARO_LAYOUT_H
#define _CLARO_LAYOUT_H

/**
 * Contains the information for each cell of a layout.
 * There are no explicit "rows" in a LEL layout, just cells
 * with some flagged as rows.  This makes processing the layout
 * very simple since we only need to rip through an array of pointers
 * to cells.
 *
 * The name for each cell is currently fixed width at 31 characters
 * (32 actually, 31+'\0').  This is done to make memory management
 * simpler and hopefully won't cause too much trouble.  There's
 * an assert if you put too much in.
 */
typedef struct cell_ {
    bounds_t bounds;
    char name[32];
    int ncols;
    
    struct {
        int row:1;
        int fixed_w:1;
        int flex_h:1;
        int set_w:1;
        int set_h:1;
    } flags;
    
} cell_t;

/** This struct is used internally so it's made opaque here.
 * Leave it the hell alone.
 */
struct LayoutHeap;

/**
 * This is the main layout specification.  It contains all the 
 * information necessary to control the parser and to control
 * future reparse calls.  It could be designed to let you add
 * cells later, but right now it's setup to be rigid.  Once
 * you make a layout with different cells in it you can change
 * their sizes around, but if you want to move them you need 
 * to make a new layout.
 *
 * Internally it uses the LayoutHeap structure to do it's
 * by name lookup and processing.  This isn't really a heap
 * but rather a kind of heap/stack hybrid.  It works like a
 * stack during initial parsing.  After this it is "locked"
 * and the widgets are sorted by name using an array heap sort.
 *
 * The heap sort makes it very fast to get a widget by name
 * and still save on memory.  This structure was also very
 * tuned and compared to alternatives.  Not much is faster
 * out there except for a very beefy Judy array.
 */

typedef struct layout_ {
    object_t object;
    
    bounds_t bounds;
    
	lelex_t *lelex;
	
	int min_w, min_h;
} layout_t;

declare_class( layout );
#define layout_type layout_get_type( )

/**
 * Creates an initial layout according to the given specification
 * string, a set of main bounds, and a min height/width parameter.
 * It uses the specification and these bounding properties to construct
 * all of the cells needed to implement the layout.
 *
 * The way layout_create works is that it uses a Ragel FSM to
 * parse the characters in the layout_spec and constructs 
 * appropriate cell_t structs as it goes.  Each character in the
 * layout_spec sets different properties of the cell_t and the act
 * of parsing this creates all the information you need to layout the
 * UI.
 *
 * Calling layout_reparse simply goes back through the layout_t and
 * recalcs the cells based on changes.  It doesn't reparse layout_spec, 
 * and infact the layout_spec isn't needed anymore after the parsing is done.
 *
 * The return value is either a working layout or NULL if there was an error.
 */
CLFEXP layout_t *layout_create(object_t *parent, const char *layout_spec, bounds_t bounds, int min_w, int min_h);

/** Destroys a layout and all of the memory it contains.  This includes all the bounds_t
 * and cell_t structures so if you have pointers to them then you better be careful.
 * It's better to attach this to a widget and let the widget destroy when it is about to
 * leave.  Other widgets use the bounds_t->owner pointer to make sure they don't delete
 * a bounds they don't own.
 */
CLFEXP void layout_destroy(layout_t *lt);

/**
 * Blazing fast way to recalculate the cells in an existing layout.  Once you create
 * the layout it has all the information it needs to recalculate all the cells at any
 * time later.  You can use any of the layout_cell_* functions to adjust a layout, and
 * the bounds you pass to this function are used to redraw the layout for a new size.
 *
 * The layout_reparse function is very fast even with large heinous layouts.  It doesn't
 * reallocate any memory, destroy any memory, copy any memory or do anything more than just
 * traverse the cells and run the calculations.
 */
CLFEXP void layout_reparse(layout_t *lt, bounds_t bounds, int min_w, int min_h);

/**
 * A little different from the other functions since it's so commonly 
 * used.  It just returns the bounds_t with that name from the layout
 * so that you can configure a widget (or widgets if you're doing sublayouts).
 * 
 * There are several asserts protecting you from doing stupid stuff.
 * Primarily if you try to get a cell by name and it doesn't exist then
 * you'll get an assert.  If asserts are disabled and you try this then
 * you'll get a NULL returned.  This will crash the rest of claro so 
 * don't do it.
 */
CLFEXP bounds_t *lt_bounds(layout_t *lt, const char *name);


/** Set the width of the given cell. Call layout_reparse afterwards to see it.  w must be >= 0.*/
CLFEXP void layout_cell_set_w(layout_t *lt, const char *name, int w);

/** Flex the cell's width so that it fills it's portion of the available non-fixed space.
 * Call layout_reparse to see it.
 */
CLFEXP void layout_cell_flex_w(layout_t *lt, const char *name); 

/** Flex the cell's height so that it takes up it's portino of the available non-fixed height.
 * Call layout_reparse to see it.  Currently can be done with any cell but only defined
 * for row cells.
 */
CLFEXP void layout_cell_flex_h(layout_t *lt, const char *name); 

/**
 * Sets the height of the cell to a given height.  Currently you can
 * do this with any cell, but it should really only be done with a row
 * cell.
 */
CLFEXP void layout_cell_set_h(layout_t *lt, const char *name, int w);


#endif

