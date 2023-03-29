/* date = March 24th 2023 4:38 pm */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

#include <GLFW/glfw3.h>

struct Display {
    GLFWwindow *handle;
    s32 width;
    s32 height;
    bool running;
    f64 time;
};

/**
 * Creates a new display and a corresponding OpenGL context
 * @param display Display handle
 * @param title Title of the window
 * @return success indication
*/
b32 display_create(Display *display, const char *title);

/**
 * Destroys the specified window
 * @param display Display handle
*/
void display_destroy(Display *display);

f64 display_update(Display *display);

b32 display_running(Display *display);

#endif //DISPLAY_H
