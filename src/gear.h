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

#include <libascom/observing_conditions.h>
#include <libascom/telescope.h>

/// Telescope specific data
typedef struct TelescopeData {
    f64 altitude;
    f64 azimuth;
} TelescopeData;

/// Observing conditions data
typedef struct ObservingCondsData {
    f64 average_period;
    f64 cloud_cover;
    f64 dew_point;
    f64 humidity;
    f64 pressure;
    f64 rain_rate;
    f64 sky_brightness;
    f64 sky_quality;
    f64 sky_temperature;
    f64 star_fwhm;
    f64 temperature;
    f64 wind_direction;
    f64 wind_gust;
    f64 wind_speed;
} ObservingCondsData;

/// All gear data
typedef struct GearData {
    TelescopeData telescope;
    ObservingCondsData observing_conds;
} GearData;

/// Gear collects data from the alpaca devices
typedef struct Gear {
    /// The telescope device handle
    AlpacaTelescope *telescope;

    /// The observing conditions device handle
    AlpacaObservingConds *observing_conds;

    /// The arena for the devices
    MemoryArena device_arena;

    /// The actual data collected from the different devices
    GearData data;

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
