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

#include <libcore/log.h>

#include "skymap.h"

// static Vector4f COLOR_BLACK = { 0.0f, 0.0f, 0.0f, 0.0f };
static Vector3f COLOR_OBJECT = { 0.2f, 0.2f, 0.2f };
static Vector3f COLOR_OBJECT_MARK = { 1.0f, 0.5f, 0.0f };

static GlobeSection skymap_object_section(Object *object, Vector2f size) {
    GlobeArea area = globe_area_make(size.x, size.y);
    GlobePosition position = globe_position_make(object->position.right_ascension - 0.5f * size.x,
                                                 object->position.declination - 0.5f * size.y);
    return globe_section_make(position, area);
}

static GlobePosition skymap_relative_position(GlobeSection *section, Object *compare) {
    GlobePosition result = { 0 };
    result.right_ascension = compare->position.right_ascension - section->position.right_ascension;
    result.declination = compare->position.declination - section->position.declination;
    return result;
}

/// Generates a skymap
void skymap_generate(MemoryArena *arena, Renderer *renderer, SkyMapInfo *info) {
    frame_buffer_bind(info->target);
    renderer_begin_batch(renderer);

    // Compute the area in the sky
    GlobeSection section = skymap_object_section(info->object, info->size);

    // Search the globe tree
    GlobeNodeList search = globe_tree_search(info->globe, arena, &section);

    // Determine the render extent
    Vector2f extent = { 0 };
    extent.x = info->size.x * info->scale;
    extent.y = info->size.y * info->scale;
    renderer_resize(renderer, (s32) extent.x, (s32) extent.y);

    // renderer_clear_color(&COLOR_BLACK);
    // renderer_clear();

    for (GlobeNode *it = search.head; it != nil; it = it->next) {
        // Determine the relative position of the object compared to the search section
        GlobePosition relative_position = skymap_relative_position(&section, it->object);

        // Turn into Vector2f for rendering
        Vector2f obj_position = { 0 };
        obj_position.x = (f32) relative_position.right_ascension;
        obj_position.y = (f32) relative_position.declination;

        // Determine color and size of object
        Vector3f *color = nil;
        Vector2f obj_size = { 0 };
        if (it->object == info->object) {
            color = &COLOR_OBJECT_MARK;
            obj_size.x = info->scale * 2.0f;
            obj_size.y = info->scale * 2.0f;
        } else {
            color = &COLOR_OBJECT;
            obj_size.x = info->scale;
            obj_size.y = info->scale;
        }


        renderer_draw_quad(renderer, &obj_position, &obj_size, color);
    }

    renderer_end_batch(renderer);
    frame_buffer_unbind();
}
