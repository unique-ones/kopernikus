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

#include <string.h>

#include <libcore/arch/thread.h>
#include <libcore/log.h>
#include <libascom/http/client.h>
#include <libascom/utils/cJSON.h>
#include <libascom/utils/cJSON_Helper.h>

#include "location.h"

/// Creates a string from a possibly nil cstr
/// @param arena The arena for the allocation
/// @param source The source string that is possibly nil
/// @return A string instance
static String string_from_possibly_nil(MemoryArena *arena, const char* source) {
    if (source == nil) {
        return (String){ 0 };
    }

    return string_new(arena, source, strlen(source));
}

/// Retrieves a string value that might not be present from a JSON object
static String string_from_json(MemoryArena *arena, cJSON *json, const char *key) {
    return string_from_possibly_nil(arena, cJSON_GetStringByName(json, key));
}

/// Retrieves a number that might not be present from a JSON object
static f64 number_from_json(cJSON *json, const char* key) {
    f64 *number = cJSON_GetNumberByName(json, key);
    return number == nil ? 0.0 : *number;
}

/// The thread runner task for the geolocation fetch
/// @param args The thread args
static void *geo_location_fetch_task(void *args) {
    GeoLocation *location = (GeoLocation *) args;

    HttpResponse response = { 0 };
    if (!http_client_get(&response, location->arena, "ip-api.com/json") || response.code != HTTP_OK) {
        return nil;
    }

    cJSON *location_response = cJSON_ParseWithLength(response.body.base, response.body.length);
    location->country = string_from_json(location->arena, location_response, "country");
    location->region = string_from_json(location->arena, location_response, "regionName");
    location->city = string_from_json(location->arena, location_response, "city");
    location->latitude = number_from_json(location_response, "lat");
    location->longitude = number_from_json(location_response, "lon");

    flogf(stderr, "[location] Country: %.*s\n", location->country.length, location->country.base);
    flogf(stderr, "[location] Region: %.*s\n", location->region.length, location->region.base);
    flogf(stderr, "[location] City: %.*s\n", location->city.length, location->city.base);
    flogf(stderr, "[location] Latitude: %lf\n", location->latitude);
    flogf(stderr, "[location] Longitude: %lf\n", location->longitude);
    return location;
}

/// Fetches the users location from the internet
void geo_location_fetch(GeoLocation *location, MemoryArena *arena) {
    location->latitude = 0.0f;
    location->longitude = 0.0f;
    location->city = (String){ 0 };
    location->region = (String){ 0 };
    location->country = (String){ 0 };
    location->arena = arena;
    thread_create(geo_location_fetch_task, location);
}
