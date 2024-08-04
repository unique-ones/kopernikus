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

#include "telescope.h"

/// Creates a new telescope device
void alpaca_telescope_make(AlpacaTelescope *telescope, StringView *address, u32 number) {
    alpaca_device_make(&telescope->device, ALPACA_DEVICE_TYPE_TELESCOPE, address, number);
}

/// Destroys the telescope device
void alpaca_telescope_destroy(AlpacaTelescope *telescope) {
    alpaca_device_destroy(&telescope->device);
}

/// Tries to retrieve the mount's current altitude (°) above the horizon
AlpacaResult alpaca_telescope_altitude(AlpacaTelescope *telescope, MemoryArena *arena, f64 *value) {
    return alpaca_device_get_f64(&telescope->device, arena, "altitude", value);
}

/// Tries to retrieve the mount's current azimuth (°)
AlpacaResult alpaca_telescope_azimuth(AlpacaTelescope *telescope, MemoryArena *arena, f64 *value) {
    return alpaca_device_get_f64(&telescope->device, arena, "azimuth", value);
}
