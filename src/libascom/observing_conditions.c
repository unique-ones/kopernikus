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

/// Tries to retrieve the average time period (hours) over which observations are averaged
AlpacaResult alpaca_observing_conds_average_period(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "averageperiod", value);
    device->payload.average_period = *value;
    return result;
}

/// Tries to retrieve the amount of by cloud obscured sky (0.0 to 1.0)
AlpacaResult alpaca_observing_conds_cloud_cover(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "cloudcover", value);
    device->payload.cloud_cover = *value;
    return result;
}

/// Tries to retrieve the atmospheric dew point temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_dew_point(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "dewpoint", value);
    device->payload.dew_point = *value;
    return result;
}

/// Tries to retrieve the atmospheric relative humidity (0 to 100%) at the observatory
AlpacaResult alpaca_observing_conds_humidity(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "humidity", value);
    device->payload.humidity = *value;
    return result;
}

/// Tries to retrieve the atmospheric pressure (hPa) at the observatory
AlpacaResult alpaca_observing_conds_pressure(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "pressure", value);
    device->payload.pressure = *value;
    return result;
}

/// Tries to retrieve the rain rate (mm/hr) at the observatory
AlpacaResult alpaca_observing_conds_rain_rate(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "rainrate", value);
    device->payload.rain_rate = *value;
    return result;
}

/// Tries to retrieve the sky brightness (lux) at the observatory
AlpacaResult alpaca_observing_conds_sky_brightness(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "skybrightness", value);
    device->payload.sky_brightness = *value;
    return result;
}

/// Tries to retrieve the sky quality (mag per sq-arcsec) at the observatory
AlpacaResult alpaca_observing_conds_sky_quality(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "skyquality", value);
    device->payload.sky_quality = *value;
    return result;
}

/// Tries to retrieve the sky temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_sky_temperature(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "skytemperature", value);
    device->payload.sky_temperature = *value;
    return result;
}

/// Tries to retrieve the seeting at the observatory measured as star full width half maximum (arcsec)
AlpacaResult alpaca_observing_conds_star_fwhm(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "starfwhm", value);
    device->payload.star_fwhm = *value;
    return result;
}

/// Tries to retrieve the temperature (°C) at the observatory
AlpacaResult alpaca_observing_conds_temperature(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "temperature", value);
    device->payload.temperature = *value;
    return result;
}

/// Tries to retrieve the wind direction (°) at the observatory
AlpacaResult alpaca_observing_conds_wind_direction(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "winddirection", value);
    device->payload.wind_direction = *value;
    return result;
}

/// Tries to retrieve the peak three second wind gust (m/s) at the observatory over the last two minutes
AlpacaResult alpaca_observing_conds_wind_gust(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "windgust", value);
    device->payload.wind_gust = *value;
    return result;
}

/// Tries to retrieve the wind speed (m/s) at the observatory
AlpacaResult alpaca_observing_conds_wind_speed(AlpacaDevice *device, MemoryArena *arena, f64 *value) {
    AlpacaResult result = alpaca_device_get_f64(device, arena, "windspeed", value);
    device->payload.wind_speed = *value;
    return result;
}
