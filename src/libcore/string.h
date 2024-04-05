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

#ifndef KOPERNIKUS_UTILITY_STRING_H
#define KOPERNIKUS_UTILITY_STRING_H

#include <solaris/string.h>

typedef struct StringBuffer {
    char* data;
    ssize size;
} StringBuffer;

typedef struct String {
    char* base;
    ssize length;
} String;

/// Creates a nil String
String string_nil();

/// Creates a new String instance
/// @param arena The arena for the allocation
/// @param data The string data
/// @return String instance
///
/// @note Strings are heap allocated, which is the reason
///       why an arena is necessary. For views to constant
///       strings see StringView
String string_new(MemoryArena* arena, const char* data, ssize length);

#endif// KOPERNIKUS_UTILITY_STRING_H
