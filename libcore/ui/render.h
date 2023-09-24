//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CORE_UI_RENDER_H
#define CORE_UI_RENDER_H

#include "libcore/display.h"

// ===================================================================================
// Generic UI functions
// ===================================================================================

/// Initialize the ui context
/// @param display
void ui_initialize(Display* display);

/// Destroy the ui context
void ui_destroy(void);

/// Begin a new ui draw pass
void ui_begin(void);

/// End the current ui draw pass
void ui_end(void);

// ===================================================================================
// UI window
// ===================================================================================

/// Begin a new UI window
/// @param label Label of the window
/// @return Boolean that indicates whether the window is open
b8 ui_window_begin(const char* label);

/// Ends the current window
void ui_window_end(void);

// ===================================================================================
// UI menu
// ===================================================================================

/// Begin the main menu
/// @return Open state of the main menu
b8 ui_main_menu_begin(void);

/// End the main menu
void ui_main_menu_end(void);

/// Begin a menu
/// @param label Label of the menu
/// @return Open state of the menu
b8 ui_menu_begin(const char* label);

/// End the menu
void ui_menu_end(void);

/// Draw a menu item
/// @param title The title of the menu item
/// @param shortcut The keyboard shortcut for the menu item
/// @return Open state of the menu item
b8 ui_menu_item(const char* title, const char* shortcut);

// ===================================================================================
// UI text
// ===================================================================================

/// Draw text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_text(const char* fmt, ...);

/// Draw wrapped text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_text_wrapped(const char* fmt, ...);

/// Draw a note text
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_note(const char* fmt, ...);

// ===================================================================================
// UI misc
// ===================================================================================

/// Draws a tooltip
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_tooltip(const char* fmt, ...);

/// Draws a tooltip whenever the last item is hovered
/// @param fmt Format string
/// @param ... Variadic arguments
void ui_tooltip_hovered(const char* fmt, ...);

/// Draw a separator
void ui_separator(void);

// ===================================================================================
// UI property
// ===================================================================================

/// Draw a readonly text property
/// @param property The property
/// @param text The text
void ui_property_text_readonly(const char* property, const char* text);

/// Draw a readonly number property
/// @param property The property
/// @param x The number
/// @param fmt The format
void ui_property_number_readonly(const char* property, s64 x, const char* fmt);

/// Draw a readonly real property
/// @param property The property
/// @param x The real
/// @param fmt The format
void ui_property_real_readonly(const char* property, f64 x, const char* fmt);

// ===================================================================================
// UI tree
// ===================================================================================

/// Draw a tree node with an optional icon
/// @param label The label of the tree node
/// @param icon Icon at end of tree node (optional)
/// @param selected The tree node is selected
/// @return Boolean that indicates whether the tree node is open
b8 ui_tree_node_begin(const char* label, const char* icon, b8 selected);

/// End the current tree node
void ui_tree_node_end(void);

/// Draws a tree node item with an optional icon
/// @param label The label of the item
/// @param icon Icon at the end of tree node item (optional)
/// @param selected Draw the item as selected
/// @return Boolean if the item is clicked
b8 ui_tree_item(const char* label, const char* icon, b8 selected);

// ===================================================================================
// UI state
// ===================================================================================

/// Check if the last ui item is selected
/// @return Selection state
b8 ui_selected(void);

/// Checks if the last ui item is hovered
/// @return Hovered state
b8 ui_hovered(void);

#endif// CORE_UI_RENDER_H
