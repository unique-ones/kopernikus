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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "alpaca.h"
#include "utils/cJSON_Helper.h"

/// Creates a new alpaca connection
void alpaca_conn_make(AlpacaConn *conn) {
    conn->arena = memory_arena_identity(ALIGNMENT1);
    conn->server = (String){ 0 };
    conn->server_name = (String){ 0 };
    conn->manufacturer = (String){ 0 };
    conn->manufacturer_version = (String){ 0 };
    conn->location = (String){ 0 };
    conn->version = ALPACA_SUPPORTED_VERSION;
}

/// Destroys the specified alpaca connection
void alpaca_conn_destroy(AlpacaConn *conn) {
    memory_arena_destroy(&conn->arena);
    conn->server = (String){ 0 };
    conn->server_name = (String){ 0 };
    conn->manufacturer = (String){ 0 };
    conn->manufacturer_version = (String){ 0 };
    conn->location = (String){ 0 };
    conn->version = ALPACA_SUPPORTED_VERSION;
}

static u32 digits(u32 x) {
    return (u32) floor(log10((f64) x)) + 1;
}


/// Tries to connect to the specified alpaca server
b8 alpaca_conn_connect(AlpacaConn *conn, StringView *hostname, u32 port) {
    u32 versions_url_length = hostname->length + digits(port) + 32;
    String versions_url = string_new_empty(&conn->arena, versions_url_length);
    sprintf(versions_url.base, "http://%.*s:%d/management/apiversions", (int) hostname->length, hostname->data, port);

    HttpResponse versions_response = { 0 };
    http_client_get(&versions_response, &conn->arena, versions_url.base);

    cJSON *versions_json = cJSON_ParseWithLength(versions_response.body.base, versions_response.body.length);
    cJSON_Delete(versions_json);

    return false;
}