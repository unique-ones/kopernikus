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

#ifndef CORE_UI_OBJECT_BROWSER_H
#define CORE_UI_OBJECT_BROWSER_H

#include <libsolaris/ephemeris/catalog.h>

typedef struct ObjectEntry {
    Classification classification;
    ssize tree_index;
    union {
        FixedObject* object;
        Planet* planet;
    };
} ObjectEntry;

typedef struct ObjectBrowser {
    Catalog catalog;
    ObjectEntry selected;
} ObjectBrowser;

/// Create a new ObjectBrowser
/// @param browser The browser
void object_browser_make(ObjectBrowser* browser);

/// Render the ObjectBrowser
/// @param browser The browser
void object_browser_render(ObjectBrowser* browser);

#endif// CORE_UI_OBJECT_BROWSER_H
