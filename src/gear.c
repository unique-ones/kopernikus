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

/// Performs the actual data sample
static void gear_sample_task_perform(Gear *gear) {
    MemoryArena *arena = &gear->sample_arena;

    // Telescope sample
    TelescopeData *telescope = &gear->data.telescope;
    if (gear->telescope != nil) {
        alpaca_telescope_altitude(gear->telescope, arena, &telescope->altitude);
        alpaca_telescope_azimuth(gear->telescope, arena, &telescope->azimuth);
    }


    // Observing conditions sample
    ObservingCondsData *obs = &gear->data.observing_conds;
    if (gear->observing_conds != nil) {
        alpaca_observing_conds_average_period(gear->observing_conds, arena, &obs->average_period);
        alpaca_observing_conds_cloud_cover(gear->observing_conds, arena, &obs->cloud_cover);
        alpaca_observing_conds_dew_point(gear->observing_conds, arena, &obs->dew_point);
        alpaca_observing_conds_humidity(gear->observing_conds, arena, &obs->humidity);
        alpaca_observing_conds_pressure(gear->observing_conds, arena, &obs->pressure);
        alpaca_observing_conds_rain_rate(gear->observing_conds, arena, &obs->rain_rate);
        alpaca_observing_conds_sky_brightness(gear->observing_conds, arena, &obs->sky_brightness);
        alpaca_observing_conds_sky_quality(gear->observing_conds, arena, &obs->sky_quality);
        alpaca_observing_conds_sky_temperature(gear->observing_conds, arena, &obs->sky_temperature);
        alpaca_observing_conds_star_fwhm(gear->observing_conds, arena, &obs->star_fwhm);
        alpaca_observing_conds_temperature(gear->observing_conds, arena, &obs->temperature);
        alpaca_observing_conds_wind_direction(gear->observing_conds, arena, &obs->wind_direction);
        alpaca_observing_conds_wind_gust(gear->observing_conds, arena, &obs->wind_gust);
        alpaca_observing_conds_wind_speed(gear->observing_conds, arena, &obs->wind_speed);
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
        f64 sleep_time = 1000.0 * gear->sampling_interval - timer_elapsed(&timer);
        if (sleep_time > 0.0) {
            thread_sleep((u64) sleep_time);
        }
    }

    return gear;
}

/// Creates a new gear instance
void gear_make(Gear *gear, f64 sampling_interval) {
    gear->telescope = nil;
    gear->observing_conds = nil;
    gear->device_arena = memory_arena_identity(ALIGNMENT1);
    gear->data = (GearData){ 0 };
    gear->sampling_interval = sampling_interval;
    gear->sample_arena = memory_arena_identity(ALIGNMENT1);
    gear->sample = false;
    gear->show_properties = true;
}

/// Destroys the gear
void gear_destroy(Gear *gear) {
    memory_arena_destroy(&gear->sample_arena);
    if (gear->observing_conds) {
        alpaca_observing_conds_destroy(gear->observing_conds);
        gear->observing_conds = nil;
    }
    if (gear->telescope) {
        alpaca_telescope_destroy(gear->telescope);
        gear->telescope = nil;
    }
}

/// Starts a sample thread
void gear_start_sample(Gear *gear) {
    if (gear->sample) {
        return;
    }

    gear->sample = true;
    thread_create(gear_sample_task, gear);
}

/// Connects to the specified device
static void gear_device_connect(Gear *gear, AlpacaDeviceType type, StringView *address) {
    switch (type) {
        case ALPACA_DEVICE_TYPE_NONE:
            break;
        case ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS: {
            gear->observing_conds =
                    (AlpacaObservingConds *) memory_arena_alloc(&gear->device_arena, sizeof(AlpacaObservingConds));
            alpaca_observing_conds_make(gear->observing_conds, address, 0);
            break;
        }
        case ALPACA_DEVICE_TYPE_TELESCOPE:
            gear->telescope = (AlpacaTelescope *) memory_arena_alloc(&gear->device_arena, sizeof(AlpacaTelescope));
            alpaca_telescope_make(gear->telescope, address, 0);
            break;
        default:
            break;
    }

    gear_start_sample(gear);
}

/// Render the device connect prompt
static void gear_render_device_connect(Gear *gear, AlpacaDeviceType type, StringBuffer *buffer, const char *label) {
    ui_note("It seems like you are not connected. Lets fix this by entering the address"
            " of the alpaca server:");

    ui_searchbar(buffer, label, ICON_FA_GLOBE " Enter address...", false);

    ui_keep_line();

    igPushID_Ptr(label + 1);
    if (ui_button_light("Connect", true)) {
        StringView address = string_view_make(buffer->data, buffer->size);
        gear_device_connect(gear, type, &address);
    }
    igPopID();
}

/// Render the device disconnect prompt
static b8 gear_render_device_disconnect(AlpacaDevice *device) {
    ui_note("Connected to device '%.*s' via alpaca server.", device->base_url.length, device->base_url.base);
    return ui_button("Disconnect", false);
}

/// Render the telescope device properties
static void gear_render_telescope(Gear *gear) {
    if (!igCollapsingHeader_BoolPtr("Telescope " ICON_FA_STAR, nil, ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }

    if (gear->telescope == nil) {
        static char telescope_buffer[512];
        StringBuffer buffer = { telescope_buffer, sizeof telescope_buffer };
        gear_render_device_connect(gear, ALPACA_DEVICE_TYPE_TELESCOPE, &buffer, "##TelescopeAddress");
        return;
    }

    if (gear_render_device_disconnect(&gear->telescope->device)) {
        alpaca_telescope_destroy(gear->telescope);
        gear->telescope = nil;
        return;
    }

    TelescopeData *data = &gear->data.telescope;
    if (ui_tree_node_begin(ICON_FA_MAP_PIN " Position", nil, false)) {
        ui_note("Horizontal");
        ui_property_real_readonly("Alt", data->altitude, "%.4f °");
        ui_tooltip_hovered("The mount's current altitude over the horizon");
        ui_property_real_readonly("Az", data->azimuth, "%.4f °");
        ui_tooltip_hovered("The mount's current azimuth");
        ui_tree_node_end();
    }
}

/// Render the observing conds device properties
static void gear_render_observing_conds(Gear *gear) {
    if (!igCollapsingHeader_BoolPtr("Observing Conditions " ICON_FA_CLOUD_SUN_RAIN, nil,
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }

    if (gear->observing_conds == nil) {
        static char observing_conditions_buffer[512];
        StringBuffer buffer = { observing_conditions_buffer, sizeof observing_conditions_buffer };
        gear_render_device_connect(gear, ALPACA_DEVICE_TYPE_OBSERVING_CONDITIONS, &buffer, "##ObsCondAddress");
        return;
    }

    if (gear_render_device_disconnect(&gear->observing_conds->device)) {
        alpaca_observing_conds_destroy(gear->observing_conds);
        gear->observing_conds = nil;
        return;
    }

    ObservingCondsData *data = &gear->data.observing_conds;
    if (ui_tree_node_begin(ICON_FA_SUN " Sky", nil, false)) {
        ui_property_real_readonly("Cloud Cover", data->cloud_cover, "%.4f%%");
        ui_tooltip_hovered("The amount of by cloud obscured sky");
        ui_property_real_readonly("Brightness", data->sky_brightness, "%.4f Lux");
        ui_tooltip_hovered("The sky brightness (Lux) at the observatory");
        ui_property_real_readonly("Quality", data->sky_quality, "%.4f Mag/arcsec^2");
        ui_tooltip_hovered("The sky quality (Mag/arcsec^2) at the observatory");
        ui_property_real_readonly("Temperature", data->sky_temperature, "%.4f °C");
        ui_tooltip_hovered("The sky temperatur (°C) at the observatory");
        ui_tree_node_end();
    }
    if (ui_tree_node_begin(ICON_FA_CLOUD_RAIN " Weather", nil, false)) {
        ui_property_real_readonly("Dew Point", data->dew_point, "%.4f °C");
        ui_tooltip_hovered("The atmospheric dew point temperature (°C) at the observatory");
        ui_property_real_readonly("Humidity", data->humidity, "%.4f%%");
        ui_tooltip_hovered("The atmoshperic relative humidity at the observatory");
        ui_property_real_readonly("Pressure", data->pressure, "%.4f hPa");
        ui_tooltip_hovered("The atmospheric pressure (hPa) at the observatory");
        ui_property_real_readonly("Rain Rate", data->rain_rate, "%.4f mm/h");
        ui_tooltip_hovered("The hourly rain rate (mm/h) at the observatory");
        ui_property_real_readonly("Temperature", data->temperature, "%.4f °C");
        ui_tooltip_hovered("The temperature (°C) at the observatory");
        ui_tree_node_end();
    }
    if (ui_tree_node_begin(ICON_FA_BINOCULARS " Seeing", nil, false)) {
        ui_property_real_readonly("Star FWHM", data->star_fwhm, "%.4f '");
        ui_tooltip_hovered("The seeing at the observatory measured as star full width half maximum (')");
        ui_tree_node_end();
    }
    if (ui_tree_node_begin(ICON_FA_WIND " Wind", nil, false)) {
        ui_property_real_readonly("Direction", data->wind_direction, "%.4f °");
        ui_tooltip_hovered("The wind direction (°) at the observatory");
        ui_property_real_readonly("Gust", data->wind_gust, "%.4f m/s");
        ui_tooltip_hovered("The peak three second wind gust (m/s) at the observatory over the last two minutes");
        ui_property_real_readonly("Speed", data->wind_speed, "%.4f m/s");
        ui_tooltip_hovered("The wind speed (m/s) at the observatory");
        ui_tree_node_end();
    }
}

/// Render the devices of the gear
static void gear_render_devices(Gear *gear) {
    if (!ui_window_begin("Devices", &gear->show_properties)) {
        return;
    }

    /// Devices
    gear_render_telescope(gear);
    gear_render_observing_conds(gear);

    ui_window_end();
}

/// Render the gear
void gear_render(Gear *gear) {
    gear_render_devices(gear);
}
