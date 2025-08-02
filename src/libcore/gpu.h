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

#ifndef CORE_GPU_H
#define CORE_GPU_H

#include "math.h"
#include "string.h"


// ===================================================================================
// GPU relevant data types
// ===================================================================================

typedef struct Vertex {
    Vector2f position;
    Vector3f color;
} Vertex;

typedef u32 Index;

enum {
    RENDERER_QUAD_VERTICES = 4,
    RENDERER_QUAD_INDICES = 6
};

// ===================================================================================
// SHADER
// ===================================================================================

typedef struct Shader {
    u32 handle;
} Shader;

/// Creates a shader from the given vertex and fragment shader files
/// @param self shader handle
/// @param vertex path to the vertex shader
/// @param fragment path to the fragment shader
/// @return b8
b8 shader_create(Shader *self, const char *vertex, const char *fragment);

/// Destroys the specified shader
/// @param self shader handle
void shader_destroy(Shader const *self);

/// Sets a sampler2d (texture) uniform
/// @param self shader handle
/// @param name uniform name
/// @param slot sampler slot for the texture
void shader_uniform_sampler(Shader const *self, const char *name, u32 slot);

/// Sets an integer (s32) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_s32(Shader const *self, const char *name, s32 value);

/// Sets a 2d-integer (Vector2s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector2s(Shader const *self, const char *name, Vector2s const *value);

/// Sets a 3d-integer (Vector3s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector3s(Shader const *self, const char *name, Vector3s const *value);

/// Sets a 4d-integer (Vector4s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector4s(Shader const *self, const char *name, Vector4s const *value);

/// Sets a float (f32) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_f32(Shader const *self, const char *name, f32 value);

/// Sets a 2d-float (Vector2f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector2f(Shader const *self, const char *name, Vector2f const *value);

/// Sets a 3d-float (Vector3f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector3f(Shader const *self, const char *name, Vector3f const *value);

/// Sets a 4d-float (Vector4f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector4f(Shader const *self, const char *name, Vector4f const *value);

/// Sets a mat4 (Matrix4x4f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_matrix4x4f(Shader const *self, const char *name, Matrix4x4f const *value);

/// Binds the specified shader
/// @param self shader handle
void shader_bind(Shader const *self);

/// Unbinds the currently bound shader
void shader_unbind(void);

typedef enum ShaderType {
    INT = 0,
    INT2,
    INT3,
    INT4,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    SAMPLER = INT
} ShaderType;

// ===================================================================================
// VERTEX BUFFER
// ===================================================================================

typedef struct VertexBufferLayout {
    ShaderType *attributes;
    u32 count;
} VertexBufferLayout;

typedef struct VertexBuffer {
    u32 handle;
    VertexBufferLayout *layout;
} VertexBuffer;

/// Creates a vertex buffer on the gpu
/// @param self buffer handle
void vertex_buffer_create(VertexBuffer *self);

/// Destroys the vertex buffer
/// @param self buffer Handle
void vertex_buffer_destroy(VertexBuffer *self);

/// Sets the data for the specified buffer
/// @param self The vertex buffer handle
/// @param data pointer to the first element of the data
/// @param size size in bytes
void vertex_buffer_data(VertexBuffer const *self, const void *data, u32 size);

/// Sets the attribute layout for the specified buffer
/// @param self The vertex buffer handle
/// @param layout layout handle
void vertex_buffer_layout(VertexBuffer *self, VertexBufferLayout *layout);

/// Binds the specified buffer
/// @param self Buffer handle
void vertex_buffer_bind(VertexBuffer const *self);

/// Unbinds the currently bound buffer
void vertex_buffer_unbind(void);

// ===================================================================================
// INDEX BUFFER
// ===================================================================================

typedef struct IndexBuffer {
    u32 handle;
    u32 count;
} IndexBuffer;

/// Creates an index buffer on the gpu
/// @param self Buffer handle
void index_buffer_create(IndexBuffer *self);

/// Destroys the index buffer
/// @param self Buffer Handle
void index_buffer_destroy(IndexBuffer const *self);

/// Sets the data for the specified buffer
/// @param self buffer handle
/// @param data pointer to the first index
/// @param count number of indices
void index_buffer_data(IndexBuffer *self, const u32 *data, u32 count);

/// Binds the specified buffer
/// @param self The vertex buffer handle
void index_buffer_bind(IndexBuffer const *self);

/// Unbinds the currently bound index buffer
void index_buffer_unbind(void);

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

typedef struct VertexArray {
    u32 handle;
    VertexBuffer *vertex_buffer;
    IndexBuffer *index_buffer;
} VertexArray;

/// Creates a new vertex array
/// @param self vertex array handle
void vertex_array_create(VertexArray *self);

/// Destroys the specified vertex array
/// @param self vertex array handle
void vertex_array_destroy(VertexArray const *self);

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
/// @param self vertex array handle
/// @param vertex_buffer vertex buffer handle
void vertex_array_vertex_buffer(VertexArray *self, VertexBuffer *vertex_buffer);

/// Sets the index buffer for the vertex array
/// @param self vertex array handle
/// @param index_buffer index buffer handle
void vertex_array_index_buffer(VertexArray *self, IndexBuffer *index_buffer);

/// Binds the specified vertex array
/// @param self vertex array handle
void vertex_array_bind(VertexArray const *self);

/// Unbinds the currently bound vertex array
void vertex_array_unbind(void);

typedef struct FrameBufferInfo {
    s32 width;
    s32 height;
    s32 internal_format;
    u32 pixel_type;
    u32 pixel_format;
} FrameBufferInfo;

typedef struct FrameBuffer {
    u32 handle;
    u32 texture_handle;
    u32 render_handle;
    FrameBufferInfo spec;
} FrameBuffer;

/// Creates a frame buffer of specified size
/// @param self The frame buffer handle
/// @param spec The frame buffer specification
/// @return A boolean value that indicates whether the frame buffer could be created
b8 frame_buffer_create(FrameBuffer *self, FrameBufferInfo const *spec);

/// Destroys the frame buffer
/// @param self The frame buffer handle
void frame_buffer_destroy(FrameBuffer const *self);

/// Invalidates the frame buffer, this needs to be called whenever the frame buffer is resized
/// @param self The frame buffer handle
/// @return A boolean value that indicates whether the frame buffer could be invalidated
b8 frame_buffer_invalidate(FrameBuffer *self);

/// Resizes the frame buffer
/// @param self The frame buffer handle
/// @param width The new width
/// @param height The new height
/// @return A boolean value that indicates whether the frame buffer could be resized
b8 frame_buffer_resize(FrameBuffer *self, s32 width, s32 height);

/// Binds the specified frame buffer for rendering
/// @param self The frame buffer handle
void frame_buffer_bind(FrameBuffer const *self);

/// Binds the texture of the frame buffer at the specified sampler slot
/// @param self The frame buffer handle
/// @param slot The sampler slot
void frame_buffer_bind_texture(FrameBuffer const *self, u32 slot);

/// Unbinds the currently bound frame buffer
void frame_buffer_unbind(void);

/// Forward declare render command
typedef struct RenderCommand RenderCommand;

/// A render command enables lazy drawing, i.e., submitting draw data
/// without rendering it immediately. It is grouped together by a so-
/// called render group, and sent to the GPU in one batch when necessary.
///
/// Due to the render group being a linked list, each render command
/// stores the previous and next command.
typedef struct RenderCommand {
    RenderCommand *previous;
    RenderCommand *next;
    Vertex vertices[RENDERER_QUAD_VERTICES];
    Index indices[RENDERER_QUAD_INDICES];
} RenderCommand;

/// Creates a new render command
/// @param arena The arena where the command lives in
/// @param vertices A list of vertices, which must be exactly 4
/// @param indices A list of indices, which must be exactly 6
/// @return A new render command instance
RenderCommand *render_command_new(MemoryArena *arena, Vertex const *vertices, Index const *indices);

/// A render group enables the lazy drawing of data. Each render
/// group consists of a linked list of render commands, with a render
/// command being one draw call.
typedef struct RenderGroup {
    /// Linked list of render commands
    RenderCommand *begin;
    RenderCommand *end;
    u32 commands;
    MemoryArena command_memory;

    /// Drawing data
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    IndexBuffer index_buffer;
} RenderGroup;

/// Creates a new render group
/// @param self The render group
void render_group_create(RenderGroup *self);

/// Destroys the specified render group (i.e., delete the commands and free memory)
/// @param self The render group handle
void render_group_destroy(RenderGroup *self);

/// Clears the specified render group (i.e., deletes the commands)
/// @param self The render group handle
void render_group_clear(RenderGroup *self);

/// Pushes a set of vertices and indices to the render group
/// @param self The render group handle
/// @param vertices The vertex data, which must be exactly 4
/// @param indices The indices data, which must be exactly 6
void render_group_push(RenderGroup *self, Vertex const *vertices, Index const *indices);

typedef struct Renderer {
    RenderGroup group;
    Shader shader;
    FrameBuffer capture;
} Renderer;

/// Clears the currently bound frame buffer
void renderer_clear(void);

/// Sets the clear color
/// @param color The color value for clear calls
void renderer_clear_color(Vector4f const *color);

/// Creates a new renderer and initializes its pipeline
/// @param self The renderer handle
/// @param width The initial frame buffer width
/// @param height The initial frame buffer height
void renderer_create(Renderer *self, s32 width, s32 height);

/// Destroys the specified renderer
/// @param self The renderer handle
void renderer_destroy(Renderer *self);

/// Begins a renderer batch by resetting all render groups
/// @param self The renderer handle
void renderer_begin_batch(Renderer *self);

/// Ends a renderer batch by submitting the commands of all render groups
/// @param self The renderer handle
void renderer_end_batch(Renderer *self);

/// Indicate to the renderer that a resize is necessary
/// @param self The renderer handle
/// @param width The new width
/// @param height The new height
void renderer_resize(Renderer *self, s32 width, s32 height);

/// Draws a quad at the given position
/// @param self The renderer handle
/// @param position The position where the quad shall be drawn
/// @param size The size of the quad
/// @param color The color of the quad
void renderer_draw_quad(Renderer *self, Vector2f const *position, Vector2f const *size, Vector3f const *color);

/// Captures all draw commands that follow into a frame buffer
/// @param self The renderer handle
void renderer_begin_capture(Renderer const *self);

/// Ends the capture of draw commands
void renderer_end_capture();

#endif// CORE_GPU_H
