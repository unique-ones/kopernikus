#include <libcore/display.h>
#include <libcore/ui.h>

int main() {
    Display display;
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 800, 600);

    ui_initialize(&display);

    while (display_running(&display)) {
        ui_begin();
        ui_end();
        display_update_frame(&display);
    }
    ui_destroy();
    display_destroy(&display);
    return 0;
}