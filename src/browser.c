//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
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

#include <solaris/arena.h>
#include <solaris/globe.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <libcore/input.h>
#include <libcore/log.h>

#include "browser.h"
#include "solaris/math.h"
#include "ui.h"

#include <cimgui.h>
#include <cimplot.h>


/// Create a new ObjectBrowser
void object_browser_make(ObjectBrowser* browser) {
    browser->catalog = catalog_acquire();
    browser->arena = memory_arena_identity(ALIGNMENT8);
    browser->globe_tree = globe_tree_make_root(&browser->arena);
    browser->selected = (ObjectEntry){
        .classification = CLASSIFICATION_COUNT,
        .tree_index = -1,
        .object = nil,
    };
    memset(browser->search_buffer, 0, sizeof browser->search_buffer);
    browser->show_browser = true;
    browser->show_properties = true;

    usize object_count = browser->catalog.object_count;
    browser->heatmap.right_ascensions = (f64*) memory_arena_alloc(&browser->arena, sizeof(f64) * object_count);
    browser->heatmap.declinations = (f64*) memory_arena_alloc(&browser->arena, sizeof(f64) * object_count);

    for (usize i = 0; i < browser->catalog.object_count; ++i) {
        Object* object = browser->catalog.objects + i;
        browser->heatmap.right_ascensions[i] = object->position.right_ascension;
        browser->heatmap.declinations[i] = object->position.declination;
    }
}

/// Destroys the ObjectBrowser
void object_browser_destroy(ObjectBrowser* browser) {
    memory_arena_destroy(&browser->arena);
}

/// Render the catalog map
static void render_catalog_map(ObjectBrowser* browser, bool fill_region) {
    ImVec2 region = { 0 };
    igGetContentRegionAvail(&region);
    if (!fill_region) {
        region.y = region.x / 2;
    }

    static ImPlotRect selection = { { 0.0, 360.0 }, { -90.0, 90.0 } };

    ImPlot_PushColormap_PlotColormap(ImPlotColormap_Plasma);
    if (ImPlot_BeginPlot("##Region", region, 0)) {
        ImPlot_SetupAxes(nil, nil, ImPlotAxisFlags_Foreground, ImPlotAxisFlags_Foreground);
        ImPlot_PlotHistogram2D_doublePtr("Object Density", browser->heatmap.right_ascensions,
                                         browser->heatmap.declinations, (int) browser->catalog.object_count, 100,
                                         100 * region.y / region.x, selection, ImPlotHistogramFlags_Density);
        ImPlot_GetPlotLimits(&selection, -1, -1);
        ImPlot_EndPlot();
    }
    ImPlot_PopColormap(1);
}

/// Render the tree view of the ObjectBrowser
static void object_browser_render_tree(ObjectBrowser* browser) {
    if (!ui_window_begin("Object Browser", &browser->show_browser)) {
        return;
    }

    if (igCollapsingHeader_BoolPtr("Catalog Map", nil, ImGuiTreeNodeFlags_DefaultOpen)) {
        render_catalog_map(browser, false);
    }

    if (igCollapsingHeader_BoolPtr("Objects", nil, ImGuiTreeNodeFlags_DefaultOpen)) {
        StringBuffer buffer = { browser->search_buffer, sizeof browser->search_buffer };
        ui_searchbar(&buffer, "##ObjectBrowserSearch", ICON_FA_MAGNIFYING_GLASS " Search for object...");

        // Check how many bytes are typed into the search bar
        usize search_fill = strlen(buffer.data);

        // We need to keep track of selected objects, for which we will
        // use this tree index
        ssize tree_index = 0;

        // Planet tree
        if (ui_tree_node_begin(ICON_FA_EARTH_EUROPE " Planets", nil, false)) {
            for (usize i = 0; i < browser->catalog.planet_count; ++i) {
                // Fetch the planet from the browser catalog
                Planet* planet = browser->catalog.planets + i;
                const char* name = planet_string(planet->name);

                // Check if search matches
                StringView view_name = string_view_from_native(name);
                StringView view_search = string_view_make(buffer.data, (ssize) search_fill);
                if (search_fill > 0 && !string_view_contains(&view_name, &view_search)) {
                    // If the search does not match, do not draw, however, we still need
                    // to increment the tree index to keep track of entries
                    tree_index += 1;
                    continue;
                }

                // Check if the current planet is selected
                b8 selected = browser->selected.tree_index == tree_index;
                if (ui_tree_item(name, ICON_FA_FLASK, selected)) {
                    browser->selected.tree_index = tree_index;
                    browser->selected.classification = CLASSIFICATION_PLANET;
                    browser->selected.planet = planet;
                }
                tree_index += 1;
            }
            ui_tree_node_end();
        }

        // Object tree
        if (ui_tree_node_begin(ICON_FA_STAR " Objects", nil, false)) {
            for (usize i = 0; i < browser->catalog.object_count; ++i) {
                // Check if the current planet is selected
                b8 selected = browser->selected.tree_index == tree_index;

                // Fetch the object from the browser catalog
                Object* object = browser->catalog.objects + i;

                char object_name[128] = { 0 };
                sprintf(object_name, "%llu (%s)", object->designation.index,
                        catalog_string(object->designation.catalog));

                // Check if search matches
                StringView view_name = string_view_from_native(object_name);
                StringView view_search = string_view_make(buffer.data, (ssize) search_fill);
                if (search_fill > 0 && !string_view_contains(&view_name, &view_search)) {
                    // If the search does not match, do not draw, however, we still need
                    // to increment the tree index to keep track of entries
                    tree_index += 1;
                    continue;
                }

                if (ui_tree_item(object_name, ICON_FA_FLASK, selected)) {
                    browser->selected.tree_index = tree_index;
                    browser->selected.classification = object->classification;
                    browser->selected.object = object;
                }
                tree_index += 1;
            }
            ui_tree_node_end();
        }
    }
    ui_window_end();
}

static void object_browser_render_properties_planet(Planet* planet) {
    if (ui_tree_node_begin(ICON_FA_BOOK " General", nil, false)) {
        ui_note("Designation");
        ui_property_text_readonly("Name", planet_string(planet->name));

        Time now = time_now();
        Elements elements = planet_position_orbital(planet, &now);
        Equatorial position = planet_position_equatorial(planet, &now);

        ui_note("Observation Data (now)");
        ui_property_real_readonly("Ra", position.right_ascension, "%f °");
        ui_tooltip_hovered(
                "Right Ascension (Ra) is the angular distance of a particular point measured eastward along the "
                "celestial equator from the Sun at the March equinox to the point in question above the Earth");

        ui_property_real_readonly("Dec", position.declination, "%f °");
        ui_tooltip_hovered(
                "Declination (Dec) is one of the two angles that locate a point on the celestial sphere in the "
                "equatorial coordinate system, the other being right ascension. Declination's angle is measured north "
                "or south of the celestial equator, along the hour circle passing through the point in question.");

        ui_property_real_readonly("a", elements.semi_major_axis, "%f au");
        ui_tooltip_hovered(
                "The semi-major axis (a) is half of the longest diameter of an elliptical orbit, representing the "
                "average "
                "distance between an object and the central body it orbits.");

        ui_property_real_readonly("e", elements.eccentricity, "%f");
        ui_tooltip_hovered(
                "The eccentricity (e) quantifies how stretched or elongated an elliptical orbit is, ranging from "
                "0 (perfect circle) to 1 (highly elongated).");

        ui_property_real_readonly("I", elements.inclination, "%f °");
        ui_tooltip_hovered(
                "Inclination (I) refers to the angle between the plane of an object's orbit and a reference "
                "plane, typically the plane of the Earth's orbit (the ecliptic). It describes how tilted or inclined "
                "an object's orbital path is relative to the reference plane.");

        ui_property_real_readonly("L", elements.mean_longitude, "%f °");
        ui_tooltip_hovered(
                "The mean longitude (L) represents the average angular position of a celestial object along "
                "its elliptical orbit over time, measured from a reference point, such as the vernal equinox.");

        ui_property_real_readonly("w", elements.lon_perihelion, "%f °");
        ui_tooltip_hovered(
                "The longitude of the perihelion (w) refers to the angular position where an object in an "
                "elliptical orbit is closest to the Sun (perihelion), measured from a reference point. It helps define "
                "the orientation of the object's orbit within the plane of its elliptical path.");

        ui_property_real_readonly("W", elements.lon_asc_node, "%f °");
        ui_tooltip_hovered(
                "The longitude of the ascending node (W) refers to the angle at which a celestial object's "
                "orbit intersects a reference plane, typically the plane of the ecliptic. It defines the point where "
                "the object crosses this plane as it moves from below to above it. ");

        ui_tree_node_end();
    }
    if (ui_tree_node_begin(ICON_FA_GLOBE " GlobeTree Settings", nil, false)) {
        ui_note("Spatial acceleration is unavailable at this point.");
        ui_tree_node_end();
    }
}

static void object_browser_render_properties_object(ObjectBrowser* browser, Object* object) {
    if (ui_tree_node_begin(ICON_FA_BOOK " General", nil, false)) {
        ui_note("Designation");
        ui_property_text_readonly("Catalog", catalog_string(object->designation.catalog));
        ui_property_number_readonly("Index", (s64) object->designation.index, nil);
        ui_property_text_readonly("Type", classification_string(object->classification));
        ui_property_text_readonly("Const", constellation_string(object->constellation));

        Time now = time_now();
        Equatorial position = object_position(object, &now);

        ui_note("Observation Data (now)");
        ui_property_real_readonly("Ra", position.right_ascension, "%f °");
        ui_tooltip_hovered(
                "Right Ascension (Ra) is the angular distance of a particular point measured eastward along the "
                "celestial equator from the Sun at the March equinox to the point in question above the Earth");

        ui_property_real_readonly("Dec", position.declination, "%f °");
        ui_tooltip_hovered(
                "Declination (Dec) is one of the two angles that locate a point on the celestial sphere in the "
                "equatorial coordinate system, the other being right ascension. Declination's angle is measured north "
                "or south of the celestial equator, along the hour circle passing through the point in question.");

        ui_property_real_readonly("m", object->magnitude, "%f");
        ui_tooltip_hovered(
                "Apparent magnitude (m) is a measure of the brightness of a star or other astronomical object.");

        ui_property_real_readonly("dim", object->dimension, "%f '");
        ui_tooltip_hovered(
                "Angular diameter or dimension (dim) is an angular distance describing how large an astronomical "
                "object appears from a given point of view, in this case the Earth.");

        ui_tree_node_end();
    }
    if (ui_tree_node_begin(ICON_FA_GLOBE " GlobeTree Settings", nil, false)) {
        ui_note("Spatial acceleration is unavailable at this point.");
        ui_tree_node_end();
    }
}

/// Render the properties of the selected entry
static void object_browser_render_properties(ObjectBrowser* browser) {
    if (!ui_window_begin("Object Properties", &browser->show_properties)) {
        return;
    }

    // Check if no object is selected, early out if that is the case
    if (browser->selected.tree_index == -1) {
        ui_note("Select any object to see properties");
        ui_window_end();
        return;
    }

    if (browser->selected.classification == CLASSIFICATION_PLANET) {
        object_browser_render_properties_planet(browser->selected.planet);
    } else {
        object_browser_render_properties_object(browser, browser->selected.object);
    }

    ui_window_end();
}

/// Render the ObjectBrowser
void object_browser_render(ObjectBrowser* browser) {
    object_browser_render_tree(browser);
    object_browser_render_properties(browser);
}
