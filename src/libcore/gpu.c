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
#include <string.h>
#include <stddef.h>

#include "gpu.h"
#include "log.h"

// ===================================================================================
// SHADER SOURCE MACRO HELPERS
// ===================================================================================

#define SHADER_VERSION "#version 450 core\n"
#define DEFINE_SHADER_VARIABLE_IMPL(type, name) static type name
#define DEFINE_SHADER_VARIABLE(type, name) DEFINE_SHADER_VARIABLE_IMPL(type, name)
#define DEFINE_SHADER_IMPL(type, source) DEFINE_SHADER_VARIABLE(const char *, type) = SHADER_VERSION #source
#define DEFINE_SHADER(type, source) DEFINE_SHADER_IMPL(type, source)

// ===================================================================================
// VERTEX SHADER SOURCE
// ===================================================================================

// clang-format off
DEFINE_SHADER(SHADER_VERTEX,
layout(location = 0) in vec2 attrib_position;
layout(location = 1) in vec3 attrib_color;
layout(location = 0) out vec3 passed_color;

// matrix for transforming vertex
uniform mat4 uniform_transform;

void main() {
    gl_Position = uniform_transform * vec4(attrib_position, 0.0, 1.0);
    passed_color = attrib_color;
});
// clang-format on

// ===================================================================================
// FRAGMENT SHADER SOURCE
// ===================================================================================

// clang-format off
DEFINE_SHADER(SHADER_FRAGMENT,
layout(location = 0) out vec4 output_color;
layout(location = 0) in vec3 passed_color;

void main() { 
    output_color = vec4(passed_color, 1.0);
});
// clang-format on

// ===================================================================================
// SHADER
// ===================================================================================

/// Compiles the given shader source to a shader program
static u32 shader_compile(const char *source, u32 type) {
    u32 program = glCreateShader(type);
    const GLchar *shader_source = source;
    glShaderSource(program, 1, &shader_source, nil);
    glCompileShader(program);

    s32 success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_length);

        StringBuffer failure_info;
        failure_info.data = (char *) malloc((size_t) info_length);
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
b8 shader_create(Shader *self, const char *vertex, const char *fragment) {
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

        StringBuffer failure_info;
        failure_info.data = (char *) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetProgramInfoLog(handle, info_length, nil, failure_info.data);
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
        StringBuffer uniform_name;
        uniform_name.data = (char *) malloc(uniform_length);
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
void shader_destroy(Shader *self) {
    glDeleteProgram(self->handle);
}

/// Sets a sampler2d (texture) uniform
void shader_uniform_sampler(Shader *self, const char *name, u32 slot) {
    shader_uniform_s32(self, name, (s32) slot);
}

/// Sets an integer (s32) uniform
void shader_uniform_s32(Shader *self, const char *name, s32 value) {
    glUseProgram(self->handle);
    glUniform1i(glGetUniformLocation(self->handle, name), value);
}

/// Sets a 2d-integer (Vector2s) uniform
void shader_uniform_vector2s(Shader *self, const char *name, Vector2s *value) {
    glUseProgram(self->handle);
    glUniform2i(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets a 3d-integer (Vector3s) uniform
void shader_uniform_vector3s(Shader *self, const char *name, Vector3s *value) {
    glUseProgram(self->handle);
    glUniform3i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets a 4d-integer (Vector4s) uniform
void shader_uniform_vector4s(Shader *self, const char *name, Vector4s *value) {
    glUseProgram(self->handle);
    glUniform4i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a float (f32) uniform
void shader_uniform_f32(Shader *self, const char *name, f32 value) {
    glUseProgram(self->handle);
    glUniform1f(glGetUniformLocation(self->handle, name), value);
}

/// Sets a 2d-float (Vector2f) uniform
void shader_uniform_vector2f(Shader *self, const char *name, Vector2f *value) {
    glUseProgram(self->handle);
    glUniform2f(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets a 3d-float (Vector3f) uniform
void shader_uniform_vector3f(Shader *self, const char *name, Vector3f *value) {
    glUseProgram(self->handle);
    glUniform3f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets a 4d-float (Vector4f) uniform
void shader_uniform_vector4f(Shader *self, const char *name, Vector4f *value) {
    glUseProgram(self->handle);
    glUniform4f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a mat4 (Matrix4x4f) uniform
void shader_uniform_matrix4x4f(Shader *self, const char *name, Matrix4x4f *value) {
    glUseProgram(self->handle);
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, &value->value[0].x);
}

/// Binds the specified shader
void shader_bind(Shader *self) {
    glUseProgram(self->handle);
}

/// Unbinds the currently bound shader
void shader_unbind(void) {
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
static s32 vertex_buffer_layout_stride(VertexBufferLayout *layout) {
    s32 stride = 0;
    for (u32 i = 0; i < layout->count; i++) {
        ShaderType attribute = *(layout->attributes + i);
        stride += shader_type_stride(attribute);
    }
    return stride;
}

/// Creates a vertex buffer on the gpu
void vertex_buffer_create(VertexBuffer *self) {
    self->handle = 0;
    self->layout = nil;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Destroys the vertex buffer
void vertex_buffer_destroy(VertexBuffer *self) {
    glDeleteBuffers(1, &self->handle);
    self->layout = nil;
}

/// Sets the data for the specified buffer
void vertex_buffer_data(VertexBuffer *self, const void *data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

/// Sets the attribute layout for the specified buffer
void vertex_buffer_layout(VertexBuffer *self, VertexBufferLayout *layout) {
    self->layout = layout;
}

/// Binds the specified buffer
void vertex_buffer_bind(VertexBuffer *self) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound buffer
void vertex_buffer_unbind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ===================================================================================
// INDEX BUFFER
// ===================================================================================

/// Creates an index buffer on the gpu
void index_buffer_create(IndexBuffer *self) {
    self->handle = 0;
    self->count = 0;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Destroys the index buffer
void index_buffer_destroy(IndexBuffer *self) {
    glDeleteBuffers(1, &self->handle);
}

/// Sets the data for the specified buffer
void index_buffer_data(IndexBuffer *self, const u32 *data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    self->count = count;
}

/// Binds the specified buffer
void index_buffer_bind(IndexBuffer *self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound index buffer
void index_buffer_unbind(void) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

/// Creates a new vertex array
void vertex_array_create(VertexArray *self) {
    self->handle = 0;
    self->vertex_buffer = nil;
    self->index_buffer = nil;
    glGenVertexArrays(1, &self->handle);
    glBindVertexArray(self->handle);
}

/// Destroys the specified vertex array
void vertex_array_destroy(VertexArray *self) {
    glDeleteVertexArrays(1, &self->handle);
}

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
void vertex_array_vertex_buffer(VertexArray *self, VertexBuffer *vertex_buffer) {
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
                                  (const void *) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void *) offset);
        }
        offset += shader_type_stride(attribute);
    }
    self->vertex_buffer = vertex_buffer;
}

/// Sets the index buffer for the vertex array
void vertex_array_index_buffer(VertexArray *self, IndexBuffer *index_buffer) {
    glBindVertexArray(self->handle);
    index_buffer_bind(index_buffer);
    self->index_buffer = index_buffer;
}

/// Binds the specified vertex array
void vertex_array_bind(VertexArray *self) {
    glBindVertexArray(self->handle);
}

/// Unbinds the currently bound vertex array
void vertex_array_unbind(void) {
    glBindVertexArray(0);
}

/// Creates a frame buffer of specified size
b8 frame_buffer_create(FrameBuffer *self, FrameBufferInfo *spec) {
    self->handle = 0;
    self->texture_handle = 0;
    self->render_handle = 0;
    self->spec = *spec;
    return frame_buffer_invalidate(self);
}

/// Destroys the frame buffer
void frame_buffer_destroy(FrameBuffer *self) {
    glDeleteFramebuffers(1, &self->handle);
    glDeleteTextures(1, &self->texture_handle);
    glDeleteRenderbuffers(1, &self->render_handle);
}

/// Checks if the frame buffer is complete
static b8 frame_buffer_is_valid(FrameBuffer *buffer) {
    frame_buffer_bind(buffer);
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/// Invalidates the frame buffer, this needs to be called whenever the frame buffer is resized
b8 frame_buffer_invalidate(FrameBuffer *self) {
    if (self->handle) {
        glDeleteFramebuffers(1, &self->handle);
        glDeleteTextures(1, &self->texture_handle);
        glDeleteRenderbuffers(1, &self->render_handle);
    }

    glGenFramebuffers(1, &self->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);

    glGenTextures(1, &self->texture_handle);
    glBindTexture(GL_TEXTURE_2D, self->texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, self->spec.internal_format, self->spec.width, self->spec.height, 0,
                 self->spec.pixel_format, self->spec.pixel_type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture_handle, 0);

    glGenRenderbuffers(1, &self->render_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, self->render_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, self->spec.width, self->spec.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, self->render_handle);

    if (!frame_buffer_is_valid(self)) {
        flogf(stderr, "[framebuffer] invalid frame buffer\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

/// Resizes the frame buffer
b8 frame_buffer_resize(FrameBuffer *self, s32 width, s32 height) {
    if (width <= 0 || height <= 0 || (width == self->spec.width && height == self->spec.height)) {
        return false;
    }
    self->spec.width = width;
    self->spec.height = height;
    return frame_buffer_invalidate(self);
}

/// Binds the specified frame buffer for rendering
void frame_buffer_bind(FrameBuffer *self) {
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);
    glViewport(0, 0, self->spec.width, self->spec.height);
}

/// Binds the texture of the frame buffer at the specified sampler slot
void frame_buffer_bind_texture(FrameBuffer *self, u32 slot) {
    glBindTextureUnit(slot, self->texture_handle);
}

/// Unbinds the currently bound frame buffer
void frame_buffer_unbind(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// Creates a new render command
RenderCommand *render_command_new(MemoryArena *arena, Vertex *vertices, Index *indices) {
    RenderCommand *self = (RenderCommand *) memory_arena_alloc(arena, sizeof(RenderCommand));
    self->previous = nil;
    self->next = nil;
    memcpy(self->vertices, vertices, sizeof self->vertices);
    memcpy(self->indices, indices, sizeof self->indices);
    return self;
}

/// Creates a new render group
void render_group_create(RenderGroup *self) {
    self->begin = nil;
    self->end = nil;
    self->commands = 0;
    self->command_memory = memory_arena_identity(ALIGNMENT8);

    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);

    /// Attributes are `attrib_position` and `attrib_color`
    static ShaderType attributes[] = { FLOAT2, FLOAT3 };
    static VertexBufferLayout layout = { .attributes = attributes, .count = ARRAY_SIZE(attributes) };

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);
}

/// Destroys the specified render group (i.e. delete the commands and free memory)
void render_group_destroy(RenderGroup *self) {
    render_group_clear(self);
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
}

/// Clears the specified render group (i.e. deletes the commands)
void render_group_clear(RenderGroup *self) {
    self->begin = nil;
    self->end = nil;
    self->commands = 0;
    memory_arena_destroy(&self->command_memory);
    self->command_memory = memory_arena_identity(ALIGNMENT8);
}

/// Pushes a set of vertices and indices to the render group
void render_group_push(RenderGroup *self, Vertex *vertices, Index *indices) {
    RenderCommand *command = render_command_new(&self->command_memory, vertices, indices);
    if (self->begin == nil) {
        self->begin = command;
        self->end = command;
        self->commands++;
        return;
    }

    self->end->next = command;
    command->previous = self->end;
    self->end = command;
    self->commands++;
}

/// Submits an actual indexed OpenGL draw call to the GPU
static void renderer_draw_indexed(VertexArray *vertex_array, Shader *shader, u32 mode) {
    vertex_array_bind(vertex_array);
    shader_bind(shader);
    glDrawElements(mode, (s32) vertex_array->index_buffer->count, GL_UNSIGNED_INT, NULL);
    vertex_array_unbind();
}

/// Clears the currently bound frame buffer
void renderer_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// Sets the clear color
void renderer_clear_color(Vector4f *color) {
    glClearColor(color->x, color->y, color->z, color->w);
}

/// Creates a new renderer and initializes its pipeline
void renderer_create(Renderer *self, f32 width, f32 height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_create(&self->shader, SHADER_VERTEX, SHADER_FRAGMENT);
    render_group_create(&self->group);

    FrameBufferInfo info = { 0 };
    info.width = width;
    info.height = height;
    info.internal_format = GL_RGBA16F;
    info.pixel_type = GL_FLOAT;
    info.pixel_format = GL_RGB;

    frame_buffer_create(&self->capture, &info);
}

/// Destroys the specified renderer
void renderer_destroy(Renderer *self) {
    shader_destroy(&self->shader);
    render_group_destroy(&self->group);
    frame_buffer_destroy(&self->capture);
}

/// Begins a renderer batch by resetting all render groups
void renderer_begin_batch(Renderer *self) {
    render_group_clear(&self->group);
}

/// Submits the specified render group and issues an indexed draw call
static void render_group_submit(RenderGroup *group, Shader *shader) {
    if (group->commands == 0) {
        return;
    }

    usize vertices_size = RENDERER_QUAD_VERTICES * sizeof(Vertex);
    usize indices_size = RENDERER_QUAD_INDICES * sizeof(Index);
    Vertex *vertices = (Vertex *) memory_arena_alloc(&group->command_memory, vertices_size * group->commands);
    Index *indices = (Index *) memory_arena_alloc(&group->command_memory, indices_size * group->commands);

    usize insert_index = 0;
    for (RenderCommand *it = group->begin; it != NULL; it = it->next) {
        memcpy((u8 *) vertices + (ptrdiff_t) (vertices_size * insert_index), it->vertices, vertices_size);
        memcpy((u8 *) indices + (ptrdiff_t) (indices_size * insert_index), it->indices, indices_size);
        insert_index++;
    }

    vertex_buffer_data(&group->vertex_buffer, vertices, group->commands * vertices_size);
    index_buffer_data(&group->index_buffer, indices, group->commands * RENDERER_QUAD_INDICES);
    renderer_draw_indexed(&group->vertex_array, shader, GL_TRIANGLES);
}

/// Ends a renderer batch by submitting the commands of all render groups
void renderer_end_batch(Renderer *self) {
    render_group_submit(&self->group, &self->shader);
}

/// Indicate to the renderer that a resize is necessary
void renderer_resize(Renderer *self, s32 width, s32 height) {
    Matrix4x4f orthogonal;
    matrix4x4f_create_orthogonal(&orthogonal, 0.0f, (f32) width, (f32) height, 0.0f);
    shader_uniform_matrix4x4f(&self->shader, "uniform_transform", &orthogonal);

    // frame buffer requires resize
    frame_buffer_resize(&self->capture, width, height);
}

/// Draws a quad at the given position
void renderer_draw_quad(Renderer *self, Vector2f *position, Vector2f *size, Vector3f *color) {
    Vertex vertices[] = { { .position = { position->x, position->y }, .color = *color },
                          { .position = { position->x, position->y + size->y }, .color = *color },
                          { .position = { position->x + size->x, position->y + size->y }, .color = *color },
                          { .position = { position->x + size->x, position->y }, .color = *color } };

    u32 index_offset = self->group.commands * 4;
    Index indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                        2 + index_offset, 0 + index_offset, 3 + index_offset };
    render_group_push(&self->group, vertices, indices);
}

/// Captures all following draw commands into a frame buffer
void renderer_begin_capture(Renderer *self) {
    frame_buffer_bind(&self->capture);
}

/// Ends the capture of draw commands
void renderer_end_capture(Renderer *self) {
    frame_buffer_unbind();
}