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

#include "icons.h"
#include "render.h"

#include <cimgui.h>
#include <cimgui_impl.h>

#include <float.h>
#include <string.h>

/// Internal ID for the ImGui DockSpace
static const char* UI_DOCK_SPACE_ID = "##KopernikusDockSpace";


static void ui_setup_style() {
    ImGuiIO* io = igGetIO();

    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons = { 0 };
    icons.FontDataOwnedByAtlas = true;
    icons.OversampleH = 2;
    icons.OversampleV = 1;
    icons.GlyphMaxAdvanceX = FLT_MAX;
    icons.RasterizerMultiply = 1.0f;
    icons.EllipsisChar = (ImWchar) -1;
    icons.MergeMode = true;
    icons.PixelSnapH = true;

    f32 font_size = 15.0f;
    f32 font_size_icons = 2.0f / 3.0f * font_size;
    io->FontDefault = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/caskaydia-cove.ttf", font_size, nil, nil);
    ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/fa-regular-400.ttf", font_size_icons, &icons, icons_ranges);
    ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/fa-solid-900.ttf", font_size_icons, &icons, icons_ranges);

    ImGuiStyle* style = igGetStyle();
    style->Colors[ImGuiCol_WindowBg] = (ImVec4){ 0.1f, 0.1f, 0.13f, 1.0f };
    style->Colors[ImGuiCol_MenuBarBg] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Child
    style->Colors[ImGuiCol_ChildBg] = (ImVec4) { 0.16f, 0.16f, 0.21f, 1.0f };

    // Border
    style->Colors[ImGuiCol_Border] = (ImVec4){ 0.44f, 0.37f, 0.61f, 0.29f };
    style->Colors[ImGuiCol_BorderShadow] = (ImVec4){ 0.0f, 0.0f, 0.0f, 0.24f };

    // Text
    style->Colors[ImGuiCol_Text] = (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f };
    style->Colors[ImGuiCol_TextDisabled] = (ImVec4){ 0.5f, 0.5f, 0.5f, 1.0f };
    style->Colors[ImGuiCol_TextSelectedBg] = (ImVec4){ 0.90f, 0.44f, 0.89f, 1.0f };

    // Headers
    style->Colors[ImGuiCol_Header] = (ImVec4){ 0.13f, 0.13f, 0.17f, 1.0f };
    style->Colors[ImGuiCol_HeaderHovered] = (ImVec4){ 0.19f, 0.2f, 0.25f, 1.0f };
    style->Colors[ImGuiCol_HeaderActive] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Buttons
    style->Colors[ImGuiCol_Button] = (ImVec4){ 0.13f, 0.13f, 0.17f, 1.0f };
    style->Colors[ImGuiCol_ButtonHovered] = (ImVec4){ 0.19f, 0.2f, 0.25f, 1.0f };
    style->Colors[ImGuiCol_ButtonActive] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_CheckMark] = (ImVec4){ 0.74f, 0.58f, 0.98f, 1.0f };

    // Popups
    style->Colors[ImGuiCol_PopupBg] = (ImVec4){ 0.1f, 0.1f, 0.13f, 0.92f };

    // Slider
    style->Colors[ImGuiCol_SliderGrab] = (ImVec4){ 0.44f, 0.37f, 0.61f, 0.54f };
    style->Colors[ImGuiCol_SliderGrabActive] = (ImVec4){ 0.74f, 0.58f, 0.98f, 0.54f };

    // Frame BG
    style->Colors[ImGuiCol_FrameBg] = (ImVec4){ 0.13f, 0.13f, 0.17f, 1.0f };
    style->Colors[ImGuiCol_FrameBgHovered] = (ImVec4){ 0.19f, 0.2f, 0.25f, 1.0f };
    style->Colors[ImGuiCol_FrameBgActive] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Tabs
    style->Colors[ImGuiCol_Tab] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_TabHovered] = (ImVec4){ 0.24f, 0.24f, 0.32f, 1.0f };
    style->Colors[ImGuiCol_TabActive] = (ImVec4){ 0.2f, 0.22f, 0.27f, 1.0f };
    style->Colors[ImGuiCol_TabUnfocused] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_TabUnfocusedActive] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Title
    style->Colors[ImGuiCol_TitleBg] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_TitleBgActive] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_TitleBgCollapsed] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Scrollbar
    style->Colors[ImGuiCol_ScrollbarBg] = (ImVec4){ 0.1f, 0.1f, 0.13f, 1.0f };
    style->Colors[ImGuiCol_ScrollbarGrab] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = (ImVec4){ 0.19f, 0.2f, 0.25f, 1.0f };
    style->Colors[ImGuiCol_ScrollbarGrabActive] = (ImVec4){ 0.24f, 0.24f, 0.32f, 1.0f };

    // Separator
    style->Colors[ImGuiCol_Separator] = (ImVec4){ 0.44f, 0.37f, 0.61f, 1.0f };
    style->Colors[ImGuiCol_SeparatorHovered] = (ImVec4){ 0.74f, 0.58f, 0.98f, 1.0f };
    style->Colors[ImGuiCol_SeparatorActive] = (ImVec4){ 0.84f, 0.58f, 1.0f, 1.0f };

    // Resize Grip
    style->Colors[ImGuiCol_ResizeGrip] = (ImVec4){ 0.44f, 0.37f, 0.61f, 0.29f };
    style->Colors[ImGuiCol_ResizeGripHovered] = (ImVec4){ 0.74f, 0.58f, 0.98f, 0.29f };
    style->Colors[ImGuiCol_ResizeGripActive] = (ImVec4){ 0.84f, 0.58f, 1.0f, 0.29f };

    // Docking
    style->Colors[ImGuiCol_DockingPreview] = (ImVec4){ 0.44f, 0.37f, 0.61f, 1.0f };
    style->TabRounding = 4;
    style->ScrollbarRounding = 9;
    style->WindowRounding = 7;
    style->GrabRounding = 3;
    style->FrameRounding = 3;
    style->PopupRounding = 4;
    style->ChildRounding = 4;
}

/// Initializes the ui
void ui_initialize(Display* display) {
    igCreateContext(nil);
    ImGuiIO* io = igGetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ui_setup_style();

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

/// Begins a new UI window
b8 ui_window_begin(const char* label) {
    return igBegin(label, nil, ImGuiWindowFlags_None);
}

/// Ends the current window
void ui_window_end() {
    igEnd();
}

/// Begin the main menu
b8 ui_main_menu_begin() {
    return igBeginMainMenuBar();
}

/// End the main menu
void ui_main_menu_end() {
    igEndMainMenuBar();
}

/// Begin a menu
b8 ui_menu_begin(const char* label) {
    return igBeginMenu(label, true);
}

/// End the menu
void ui_menu_end() {
    igEndMenu();
}

/// Draw a menu item
b8 ui_menu_item(const char* title, const char* shortcut) {
    return igMenuItem_Bool(title, shortcut, false, true);
}

/// Draws text
void ui_text(const char* fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igTextV(fmt, list);
    va_end(list);
}

/// Draws wrapped text
void ui_text_wrapped(const char* fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igTextWrappedV(fmt, list);
    va_end(list);
}

/// Draw a note text
void ui_note(const char* fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igBeginDisabled(true);
    igTextWrappedV(fmt, list);
    igEndDisabled();
    va_end(list);
}

/// Draws a tooltip
void ui_tooltip(const char* fmt, ...) {
    igSetNextWindowSize((ImVec2){ 250.0f, 0.0f }, ImGuiCond_Always);
    igBeginTooltip();
    va_list list;
    va_start(list, fmt);
    igBeginDisabled(true);
    igTextWrappedV(fmt, list);
    igEndDisabled();
    va_end(list);
    igEndTooltip();
}

/// Draws a tooltip whenever the last item is hovered
void ui_tooltip_hovered(const char* fmt, ...) {
    if (ui_hovered()) {
        igSetNextWindowSize((ImVec2){ 250.0f, 0.0f }, ImGuiCond_Always);
        igBeginTooltip();
        va_list list;
        va_start(list, fmt);
        igBeginDisabled(true);
        igTextWrappedV(fmt, list);
        igEndDisabled();
        va_end(list);
        igEndTooltip();
    }
}

/// Draw a separator
void ui_separator() {
    igSeparator();
}

/// Draw a readonly text property
void ui_property_text_readonly(const char* property, const char* text) {
    igInputText(property, (char*) text, strlen(text), ImGuiInputTextFlags_ReadOnly, nil, nil);
}

/// Draw a readonly text property
void ui_property_number_readonly(const char* property, s64 x, const char* fmt) {
    igInputScalar(property, ImGuiDataType_S64, &x, nil, nil, fmt, ImGuiInputTextFlags_ReadOnly);
}

/// Draw a readonly real property
void ui_property_real_readonly(const char* property, f64 x, const char* fmt) {
    igInputScalar(property, ImGuiDataType_Double, &x, nil, nil, fmt, ImGuiInputTextFlags_ReadOnly);
}

/// Draws an icon at the end of the current line
static void ui_icon_end_of_line(const char* icon) {
    ImVec2 size;
    igGetContentRegionAvail(&size);
    igSameLine(size.x - igGetStyle()->FramePadding.x, -1.0f);
    ui_text("%s", icon);
}

/// Draws a tree node with an optional icon
b8 ui_tree_node_begin(const char* label, const char* icon, b8 selected) {
    ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;
    if (selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    ImGuiID id = igGetID_Str(label);
    b8 open = igTreeNodeBehavior(id, flags, label, nil);
    if (icon != nil) {
        ui_icon_end_of_line(icon);
    }
    return open;
}

/// Ends the current tree node
void ui_tree_node_end() {
    igTreePop();
}

/// Draws a tree node item with an optional icon
b8 ui_tree_item(const char* label, const char* icon, b8 selected) {
    ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Bullet;
    if (selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
        igPushStyleColor_Vec4(ImGuiCol_Text, igGetStyle()->Colors[ImGuiCol_TextSelectedBg]);
    }
    ImGuiID id = igGetID_Str(label);
    if (igTreeNodeBehavior(id, flags, label, nil)) {
        igTreePop();
    }
    b8 clicked = ui_selected();
    if (icon != nil) {
        ui_icon_end_of_line(icon);
    }
    if (selected) {
        igPopStyleColor(1);
    }
    return clicked;
}

/// Checks if the last ui item is selected
b8 ui_selected(void) {
    return igIsItemClicked(ImGuiMouseButton_Left);
}

/// Checks if the last ui item is hovered
b8 ui_hovered(void) {
    return igIsItemHovered(ImGuiHoveredFlags_None);
}

