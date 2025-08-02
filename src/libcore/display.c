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

#include <ctype.h>
#include <stdio.h>

#include "display.h"

/// Internal singleton instance
static Display *instance = nil;

/// Framebuffer resize callback
static void display_framebuffer_callback(GLFWwindow *handle, s32 width, s32 height) {
    Display *self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->width = (u32) width;
        self->height = (u32) height;
    }
    glViewport(0, 0, width, height);
}

/// Creates a new window and a corresponding OpenGL context
b8 display_create(Display *self, const char *title, u32 width, u32 height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    self->handle = glfwCreateWindow((int) width, (int) height, title, nil, nil);
    // Context is set later on
    self->context = nil;
    self->running = true;
    self->width = width;
    self->height = height;
    self->time = glfwGetTime();

    if (!self->handle) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(self->handle);
    if (!gladLoadGLLoader((GLADloadproc) (glfwGetProcAddress))) {
        glfwDestroyWindow(self->handle);
        glfwTerminate();
    }

    glfwSwapInterval(0);
    glfwSetInputMode(self->handle, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetWindowUserPointer(self->handle, self);
    glfwSetFramebufferSizeCallback(self->handle, display_framebuffer_callback);

    instance = self;
    return true;
}

/// Destroys the window
void display_destroy(Display *self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();
    instance = nil;
}

/// Swaps front and back buffer, polls for events
f64 display_update_frame(Display *self) {
    glfwPollEvents();
    glfwSwapBuffers(self->handle);
    f64 time = glfwGetTime();
    f64 frame_time = time - self->time;
    self->time = time;
    return frame_time;
}

/// Checks if the window should be closed or not
b8 display_running(Display *self) {
    return self->running && !glfwWindowShouldClose(self->handle);
}

/// Tells the display that it should no longer be running
void display_exit(Display *self) {
    self->running = false;
}

/// Retrieves the current display instance
Display *display_instance(void) {
    return instance;
}
