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

#ifndef CORE_STRING_H
#define CORE_STRING_H

#include <solaris/arena.h>

typedef struct StringBuffer {
    char *data;
    ssize size;
} StringBuffer;

typedef struct StringView {
    const char *data;
    ssize length;
} StringView;

/// Creates a nil StringView
StringView string_view_nil(void);

/// Creates a new StringView instance
/// @param str The string
/// @param length The length of the string
/// @return StringView instance
StringView string_view_make(const char *str, ssize length);

/// Creates a new StringView instance from a zero terminated C string
/// @param str The string
/// @return StringView instance
StringView string_view_from_native(const char *str);

/// Checks whether the provided StringViews are equal in terms of
/// length and data
/// @param left
/// @param right
/// @return Boolean that indicates equality
b8 string_view_equal(StringView const *left, StringView const *right);

/// Checks whether the provided hay StringView contains the needle, without case sensitivity
/// @param hay The hay which is searched for the needle
/// @param needle The needle which is searched in the hay
/// @return Boolean that indicates whether the hay contains the needle
b8 string_view_contains(StringView const *hay, StringView const *needle);

/// Creates a substring
/// @param view The view that contains the substring
/// @param offset The offset where the substring should start
/// @param length The length of the substring
/// @return A substring
StringView string_view_substring(StringView const *view, ssize offset, ssize length);

/// Retrieves the leftmost index of the specified symbol in the view
/// @param view The view for the index search
/// @param symbol The symbol to search for
/// @return The index if the symbol was found, -1 if there are zero occurrences
ssize string_view_index_of(StringView const *view, char symbol);

typedef struct String {
    char *base;
    ssize length;
} String;

/// Creates a nil String
String string_nil(void);

/// Creates a new String instance
/// @param arena The arena for the allocation
/// @param data The string data
/// @param length The length of the string
/// @return String instance
///
/// @note Strings are heap allocated, which is the reason
///       why an arena is necessary. For views to constant
///       strings see StringView
String string_new(MemoryArena *arena, const char *data, ssize length);

/// Creates a new empty String instance
/// @param arena The arena for the allocation
/// @param length The length of the string
/// @return String instance
///
/// @note Strings are heap allocated, which is the reason
///       why an arena is necessary. For views to constant
///       strings see StringView
String string_new_empty(MemoryArena *arena, ssize length);

#endif// CORE_STRING_H
