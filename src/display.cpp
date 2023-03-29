#include <stdio.h>

#include "display.h"

static void display_framebuffer_callback(GLFWwindow *handle, s32 width, s32 height) {
    Display *display = (Display *) glfwGetWindowUserPointer(handle);
    display->width = width;
    display->height = height;
    glViewport(0, 0, width, height);
}

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


b32 display_create(Display *display, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    display->handle = glfwCreateWindow(1280, 720, title, NULL, NULL);
    display->running = true;
    display->width = 1280;
    display->height = 720;

    if (!display->handle) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(display->handle);
    if (!gladLoadGLLoader((GLADloadproc) (glfwGetProcAddress))) {
        glfwDestroyWindow(display->handle);
        glfwTerminate();
        return false;
    }

    glfwSwapInterval(0);
    glfwSetWindowUserPointer(display->handle, display);
    glfwSetFramebufferSizeCallback(display->handle, display_framebuffer_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(display_error_callback, NULL);

    return true;
}

void display_destroy(Display *display) {
    glfwDestroyWindow(display->handle);
    glfwTerminate();
}

f64 display_update(Display *display) {
    glfwSwapBuffers(display->handle);
    glfwPollEvents();
    f64 time = glfwGetTime();
    f64 frame_time = time - display->time;
    display->time = time;
    return frame_time;
}

b32 display_running(Display *display) {
    return display->running && !glfwWindowShouldClose(display->handle);
}
