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

#include <stdlib.h>
#include <string.h>

#include <libascom/device.h>
#include <libascom/utils/cJSON.h>
#include <libascom/utils/cJSON_Helper.h>
#include <libascom/utils/url.h>

#include "client.h"

/// Creates a new alpaca client with the given server URL
void alpaca_client_make(AlpacaClient *client, StringView *server) {
    memset(client->server, 0, sizeof client->server);
    memcpy(client->server, server->data, server->length);
    client->id = rand() % (1 << 16);
    client->mutex = mutex_new();
}

/// Destroys the alpaca client
void alpaca_client_destroy(AlpacaClient *client) {
    mutex_free(client->mutex);
    client->id = 0;
    memset(client->server, 0, sizeof client->server);
}

/// Enables a request
static AlpacaResponse alpaca_client_get(AlpacaClient *client, MemoryArena *arena, const char *path) {
    mutex_lock(client->mutex);
    client->tx_id++;

    String url = { 0 };
    StringView base = string_view_from_native(client->server);
    alpaca_make_path_url(&base, arena, &url, path);

    // Execute the HTTP request
    HttpResponse response = { 0 };
    if (!http_client_get(&response, arena, url.base)) {
        // If the request fails, we must create a failed alpaca result
        AlpacaResponse result = { 0 };
        alpaca_response_make_failed(&result);
        mutex_unlock(client->mutex);
        return result;
    }

    AlpacaResponse result = { 0 };
    alpaca_response_make(&result, &response);
    mutex_unlock(client->mutex);
    return result;
}

/// Creates an alpaca device from the specified json
static b8 alpaca_client_device_from_json(AlpacaClient *client, AlpacaDevice *device, MemoryArena *arena, cJSON *json) {
    const char *name_json = cJSON_GetNativeStringByName(json, "DeviceName");
    const char *type_json = cJSON_GetNativeStringByName(json, "DeviceType");
    u32 number = (u32) cJSON_GetNumberByName(json, "DeviceNumber");

    if (!(name_json && type_json)) {
        return false;
    }

    StringView device_type = string_view_from_native(type_json);
    AlpacaDeviceType type = alpaca_device_type_make(&device_type);
    if (type == ALPACA_DEVICE_TYPE_NONE) {
        return false;
    }

    StringView address = string_view_from_native(client->server);
    StringView name = string_view_from_native(name_json);
    alpaca_device_make(device, type, &address, &name, number);

    b8 connected = false;
    if (alpaca_device_connected(device, arena, &connected).ok && connected == false) {
        alpaca_device_update_connected(device, arena, true);
    }

    return true;
}

/// Queries all configured devices from the alpaca client
AlpacaResult alpaca_client_devices(AlpacaClient *client, AlpacaDeviceList *devices) {
    // It is guaranteed that the devices arena only contains the actual devices,
    // therefore it is necessary to create a temporary arena for the request
    MemoryArena request = memory_arena_identity(ALIGNMENT1);
    AlpacaResponse response = alpaca_client_get(client, &request, "management/v1/configureddevices");
    AlpacaResult result = response.result;

    if (!cJSON_IsArray(response.value)) {
        return result;
    }

    usize device_count = cJSON_GetArraySize(response.value);
    if (device_count == 0) {
        return result;
    }

    /// Reserve space for devices
    alpaca_device_list_reserve(devices, device_count);

    cJSON *device_json = nil;
    cJSON_ArrayForEach(device_json, response.value) {
        if (!cJSON_IsObject(device_json)) {
            continue;
        }

        AlpacaDevice device = { 0 };
        if (!alpaca_client_device_from_json(client, &device, &request, device_json)) {
            continue;
        }

        alpaca_device_list_append(devices, &device);
    }

    alpaca_response_destroy(&response);
    memory_arena_destroy(&request);
    return result;
}
