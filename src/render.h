/* date = March 27th 2023 10:06 pm */

#ifndef KOPERNIKUS_RENDER_H
#define KOPERNIKUS_RENDER_H

#include "display.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <imnodes/imnodes.h>

void render_create_context(Display *display);
void render_destroy_context();

void render_begin_frame();
void render_end_frame();

#endif //KOPERNIKUS_RENDER_H
