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

#include "ui.h"

#include <cimgui.h>
#include <cimgui_impl.h>

/// Internal ID for the ImGui DockSpace
static const char* UI_DOCK_SPACE_ID = "##KopernikusDockSpace";

/// Font mapping
typedef struct FontMap {
    ImFont* light;
    ImFont* regular;
    ImFont* semi_bold;
    ImFont* bold;
    ImFont* italic;
} FontMap;

/// Internal Font mapping structure
static FontMap UI_FONT_MAP;

/// Retrieves the font by style
static ImFont* ui_font_by_style(FontStyle style) {
    switch (style) {
        case LIGHT:
            return UI_FONT_MAP.light;
        case REGULAR:
            return UI_FONT_MAP.regular;
        case SEMI_BOLD:
            return UI_FONT_MAP.semi_bold;
        case BOLD:
            return UI_FONT_MAP.bold;
        case ITALIC:
            return UI_FONT_MAP.italic;
    }
}

static void ui_setup_fonts() {
    ImGuiIO* io = igGetIO();
    const f32 font_size = 15.0f;
    UI_FONT_MAP.light = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/CaskaydiaCoveNerdFontMono-Light.ttf", font_size, nil, nil);
    UI_FONT_MAP.regular = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/CaskaydiaCoveNerdFontMono-Regular.ttf", font_size, nil, nil);
    UI_FONT_MAP.semi_bold = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/CaskaydiaCoveNerdFontMono-SemiBold.ttf", font_size, nil, nil);
    UI_FONT_MAP.bold = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/CaskaydiaCoveNerdFontMono-Bold.ttf", font_size, nil, nil);
    UI_FONT_MAP.italic = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/CaskaydiaCoveNerdFontMono-Italic.ttf", font_size, nil, nil);
    io->FontDefault = UI_FONT_MAP.regular;
}

/// Initializes the ui
void ui_initialize(Display* display) {
    igCreateContext(nil);
    ImGuiIO* io = igGetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ui_setup_fonts();

    ImGui_ImplGlfw_InitForOpenGL(display->handle, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

/// Destroys the ui context
void ui_destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(nil);
}

/// Begins a new ui draw pass
void ui_begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    ImGuiViewport* viewport = igGetMainViewport();
    igSetNextWindowPos(viewport->Pos, 0, (ImVec2){ 0.0f, 0.0f });
    igSetNextWindowSize(viewport->Size, 0);
    igPushStyleVar_Float(ImGuiStyleVar_WindowRounding, 0.0f);
    igPushStyleVar_Float(ImGuiStyleVar_WindowBorderSize, 0.0f);

    static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
        windowFlags |= ImGuiWindowFlags_NoBackground;
    }

    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2){ 0.0f, 0.0f });

    static bool dockSpaceOpen = true;
    igBegin(UI_DOCK_SPACE_ID, &dockSpaceOpen, windowFlags);
    igPopStyleVar(3);
    igDockSpace(igGetID_Str(UI_DOCK_SPACE_ID), (ImVec2){ 0.0f, 0.0f }, dockSpaceFlags, nil);
}

/// Ends the current ui draw pass
void ui_end() {
    igEnd();
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

    GLFWwindow* backup_context = glfwGetCurrentContext();
    igUpdatePlatformWindows();
    igRenderPlatformWindowsDefault(nil, nil);
    glfwMakeContextCurrent(backup_context);
}
