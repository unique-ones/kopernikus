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


#include "object-browser.h"
#include "icons.h"
#include "render.h"

#include <stdio.h>

/// Create a new ObjectBrowser
void object_browser_make(ObjectBrowser* browser) {
    browser->catalog = catalog_acquire();
    browser->selected = (ObjectEntry){
        .classification = CLASSIFICATION_COUNT,
        .tree_index = -1,
        .object = nil,
    };
}


/// Render the tree view of the ObjectBrowser
static void object_browser_render_tree(ObjectBrowser* browser) {
    ui_window_begin("Object Browser");

    // We need to keep track of selected objects, for which we will
    // use this tree index
    ssize tree_index = 0;

    // Planet tree
    if (ui_tree_node_begin(ICON_FA_EARTH_EUROPE " Planets", nil, false)) {
        for (usize i = 0; i < browser->catalog.planet_count; ++i) {
            // Check if the current planet is selected
            b8 selected = browser->selected.tree_index == tree_index;

            // Fetch the planet from the browser catalog
            Planet* planet = browser->catalog.planets + i;
            if (ui_tree_item(planet_string(planet->name), ICON_FA_FLASK, selected)) {
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
            FixedObject* object = browser->catalog.objects + i;

            char object_name[128] = { 0 };
            sprintf(object_name, "%llu (%s)", object->designation.index, catalog_string(object->designation.catalog));

            if (ui_tree_item(object_name, ICON_FA_FLASK, selected)) {
                browser->selected.tree_index = tree_index;
                browser->selected.classification = object->classification;
                browser->selected.object = object;
            }
            tree_index += 1;
        }
        ui_tree_node_end();
    }

    ui_window_end();
}

static void object_browser_render_properties_planet(Planet* planet) {
    if (ui_tree_node_begin(ICON_FA_BOOK " General", nil, false)) {
        ui_note("Designation");
        ui_property_text_readonly("Name", planet_string(planet->name));

        DateTime now = date_time_now();
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

static void object_browser_render_properties_object(FixedObject* object) {
    if (ui_tree_node_begin(ICON_FA_BOOK " General", nil, false)) {
        ui_note("Designation");
        ui_property_text_readonly("Catalog", catalog_string(object->designation.catalog));
        ui_property_number_readonly("Index", (s64) object->designation.index, nil);
        ui_property_text_readonly("Type", classification_string(object->classification));
        ui_property_text_readonly("Const", constellation_string(object->constellation));

        DateTime now = date_time_now();
        Equatorial position = fixed_object_position(object, &now);

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
    ui_window_begin("Object Properties");

    // Check if no object is selected, early out if that is the case
    if (browser->selected.tree_index == -1) {
        ui_note("Select any object to see properties");
        ui_window_end();
        return;
    }

    if (browser->selected.classification == CLASSIFICATION_PLANET) {
        object_browser_render_properties_planet(browser->selected.planet);
    } else {
        object_browser_render_properties_object(browser->selected.object);
    }

    ui_window_end();
}

/// Render the ObjectBrowser
void object_browser_render(ObjectBrowser* browser) {
    object_browser_render_tree(browser);
    object_browser_render_properties(browser);
}
