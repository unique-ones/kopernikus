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

#ifndef CORE_TIMER_H
#define CORE_TIMER_H

#include <time.h>

#include "types.h"

typedef struct Timer {
    clock_t start;
    clock_t end;
} Timer;

/// Creates a new timer
/// @param timer The timer handle
void timer_make(Timer *timer);

/// Starts the timer
/// @param timer The timer handle
void timer_start(Timer *timer);

/// Ends the timer
/// @param timer The timer handle
void timer_end(Timer *timer);

/// Computes the elapsed milliseconds of the timer
/// @param timer The timer handle
/// @return The elapsed milliseconds between start and end
f64 timer_elapsed(Timer *timer);

#endif// CORE_TIMER_H
