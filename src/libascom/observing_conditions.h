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

typedef struct AlpacaObservingConds {
    AlpacaDevice device;
} AlpacaObservingConds;

/// Creates a new observing conditions device
/// @param observing_conditions The observing conditions device
/// @param address The address of the alpaca server, can also specify port if needed
/// @param number Zero based device number as set on the server
void alpaca_observing_conds_make(AlpacaObservingConds *observing_conditions, StringView *address, u32 number);

/// Destroys the observing conditions device
/// @param observing_conditions The observing conditions device
void alpaca_observing_conds_destroy(AlpacaObservingConds *observing_conditions);

/// Tries to retrieve the average time period (hours) over which observations are averaged
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_average_period(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value);

/// Tries to retrieve the amount of by cloud obscured sky (0.0 to 100.0%)
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_cloud_cover(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value);

/// Tries to retrieve the atmospheric dew point temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_dew_point(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the atmospheric relative humidity (0 to 100%) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_humidity(AlpacaObservingConds *observing_conditions,
                                             MemoryArena *arena,
                                             f64 *value);

/// Tries to retrieve the atmospheric pressure (hPa) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_pressure(AlpacaObservingConds *observing_conditions,
                                             MemoryArena *arena,
                                             f64 *value);

/// Tries to retrieve the rain rate (mm/hr) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_rain_rate(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the sky brightness (lux) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_sky_brightness(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value);

/// Tries to retrieve the sky quality (mag per sq-arcsec) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_sky_quality(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value);

/// Tries to retrieve the sky temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_sky_temperature(AlpacaObservingConds *observing_conditions,
                                                    MemoryArena *arena,
                                                    f64 *value);

/// Tries to retrieve the seeing at the observatory measured as star full width half maximum (arcsec)
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_star_fwhm(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the temperature (°C) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_temperature(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value);

/// Tries to retrieve the wind direction (°) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_wind_direction(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value);

/// Tries to retrieve the peak three second wind gust (m/s) at the observatory over the last two minutes
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_wind_gust(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value);

/// Tries to retrieve the wind speed (m/s) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_wind_speed(AlpacaObservingConds *observing_conditions,
                                               MemoryArena *arena,
                                               f64 *value);


#endif// ASCOM_OBSERVING_CONDITIONS_H
