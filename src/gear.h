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

#ifndef KOPERNIKUS_GEAR_H
#define KOPERNIKUS_GEAR_H

#include <libascom/client.h>
#include <libascom/observing_conditions.h>
#include <libascom/telescope.h>

/// Gear collects data from the alpaca devices
typedef struct Gear {
    /// The alpaca client
    AlpacaClient *client;

    /// The memory arena
    MemoryArena arena;

    /// The devices
    AlpacaDeviceList devices;

    /// The sampling interval
    f64 sampling_interval;

    /// Arena for requests, this gets cleared on every sample
    MemoryArena sample_arena;

    /// Controls whether active sampling should continue
    b8 sample;

    /// Controls whether the device properties are shown
    b8 show_properties;
} Gear;

/// Creates a new gear instance
/// @param gear The gear handle
/// @param sampling_interval The sampling interval in seconds
void gear_make(Gear *gear, f64 sampling_interval);

/// Destroys the gear
/// @param gear The gear handle
void gear_destroy(Gear *gear);

/// Starts a sample thread
/// @param gear The gear handle
void gear_start_sample(Gear *gear);

/// Renders the gear
/// @param gear The gear handle
void gear_render(Gear *gear);

#endif// KOPERNIKUS_GEAR_H
