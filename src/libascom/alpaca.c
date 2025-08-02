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

#include "utils/cJSON_Helper.h"

#include "alpaca.h"

/// Creates a new alpaca response
void alpaca_response_make(AlpacaResponse *response, HttpResponse const *http) {
    cJSON *data = cJSON_ParseWithLength(http->body.base, http->body.length);
    response->result.client_tx_id = (u32) cJSON_GetNumberByName(data, "ClientTransactionID");
    response->result.server_tx_id = (u32) cJSON_GetNumberByName(data, "ServerTransactionID");
    response->result.err_number = (AlpacaError) cJSON_GetNumberByName(data, "ErrorNumber");
    response->result.status = (AlpacaStatus) http->code;
    response->result.ok = response->result.err_number == ALPACA_ERROR_SUCCESSFUL_TX;

    response->value = cJSON_Duplicate(cJSON_GetObjectItem(data, "Value"), true);
    cJSON_Delete(data);
}

/// Creates a failed alpaca response
void alpaca_response_make_failed(AlpacaResponse *response) {
    response->result = (AlpacaResult) { 0 };
    response->result.ok = false;
    response->value = nil;
}

/// Destroys the alpaca result
void alpaca_response_destroy(AlpacaResponse const *result) {
    if (result->value == nil) {
        return;
    }

    cJSON_Delete(result->value);
}
