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

#ifndef CORE_UI_SEQUENCER_H
#define CORE_UI_SEQUENCER_H

#include "browser.h"

#include <libcore/types.h>

typedef enum SequenceNodeType { SEQUENCE_NODE_TRACK, SEQUENCE_NODE_WAIT } SequenceNodeType;

/// Forward declare Sequencer
typedef struct SequenceNode SequenceNode;

typedef struct SequenceNode {
    /// The previous node in the sequence
    SequenceNode* previous;

    /// The next node in the sequence
    SequenceNode* next;

    /// The type of the node
    SequenceNodeType type;

    /// The object if the node is of an object related type
    ObjectEntry object;

    /// The duration of the node in seconds
    Duration duration;

    /// The ID of the node which is required for linking
    u64 id;
} SequenceNode;

/// Forward declare Sequencer
typedef struct Sequencer Sequencer;

/// Create a new track sequence node
/// @param sequencer The sequencer handle
/// @param entry The object entry
/// @param duration The sequence duration
/// @return A track sequence node that lives inside the arena
SequenceNode* sequence_node_make_track(Sequencer* sequencer, ObjectEntry* entry, Duration duration);

/// Create a new wait sequence node
/// @param sequencer The sequencer handle
/// @param duration The sequence duration
/// @return A wait sequence node that lives inside the arena
SequenceNode* sequence_node_make_wait(Sequencer* sequencer, Duration duration);

typedef struct Sequencer {
    /// Start of the sequence
    SequenceNode* head;

    /// End of the sequence
    SequenceNode* tail;

    /// Number of nodes
    usize length;

    /// Sequencer arena, this stores all the nodes in blocks
    MemoryArena arena;

    /// This flag controls whether the sequencer node editor is displayed
    b8 show_editor;

    /// This flag controls whether the sequencer timeline is displayed
    b8 show_timeline;
} Sequencer;

/// Create a new sequencer
/// @param sequencer The sequencer handle
void sequencer_make(Sequencer* sequencer);

/// Destroy the sequencer
/// @param sequencer The sequencer handle
void sequencer_destroy(Sequencer* sequencer);

/// Clear the sequencer
/// @param sequencer The sequencer handle
void sequencer_clear(Sequencer* sequencer);

/// Emplace a node into the sequencer
/// @param sequencer The sequencer handle
/// @param node The sequence node
void sequencer_emplace(Sequencer* sequencer, SequenceNode* node);

/// Draw the sequencer
/// @param sequencer The sequencer handle
void sequencer_render(Sequencer* sequencer);

#endif// CORE_UI_SEQUENCER_H
