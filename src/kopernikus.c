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

#include <libascom/device.h>
#include <libascom/http/client.h>
#include <libascom/observing_conditions.h>
#include <libascom/utils/cJSON.h>
#include <libcore/display.h>
#include <libcore/log.h>

#include "browser.h"
#include "gear.h"
#include "sequencer.h"
#include "settings.h"
#include "ui.h"

int main(void) {
    http_client_init();

    Display display = { 0 };
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 1600, 900);
    ui_initialize(&display);

    Settings settings = { 0 };
    settings_make(&settings);

    ObjectBrowser browser = { 0 };
    object_browser_make(&browser, &settings);

    Sequencer sequencer = { 0 };
    sequencer_make(&sequencer, &browser);

    Gear gear = { 0 };
    gear_make(&gear, 1.0f);

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
                ui_note("Devices");
                if (ui_selectable("Properties\t", ICON_FA_BOOK)) {
                    gear.show_properties = true;
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
                ui_text("Copyright " ICON_FA_COPYRIGHT " 2024 Elias Engelbert Plank");
                ui_menu_end();
            }
            ui_main_menu_end();
        }

        object_browser_render(&browser);
        sequencer_render(&sequencer);
        gear_render(&gear);

        ui_end();
        display_update_frame(&display);
    }

    gear_destroy(&gear);
    sequencer_destroy(&sequencer);
    object_browser_destroy(&browser);
    settings_destroy(&settings);

    ui_destroy();
    display_destroy(&display);

    http_client_destroy();
    return 0;
}
