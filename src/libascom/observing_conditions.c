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

#include "observing_conditions.h"

/// Creates a new observing conditions device
void alpaca_observing_conds_make(AlpacaObservingConds *observing_conditions, StringView *address, u32 number) {
    alpaca_device_make(&observing_conditions->device, ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS, address, number);
}

/// Destroys the observing conditions device
void alpaca_observing_conds_destroy(AlpacaObservingConds *observing_conditions) {
    alpaca_device_destroy(&observing_conditions->device);
}

/// Tries to retrieve the average time period (hours) over which observations are averaged
AlpacaResult alpaca_observing_conds_average_period(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "averageperiod", value);
}

/// Tries to retrieve the amount of by cloud obscured sky (0.0 to 1.0)
AlpacaResult alpaca_observing_conds_cloud_cover(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "cloudcover", value);
}

/// Tries to retrieve the atmospheric dew point temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_dew_point(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "dewpoint", value);
}

/// Tries to retrieve the atmospheric relative humidity (0 to 100%) at the observatory
AlpacaResult alpaca_observing_conds_humidity(AlpacaObservingConds *observing_conditions,
                                             MemoryArena *arena,
                                             f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "humidity", value);
}

/// Tries to retrieve the atmospheric pressure (hPa) at the observatory
AlpacaResult alpaca_observing_conds_pressure(AlpacaObservingConds *observing_conditions,
                                             MemoryArena *arena,
                                             f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "pressure", value);
}

/// Tries to retrieve the rain rate (mm/hr) at the observatory
AlpacaResult alpaca_observing_conds_rain_rate(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "rainrate", value);
}

/// Tries to retrieve the sky brightness (lux) at the observatory
AlpacaResult alpaca_observing_conds_sky_brightness(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "skybrightness", value);
}

/// Tries to retrieve the sky quality (mag per sq-arcsec) at the observatory
/// @param observing_conditions The observing conditions device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_observing_conds_sky_quality(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "skyquality", value);
}

/// Tries to retrieve the sky temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_sky_temperature(AlpacaObservingConds *observing_conditions,
                                                    MemoryArena *arena,
                                                    f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "skytemperature", value);
}

/// Tries to retrieve the seeting at the observatory measured as star full width half maximum (arcsec)
AlpacaResult alpaca_observing_conds_star_fwhm(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "starfwhm", value);
}

/// Tries to retrieve the temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_temperature(AlpacaObservingConds *observing_conditions,
                                                MemoryArena *arena,
                                                f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "temperature", value);
}

/// Tries to retrieve the wind direction (°) at the observatory
AlpacaResult alpaca_observing_conds_wind_direction(AlpacaObservingConds *observing_conditions,
                                                   MemoryArena *arena,
                                                   f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "winddirection", value);
}

/// Tries to retrieve the peak three second wind gust (m/s) at the observatory over the last two minutes
AlpacaResult alpaca_observing_conds_wind_gust(AlpacaObservingConds *observing_conditions,
                                              MemoryArena *arena,
                                              f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "windgust", value);
}

/// Tries to retrieve the wind speed (m/s) at the observatory
AlpacaResult alpaca_observing_conds_wind_speed(AlpacaObservingConds *observing_conditions,
                                               MemoryArena *arena,
                                               f64 *value) {
    return alpaca_device_get_f64(&observing_conditions->device, arena, "windspeed", value);
}
