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

#include <libcore/display.h>
#include <libcore/ui/icons.h>
#include <libcore/ui/object-browser.h>
#include <libcore/ui/render.h>

int main() {
    Display display;
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 1280, 720);
    ui_initialize(&display);

    ObjectBrowser browser;
    object_browser_make(&browser);

    while (display_running(&display)) {
        ui_begin();

        if (ui_main_menu_begin()) {
            if (ui_menu_begin(ICON_FA_HOUSE " Home")) {
                if (ui_menu_item("Exit", "ALT + F4")) {
                    display_exit(&display);
                }
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_EYE " View")) {
                ui_text("Nothing to view at the moment.");
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_WRENCH " Tools")) {
                ui_text("Tools are unavailable.");
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_GEARS "Settings")) {
                ui_text("No configurable settings.");
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_CIRCLE_QUESTION " About")) {
                ui_text("Copyright " ICON_FA_COPYRIGHT " 2023 Elias Engelbert Plank");
                ui_menu_end();
            }
            ui_main_menu_end();
        }

        object_browser_render(&browser);

        ui_end();
        display_update_frame(&display);
    }

    ui_destroy();
    display_destroy(&display);
    return 0;
}