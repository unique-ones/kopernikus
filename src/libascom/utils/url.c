//
// MIT License
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

#include <stdio.h>

#include "url.h"

/// Builds the path URL for the specified base URL using the provided buffer
static usize alpaca_make_path_url_internal(StringView *base, const char *path, String *buffer) {
    const char *fmt = "%.*s/%s";
    return snprintf(buffer->base, buffer->length, fmt, base->length, base->data, path);
}

/// Builds the path URL for the specified base URL using memory from the provided arena
void alpaca_make_path_url(StringView *base, MemoryArena *arena, String *url, const char *path) {
    String empty = { 0 };
    usize length = alpaca_make_path_url_internal(base, path, &empty);
    *url = string_new_empty(arena, length + 1);
    alpaca_make_path_url_internal(base, path, url);
}
