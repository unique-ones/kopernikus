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

#include "device.h"

#include <libcore/string.h>
#include <solaris/arena.h>
#include "libascom/alpaca.h"
#include "libascom/http/client.h"
#include "libascom/utils/cJSON.h"
#include "libascom/utils/cJSON_Helper.h"

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
static usize alpaca_device_make_base_url_internal(AlpacaDevice *device,
                                                  StringView *address,
                                                  char *buffer,
                                                  usize length) {
    const char *path = "%.*s/api/v%d/%s/%d";
    const char *type = alpaca_device_type_to_string(device->type);
    return snprintf(buffer, length, path, address->length, address->data, device->api_version, type, device->number);
}

/// Builds the base URL for the device
static void alpaca_device_make_base_url(AlpacaDevice *device, StringView *address) {
    usize length = alpaca_device_make_base_url_internal(device, address, nil, 0);
    device->base_url = string_new_empty(&device->arena, length + 1);
    alpaca_device_make_base_url_internal(device, address, device->base_url.base, device->base_url.length);
}

/// Adds the client headers to the data that gets sent to the device
static void alpaca_device_add_client_headers(AlpacaDevice *device, cJSON *data) {
    cJSON_AddNumberToObject(data, "ClientTransactionID", device->client_tx_id);
    cJSON_AddNumberToObject(data, "ClientID", device->client_id);
}

/// Builds the attribute URL for the device
static usize alpaca_device_make_attribute_url_internal(AlpacaDevice *device,
                                                       const char *attribute,
                                                       char *buffer,
                                                       usize length) {
    const char *path = "%.*s/%s";
    return snprintf(buffer, length, path, device->base_url.length, device->base_url.base, attribute);
}

/// Builds the attribute URL for the device
static void alpaca_device_make_attribute_url(AlpacaDevice *device,
                                             MemoryArena *arena,
                                             String *url,
                                             const char *attribute) {
    usize length = alpaca_device_make_attribute_url_internal(device, attribute, nil, 0);
    *url = string_new_empty(arena, length + 1);
    alpaca_device_make_attribute_url_internal(device, attribute, url->base, url->length);
}

/// Retrieves the result from the provided response
static AlpacaResponse alpaca_device_make_response(String *response) {
    AlpacaResult result = { 0 };

    cJSON *data = cJSON_ParseWithLength(response->base, response->length);
    result.client_tx_id = cJSON_GetNumberByName(data, "ClientTransactionID");
    result.server_tx_id = cJSON_GetNumberByName(data, "ServerTransactionID");
    result.err_number = (AlpacaError) cJSON_GetNumberByName(data, "ErrorNumber");

    AlpacaResponse request_result = { 0 };
    request_result.result = result;
    request_result.value = cJSON_GetObjectItem(data, "Value");
    return request_result;
}

/// Creates a new alpaca device
void alpaca_device_make(AlpacaDevice *device, AlpacaDeviceType type, StringView *address, u32 number) {
    device->arena = memory_arena_identity(ALIGNMENT1);
    device->type = type;
    device->number = number;
    device->api_version = ALPACA_API_VERSION;

    srand(time(nil));
    device->client_id = rand() % (1 << 16);
    device->client_tx_id = 1;

    device->mutex = mutex_new();
    alpaca_device_make_base_url(device, address);
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
    alpaca_device_make_attribute_url(device, arena, &url, attribute);

    // Execute the HTTP request
    HttpResponse response = { 0 };
    if (!http_client_get(&response, arena, url.base)) {
        // If the request fails, we must create some invalid alpaca result
        AlpacaResponse result = { 0 };
        alpaca_response_make(&result, ALPACA_BAD_REQUEST, device->client_tx_id, 0, ALPACA_ERROR_VALUE_NOT_SET, nil);
        mutex_unlock(device->mutex);
        return result;
    }

    AlpacaResponse result = alpaca_device_make_response(&response.body);
    mutex_unlock(device->mutex);
    return result;
}

/// Send an HTTP PUT request to the device
AlpacaResponse alpaca_device_put(AlpacaDevice *device, MemoryArena *arena, const char *attribute, cJSON *data) {
    mutex_lock(device->mutex);
    alpaca_device_add_client_headers(device, data);
    device->client_tx_id++;

    String url = { 0 };
    alpaca_device_make_attribute_url(device, arena, &url, attribute);

    // NOTE(elias): We might want to move to preallocated buffer inside the provided arena
    StringView payload = string_view_from_native(cJSON_PrintUnformatted(data));

    // Execute the HTTP request
    HttpResponse response = { 0 };
    if (!http_client_put(&response, arena, url.base, &payload)) {
        // If the request fails, we must create some invalid alpaca result
        free((void *) payload.data);
        AlpacaResponse result = { 0 };
        alpaca_response_make(&result, ALPACA_BAD_REQUEST, device->client_tx_id, 0, ALPACA_ERROR_VALUE_NOT_SET, nil);
        mutex_unlock(device->mutex);
        return result;
    }

    free((void *) payload.data);
    AlpacaResponse result = alpaca_device_make_response(&response.body);
    mutex_unlock(device->mutex);
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
    *value = cJSON_IsTrue(response.value);
    AlpacaResult result = response.result;
    alpaca_response_destroy(&response);
    return result;
}
