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

#ifndef ASCOM_OBSERVING_CONDITIONS_H
#define ASCOM_OBSERVING_CONDITIONS_H

#include "device.h"

typedef struct ObservingConditions {
    AlpacaDevice device;
} ObservingConditions;

/// Creates a new observing conditions device
/// @param observing_conditions The observing conditions device
/// @param address The address of the alpaca server, can also specify port if needed
/// @param number Zero based device number as set on the server
void observing_conditions_make(ObservingConditions *observing_conditions, StringView *address, u32 number);

/// Destroys the observing conditions device
/// @param observing_conditions The observing conditions device
void observing_conditions_destroy(ObservingConditions *observing_conditions);

/// Tries to retrieve the average time period (hours) over which observations are averaged
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_average_period(ObservingConditions *observing_conditions,
                                                 MemoryArena *arena,
                                                 f64 *value);

/// Tries to retrieve the amount of by cloud obscured sky (0.0 to 1.0)
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_cloud_cover(ObservingConditions *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the atmospheric dew point temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_dew_point(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the atmospheric relative humidity (0 to 100%) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_humidity(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the atmospheric pressure (hPa) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_pressure(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the rain rate (mm/hr) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_rain_rate(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the sky brightness (lux) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_sky_brightness(ObservingConditions *observing_conditions,
                                                 MemoryArena *arena,
                                                 f64 *value);

/// Tries to retrieve the sky quality (mag per sq-arcsec) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_sky_quality(ObservingConditions *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the sky temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_sky_temperature(ObservingConditions *observing_conditions,
                                                  MemoryArena *arena,
                                                  f64 *value);

/// Tries to retrieve the seeting at the observatory measured as star full width half maximum (arcsec)
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_star_fwhm(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_temperature(ObservingConditions *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the wind direction (°) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_wind_direction(ObservingConditions *observing_conditions,
                                                 MemoryArena *arena,
                                                 f64 *value);

/// Tries to retrieve the peak three second wind gust (m/s) at the observatory over the last two minutes
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_wind_gust(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);

/// Tries to retrieve the wind speed (m/s) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult observing_conditions_wind_speed(ObservingConditions *observing_conditions, MemoryArena *arena, f64 *value);


#endif// ASCOM_OBSERVING_CONDITIONS_H
