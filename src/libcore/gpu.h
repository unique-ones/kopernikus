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

#include "types.h"

// ===================================================================================
// GPU relevant data types
// ===================================================================================

typedef struct Vertex {
    Vector4f position;
} Vertex;

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
b8 shader_create(Shader* self, const char* vertex, const char* fragment);

/// Destroys the specified shader
/// @param self shader handle
void shader_destroy(Shader* self);

/// Sets a sampler2d (texture) uniform
/// @param self shader handle
/// @param name uniform name
/// @param slot sampler slot for the texture
void shader_uniform_sampler(Shader* self, const char* name, u32 slot);

/// Sets an integer (s32) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_s32(Shader* self, const char* name, s32 value);

/// Sets a 2d-integer (Vector2s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector2s(Shader* self, const char* name, Vector2s* value);

/// Sets a 3d-integer (Vector3s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector3s(Shader* self, const char* name, Vector3s* value);

/// Sets a 4d-integer (Vector4s) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector4s(Shader* self, const char* name, Vector4s* value);

/// Sets a float (f32) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_f32(Shader* self, const char* name, f32 value);

/// Sets a 2d-float (Vector2f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector2f(Shader* self, const char* name, Vector2f* value);

/// Sets a 3d-float (Vector3f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector3f(Shader* self, const char* name, Vector3f* value);

/// Sets a 4d-float (Vector4f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_vector4f(Shader* self, const char* name, Vector4f* value);

/// Sets a mat4 (Matrix4x4f) uniform
/// @param self shader handle
/// @param name uniform name
/// @param value value
void shader_uniform_matrix4x4f(Shader* self, const char* name, Matrix4x4f* value);

/// Binds the specified shader
/// @param self shader handle
void shader_bind(Shader* self);

/// Unbinds the currently bound shader
void shader_unbind(void);

typedef enum ShaderType { INT = 0, INT2, INT3, INT4, FLOAT, FLOAT2, FLOAT3, FLOAT4, SAMPLER = INT } ShaderType;

// ===================================================================================
// VERTEX BUFFER
// ===================================================================================

typedef struct VertexBufferLayout {
    ShaderType* attributes;
    u32 count;
} VertexBufferLayout;

typedef struct VertexBuffer {
    u32 handle;
    VertexBufferLayout* layout;
} VertexBuffer;

/// Creates a vertex buffer on the gpu
/// @param self buffer handle
void vertex_buffer_create(VertexBuffer* self);

/// Destroys the vertex buffer
/// @param self buffer Handle
void vertex_buffer_destroy(VertexBuffer* self);

/// Sets the data for the specified buffer
/// @param self self handle
/// @param data pointer to the first element of the data
/// @param size size in bytes
void vertex_buffer_data(VertexBuffer* self, const void* data, u32 size);

/// Sets the attribute layout for the specified buffer
/// @param self self handle
/// @param layout layout handle
void vertex_buffer_layout(VertexBuffer* self, VertexBufferLayout* layout);

/// Binds the specified buffer
/// @param self Buffer handle
void vertex_buffer_bind(VertexBuffer* self);

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
void index_buffer_create(IndexBuffer* self);

/// Destroys the index buffer
/// @param self Buffer Handle
void index_buffer_destroy(IndexBuffer* self);

/// Sets the data for the specified buffer
/// @param self buffer handle
/// @param data pointer to the first index
/// @param count number of indices
void index_buffer_data(IndexBuffer* self, const u32* data, u32 count);

/// Binds the specified buffer
/// @param self self handle
void index_buffer_bind(IndexBuffer* self);

/// Unbinds the currently bound index buffer
void index_buffer_unbind(void);

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

typedef struct VertexArray {
    u32 handle;
    VertexBuffer* vertex_buffer;
    IndexBuffer* index_buffer;
} VertexArray;

/// Creates a new vertex array
/// @param self vertex array handle
void vertex_array_create(VertexArray* self);

/// Destroys the specified vertex array
/// @param self vertex array handle
void vertex_array_destroy(VertexArray* self);

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
/// @param self vertex array handle
/// @param vertex_buffer vertex buffer handle
void vertex_array_vertex_buffer(VertexArray* self, VertexBuffer* vertex_buffer);

/// Sets the index buffer for the vertex array
/// @param self vertex array handle
/// @param index_buffer index buffer handle
void vertex_array_index_buffer(VertexArray* self, IndexBuffer* index_buffer);

/// Binds the specified vertex array
/// @param self vertex array handle
void vertex_array_bind(VertexArray* self);

/// Unbinds the currently bound vertex array
void vertex_array_unbind(void);

#endif// CORE_GPU_H
