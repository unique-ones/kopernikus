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

#ifndef KOPERNIKUS_SEQUENCER_H
#define KOPERNIKUS_SEQUENCER_H

#include <libcore/types.h>

#include "browser.h"

typedef enum SequenceNodeType {
    SEQUENCE_NODE_TRACK,
    SEQUENCE_NODE_WAIT
} SequenceNodeType;

/// Forward declare Sequencer types
typedef struct Sequencer Sequencer;
typedef struct SequenceNode SequenceNode;
typedef struct SequenceLink SequenceLink;

typedef struct SequenceNodeTrackData {
    /// The object if the node is of an object related type
    ObjectEntry object;
} SequenceNodeTrackData;

typedef struct SequenceNode {
    /// The previous node in the sequence
    SequenceNode *previous;

    /// The next node in the sequence
    SequenceNode *next;

    /// The type of the node
    SequenceNodeType type;

    /// The duration of the node
    Duration duration;

    /// The unit of the duration
    TimeUnit unit;

    /// Tracking specific data
    SequenceNodeTrackData track;

    /// The ID of the node which is required for linking
    s32 id;
} SequenceNode;


/// Create a new track sequence node
/// @param sequencer The sequencer handle
/// @param duration The duration of the node
/// @param unit The unit of the duration of the node
/// @param data The track data
/// @return A track sequence node that lives inside the sequencer arena
SequenceNode *sequence_node_make_track(Sequencer *sequencer,
                                       Duration duration,
                                       TimeUnit unit,
                                       SequenceNodeTrackData *data);

/// Create a new wait sequence node
/// @param sequencer The sequencer handle
/// @param duration The duration of the node
/// @param unit The unit of the duration of the node
/// @return A wait sequence node that lives inside the sequencer arena
SequenceNode *sequence_node_make_wait(Sequencer *sequencer, Duration duration, TimeUnit unit);

typedef struct SequenceLink {
    /// The previous link in the sequence
    SequenceLink *previous;

    /// The next link in the sequence
    SequenceLink *next;

    /// The origin ID, which resembles the ID of the origin node where the link starts
    s32 origin_id;

    /// The target ID, which resembles the ID of the target node where the link points to
    s32 target_id;

    /// The id of the link itself
    s32 id;
} SequenceLink;

/// Create a new link instance
/// @param sequencer The sequencer handle
/// @param origin_id The ID of the origin node
/// @param target_id The ID of the target node
/// @return A link that lives inside the sequencer arena
SequenceLink *sequence_link_make(Sequencer *sequencer, s32 origin_id, s32 target_id);

typedef struct Sequencer {
    /// Start of the node sequence
    SequenceNode *node_head;

    /// End of the node sequence
    SequenceNode *node_tail;

    /// Start of the link sequence
    SequenceLink *link_head;

    /// End of the link sequence
    SequenceLink *link_tail;

    /// Number of nodes
    usize node_count;

    /// Number of links
    usize link_count;

    /// Sequencer arena, this stores all the nodes in blocks
    MemoryArena arena;

    /// This flag controls whether the sequencer node editor is displayed
    b8 show_editor;

    /// This flag controls whether the sequencer timeline is displayed
    b8 show_timeline;

    /// The object browser
    ObjectBrowser *browser;
} Sequencer;

/// Create a new sequencer
/// @param sequencer The sequencer handle
/// @param browser The browser handle
void sequencer_make(Sequencer *sequencer, ObjectBrowser *browser);

/// Destroy the sequencer
/// @param sequencer The sequencer handle
void sequencer_destroy(Sequencer *sequencer);

/// Clear the sequencer
/// @param sequencer The sequencer handle
void sequencer_clear(Sequencer *sequencer);

/// Emplace a node into the sequencer node list
/// @param sequencer The sequencer handle
/// @param node The sequence node
void sequencer_emplace_node(Sequencer *sequencer, SequenceNode *node);

/// Emplace a link into the sequencer link list
/// @param sequencer The sequencer handle
/// @param link The sequence link
void sequencer_emplace_link(Sequencer *sequencer, SequenceLink *link);

/// Remove a link by its ID
/// @param sequencer The sequencer handle
/// @param link_id The ID of the sequence link
void sequencer_remove_link(Sequencer *sequencer, s32 link_id);

/// Remove a link by a node ID
/// @param sequencer The sequencer handle
/// @param node_id The ID of a node
void sequencer_remove_link_by_node(Sequencer *sequencer, s32 node_id);

/// Draw the sequencer
/// @param sequencer The sequencer handle
void sequencer_render(Sequencer *sequencer);

#endif// KOPERNIKUS_SEQUENCER_H
