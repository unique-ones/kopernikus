//
// Copyright (c) 2024 Elias Engelbert Plank
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

#ifndef KOPERNIKUS_MATH_H
#define KOPERNIKUS_MATH_H

#include "types.h"

/// Creates an identity matrix
/// @param self The matrix handle
void matrix4x4f_create_identity(Matrix4x4f *self);

/// Creates an orthogonal projection matrix
/// @param self The matrix handle
/// @param left The left coordinate of the orthogonal frustum
/// @param right The right coordinate of the orthogonal frustum
/// @param bottom The bottom coordinate of the orthogonal frustum
/// @param top The top coordinate of the orthogonal frustum
void matrix4x4f_create_orthogonal(Matrix4x4f *self, f32 left, f32 right, f32 bottom, f32 top);

#endif// KOPERNIKUS_MATH_H
