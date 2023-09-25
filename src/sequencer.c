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
#include "ui.h"

#include <libcore/log.h>

#include <cimnodes.h>

static const char* SEQUENCE_NODE_POPUP_ID = "##CreateSequenceNode";

/// Create a new sequence node
SequenceNode* sequence_node_make_track(Sequencer* sequencer, ObjectEntry* entry, Duration duration) {
    SequenceNode* node = (SequenceNode*) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_TRACK;
    node->object = *entry;
    node->duration = duration;
    node->id = sequencer->length;
    return node;
}

/// Create a new wait sequence node
SequenceNode* sequence_node_make_wait(Sequencer* sequencer, Duration duration) {
    SequenceNode* node = (SequenceNode*) memory_arena_alloc(&sequencer->arena, sizeof(SequenceNode));
    node->previous = nil;
    node->next = nil;
    node->type = SEQUENCE_NODE_WAIT;
    node->object = (ObjectEntry){ .classification = CLASSIFICATION_COUNT, .tree_index = -1, .object = nil };
    node->duration = duration;
    node->id = sequencer->length + 1;
    return node;
}

/// Create a new sequencer
void sequencer_make(Sequencer* sequencer) {
    sequencer->head = nil;
    sequencer->tail = nil;
    sequencer->length = 0;
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
    sequencer->show_editor = true;
    sequencer->show_timeline = true;
}

/// Destroy the sequencer
void sequencer_destroy(Sequencer* sequencer) {
    // No need to traverse the list and free the nodes individually,
    // since all live inside the arena which we can free.
    sequencer->head = nil;
    sequencer->tail = nil;
    sequencer->length = 0;
    memory_arena_destroy(&sequencer->arena);
}

/// Clear the sequencer
void sequencer_clear(Sequencer* sequencer) {
    sequencer->head = nil;
    sequencer->tail = nil;
    sequencer->length = 0;
    memory_arena_destroy(&sequencer->arena);
    sequencer->arena = memory_arena_identity(ALIGNMENT8);
}

/// Emplace a node into the sequencer
void sequencer_emplace(Sequencer* sequencer, SequenceNode* node) {
    if (sequencer->head == nil) {
        sequencer->head = node;
        sequencer->length++;
        return;
    }

    /// Traverse all nodes
    SequenceNode* it = sequencer->head;
    for (; it->next != nil; it = it->next) { }

    it->next = node;
    node->previous = it;
    sequencer->tail = node;
    sequencer->length++;
}

/// Draw a wait node
static void sequencer_render_node_wait(SequenceNode* node) {
    imnodes_BeginNodeTitleBar();
    ui_text("Wait");
    imnodes_EndNodeTitleBar();
}

/// Draw a track node
static void sequencer_render_node_track(SequenceNode* node) {
    imnodes_BeginNodeTitleBar();
    ui_text("Track");
    imnodes_EndNodeTitleBar();
}

/// Draw a node
static void sequencer_render_node(SequenceNode* node) {
    imnodes_BeginNode((s32) node->id);
    switch (node->type) {
        case SEQUENCE_NODE_TRACK:
            sequencer_render_node_track(node);
            break;
        case SEQUENCE_NODE_WAIT:
            sequencer_render_node_wait(node);
            break;
    }
    imnodes_EndNode();
}

/// Draw all the nodes
static void sequencer_render_nodes(Sequencer* sequencer) {
    for (SequenceNode* it = sequencer->head; it != nil; it = it->next) {
        sequencer_render_node(it);
    }
}

/// Draw the timeline
static void sequencer_render_timeline(Sequencer* sequencer) {
    if (!ui_window_begin("Sequence Timeline", &sequencer->show_timeline)) {
        return;
    }
    ui_window_end();
}

/// Draw the editor
static void sequencer_render_editor(Sequencer* sequencer) {
    if (!ui_window_begin("Sequence Editor", &sequencer->show_editor)) {
        return;
    }

    // Unfortunately, we have to save window padding and frame padding as ImNodes changes it :(
    ImGuiStyle* style = igGetStyle();
    ImVec2 frame_padding = style->FramePadding;
    ImVec2 window_padding = style->WindowPadding;

    // Begin the node editor itself
    imnodes_BeginNodeEditor();
    if (imnodes_IsEditorHovered() && ui_click_right()) {
        igOpenPopup_Str(SEQUENCE_NODE_POPUP_ID, ImGuiPopupFlags_None);
    }

    // Push the saved style
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, window_padding);
    igPushStyleVar_Vec2(ImGuiStyleVar_FramePadding, frame_padding);
    if (igBeginPopup(SEQUENCE_NODE_POPUP_ID, ImGuiWindowFlags_None)) {
        igSeparatorText("Create Node");

        SequenceNode* node = nil;
        if (ui_selectable("Track", ICON_FA_CROSSHAIRS)) {
            node = sequence_node_make_track(sequencer, &(ObjectEntry){ 0 }, 0);
        }
        if (ui_selectable("Wait", ICON_FA_CLOCK)) {
            node = sequence_node_make_wait(sequencer, 0);
        }
        if (node != nil) {
            ImVec2 mouse_pos;
            igGetMousePos(&mouse_pos);
            imnodes_SetNodeScreenSpacePos((s32) node->id, mouse_pos);
            imnodes_SnapNodeToGrid((s32) node->id);
            sequencer_emplace(sequencer, node);
        }
        igEndPopup();
    }
    igPopStyleVar(2);
    sequencer_render_nodes(sequencer);
    imnodes_EndNodeEditor();
    ui_window_end();
}

/// Draw the sequencer
void sequencer_render(Sequencer* sequencer) {
    sequencer_render_editor(sequencer);
    sequencer_render_timeline(sequencer);
}
