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

#ifndef ASCOM_UTILS_CJSON_HELPER_H
#define ASCOM_UTILS_CJSON_HELPER_H

#include "cJSON.h"

#include <libcore/string.h>
#include <libcore/types.h>

// Retrieves a string by name from a cJSON instance
const char *cJSON_GetNativeStringByName(cJSON const *json, const char *key);

// Retrieves a number by name from a cJSON instance
f64 cJSON_GetNumberByName(cJSON const *json, const char *key);

/// Retrieves a string value that might not be present from a JSON object
String cJSON_GetStringByName(MemoryArena *arena, cJSON const *json, const char *key);

#endif// ASCOM_UTILS_CJSON_HELPER_H
