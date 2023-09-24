#include <libcore/display.h>
#include <libcore/ui/icons.h>
#include <libcore/ui/object-browser.h>
#include <libcore/ui/render.h>

#include <cimgui.h>

int main() {
    Display display;
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 1280, 720);
    ui_initialize(&display);

    ObjectBrowser browser;
    object_browser_make(&browser);

    while (display_running(&display)) {
        ui_begin();

        if (igBeginMainMenuBar()) {
            if (igBeginMenu(ICON_FA_HOUSE " Home", true)) {
                if (igMenuItem_Bool("Exit", "ALT + F4", false, true)) {
                    display_exit(&display);
                }
                igEndMenu();
            }
            if (igBeginMenu(ICON_FA_EYE " View", true)) {
                igEndMenu();
            }
            if (igBeginMenu(ICON_FA_WRENCH " Tools", true)) {
                igEndMenu();
            }
            if (igBeginMenu(ICON_FA_GEARS "Settings", true)) {
                igEndMenu();
            }
            if (igBeginMenu(ICON_FA_CIRCLE_QUESTION " Help", true)) {
                igEndMenu();
            }
            igEndMainMenuBar();
        }

        object_browser_render(&browser);

        ui_end();
        display_update_frame(&display);
    }

    ui_destroy();
    display_destroy(&display);
    return 0;
}