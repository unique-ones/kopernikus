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

/// Initializes the ui context
/// @param display
void ui_initialize(Display* display);

/// Destroys the ui context
void ui_destroy(void);

/// Begins a new ui draw pass
void ui_begin(void);

/// Ends the current ui draw pass
void ui_end(void);

// ===================================================================================
// UI Widgets
// ===================================================================================

/// Begins a new UI window
/// @param label Label of the window
/// @return Boolean that indicates whether the window is open
b8 ui_window_begin(const char* label);

/// Ends the current window
void ui_window_end(void);

/// Draws a tree node with an optional icon
/// @param label The label of the tree node
/// @param icon Icon at end of tree node (optional)
/// @return Boolean that indicates whether the tree node is open
b8 ui_tree_node_begin(const char* label, const char* icon);

/// Ends the current tree node
void ui_tree_node_end(void);

/// Draws a tree node item with an optional icon
/// @param label The label of the item
/// @param icon Icon at the end of tree node item (optional)
void ui_tree_item(const char* label, const char* icon);

#endif// CORE_UI_RENDER_H
