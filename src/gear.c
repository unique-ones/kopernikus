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

#include <cimgui.h>
#include <libcore/arch/thread.h>
#include <libcore/timer.h>

#include "gear.h"
#include "ui.h"

/// Performs the actual data sample for a telescope device
static void gear_sample_task_perform_telescope(MemoryArena *arena, AlpacaDevice *device) {
    // TODO: handle results
    alpaca_telescope_altitude(device, arena, &device->payload.altitude);
    alpaca_telescope_azimuth(device, arena, &device->payload.azimuth);
}

/// Performs the actual data sample for a observing conditions devie
static void gear_sample_task_perform_observing_conds(MemoryArena *arena, AlpacaDevice *device) {
    // TODO: handle results
    alpaca_observing_conds_average_period(device, arena, &device->payload.average_period);
    alpaca_observing_conds_cloud_cover(device, arena, &device->payload.cloud_cover);
    alpaca_observing_conds_dew_point(device, arena, &device->payload.dew_point);
    alpaca_observing_conds_humidity(device, arena, &device->payload.humidity);
    alpaca_observing_conds_pressure(device, arena, &device->payload.pressure);
    alpaca_observing_conds_rain_rate(device, arena, &device->payload.rain_rate);
    alpaca_observing_conds_sky_brightness(device, arena, &device->payload.sky_brightness);
    alpaca_observing_conds_sky_quality(device, arena, &device->payload.sky_quality);
    alpaca_observing_conds_sky_temperature(device, arena, &device->payload.sky_temperature);
    alpaca_observing_conds_star_fwhm(device, arena, &device->payload.star_fwhm);
    alpaca_observing_conds_temperature(device, arena, &device->payload.temperature);
    alpaca_observing_conds_wind_direction(device, arena, &device->payload.wind_direction);
    alpaca_observing_conds_wind_gust(device, arena, &device->payload.wind_gust);
    alpaca_observing_conds_wind_speed(device, arena, &device->payload.wind_speed);
}

/// Performs the actual data sample
static void gear_sample_task_perform(Gear *gear) {
    MemoryArena *arena = &gear->sample_arena;

    // Telescope sample
    for (usize i = 0; i < gear->devices.count; ++i) {
        AlpacaDevice *device = gear->devices.devices + i;
        switch (device->type) {
            case ALPACA_DEVICE_TYPE_NONE:
                break;
            case ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS:
                gear_sample_task_perform_observing_conds(arena, device);
                break;
            case ALPACA_DEVICE_TYPE_TELESCOPE:
                gear_sample_task_perform_telescope(arena, device);
                break;
            default:
                break;
        }
    }
}

/// The thread runner for the gear sample
static void *gear_sample_task(void *args) {
    Gear *gear = (Gear *) args;

    Timer timer = { 0 };
    timer_make(&timer);

    while (gear->sample) {
        // Start the timer
        timer_start(&timer);

        // Clear the arena before sample
        memory_arena_clear(&gear->sample_arena);
        gear_sample_task_perform(gear);

        // End the timer
        timer_end(&timer);

        // Check whether a sleep is necessary to hit the sampling interval
        f64 const sleep_time = 1000.0 * gear->sampling_interval - timer_elapsed(&timer);
        if (sleep_time > 0.0) {
            thread_sleep((u64) sleep_time);
        }
    }

    return gear;
}

/// Creates a new gear instance
void gear_make(Gear *gear, f64 const sampling_interval) {
    gear->client = nil;
    gear->arena = memory_arena_identity(ALIGNMENT1);
    alpaca_device_list_make(&gear->devices);
    gear->sampling_interval = sampling_interval;
    gear->sample_arena = memory_arena_identity(ALIGNMENT1);
    gear->sample = false;
    gear->show_properties = true;
}

/// Destroys the gear
void gear_destroy(Gear *gear) {
    alpaca_device_list_destroy(&gear->devices);
    if (gear->client) {
        alpaca_client_destroy(gear->client);
    }
    memory_arena_destroy(&gear->sample_arena);
    memory_arena_destroy(&gear->arena);
}

/// Starts a sample thread
void gear_start_sample(Gear *gear) {
    if (gear->sample) {
        return;
    }

    gear->sample = true;
    thread_create(gear_sample_task, gear);
}

/// Connects to the specified alpaca server
static void gear_connect(Gear *gear, StringView *server) {
    gear->client = (AlpacaClient *) memory_arena_alloc(&gear->arena, sizeof(AlpacaClient));
    alpaca_client_make(gear->client, server);
    alpaca_client_devices(gear->client, &gear->devices);
    gear_start_sample(gear);
}

/// Render the device connect prompt
static void gear_render_connect(Gear *gear) {
    ui_note("It seems like you are not connected. Lets fix this by entering the address"
            " of the ASCOM Alpaca server:");

    static char search_buffer[512];
    StringBuffer buffer = { search_buffer, sizeof search_buffer };
    ui_searchbar(&buffer, "##AlpacaServerBar", ICON_FA_GLOBE " Enter address...", false);

    ui_keep_line();

    if (ui_button_light("Connect", true)) {
        StringView server = string_view_make(buffer.data, buffer.size);
        gear_connect(gear, &server);
    }
}

/// Render the device disconnect prompt
static void gear_render_disconnect(Gear *gear) {
    ui_note("Connected to ASCOM Alpaca server '%s'.", gear->client->server);
    if (ui_button("Disconnect", false)) {
        gear->client = nil;
        memory_arena_clear(&gear->arena);
        alpaca_device_list_clear(&gear->devices);
    }
}

/// Render the telescope device properties
static void gear_render_telescope(AlpacaDevice const *device) {
    if (!igCollapsingHeader_BoolPtr("Telescopes " ICON_FA_STAR, nil, ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }

    if (ui_tree_node_begin(device->name.base, nil, false)) {
        if (ui_tree_node_begin(ICON_FA_MAP_PIN " Position", nil, false)) {
            ui_note("Horizontal");
            ui_property_real_readonly("Alt", device->payload.altitude, "%.4f °");
            ui_tooltip_hovered("The mount's current altitude over the horizon");
            ui_property_real_readonly("Az", device->payload.azimuth, "%.4f °");
            ui_tooltip_hovered("The mount's current azimuth");
            ui_tree_node_end();
        }
        ui_tree_node_end();
    }
}

/// Render the observing conditions device properties
static void gear_render_observing_conditions(AlpacaDevice const *device) {
    if (!igCollapsingHeader_BoolPtr("Observatories " ICON_FA_CLOUD_SUN_RAIN, nil, ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }

    if (ui_tree_node_begin(device->name.base, nil, false)) {
        if (ui_tree_node_begin(ICON_FA_SUN " Sky", nil, false)) {
            ui_property_real_readonly("Cloud Cover", device->payload.cloud_cover, "%.4f%%");
            ui_tooltip_hovered("The amount of by cloud obscured sky");
            ui_property_real_readonly("Brightness", device->payload.sky_brightness, "%.4f Lux");
            ui_tooltip_hovered("The sky brightness (Lux) at the observatory");
            ui_property_real_readonly("Quality", device->payload.sky_quality, "%.4f Mag/arcsec^2");
            ui_tooltip_hovered("The sky quality (Mag/arcsec^2) at the observatory");
            ui_property_real_readonly("Temperature", device->payload.sky_temperature, "%.4f °C");
            ui_tooltip_hovered("The sky temperature (°C) at the observatory");
            ui_tree_node_end();
        }
        if (ui_tree_node_begin(ICON_FA_CLOUD_RAIN " Weather", nil, false)) {
            ui_property_real_readonly("Dew Point", device->payload.dew_point, "%.4f °C");
            ui_tooltip_hovered("The atmospheric dew point temperature (°C) at the observatory");
            ui_property_real_readonly("Humidity", device->payload.humidity, "%.4f%%");
            ui_tooltip_hovered("The atmospheric relative humidity at the observatory");
            ui_property_real_readonly("Pressure", device->payload.pressure, "%.4f hPa");
            ui_tooltip_hovered("The atmospheric pressure (hPa) at the observatory");
            ui_property_real_readonly("Rain Rate", device->payload.rain_rate, "%.4f mm/h");
            ui_tooltip_hovered("The hourly rain rate (mm/h) at the observatory");
            ui_property_real_readonly("Temperature", device->payload.temperature, "%.4f °C");
            ui_tooltip_hovered("The temperature (°C) at the observatory");
            ui_tree_node_end();
        }
        if (ui_tree_node_begin(ICON_FA_BINOCULARS " Seeing", nil, false)) {
            ui_property_real_readonly("Star FWHM", device->payload.star_fwhm, "%.4f '");
            ui_tooltip_hovered("The seeing at the observatory measured as star full width half maximum (')");
            ui_tree_node_end();
        }
        if (ui_tree_node_begin(ICON_FA_WIND " Wind", nil, false)) {
            ui_property_real_readonly("Direction", device->payload.wind_direction, "%.4f °");
            ui_tooltip_hovered("The wind direction (°) at the observatory");
            ui_property_real_readonly("Gust", device->payload.wind_gust, "%.4f m/s");
            ui_tooltip_hovered("The peak three second wind gust (m/s) at the observatory over the last two minutes");
            ui_property_real_readonly("Speed", device->payload.wind_speed, "%.4f m/s");
            ui_tooltip_hovered("The wind speed (m/s) at the observatory");
            ui_tree_node_end();
        }
        ui_tree_node_end();
    }
}

/// Render the device
static void gear_render_device(AlpacaDevice const *device) {
    switch (device->type) {
        case ALPACA_DEVICE_TYPE_NONE:
            break;
        case ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS:
            gear_render_observing_conditions(device);
            break;
        case ALPACA_DEVICE_TYPE_TELESCOPE:
            gear_render_telescope(device);
            break;
        default:
            break;
    }
}

/// Render the devices of the gear
static void gear_render_devices(Gear *gear) {
    if (!ui_window_begin("Devices", &gear->show_properties)) {
        return;
    }

    // Connect
    if (gear->client == nil) {
        gear_render_connect(gear);
    } else {
        gear_render_disconnect(gear);
    }

    // Devices
    for (usize i = 0; i < gear->devices.count; ++i) {
        gear_render_device(gear->devices.devices + i);
    }

    ui_window_end();
}

/// Render the gear
void gear_render(Gear *gear) {
    gear_render_devices(gear);
}
