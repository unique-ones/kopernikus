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

#include <pthread.h>
#include <stdlib.h>

#include <libcore/arch/thread.h>

/// Creates a new thread with the specified runner
Thread thread_create(ThreadRunner runner, void *arg) {
    pthread_t thread;
    pthread_create(&thread, nil, runner, arg);
    return (Thread) thread;
}

typedef struct Mutex {
    pthread_mutex_t handle;
} Mutex;

/// Creates a new mutex
Mutex *mutex_new(void) {
    Mutex *self = (Mutex *) malloc(sizeof(Mutex));
    pthread_mutex_init(&self->handle, nil);
    return self;
}

/// Frees the mutex
void mutex_free(Mutex *self) {
    pthread_mutex_destroy(&self->handle);
    free(self);
}

/// Exclusively locks the mutex
void mutex_lock(Mutex *self) {
    pthread_mutex_lock(&self->handle);
}

/// Unlocks the mutex
void mutex_unlock(Mutex *self) {
    pthread_mutex_unlock(&self->handle);
}