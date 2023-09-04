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

#include <stdlib.h>

#include "gpu.h"
#include "log.h"

// ===================================================================================
// SHADER
// ===================================================================================

/// Compiles the given shader source to a shader program
static u32 shader_compile(const char* source, u32 type) {
    u32 program = glCreateShader(type);
    const GLchar* shader_source = source;
    glShaderSource(program, 1, &shader_source, NULL);
    glCompileShader(program);

    s32 success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_length);

        BinaryBuffer failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetShaderInfoLog(program, info_length, &info_length, failure_info.data);
        glDeleteProgram(program);
        flogf(stderr, "[shader] compilation failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return 0;
    }
    return program;
}

/// Creates a shader from the given vertex and fragment shader files
b8 shader_create(Shader* self, const char* vertex, const char* fragment) {
    u32 vertex_program = shader_compile(vertex, GL_VERTEX_SHADER);
    u32 fragment_program = shader_compile(fragment, GL_FRAGMENT_SHADER);

    u32 handle = glCreateProgram();
    glAttachShader(handle, vertex_program);
    glAttachShader(handle, fragment_program);
    glLinkProgram(handle);

    s32 link_success;
    glGetProgramiv(handle, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        s32 info_length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &info_length);

        BinaryBuffer failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetProgramInfoLog(handle, info_length, NULL, failure_info.data);
        glDeleteProgram(handle);
        glDeleteProgram(vertex_program);
        glDeleteProgram(fragment_program);
        flogf(stderr, "[shader] linking failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return false;
    }

    s32 uniform_count;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    s32 uniform_length;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

    if (uniform_count > 0 && uniform_length > 0) {
        BinaryBuffer uniform_name;
        uniform_name.data = (char*) malloc(uniform_length);
        uniform_name.size = (u32) uniform_length;

        for (u32 i = 0; i < (u32) uniform_count; i++) {
            s32 length, size;
            u32 data_type;

            glGetActiveUniform(handle, i, uniform_length, &length, &size, &data_type, uniform_name.data);
            s32 location = glGetUniformLocation(handle, uniform_name.data);
            flogf(stderr, "[shader] uniform %s has location %d\n", uniform_name.data, location);
        }
        free(uniform_name.data);
        uniform_name.size = 0;
    }
    self->handle = handle;
    return true;
}

/// Destroys the specified shader
void shader_destroy(Shader* self) {
    glDeleteProgram(self->handle);
}

/// Sets a sampler2d (texture) uniform
void shader_uniform_sampler(Shader* self, const char* name, u32 slot) {
    shader_uniform_s32(self, name, (s32) slot);
}

/// Sets an integer (s32) uniform
void shader_uniform_s32(Shader* self, const char* name, s32 value) {
    glUseProgram(self->handle);
    glUniform1i(glGetUniformLocation(self->handle, name), value);
}

/// Sets a 2d-integer (Vector2s) uniform
void shader_uniform_vector2s(Shader* self, const char* name, Vector2s* value) {
    glUseProgram(self->handle);
    glUniform2i(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets a 3d-integer (Vector3s) uniform
void shader_uniform_vector3s(Shader* self, const char* name, Vector3s* value) {
    glUseProgram(self->handle);
    glUniform3i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets a 4d-integer (Vector4s) uniform
void shader_uniform_vector4s(Shader* self, const char* name, Vector4s* value) {
    glUseProgram(self->handle);
    glUniform4i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a float (f32) uniform
void shader_uniform_f32(Shader* self, const char* name, f32 value) {
    glUseProgram(self->handle);
    glUniform1f(glGetUniformLocation(self->handle, name), value);
}

/// Sets a 2d-float (Vector2f) uniform
void shader_uniform_vector2f(Shader* self, const char* name, Vector2f* value) {
    glUseProgram(self->handle);
    glUniform2f(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets a 3d-float (Vector3f) uniform
void shader_uniform_vector3f(Shader* self, const char* name, Vector3f* value) {
    glUseProgram(self->handle);
    glUniform3f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets a 4d-float (Vector4f) uniform
void shader_uniform_vector4f(Shader* self, const char* name, Vector4f* value) {
    glUseProgram(self->handle);
    glUniform4f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a mat4 (Matrix4x4f) uniform
void shader_uniform_matrix4x4f(Shader* self, const char* name, Matrix4x4f* value) {
    glUseProgram(self->handle);
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, &value->value[0].x);
}

/// Binds the specified shader
void shader_bind(Shader* self) {
    glUseProgram(self->handle);
}

/// Unbinds the currently bound shader
void shader_unbind() {
    glUseProgram(0);
}

/// Retrieves the stride of the specified ShaderType
static s32 shader_type_stride(ShaderType type) {
    switch (type) {
        case INT:
            return sizeof(GLint);
        case INT2:
            return 2 * sizeof(GLint);
        case INT3:
            return 3 * sizeof(GLint);
        case INT4:
            return 4 * sizeof(GLint);
        case FLOAT:
            return sizeof(float);
        case FLOAT2:
            return 2 * sizeof(float);
        case FLOAT3:
            return 3 * sizeof(float);
        case FLOAT4:
            return 4 * sizeof(float);
        default:
            return 0;
    }
}

/// Translates libcore ShaderTypes to OpenGL types
static s32 shader_type_opengl(ShaderType type) {
    switch (type) {
        case INT:
        case INT2:
        case INT3:
        case INT4:
            return GL_INT;
        case FLOAT:
        case FLOAT2:
        case FLOAT3:
        case FLOAT4:
            return GL_FLOAT;
        default:
            return 0;
    }
}

/// Retrieves the number of primitives a composed ShaderType can hold
static s32 shader_type_primitives(ShaderType type) {
    switch (type) {
        case INT:
            return 1;
        case INT2:
            return 2;
        case INT3:
            return 3;
        case INT4:
            return 4;
        case FLOAT:
            return 1;
        case FLOAT2:
            return 2;
        case FLOAT3:
            return 3;
        case FLOAT4:
            return 4;
        default:
            return 0;
    }
}

// ===================================================================================
// VERTEX BUFFER
// ===================================================================================

/// Calculates the total stride of the buffer layout
static s32 vertex_buffer_layout_stride(VertexBufferLayout* layout) {
    s32 stride = 0;
    for (u32 i = 0; i < layout->count; i++) {
        ShaderType attribute = *(layout->attributes + i);
        stride += shader_type_stride(attribute);
    }
    return stride;
}

/// Creates a vertex buffer on the gpu
void vertex_buffer_create(VertexBuffer* self) {
    self->handle = 0;
    self->layout = NULL;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Destroys the vertex buffer
void vertex_buffer_destroy(VertexBuffer* self) {
    glDeleteBuffers(1, &self->handle);
    self->layout = NULL;
}

/// Sets the data for the specified buffer
void vertex_buffer_data(VertexBuffer* self, const void* data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

/// Sets the attribute layout for the specified buffer
void vertex_buffer_layout(VertexBuffer* self, VertexBufferLayout* layout) {
    self->layout = layout;
}

/// Binds the specified buffer
void vertex_buffer_bind(VertexBuffer* self) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound buffer
void vertex_buffer_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ===================================================================================
// INDEX BUFFER
// ===================================================================================

/// Creates an index buffer on the gpu
void index_buffer_create(IndexBuffer* self) {
    self->handle = 0;
    self->count = 0;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Destroys the index buffer
void index_buffer_destroy(IndexBuffer* self) {
    glDeleteBuffers(1, &self->handle);
}

/// Sets the data for the specified buffer
void index_buffer_data(IndexBuffer* self, const u32* data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    self->count = count;
}

/// Binds the specified buffer
void index_buffer_bind(IndexBuffer* self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound index buffer
void index_buffer_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

/// Creates a new vertex array
void vertex_array_create(VertexArray* self) {
    self->handle = 0;
    self->vertex_buffer = NULL;
    self->index_buffer = NULL;
    glGenVertexArrays(1, &self->handle);
    glBindVertexArray(self->handle);
}

/// Destroys the specified vertex array
void vertex_array_destroy(VertexArray* self) {
    glDeleteVertexArrays(1, &self->handle);
}

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
void vertex_array_vertex_buffer(VertexArray* self, VertexBuffer* vertex_buffer) {
    glBindVertexArray(self->handle);
    vertex_buffer_bind(vertex_buffer);

    s64 offset = 0;
    s32 stride = vertex_buffer_layout_stride(vertex_buffer->layout);
    for (u32 i = 0; i < vertex_buffer->layout->count; i++) {
        glEnableVertexAttribArray(i);
        ShaderType attribute = *(vertex_buffer->layout->attributes + i);
        GLenum opengl_type = (GLenum) shader_type_opengl(attribute);
        if (opengl_type == GL_FLOAT) {
            glVertexAttribPointer(i, shader_type_primitives(attribute), opengl_type, GL_FALSE, stride,
                                  (const void*) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void*) offset);
        }
        offset += shader_type_stride(attribute);
    }
    self->vertex_buffer = vertex_buffer;
}

/// Sets the index buffer for the vertex array
void vertex_array_index_buffer(VertexArray* self, IndexBuffer* index_buffer) {
    glBindVertexArray(self->handle);
    index_buffer_bind(index_buffer);
    self->index_buffer = index_buffer;
}

/// Binds the specified vertex array
void vertex_array_bind(VertexArray* self) {
    glBindVertexArray(self->handle);
}

/// Unbinds the currently bound vertex array
void vertex_array_unbind() {
    glBindVertexArray(0);
}
