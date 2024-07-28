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

#include "sequencer.h"

#include <assert.h>

#include "browser.h"
#include "skymap.h"
#include "ui.h"

#include <cimgui.h>
#include <cimnodes.h>
#include <cimplot.h>
#include <libcore/gpu.h>
#include <libcore/input.h>
#include <libcore/log.h>
#include <solaris/arena.h>
#include <solaris/object.h>
#include <solaris/planet.h>
#include <solaris/time.h>
#include <solaris/types.h>


static const char *SEQUENCE_NODE_POPUP_ID = "##CreateSequenceNode";
static const f32 SEQUENCE_NODE_WIDTH = 100.0f;
static const f32 TIMELINE_PREVIEW_WIDTH = 180.0f;
static const f32 TIMELINE_PREVIEW_HEIGHT = 90.0f;
static u32 xorshift_state = 1337;

/* The state must be initialized to non-zero */
static u32 xorshift32() {
    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    u32 x = xorshift_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return xorshift_state = x;
}

/// Create a new start sequence node
SequenceNode *sequence_node_make_start(Sequencer *sequencer, SequenceNodeStartData *data) {
    SequenceNode *node = (SequenceNode *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_START;
    node->start = *data;
    node->id = sequencer->node_count + 1;
    node->previous_id = (s32) xorshift32();
    node->next_id = (s32) xorshift32();
    return node;
}

/// Create a new track sequence node
SequenceNode *sequence_node_make_track(Sequencer *sequencer, SequenceNodeTrackData *data) {
    SequenceNode *node = (SequenceNode *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_TRACK;
    node->track = *data;
    node->id = sequencer->node_count + 1;
    node->previous_id = (s32) xorshift32();
    node->next_id = (s32) xorshift32();
    return node;
}

/// Create a new wait sequence node
SequenceNode *sequence_node_make_wait(Sequencer *sequencer, SequenceNodeWaitData *data) {
    SequenceNode *node = (SequenceNode *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_WAIT;
    node->wait = *data;
    node->id = sequencer->node_count + 1;
    node->previous_id = (s32) xorshift32();
    node->next_id = (s32) xorshift32();
    return node;
}

/// Create a new link instance
SequenceLink *sequence_link_make(Sequencer *sequencer, s32 from, s32 to) {
    SequenceLink *link = (SequenceLink *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    link->previous = nil;
    link->next = nil;
    link->from = from;
    link->to = to;
    link->id = 0xFFFF + sequencer->link_count + 1;
    return link;
}

/// Create a new sequencer
void sequencer_make(Sequencer *sequencer, ObjectBrowser *browser) {
    sequencer->node_head = nil;
    sequencer->node_tail = nil;
    sequencer->node_count = 0;
    sequencer->link_head = nil;
    sequencer->link_tail = nil;
    sequencer->link_count = 0;
    sequencer->has_start_node = false;
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
    sequencer->position_arena = memory_arena_identity(ALIGNMENT1);
    sequencer->show_editor = true;
    sequencer->show_timeline = true;
    sequencer->browser = browser;
    renderer_create(&sequencer->renderer, TIMELINE_PREVIEW_WIDTH, TIMELINE_PREVIEW_HEIGHT);
}

/// Destroy the sequencer
void sequencer_destroy(Sequencer *sequencer) {
    // No need to traverse the list and free the nodes individually,
    // since all live inside the arena which we can free.
    sequencer->node_head = nil;
    sequencer->node_tail = nil;
    sequencer->node_count = 0;
    sequencer->link_head = nil;
    sequencer->link_tail = nil;
    sequencer->link_count = 0;
    memory_arena_destroy(&sequencer->position_arena);
    memory_arena_destroy(&sequencer->arena);
    renderer_destroy(&sequencer->renderer);
}

/// Clear the sequencer
void sequencer_clear(Sequencer *sequencer) {
    sequencer->node_head = nil;
    sequencer->node_tail = nil;
    sequencer->node_count = 0;
    sequencer->link_head = nil;
    sequencer->link_tail = nil;
    sequencer->link_count = 0;
    sequencer->has_start_node = false;
    memory_arena_destroy(&sequencer->arena);
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
}

/// Emplace a node into the sequencer
void sequencer_emplace_node(Sequencer *sequencer, SequenceNode *node) {
    if (node->type == SEQUENCE_NODE_START) {
        sequencer->has_start_node = true;
    }

    if (sequencer->node_head == nil) {
        sequencer->node_head = node;
        sequencer->node_tail = node;
    } else {
        sequencer->node_tail->next = node;
        node->previous = sequencer->node_tail;
        sequencer->node_tail = node;
    }
    sequencer->node_count++;
}

/// Remove a node by its ID
void sequencer_remove_node(Sequencer *sequencer, s32 node_id) {
    for (SequenceNode *it = sequencer->node_head; it != nil; it = it->next) {
        if (it->id == node_id) {
            if (it->type == SEQUENCE_NODE_START) {
                sequencer->has_start_node = false;
            }

            if (it->previous != nil) {
                it->previous->next = it->next;
            } else {
                sequencer->node_head = it->next;
            }

            if (it->next != nil) {
                it->next->previous = it->previous;
            } else {
                sequencer->node_tail = it->previous;
            }

            sequencer_remove_link_by_node(sequencer, node_id);
            return;
        }
    }
}

/// Emplace a link into the sequencer link list
void sequencer_emplace_link(Sequencer *sequencer, SequenceLink *link) {
    if (sequencer->link_head == nil) {
        sequencer->link_head = link;
        sequencer->link_tail = link;
    } else {
        sequencer->link_tail->next = link;
        link->previous = sequencer->link_tail;
        sequencer->link_tail = link;
    }
    sequencer->link_count++;
}

/// Remove a link by its ID
void sequencer_remove_link(Sequencer *sequencer, s32 link_id) {
    for (SequenceLink *it = sequencer->link_head; it != nil; it = it->next) {
        if (it->id == link_id) {
            if (it->previous != nil) {
                it->previous->next = it->next;
            } else {
                sequencer->link_head = it->next;
            }

            if (it->next != nil) {
                it->next->previous = it->previous;
            } else {
                sequencer->link_tail = it->previous;
            }
            return;
        }
    }
}

/// Remove a link by a node ID
void sequencer_remove_link_by_node(Sequencer *sequencer, s32 node_id) {
    for (SequenceLink *it = sequencer->link_head; it != nil; it = it->next) {
        if (it->from == node_id || it->to == node_id) {
            if (it->previous != nil) {
                it->previous->next = it->next;
            } else {
                sequencer->link_head = it->next;
            }

            if (it->next != nil) {
                it->next->previous = it->previous;
            } else {
                sequencer->link_tail = it->previous;
            }
        }
    }
}

/// Draw common properties of a node
static void sequencer_render_node_time_data(Duration *duration) {
    ui_property_real("Duration", &duration->amount, "%.4f");

    static const char *UNITS[] = { "Seconds", "Minutes", "Hours", "Days", "Months", "Years" };

    s32 unit = duration->unit;
    ui_combobox("Unit", &unit, UNITS, ARRAY_SIZE(UNITS));
    duration->unit = unit;
}

/// Draw a start node
static void sequencer_render_node_start(SequenceNode *node, f32 width) {
    imnodes_BeginNodeTitleBar();
    ui_text(ICON_FA_PLAY " Start");
    imnodes_EndNodeTitleBar();

    imnodes_BeginOutputAttribute(node->next_id, ImNodesPinShape_Circle);
    ui_text("Next");
    imnodes_EndOutputAttribute();

    bool use_current_time = node->start.now;
    igCheckbox("Use current time", &use_current_time);
    node->start.now = use_current_time;

    Time now = time_now();
    if (use_current_time) {
        node->start.time = now;
        return;
    }

    b8 date_changed = false;
    Time validation = node->start.time;

    ImGuiStyle *style = igGetStyle();
    ImVec2 item_spacing = style->ItemSpacing;
    ui_item_width_begin(width / 2.0f - style->ItemSpacing.x);
    igPushStyleVar_Vec2(ImGuiStyleVar_ItemSpacing, (const ImVec2) { .x = 0, .y = item_spacing.y });
    date_changed |= ui_property_number("##Day", &validation.day, "%d");
    ui_keep_line();
    ui_note(".");
    ui_keep_line();
    date_changed |= ui_property_number("##Month", &validation.month, "%d");
    ui_keep_line();
    ui_note(".");
    ui_keep_line();
    date_changed |= ui_property_number("##Year", &validation.year, "%d");

    igPushStyleVar_Vec2(ImGuiStyleVar_ItemSpacing, (const ImVec2) { .x = item_spacing.x, .y = item_spacing.y });
    ui_keep_line();
    ui_text("Date");
    igPopStyleVar(1);

    date_changed |= ui_property_number("##Hour", &validation.hour, "%d");
    ui_keep_line();
    ui_note(":");
    ui_keep_line();
    date_changed |= ui_property_number("##Minute", &validation.minute, "%d");
    ui_keep_line();
    ui_note(":");
    ui_keep_line();
    date_changed |= ui_property_number("##Second", &validation.second, "%d");
    igPopStyleVar(1);
    ui_keep_line();
    ui_text("Time");
    ui_item_width_end();

    if (date_changed && time_valid(&validation)) {
        node->start.time = validation;
    }
    if (time_lt(&node->start.time, &now)) {
        node->start.time = now;
    }
}

/// Draw a track node
static void sequencer_render_node_track(SequenceNode *node, f32 width) {
    imnodes_BeginNodeTitleBar();
    ui_text(ICON_FA_CROSSHAIRS " Track");
    imnodes_EndNodeTitleBar();

    imnodes_BeginInputAttribute(node->previous_id, ImNodesPinShape_Circle);
    ui_text("Previous\t");
    imnodes_EndInputAttribute();
    ui_keep_line();
    imnodes_BeginOutputAttribute(node->next_id, ImNodesPinShape_Circle);
    ui_text("Next");
    imnodes_EndOutputAttribute();

    ui_item_width_begin(width);
    sequencer_render_node_time_data(&node->track.duration);

    ObjectEntry *entry = &node->track.object;
    if (entry->classification == CLASSIFICATION_PLANET) {
        ui_property_text_readonly("Object", planet_string(entry->planet->name));
    } else {
        Object *object = entry->object;

        char object_name[128] = { 0 };
        sprintf(object_name, "%" PRIu64 " (%s)", object->designation.index,
                catalog_string(object->designation.catalog));
        ui_property_text_readonly("Object", object_name);
    }
    ui_item_width_end();
}

/// Draw a wait node
static void sequencer_render_node_wait(SequenceNode *node, f32 width) {
    imnodes_BeginNodeTitleBar();
    ui_text(ICON_FA_CLOCK " Wait");
    imnodes_EndNodeTitleBar();

    imnodes_BeginInputAttribute(node->previous_id, ImNodesPinShape_Circle);
    ui_text("Previous\t");
    imnodes_EndInputAttribute();
    ui_keep_line();
    imnodes_BeginOutputAttribute(node->next_id, ImNodesPinShape_Circle);
    ui_text("Next");
    imnodes_EndOutputAttribute();

    ui_item_width_begin(width);
    sequencer_render_node_time_data(&node->wait.duration);
    ui_item_width_end();
}

/// Draw a node
static void sequencer_render_node(Sequencer *sequencer, SequenceNode *node, f32 width) {
    ui_node_begin(node->id);
    switch (node->type) {
        case SEQUENCE_NODE_START:
            sequencer_render_node_start(node, width);
            break;
        case SEQUENCE_NODE_TRACK: {
            sequencer_render_node_track(node, width);
        } break;
        case SEQUENCE_NODE_WAIT:
            sequencer_render_node_wait(node, width);
            break;
        default:
            break;
    }
    ui_node_end();

    if (node->type == SEQUENCE_NODE_TRACK && igBeginDragDropTarget()) {
        const ImGuiPayload *payload = igAcceptDragDropPayload(object_browser_payload_id(), ImGuiDragDropFlags_None);
        if (payload != nil && payload->DataSize == sizeof(ObjectEntry)) {
            node->track.object = sequencer->browser->selected;
        }
        igEndDragDropTarget();
    }
}

/// Draw all the nodes
static void sequencer_render_nodes(Sequencer *sequencer) {
    for (SequenceNode *it = sequencer->node_head; it != nil; it = it->next) {
        sequencer_render_node(sequencer, it, SEQUENCE_NODE_WIDTH);
    }
    for (SequenceLink *it = sequencer->link_head; it != nil; it = it->next) {
        imnodes_Link(it->id, it->from, it->to);
    }
}

static SequenceNode *sequencer_find_node_by_type(Sequencer *sequencer, SequenceNodeType type) {
    for (SequenceNode *it = sequencer->node_head; it != nil; it = it->next) {
        if (it->type == type) {
            return it;
        }
    }
    return nil;
}

typedef struct SequenceNodeList {
    MemoryArena arena;
    SequenceNode **nodes;
    usize reserved;
    usize count;
} SequenceNodeList;

/// Reserves space inside the sequence node list
static void sequence_node_list_reserve(SequenceNodeList *list, usize count) {
    if (count <= list->reserved) {
        return;
    }

    SequenceNode **temp = (SequenceNode **) memory_arena_alloc(&list->arena, sizeof(SequenceNode *) * count);
    for (usize i = 0; i < list->count; ++i) {
        temp[i] = list->nodes[i];
    }

    list->nodes = temp;
    list->reserved = count;
}

/// Creates a new sequence node list
static void sequence_node_list_make(SequenceNodeList *list) {
    list->arena = memory_arena_identity(ALIGNMENT1);
    list->nodes = nil;
    list->reserved = 0;
    list->count = 0;

    sequence_node_list_reserve(list, 8);
}

/// Destroys the sequence node list
static void sequence_node_list_destroy(SequenceNodeList *list) {
    memory_arena_destroy(&list->arena);
    list->nodes = nil;
    list->count = 0;
    list->reserved = 0;
}

/// Adds to the sequence node list
static void sequence_node_list_add(SequenceNodeList *list, SequenceNode *node) {
    if (list->count == list->reserved) {
        sequence_node_list_reserve(list, list->reserved * 2);
    }

    list->nodes[list->count] = node;
    list->count++;
}

void sequencer_format_date_time(StringBuffer *buffer, Time *time) {
    time_t stamp = time_unix(time);
    struct tm *time_info = localtime(&stamp);
    strftime(buffer->data, buffer->size, "%d.%m.%Y - %H:%M:%S", time_info);// Format the time as a string
}

/// Retrieves the unit conversion factor
static f64 unit_conversion_factor(TimeUnit from, TimeUnit to) {
    // Conversion factors to seconds for each unit
    static const f64 seconds_per_unit[] = {
        1.0,      // UNIT_SECONDS
        60.0,     // UNIT_MINUTES
        3600.0,   // UNIT_HOURS
        86400.0,  // UNIT_DAYS
        2629746.0,// UNIT_MONTHS (average month in seconds)
        31556952.0// UNIT_YEARS (average year in seconds)
    };

    // Conversion factor from 'from' unit to 'to' unit
    return seconds_per_unit[from] / seconds_per_unit[to];
}

static void sequencer_render_timeline_node_track(Sequencer *sequencer, SequenceNodeTrackData *data, Time *start) {
    ImVec2 inner_spacing = igGetStyle()->ItemInnerSpacing;
    ui_draw_cursor_advance(inner_spacing.x, inner_spacing.y);
    ui_note("Tracking");
    ui_draw_cursor_advance(inner_spacing.x, 0);

    ImVec2 available = { 0 };
    igGetContentRegionAvail(&available);

    if (!igBeginTableEx("", igGetID_Ptr(data), 3, ImGuiTableFlags_RowBg, (ImVec2) { available.x - inner_spacing.x, 0 },
                        0)) {
        return;
    }

    igTableSetupColumn("Timing", ImGuiTableColumnFlags_None, 0, 0);
    igTableSetupColumn("Target", ImGuiTableColumnFlags_None, 0, 0);
    igTableSetupColumn("Position", ImGuiTableColumnFlags_None, 0, 0);
    igTableHeadersRow();

    igTableNextRow(ImGuiTableRowFlags_None, 0);
    igTableNextColumn();

    char start_time_buffer[52] = { 0 };
    sequencer_format_date_time(&(StringBuffer) { start_time_buffer, sizeof start_time_buffer }, start);
    ui_property_text_readonly("Start", start_time_buffer);

    Time end = *start;
    time_add(&end, data->duration.amount, data->duration.unit);

    char end_time_buffer[52] = { 0 };
    sequencer_format_date_time(&(StringBuffer) { end_time_buffer, sizeof end_time_buffer }, &end);
    ui_property_text_readonly("End", end_time_buffer);
    sequencer_render_node_time_data(&data->duration);
    igTableNextColumn();

    switch (data->object.classification) {
        case CLASSIFICATION_PLANET:
            ui_property_text_readonly("Name", planet_string(data->object.planet->name));
            break;
        default: {
            Object *object = data->object.object;
            ui_property_text_readonly("Catalog", catalog_string(object->designation.catalog));
            ui_property_number_readonly("Index", (s64) object->designation.index, nil);
            ui_property_text_readonly("Type", classification_string(object->classification));
            ui_property_text_readonly("Const", constellation_string(object->constellation));
            break;
        }
    }

    igTableNextColumn();

    memory_arena_clear(&sequencer->position_arena);

    Geographic observer = { 0 };
    observer.latitude = sequencer->browser->settings->location.latitude;
    observer.longitude = sequencer->browser->settings->location.longitude;

    ComputeSpecification compute = { 0 };
    compute.date = *start;
    compute.unit = data->duration.unit > UNIT_SECONDS ? data->duration.unit - 1 : UNIT_SECONDS;
    compute.steps = (usize) time_difference(start, &end) * unit_conversion_factor(UNIT_SECONDS, compute.unit);
    compute.step_size = 1;
    compute.observer = observer;

    ComputeResult result = { 0 };
    switch (data->object.classification) {
        case CLASSIFICATION_PLANET:
            compute_geographic_planet(&sequencer->position_arena, &result, data->object.planet, &compute);
            break;
        default:
            compute_geographic_fixed(&sequencer->position_arena, &result, data->object.object, &compute);
            break;
    }

    ImPlotFlags plot_flags = ImPlotFlags_NoFrame;
    ImPlotAxisFlags axis_flags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels;
    usize count = compute.steps;

    Time now = time_now();
    f64 now_mark = (f64) time_difference(start, &now);

    ImVec4 COLOR_RED = (ImVec4) { 1.0f, 0.0f, 0.0f, 1.0f };
    ImVec4 COLOR_GREEN = (ImVec4) { 0.0f, 1.0f, 0.0f, 1.0f };

    ImVec2 plot_size = { 0 };
    igGetContentRegionAvail(&plot_size);

    char plot_title[64];
    snprintf(plot_title, sizeof plot_title, "##idPlot%p", (void *) data);
    if (ImPlot_BeginPlot(plot_title, plot_size, plot_flags)) {
        ImPlot_SetupAxis(ImAxis_X1, "Seconds", axis_flags);
        ImPlot_SetupAxis(ImAxis_Y1, "Angle", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_Opposite);
        ImPlot_SetupAxisFormat_Str(ImAxis_X1, "%g s");
        ImPlot_SetupAxisFormat_Str(ImAxis_Y1, "%g °");
        ImPlot_SetupAxesLimits(0.0, count - 1, -90.0, 360.0, ImPlotCond_Always);
        ImPlot_TagX_Str(now_mark, (ImVec4) { 0.0f, 0.0f, 1.0f, 1.0f }, "Now");
        ImPlot_DragLineX(0, &now_mark, (ImVec4) { 0.33f, 0.33f, 0.33f, 1.0f }, 1, 0, nil, nil, nil);
        ImPlot_SetNextLineStyle(COLOR_RED, 1.0f);
        ImPlot_PlotLine_doublePtrInt("azimuth(t)", result.azimuths, count, 1, 0, 0, 0, sizeof(f64));
        ImPlot_SetNextLineStyle(COLOR_GREEN, 1.0f);
        ImPlot_PlotLine_doublePtrInt("altitude(t)", result.altitudes, count, 1, 0, 0, 0, sizeof(f64));
        ImPlot_EndPlot();
    }

    igEndTable();
}

/// Draw the timeline node
static Duration *sequencer_render_timeline_node(Sequencer *sequencer, SequenceNode *node, Time *start) {
    if (node->type == SEQUENCE_NODE_START) {
        return nil;
    }
    if (node->type == SEQUENCE_NODE_WAIT) {
        return &node->wait.duration;
    }

    ImVec2 size = { 0 };
    igGetContentRegionAvail(&size);
    size.y = TIMELINE_PREVIEW_HEIGHT * 2;

    if (igBeginChild_ID(node->id, size, false, ImGuiWindowFlags_NoScrollbar)) {
        sequencer_render_timeline_node_track(sequencer, &node->track, start);
        igEndChild();
    }

    return &node->track.duration;
}

static void sequencer_build_node_list(Sequencer *sequencer, SequenceNodeList *list) {
    SequenceNode *node = sequencer_find_node_by_type(sequencer, SEQUENCE_NODE_START);
    if (node == nil) {
        return;
    }

    while (node != nil) {
        sequence_node_list_add(list, node);

        SequenceLink *next = nil;
        for (SequenceLink *it = sequencer->link_head; it != nil; it = it->next) {
            if (it->from == node->next_id) {
                next = it;
                break;
            }
        }

        if (next == nil) {
            break;
        }

        for (node = sequencer->node_head; node != nil; node = node->next) {
            if (node->previous_id == next->to) {
                break;
            }
        }
    }
}

/// Draw the timeline
static void sequencer_render_timeline(Sequencer *sequencer) {
    if (!ui_window_begin("Sequence Timeline", &sequencer->show_timeline)) {
        return;
    }

    SequenceNodeList nodes = { 0 };
    sequence_node_list_make(&nodes);
    sequencer_build_node_list(sequencer, &nodes);

    Time start = { 0 };
    for (usize i = 0; i < nodes.count; ++i) {
        SequenceNode *node = nodes.nodes[i];
        if (node->type == SEQUENCE_NODE_START) {
            start = node->start.time;
        }
        Duration *duration = sequencer_render_timeline_node(sequencer, node, &start);
        if (duration != nil) {
            time_add(&start, duration->amount, duration->unit);
        }
    }

    sequence_node_list_destroy(&nodes);
    ui_window_end();
}

/// Draw the editor
static void sequencer_render_editor(Sequencer *sequencer) {
    if (!ui_window_begin("Sequence Editor", &sequencer->show_editor)) {
        return;
    }

    // Unfortunately, we have to save window padding and frame padding as ImNodes changes it :(
    ImGuiStyle *style = igGetStyle();
    ImVec2 frame_padding = style->FramePadding;
    ImVec2 window_padding = style->WindowPadding;

    // Begin the node editor itself
    ui_node_editor_begin();
    if (ui_node_editor_action()) {
        ui_popup_open(SEQUENCE_NODE_POPUP_ID);
    }

    // Push the saved style
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, window_padding);
    igPushStyleVar_Vec2(ImGuiStyleVar_FramePadding, frame_padding);

    if (ui_popup_begin(SEQUENCE_NODE_POPUP_ID)) {
        ui_note("Create Node");
        SequenceNode *node = nil;
        if (!sequencer->has_start_node && ui_selectable("Start", ICON_FA_PLAY)) {
            SequenceNodeStartData data = { 0 };
            data.time = time_now();
            node = sequence_node_make_start(sequencer, &data);
        }
        if (sequencer->browser->selected.tree_index != -1 && ui_selectable("Track", ICON_FA_CROSSHAIRS)) {
            SequenceNodeTrackData data = { 0 };
            data.duration.amount = 10;
            data.duration.unit = UNIT_MINUTES;
            data.object = sequencer->browser->selected;
            node = sequence_node_make_track(sequencer, &data);
        }
        if (ui_selectable("Wait", ICON_FA_CLOCK)) {
            SequenceNodeWaitData data = { 0 };
            data.duration.amount = 10;
            data.duration.unit = UNIT_MINUTES;
            node = sequence_node_make_wait(sequencer, &data);
        }
        if (node != nil) {
            ImVec2 mouse_pos;
            igGetMousePos(&mouse_pos);
            imnodes_SetNodeScreenSpacePos(node->id, mouse_pos);
            imnodes_SnapNodeToGrid(node->id);
            sequencer_emplace_node(sequencer, node);
        }
        ui_separator();
        igPushStyleColor_Vec4(ImGuiCol_Text, (ImVec4) { 1.0f, 0.0f, 0.2f, 1.0f });
        if (ui_selectable("Clear Nodes\t", ICON_FA_TRASH)) {
            sequencer_clear(sequencer);
        }
        igPopStyleColor(1);
        igEndPopup();
    }

    igPopStyleVar(2);

    sequencer_render_nodes(sequencer);
    imnodes_MiniMap(0.2f, ImNodesMiniMapLocation_TopRight, nil, nil);
    ui_node_editor_end();

    if (igBeginDragDropTarget()) {
        const ImGuiPayload *payload = igAcceptDragDropPayload(object_browser_payload_id(), ImGuiDragDropFlags_None);
        if (payload != nil && payload->DataSize == sizeof(ObjectEntry)) {
            SequenceNodeTrackData data = { 0 };
            data.duration.amount = 10;
            data.duration.unit = UNIT_MINUTES;
            data.object = sequencer->browser->selected;
            SequenceNode *node = sequence_node_make_track(sequencer, &data);

            ImVec2 mouse_pos;
            igGetMousePos(&mouse_pos);
            imnodes_SetNodeScreenSpacePos(node->id, mouse_pos);
            imnodes_SnapNodeToGrid(node->id);
            sequencer_emplace_node(sequencer, node);
        }
        igEndDragDropTarget();
    }

    s32 from, to;
    if (imnodes_IsLinkCreated_BoolPtr(&from, &to, nil)) {
        SequenceLink *link = sequence_link_make(sequencer, from, to);
        sequencer_emplace_link(sequencer, link);
    }

    s32 link_id;
    if ((imnodes_IsLinkHovered(&link_id) && key_pressed(KEY_CODE_DELETE)) || imnodes_IsLinkDestroyed(&link_id)) {
        sequencer_remove_link(sequencer, link_id);
    }
    ui_window_end();
}

/// Draw the sequencer
void sequencer_render(Sequencer *sequencer) {
    sequencer_render_editor(sequencer);
    sequencer_render_timeline(sequencer);
}
