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
static Display* instance = NULL;

/// Framebuffer resize callback
static void display_framebuffer_callback(GLFWwindow *handle, s32 width, s32 height) {
    Display*self = glfwGetWindowUserPointer(handle);
    if (self) {
        self->width = (u32) width;
        self->height = (u32) height;
    }
    glViewport(0, 0, width, height);
}

/// Converts OpenGL severities to meaningful strings
static const char *severity_string(u32 severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: {
            return "HIGH_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_MEDIUM: {
            return "MEDIUM_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_LOW: {
            return "LOW_SEVERITY";
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION: {
            return "NOTIFICATION_SEVERITY";
        }
        default: {
            return "UNKNOWN_SEVERITY";
        }
    }
}

/// OpenGL error callback function
static void
display_error_callback(u32 source, u32 type, u32 id, u32 severity, s32 length, const char *message, const void *user) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    if (type == GL_DEBUG_TYPE_ERROR) {
        fprintf(stderr, "error: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    } else {
        fprintf(stderr, "warning: %d, severity => %s, message = %s\n", type, severity_string(severity), message);
    }
}

/// Creates a new window and a corresponding OpenGL context
b8 display_create(Display*self, const char *title, u32 width, u32 height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    self->handle = glfwCreateWindow((int) width, (int) height, title, NULL, NULL);
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
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(display_error_callback, NULL);

    instance = self;
    return true;
}

/// Destroys the window
void display_destroy(Display*self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();
    instance = NULL;
}

/// Swaps front and back buffer, polls for events
f64 display_update_frame(Display*self) {
    glfwPollEvents();
    glfwSwapBuffers(self->handle);
    f64 time = glfwGetTime();
    f64 frame_time = time - self->time;
    self->time = time;
    return frame_time;
}

/// Checks if the window should be closed or not
b8 display_running(Display*self) {
    return self->running && !glfwWindowShouldClose(self->handle);
}

/// Retrieves the current display instance
Display* display_instance() {
    return instance;
}
