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

#ifndef ASCOM_ALPACA_H
#define ASCOM_ALPACA_H

#include <libascom/http/client.h>
#include <libcore/string.h>

enum {
    ALPACA_SUPPORTED_VERSION = 1,
};

typedef enum AlpacaStatus {
    ALPACA_OK = 200,
    ALPACA_BAD_REQUEST = 400,
    ALPACA_INTERNAL_SERVER_ERROR = 500,
} AlpacaStatus;

typedef enum AlpacaError {
    ALPACA_ERROR_SUCCESSFUL_TX = 0x0,
    ALPACA_ERROR_PROP_OR_METH_NOT_IMPLEMENTED = 0x400,
    ALPACA_ERROR_INVALID_VALUE = 0x401,
    ALPACA_ERROR_VALUE_NOT_SET = 0x402,
    ALPACA_ERROR_NOT_CONNECTED = 0x407,
    ALPACA_ERROR_INVALID_WHILE_PARKED = 0x408,
    ALPACA_ERROR_INVALID_WHILE_SLAVED = 0x409,
    ALPACA_ERROR_INVALID_OPERATION = 0x40B,
    ALPACA_ERROR_ACTION_NOT_IMPLEMENTED = 0x40C,
} AlpacaError;

typedef struct AlpacaResult {
    AlpacaStatus status;
    u32 client_tx_id;
    u32 server_tx_id;
    s32 err_number;
    String err_message;
} AlpacaResult;

typedef struct AlpacaDeviceInfo {
    /// The name of the device
    String device_name;

    /// The type of the device
    String device_type;

    /// The number of the device
    u32 device_number;

    /// The UUID of the device
    String unique_id;
} AlpacaDeviceInfo;

typedef struct AlpacaConn {
    /// The arena of the connection
    MemoryArena arena;

    /// The configured server
    String server;

    /// The name of the configured server
    String server_name;

    /// The name of the manufacturer
    String manufacturer;

    /// The manufacturer version
    String manufacturer_version;

    /// The location of the API
    String location;

    /// The API version
    u32 version;
} AlpacaConn;

/// Creates a new alpaca connection
/// @param conn The connection
void alpaca_conn_make(AlpacaConn *conn);

/// Destroys the specified alpaca connection
/// @param conn The connection
void alpaca_conn_destroy(AlpacaConn *conn);

/// Tries to connect to the specified alpaca server
/// @param conn The connection
/// @param hostname The server hostname
/// @param port The server port
/// @return A boolean value that indicates success
b8 alpaca_conn_connect(AlpacaConn *conn, StringView *hostname, u32 port);

#endif// ASCOM_ALPACA_H
