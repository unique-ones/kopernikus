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

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <glad/glad.h>
#include <solaris/types.h>

typedef f64 Duration;
typedef float f32;

#define ASSERT(x, ...)                \
    if (!(x)) {                       \
        fprintf(stderr, __VA_ARGS__); \
        abort();                      \
    }

typedef struct Vector2s {
    s32 x;
    s32 y;
} Vector2s;

typedef struct Vector3s {
    s32 x;
    s32 y;
    s32 z;
} Vector3s;

typedef struct Vector4s {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vector4s;

typedef struct Vector2f {
    f32 x;
    f32 y;
} Vector2f;

typedef struct Vector3f {
    f32 x;
    f32 y;
    f32 z;
} Vector3f;

typedef struct Vector4f {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vector4f;

typedef struct Matrix4x4f {
    Vector4f value[4];
} Matrix4x4f;

#endif// CORE_TYPES_H
