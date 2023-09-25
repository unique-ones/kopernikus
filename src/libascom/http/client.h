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

#ifndef ASCOM_HTTP_CLIENT_H
#define ASCOM_HTTP_CLIENT_H

#include <libcore/string.h>

typedef enum HttpResponseCode {
    HTTP_OK = 200,
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
} HttpResponseCode;

typedef struct HttpResponse {
    String body;
    String header;
    HttpResponseCode code;
} HttpResponse;

/// Performs a HTTP GET request and retrieves the response
/// @param response The HTTP response (text and code)
/// @param arena The arena for allocating the response string
/// @param client The client which holds the handle
/// @param url The HTTP url for the request
b8 http_client_get(HttpResponse* response, MemoryArena* arena, const char* url);

/// Performs a HTTP PUT request and retrieves the response
/// @param response The HTTP response (text and code)
/// @param arena The arena for allocating the response string
/// @param client The client which holds the handle
/// @param url The HTTP url for the request
/// @param data The data to send
b8 http_client_put(HttpResponse* response, MemoryArena* arena, const char* url, StringView* data);

#endif// ASCOM_HTTP_CLIENT_H
