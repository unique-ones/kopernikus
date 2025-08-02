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

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include <libascom/http/client.h>
#include <libcore/types.h>

/// Retrieves a string representation of the specified HTTP response code
/// @param code The HTTP response code
/// @return String representation of the response code
const char* http_response_code_to_string(HttpResponseCode code) {
    switch (code) {
        case HTTP_OK:
            return "OK";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
        case HTTP_NOT_FOUND:
            return "Not Found";
    }
    return "";
}

/// CURL write function for the HttpClient
static size_t http_client_write(char* const buffer, size_t const size, size_t const member_size, void* out_stream) {
    StringBuffer *out = (StringBuffer*) out_stream;
    size_t const curl_length = size * member_size;
    char* tmp = realloc(out->data, out->size + curl_length);
    ASSERT(tmp != nil, "[http] could not allocate write buffer!")
    out->data = tmp;
    memcpy(out->data + out->size, buffer, curl_length);
    out->size += (ssize) curl_length;
    return curl_length;
}

/// CURL read function for the HttpClient
static size_t http_client_read(char* buffer, size_t size, size_t member_size, void* in_stream) {
    StringView *in = (StringView*) in_stream;
    size_t curl_length = size * member_size;
    size_t length = (in->length < curl_length) ? in->length : curl_length;
    memcpy(buffer, in->data, length);
    in->data += length;
    in->length += (ssize) length;
    return length;
}

/// Performs a HTTP GET request and retrieves the response
b8 http_client_get(HttpResponse* response, MemoryArena* arena, const char* url) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_client_write);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, http_client_read);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    StringBuffer header_buffer = { 0 }, body_buffer = { 0 };
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_buffer);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        free(header_buffer.data);
        free(body_buffer.data);
        curl_easy_cleanup(curl);
        return false;
    }

    String header = string_new(arena, header_buffer.data, header_buffer.size);
    free(header_buffer.data);

    String body = string_new(arena, body_buffer.data, body_buffer.size);
    free(body_buffer.data);

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    *response = (HttpResponse) {
        .code = response_code,
        .header = header,
        .body = body
    };
    return true;
}


/// Performs a HTTP PUT request and retrieves the response
b8 http_client_put(HttpResponse* response, MemoryArena* arena, const char* url, StringView* data) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_client_write);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, http_client_read);

    struct curl_slist* headers = nil;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long) data->length);
    curl_easy_setopt(curl, CURLOPT_READDATA, data);

    StringBuffer header_buffer = { 0 }, body_buffer = { 0 };
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_buffer);

    CURLcode result = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    if (result != CURLE_OK) {
        free(header_buffer.data);
        free(body_buffer.data);
        curl_easy_cleanup(curl);
        return false;
    }

    String const header = string_new(arena, header_buffer.data, header_buffer.size);
    free(header_buffer.data);

    String const body = string_new(arena, body_buffer.data, body_buffer.size);
    free(body_buffer.data);

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    *response =  (HttpResponse) {
        .code = response_code,
        .header = header,
        .body = body
    };
    return true;
}
