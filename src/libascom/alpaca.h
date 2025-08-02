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

#include "utils/cJSON.h"

enum {
    ALPACA_API_VERSION = 1
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
    AlpacaError err_number;
    b8 ok;
} AlpacaResult;

typedef struct AlpacaResponse {
    AlpacaResult result;
    cJSON *value;
} AlpacaResponse;

/// Creates a new alpaca response
/// @param response The response
/// @param http The body
void alpaca_response_make(AlpacaResponse *response, HttpResponse const *http);

/// Creates a failed alpaca response
/// @param response The response
void alpaca_response_make_failed(AlpacaResponse *response);

/// Destroys the alpaca result
/// @param result The alpaca result
void alpaca_response_destroy(AlpacaResponse const *result);

#endif// ASCOM_ALPACA_H
