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

#ifndef ASCOM_DEVICE_H
#define ASCOM_DEVICE_H

#include "alpaca.h"
#include "utils/cJSON.h"

#include <libcore/arch/thread.h>
#include <libcore/string.h>
#include <libcore/types.h>

enum {
    ALPACA_API_VERSION = 1
};

typedef enum AlpacaDeviceType {
    ALPACA_DEVICE_TYPE_NONE = 0,
    ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS,
    ALPACA_DEVICE_TYPE_TELESCOPE,
} AlpacaDeviceType;

typedef struct AlpacaDevice {
    /// One of the recognised ASCOM device types
    AlpacaDeviceType type;

    /// Zero based device number as set on the server
    u32 number;

    /// Alpaca API version
    u32 api_version;

    /// The base url is used for all actions and therefore
    /// stored within the device
    String base_url;

    /// The ID of the client
    u32 client_id;

    /// The transaction ID, gets incremented with every command
    u32 client_tx_id;

    /// Lock in order to enable thread-safety
    Mutex *mutex;

    /// The device arena for internal allocations
    MemoryArena arena;
} AlpacaDevice;

/// Creates a new alpaca device
/// @param device The alpaca device handle
/// @param type One of the recognised ASCOM device types
/// @param address The address of the alpaca server, can also specify port if needed
/// @param Zero based device number as set on the server
void alpaca_device_make(AlpacaDevice *device, AlpacaDeviceType type, StringView *address, u32 number);

/// Destroys the alpaca device
/// @param device The alpaca device handle
void alpaca_device_destroy(AlpacaDevice *device);

/// Send an HTTP GET request to the device
/// @note It is extremely important to know that the response
///       must be destroyed by the caller.
///
/// @param device The alpaca device handle
/// @param arena The arena for the request allocation
/// @param attribute The attribute to get from the server
/// @return A response
AlpacaResponse alpaca_device_get(AlpacaDevice *device, MemoryArena *arena, const char *attribute);

/// Send an HTTP PUT request to the device
/// @note It is extremely important to know that the response
///       must be destroyed by the caller.
///
/// @param device The alpaca device handle
/// @param arena The arena for the request allocation
/// @param attribute The attribute to put to the server
/// @param data The data to send with the request
/// @return A response
AlpacaResponse alpaca_device_put(AlpacaDevice *device, MemoryArena *arena, const char *attribute, cJSON *data);

/// Send an HTTP GET request to the device and retrieve a f64 value
/// @param device The alpaca device handle
/// @param arena The arena for the request allocation
/// @param attribute The attribute to get from the server
/// @return A result
AlpacaResult alpaca_device_get_f64(AlpacaDevice *device, MemoryArena *arena, const char *attribute, f64 *value);

/// Send an HTTP GET request to the device and retrieve a s64 value
/// @param device The alpaca device handle
/// @param arena The arena for the request allocation
/// @param attribute The attribute to get from the server
/// @return A result
AlpacaResult alpaca_device_get_s64(AlpacaDevice *device, MemoryArena *arena, const char *attribute, s64 *value);

/// Send an HTTP GET request to the device and retrieve a boolean value
/// @param device The alpaca device handle
/// @param arena The arena for the request allocation
/// @param attribute The attribute to get from the server
/// @return A result
AlpacaResult alpaca_device_get_bool(AlpacaDevice *device, MemoryArena *arena, const char *attribute, b8 *value);

#endif// ASCOM_DEVICE_H
