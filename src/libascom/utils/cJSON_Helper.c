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

#include <libcore/types.h>
#include <string.h>

#include "cJSON_Helper.h"

static cJSON *cJSON_GetObjectItemChecked(cJSON const *json, const char *key) {
    if (json == nil) {
        return nil;
    }
    return cJSON_GetObjectItem(json, key);
}

// Retrieves a string by name from a cJSON instance
const char *cJSON_GetNativeStringByName(cJSON const *json, const char *key) {
    cJSON const *item = cJSON_GetObjectItemChecked(json, key);
    if (item == nil) {
        return nil;
    }
    if (!cJSON_IsString(item)) {
        return nil;
    }
    return item->valuestring;
}

// Retrieves a number by name from a cJSON instance
f64 cJSON_GetNumberByName(cJSON const *json, const char *key) {
    cJSON const *item = cJSON_GetObjectItemChecked(json, key);
    if (item == nil) {
        return 0.0;
    }
    if (!cJSON_IsNumber(item)) {
        return 0.0;
    }
    return item->valuedouble;
}

/// Creates a string from a zero terminated string that might be nil
static String string_from_possibly_nil(MemoryArena *arena, const char *source) {
    if (source == nil) {
        return (String) { 0 };
    }

    return string_new(arena, source, (ssize) strlen(source));
}

/// Retrieves a string value that might not be present from a JSON object
String cJSON_GetStringByName(MemoryArena *arena, cJSON const *json, const char *key) {
    return string_from_possibly_nil(arena, cJSON_GetNativeStringByName(json, key));
}
