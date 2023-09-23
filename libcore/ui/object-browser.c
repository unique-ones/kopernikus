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

#include "object-browser.h"
#include "icons.h"
#include "render.h"

/// Create a new ObjectBrowser
void object_browser_make(ObjectBrowser* browser) {
    browser->catalog = catalog_acquire();
    browser->selected = nil;
}


/// Render the tree view of the ObjectBrowser
static void object_browser_render_tree(ObjectBrowser* browser) {
    ui_window_begin("Object Browser");

    if (ui_tree_node_begin("Planets", ICON_FA_EARTH_EUROPE)) {
        for (usize i = 0; i < browser->catalog.planet_count; ++i) {
            Planet* planet = browser->catalog.planets + i;
            const char* name = planet_name_to_string(planet->name);
            ui_tree_item(name, ICON_FA_EARTH_EUROPE);
        }
        ui_tree_node_end();
    }
    ui_window_end();
}

/// Render the properties of the selected entry
static void object_browser_render_properties(ObjectBrowser* browser) {

}

/// Render the ObjectBrowser
void object_browser_render(ObjectBrowser* browser) {
    object_browser_render_tree(browser);
    object_browser_render_properties(browser);
}
