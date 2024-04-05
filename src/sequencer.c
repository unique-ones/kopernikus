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
#include "browser.h"
#include "cimgui.h"
#include "libcore/input.h"
#include "solaris/object.h"
#include "solaris/planet.h"
#include "solaris/time.h"
#include "solaris/types.h"
#include "ui.h"

#include <cimnodes.h>
#include <libcore/log.h>

static const char *SEQUENCE_NODE_POPUP_ID = "##CreateSequenceNode";
static const f32 SEQUENCE_NODE_WIDTH = 100.0f;

/// Create a new track sequence node
SequenceNode *sequence_node_make_track(Sequencer *sequencer,
                                       Duration duration,
                                       TimeUnit unit,
                                       SequenceNodeTrackData *data) {
    SequenceNode *node = (SequenceNode *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_TRACK;
    node->duration = duration;
    node->unit = unit;
    node->track = *data;
    node->id = sequencer->node_count + 1;
    return node;
}

/// Create a new wait sequence node
SequenceNode *sequence_node_make_wait(Sequencer *sequencer, Duration duration, TimeUnit unit) {
    SequenceNode *node = (SequenceNode *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_WAIT;
    node->duration = duration;
    node->unit = unit;
    node->track = (SequenceNodeTrackData){ 0 };
    node->id = sequencer->node_count + 1;
    return node;
}

/// Create a new link instance
SequenceLink *sequence_link_make(Sequencer *sequencer, s32 origin_id, s32 target_id) {
    SequenceLink *link = (SequenceLink *) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    link->previous = nil;
    link->next = nil;
    link->origin_id = origin_id;
    link->target_id = target_id;
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
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
    sequencer->show_editor = true;
    sequencer->show_timeline = true;
    sequencer->browser = browser;
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
    memory_arena_destroy(&sequencer->arena);
}

/// Clear the sequencer
void sequencer_clear(Sequencer *sequencer) {
    sequencer->node_head = nil;
    sequencer->node_tail = nil;
    sequencer->node_count = 0;
    sequencer->link_head = nil;
    sequencer->link_tail = nil;
    sequencer->link_count = 0;
    memory_arena_destroy(&sequencer->arena);
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
}

/// Emplace a node into the sequencer
void sequencer_emplace_node(Sequencer *sequencer, SequenceNode *node) {
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
        if (it->origin_id == node_id || it->target_id == node_id) {
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
static void sequencer_render_node_time_data(SequenceNode *node) {
    ui_property_real("Duration", &node->duration, "%.4f");

    static const char *UNITS[] = { "Seconds", "Minutes", "Hours", "Days", "Months", "Years" };
    ui_combobox("Unit", &node->unit, UNITS, ARRAY_SIZE(UNITS));
}

/// Draw a wait node
static void sequencer_render_node_wait(SequenceNode *node, f32 width) {
    imnodes_BeginNodeTitleBar();
    ui_text(ICON_FA_CLOCK " Wait");
    imnodes_EndNodeTitleBar();

    imnodes_BeginInputAttribute(node->id << 8, ImNodesPinShape_Circle);
    ui_text("Previous\t");
    imnodes_EndInputAttribute();
    ui_keep_line();
    imnodes_BeginOutputAttribute(node->id << 16, ImNodesPinShape_Circle);
    ui_text("Next");
    imnodes_EndOutputAttribute();

    ui_item_width_begin(width);
    sequencer_render_node_time_data(node);
    ui_item_width_end();
}

/// Draw a track node
static void sequencer_render_node_track(SequenceNode *node, f32 width) {
    imnodes_BeginNodeTitleBar();
    ui_text(ICON_FA_CROSSHAIRS " Track");
    imnodes_EndNodeTitleBar();

    imnodes_BeginInputAttribute(node->id << 8, ImNodesPinShape_Circle);
    ui_text("Previous\t");
    imnodes_EndInputAttribute();
    ui_keep_line();
    imnodes_BeginOutputAttribute(node->id << 16, ImNodesPinShape_Circle);
    ui_text("Next");
    imnodes_EndOutputAttribute();

    ui_item_width_begin(width);
    sequencer_render_node_time_data(node);

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

/// Draw a node
static void sequencer_render_node(Sequencer *sequencer, SequenceNode *node, f32 width) {
    ui_node_begin(node->id);
    switch (node->type) {
        case SEQUENCE_NODE_TRACK:
            sequencer_render_node_track(node, width);
            break;
        case SEQUENCE_NODE_WAIT:
            sequencer_render_node_wait(node, width);
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
        imnodes_Link(it->id, it->origin_id, it->target_id);
    }
}

/// Draw the timeline
static void sequencer_render_timeline(Sequencer *sequencer) {
    if (!ui_window_begin("Sequence Timeline", &sequencer->show_timeline)) {
        return;
    }
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
        if (ui_selectable("Track", ICON_FA_CROSSHAIRS)) {
            if (sequencer->browser->selected.tree_index != -1) {
                node = sequence_node_make_track(sequencer, 10, UNIT_MINUTES,
                                                &(SequenceNodeTrackData){ sequencer->browser->selected });
            } else {
                flogf(stderr, "[sequencer] Must select object in order to create a track node!\n");
            }
        }
        if (ui_selectable("Wait", ICON_FA_CLOCK)) {
            node = sequence_node_make_wait(sequencer, 10, UNIT_MINUTES);
        }
        if (node != nil) {
            ImVec2 mouse_pos;
            igGetMousePos(&mouse_pos);
            imnodes_SetNodeScreenSpacePos(node->id, mouse_pos);
            imnodes_SnapNodeToGrid(node->id);
            sequencer_emplace_node(sequencer, node);
        }
        ui_separator();
        igPushStyleColor_Vec4(ImGuiCol_Text, (ImVec4){ 1.0f, 0.0f, 0.2f, 1.0f });
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
            ImVec2 mouse_pos;
            igGetMousePos(&mouse_pos);

            SequenceNode *node = sequence_node_make_track(sequencer, 10, UNIT_MINUTES,
                                                          &(SequenceNodeTrackData){ sequencer->browser->selected });
            imnodes_SetNodeScreenSpacePos(node->id, mouse_pos);
            imnodes_SnapNodeToGrid(node->id);
            sequencer_emplace_node(sequencer, node);
        }
        igEndDragDropTarget();
    }

    s32 origin_id, target_id;
    if (imnodes_IsLinkCreated_BoolPtr(&origin_id, &target_id, nil)) {
        SequenceLink *link = sequence_link_make(sequencer, origin_id, target_id);
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
