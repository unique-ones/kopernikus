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

#include <libascom/http/client.h>
#include <libcore/display.h>
#include <libcore/log.h>

#include "browser.h"
#include "sequencer.h"
#include "ui.h"


int main(void) {
    Display display = { 0 };
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 1600, 900);
    ui_initialize(&display);

    ObjectBrowser browser = { 0 };
    object_browser_make(&browser);

    Sequencer sequencer = { 0 };
    sequencer_make(&sequencer, &browser);

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
                ui_note("Objects");
                if (ui_selectable("Browser", ICON_FA_MAGNIFYING_GLASS)) {
                    browser.show_browser = true;
                }
                if (ui_selectable("Properties\t", ICON_FA_BOOK)) {
                    browser.show_properties = true;
                }
                ui_separator();
                ui_note("Editor");
                if (ui_selectable("Sequencer", ICON_FA_PEN_TO_SQUARE)) {
                    sequencer.show_editor = true;
                }
                if (ui_selectable("Timeline", ICON_FA_BARS_STAGGERED)) {
                    sequencer.show_timeline = true;
                }
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_WRENCH " Tools")) {
                ui_text("Tools are unavailable.");
                ui_menu_end();
            }
            if (ui_menu_begin(ICON_FA_GEARS " Settings")) {
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
        sequencer_render(&sequencer);

        ui_end();
        display_update_frame(&display);
    }

    sequencer_destroy(&sequencer);

    ui_destroy();
    display_destroy(&display);
    return 0;
}
