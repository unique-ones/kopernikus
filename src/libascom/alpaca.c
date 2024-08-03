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

#include "alpaca.h"

/// Creates a new alpaca result
void alpaca_result_make(AlpacaResult *result,
                        AlpacaStatus status,
                        u32 client_tx_id,
                        u32 server_tx_id,
                        AlpacaError error,
                        cJSON *value) {
    result->status = status;
    result->client_tx_id = client_tx_id;
    result->server_tx_id = server_tx_id;
    result->err_number = error;
    result->value = value;
}

/// Destroys the alpaca result
void alpaca_result_destroy(AlpacaResult *result) {
    if (result->value == nil) {
        return;
    }

    cJSON_Delete(result->value);
}
