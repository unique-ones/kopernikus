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

#ifndef KOPERNIKUS_BROWSER_H
#define KOPERNIKUS_BROWSER_H

#include <solaris/catalog.h>
#include "solaris/globe.h"

typedef struct ObjectEntry {
    /// The classification is used to decide which type is stored here.
    /// If the classification is `PLANET`, then one must access the planet
    /// in the union below. For every other type, one can assume that either
    /// the object is the valid handle, or it is nil entirely.
    Classification classification;

    /// The tree index is used to easily track the entry inside the
    /// object browser.
    ssize tree_index;

    /// Reference to the actual object/planet
    union {
        Object *object;
        Planet *planet;
    };
} ObjectEntry;

typedef struct ObjectBrowser {
    /// Catalog of solaris which internally stores all the objects
    Catalog catalog;

    /// Memory arena for all ObjectBrowser allocations
    MemoryArena arena;

    /// GlobeTree for spatial acceleration
    GlobeTree *globe_tree;

    /// Heat map for displaying all the objects
    struct {
        f64 *right_ascensions;
        f64 *declinations;
    } heatmap;

    /// Selected object from the tree
    ObjectEntry selected;

    /// Search buffer for searching the tree
    char search_buffer[128];

    /// This flag controls whether the object browser window is displayed
    b8 show_browser;

    /// This flag controls whether the object properties window is displayed
    b8 show_properties;
} ObjectBrowser;

/// Create a new ObjectBrowser
/// @param browser The browser
void object_browser_make(ObjectBrowser *browser);

/// Destroys the ObjectBrowser
/// @param browser The browser
void object_browser_destroy(ObjectBrowser *browser);

/// Render the ObjectBrowser
/// @param browser The browser
void object_browser_render(ObjectBrowser *browser);

/// Retrieves the ID of object browser paylods
/// @return The ID of object browser payloads
const char *object_browser_payload_id(void);

#endif// KOPERNIKUS_BROWSER_H
