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

#ifndef ASCOM_CLIENT_H
#define ASCOM_CLIENT_H

#include <libcore/string.h>
#include <libcore/types.h>

#include "device.h"

/// The alpaca client enables querying of non-device endpoints.
/// It also provides functionality to retrieve all configured
/// devices from the specified alpaca server.
typedef struct AlpacaClient {
    /// The server URL
    char server[256];

    /// The unique client ID
    u32 id;

    /// The client transaction ID
    u32 tx_id;

    /// Lock for synchronisation
    Mutex *mutex;
} AlpacaClient;

/// Creates a new alpaca client with the given server URL
/// @param client The alpaca client handle
/// @param server The alpaca server URL
void alpaca_client_make(AlpacaClient *client, StringView *server);

/// Destroys the alpaca client
/// @param client The alpaca client handle
void alpaca_client_destroy(AlpacaClient *client);

/// Queries all configured devices from the alpaca client
/// @param client The alpaca client
/// @param devices The device list
/// @return A result
AlpacaResult alpaca_client_devices(AlpacaClient *client, AlpacaDeviceList *devices);

#endif// ASCOM_CLIENT_H
