#include <libcore/display.h>
#include <libcore/ui.h>
#include <libsolaris/ephemeris/catalog.h>

#include <cimgui.h>

static void ui_draw_elements(const char* id, Elements* elements) {
    if (igBeginTable(id, 2, ImGuiTableFlags_BordersInnerH, (ImVec2){ 0.0f, 0.0f }, 0.0f)) {
        igTableSetupColumn("Element", 0, 0.0f, 0);
        igTableSetupColumn("Value", 0, 0.0f, 0);
        igTableHeadersRow();

        igTableNextColumn();
        igText("Semi Major Axis");
        igTableNextColumn();
        igText("%f", elements->semi_major_axis);

        igTableNextColumn();
        igText("Eccentricity");
        igTableNextColumn();
        igText("%f", elements->eccentricity);

        igTableNextColumn();
        igText("Inclination");
        igTableNextColumn();
        igText("%f", elements->inclination);

        igTableNextColumn();
        igText("Mean Longitude");
        igTableNextColumn();
        igText("%f", elements->mean_longitude);

        igTableNextColumn();
        igText("Longitude Perihelion");
        igTableNextColumn();
        igText("%f", elements->lon_perihelion);

        igTableNextColumn();
        igText("Longitude Ascending Node");
        igTableNextColumn();
        igText("%f", elements->lon_asc_node);

        igEndTable();
    }
}

static void ui_draw_object(const char* id, FixedObject* object) {
    if (igBeginTable(id, 2, ImGuiTableFlags_BordersInnerH, (ImVec2){ 0.0f, 0.0f }, 0.0f)) {
        igTableSetupColumn("Property", 0, 0.0f, 0);
        igTableSetupColumn("Value", 0, 0.0f, 0);
        igTableHeadersRow();

        igTableNextColumn();
        igText("Catalog");
        igTableNextColumn();
        igText("%s", catalog_string(object->designation.catalog));

        igTableNextColumn();
        igText("Index");
        igTableNextColumn();
        igText("%llu", object->designation.index);

        igTableNextColumn();
        igText("Constellation");
        igTableNextColumn();
        igText("%s", constellation_string(object->constellation));

        DateTime now = date_time_now();
        Equatorial position = fixed_object_position(object, &now);
        igTableNextColumn();
        igText("Position");
        igTableNextColumn();
        igText("%f, %f", position.right_ascension, position.declination);

        igTableNextColumn();
        igText("Dimension");
        igTableNextColumn();
        igText("%f", object->dimension);

        igTableNextColumn();
        igText("Magnitude");
        igTableNextColumn();
        igText("%f", object->magnitude);

        igEndTable();
    }
}

int main() {
    Display display;
    display_create(&display, "Kopernikus - Advanced Tracking Sequencer", 800, 600);
    ui_initialize(&display);

    Catalog catalog = catalog_acquire();

    while (display_running(&display)) {
        ui_begin();

        if (igBeginMainMenuBar()) {
            if (igBeginMenu("Home", true)) {
                igEndMenu();
            }
            if(igBeginMenu("View", true)){
                igEndMenu();
            }
            if(igBeginMenu("Tools", true)){
                igEndMenu();
            }
            if(igBeginMenu("Settings", true)){
                igEndMenu();
            }
            if(igBeginMenu("Help", true)){
                igEndMenu();
            }
            igEndMainMenuBar();
        }

        igBegin("Object Browser", nil, ImGuiWindowFlags_None);
        igText("Left Click to Select");
        igText("Right Click to Edit");
        igSeparator();

        DateTime now = date_time_now();
        if (igTreeNode_Str("Planets")) {
            for (usize i = 0; i < catalog.planet_count; ++i) {
                Planet* planet = catalog.planets + i;
                const char* name = planet_name_to_string(planet->name);
                if (igTreeNode_Str(name)) {
                    Elements orbit = planet_position_orbital(planet, &now);
                    ui_draw_elements(name + 1, &orbit);
                    igTreePop();
                }
            }
            igTreePop();
        }
        igEnd();

        ui_end();
        display_update_frame(&display);
    }

    ui_destroy();
    display_destroy(&display);
    return 0;
}