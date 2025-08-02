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
#include <stdlib.h>

#include "alpaca.h"
#include "device.h"
#include "http/client.h"
#include "libascom/utils/cJSON.h"
#include "utils/cJSON_Helper.h"
#include "utils/url.h"

#include <libcore/string.h>
#include <solaris/arena.h>

/// Retrieves the string representation of the provided device type
static const char *alpaca_device_type_to_string(AlpacaDeviceType type) {
    switch (type) {
        case ALPACA_DEVICE_TYPE_TELESCOPE:
            return "telescope";
        case ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS:
            return "observingconditions";
        default:
            break;
    }
    return "";
}

/// Builds the base URL for the device
static usize alpaca_device_make_base_url_internal(AlpacaDevice *device, StringView *address, String *buffer) {
    const char *path = "%.*s/api/v%d/%s/%d";
    const char *type = alpaca_device_type_to_string(device->type);
    return snprintf(buffer->base, buffer->length, path, address->length, address->data, ALPACA_API_VERSION, type,
                    device->number);
}

/// Builds the base URL for the device
static void alpaca_device_make_base_url(AlpacaDevice *device, StringView *address) {
    String empty = { 0 };
    usize length = alpaca_device_make_base_url_internal(device, address, &empty);
    device->base_url = string_new_empty(&device->arena, length + 1);
    alpaca_device_make_base_url_internal(device, address, &device->base_url);
}

/// Adds the client headers to the data that gets sent to the device
static void alpaca_device_add_client_headers(AlpacaDevice *device, cJSON *data) {
    cJSON_AddNumberToObject(data, "ClientTransactionID", device->client_tx_id);
    cJSON_AddNumberToObject(data, "ClientID", device->client_id);
}

/// Retrieves the device type from a string representation
AlpacaDeviceType alpaca_device_type_make(StringView *type) {
    StringView telescope = string_view_from_native("Telescope");
    StringView observing_conds = string_view_from_native("ObservingConditions");
    if (string_view_equal(type, &telescope)) {
        return ALPACA_DEVICE_TYPE_TELESCOPE;
    }
    if (string_view_equal(type, &observing_conds)) {
        return ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS;
    }
    return ALPACA_DEVICE_TYPE_NONE;
}

/// Creates a new alpaca device
void alpaca_device_make(AlpacaDevice *device,
                        AlpacaDeviceType type,
                        StringView *address,
                        StringView *name,
                        u32 number) {
    device->arena = memory_arena_identity(ALIGNMENT1);
    device->type = type;
    device->number = number;

    srand(time(nil));
    device->client_tx_id = 1;

    device->mutex = mutex_new();
    alpaca_device_make_base_url(device, address);
    device->name = string_new(&device->arena, name->data, name->length);
}

/// Destroys the alpaca device
void alpaca_device_destroy(AlpacaDevice *device) {
    memory_arena_destroy(&device->arena);
    device->type = ALPACA_DEVICE_TYPE_NONE;
    device->number = 0;
    device->client_id = 0;
    device->client_tx_id = 0;
    mutex_free(device->mutex);
}

/// Send an HTTP GET request to the device
AlpacaResponse alpaca_device_get(AlpacaDevice *device, MemoryArena *arena, const char *attribute) {
    mutex_lock(device->mutex);
    device->client_tx_id++;

    String url = { 0 };
    StringView base = string_view_make(device->base_url.base, device->base_url.length);
    alpaca_make_path_url(&base, arena, &url, attribute);

    // Execute the HTTP request
    HttpResponse response = { 0 };
    if (!http_client_get(&response, arena, url.base)) {
        // If the request fails, we must create a failed alpaca result
        AlpacaResponse result = { 0 };
        alpaca_response_make_failed(&result);
        mutex_unlock(device->mutex);
        return result;
    }

    AlpacaResponse result = { 0 };
    alpaca_response_make(&result, &response);
    mutex_unlock(device->mutex);
    return result;
}

/// Send an HTTP PUT request to the device
AlpacaResponse alpaca_device_put(AlpacaDevice *device, MemoryArena *arena, const char *attribute, cJSON *data) {
    mutex_lock(device->mutex);
    alpaca_device_add_client_headers(device, data);
    device->client_tx_id++;

    String url = { 0 };
    StringView base = string_view_make(device->base_url.base, device->base_url.length);
    alpaca_make_path_url(&base, arena, &url, attribute);

    // Execute the HTTP request
    HttpResponse response = { 0 };
    if (!http_client_put_form(&response, arena, url.base, data)) {
        // If the request fails, we must create a failed alpaca result
        AlpacaResponse result = { 0 };
        alpaca_response_make_failed(&result);
        mutex_unlock(device->mutex);
        return result;
    }

    // Create result
    AlpacaResponse result = { 0 };
    alpaca_response_make(&result, &response);
    mutex_unlock(device->mutex);
    return result;
}

/// Checks whether the device is connected or not
AlpacaResult alpaca_device_connected(AlpacaDevice *device, MemoryArena *arena, b8 *value) {
    return alpaca_device_get_bool(device, arena, "connected", value);
}

/// Updates the connected attribute of the device
AlpacaResult alpaca_device_update_connected(AlpacaDevice *device, MemoryArena *arena, b8 value) {
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddBoolToObject(payload, "Connected", value);
    AlpacaResponse response = alpaca_device_put(device, arena, "connected", payload);
    cJSON_Delete(payload);
    AlpacaResult result = response.result;
    alpaca_response_destroy(&response);
    return result;
}

/// Send an HTTP GET request to the device and retrieve a f64 value
AlpacaResult alpaca_device_get_f64(AlpacaDevice *device, MemoryArena *arena, const char *attribute, f64 *value) {
    AlpacaResponse response = alpaca_device_get(device, arena, attribute);
    *value = cJSON_GetNumberValue(response.value);
    AlpacaResult result = response.result;
    alpaca_response_destroy(&response);
    return result;
}

/// Send an HTTP GET request to the device and retrieve a s64 value
AlpacaResult alpaca_device_get_s64(AlpacaDevice *device, MemoryArena *arena, const char *attribute, s64 *value) {
    AlpacaResponse response = alpaca_device_get(device, arena, attribute);
    *value = (s64) cJSON_GetNumberValue(response.value);
    AlpacaResult result = response.result;
    alpaca_response_destroy(&response);
    return result;
}

/// Send an HTTP GET request to the device and retrieve a boolean value
AlpacaResult alpaca_device_get_bool(AlpacaDevice *device, MemoryArena *arena, const char *attribute, b8 *value) {
    AlpacaResponse response = alpaca_device_get(device, arena, attribute);
    *value = cJSON_IsBool(response.value) && cJSON_IsTrue(response.value);
    AlpacaResult result = response.result;
    alpaca_response_destroy(&response);
    return result;
}

/// Creates a new alpaca device list
void alpaca_device_list_make(AlpacaDeviceList *list) {
    list->arena = memory_arena_identity(ALIGNMENT1);
}

/// Appends an alpaca device to the list
void alpaca_device_list_append(AlpacaDeviceList *list, AlpacaDevice *device) {
    if (list->count == list->reserved) {
        list->reserved *= 2;
        AlpacaDevice *copied = (AlpacaDevice *) memory_arena_alloc(&list->arena, list->reserved);
        for (usize i = 0; i < list->count; ++i) {
            copied[i] = list->devices[i];
        }
        list->devices = copied;
    }

    list->devices[list->count] = *device;
    list->count++;
}

/// Clears the device list
void alpaca_device_list_clear(AlpacaDeviceList *list) {
    list->count = 0;
    list->reserved = 0;
    list->devices = nil;
    memory_arena_clear(&list->arena);
}

/// Reserves space for the provided amount of devices
void alpaca_device_list_reserve(AlpacaDeviceList *list, usize count) {
    list->devices = (AlpacaDevice *) memory_arena_alloc(&list->arena, sizeof(AlpacaDevice) * count);
}

/// Destroys the alpaca device list and the associated devices
void alpaca_device_list_destroy(AlpacaDeviceList *list) {
    for (usize i = 0; i < list->count; ++i) {
        alpaca_device_destroy(list->devices + i);
    }
    list->devices = nil;
    memory_arena_destroy(&list->arena);
}
