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

#include "input.h"
#include "display.h"

/// Checks if the mouse is currently pressed
b8 input_mouse_pressed(MouseButton button) {
    Display* instance = display_instance();
    return glfwGetMouseButton(instance->handle, button) == GLFW_PRESS;
}

/// Retrieves the current position of the mouse cursor in world coordinates
Vector2f input_mouse_position() {
    Display* instance = display_instance();
    double x, y;
    glfwGetCursorPos(instance->handle, &x, &y);
    return (Vector2f){ .x = (f32) x, .y = (f32) y };
}

/// Checks if the specified key code was pressed
b8 key_pressed(key_code_t code) {
    Display* instance = display_instance();
    return glfwGetKey(instance->handle, code) == GLFW_PRESS;
}
