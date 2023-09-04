#include <libcore/display.h>

int main() {
    Display display;
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 800, 600);
    while (display_running(&display)) {
        display_update_frame(&display);
    }
    display_destroy(&display);
    return 0;
}