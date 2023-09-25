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

#include <libascom/http/client.h>
#include <libcore/types.h>

// CURL write function for the HttpClient
static usize http_client_write(address content, usize size, usize member_size, StringBuffer* buffer) {
    usize curl_length = size * member_size;
    char* tmp = realloc(buffer->data, buffer->size + curl_length);
    ASSERT(tmp != nil, "[http] could not allocate write buffer!")
    buffer->data = tmp;
    memcpy(buffer->data + buffer->size, content, curl_length);
    buffer->size += (ssize) curl_length;
    return curl_length;
}

static usize http_client_read(address content, usize size, usize member_size, StringView* data) {
    usize curl_length = size * member_size;
    usize length = (data->length < curl_length) ? data->length : curl_length;
    memcpy(content, data->data, length);
    data->data += length;
    data->length += (ssize) length;
    return length;
}

/// Performs a HTTP GET request and retrieves the response
b8 http_client_get(HttpResponse* response, MemoryArena* arena, const char* url) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_client_write);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, http_client_read);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

    StringBuffer header_buffer = { 0 }, body_buffer = { 0 };
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_buffer);

    CURLcode  result = curl_easy_perform(curl);
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
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, data->length);
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

    String header = string_new(arena, header_buffer.data, header_buffer.size);
    free(header_buffer.data);

    String body = string_new(arena, body_buffer.data, body_buffer.size);
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
