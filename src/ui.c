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
#include "browser.h"

#include <cimgui.h>
#include <cimgui_impl.h>

#include <cimnodes.h>
#include <cimplot.h>

#include <float.h>
#include <string.h>

/// Macro for generating a ImU32 color
/// #define IM_COL32_R_SHIFT    0
#define UI_COLOR32(R, G, B, A)                                                                      \
    (((ImU32) ((A) *255.0f) << 24) | ((ImU32) ((B) *255.0f) << 16) | ((ImU32) ((G) *255.0f) << 8) | \
     ((ImU32) ((R) *255.0f) << 0))

/// Internal ID for the ImGui DockSpace
static const char *UI_DOCK_SPACE_ID = "##KopernikusDockSpace";


static void ui_setup_style(void) {
    ImGuiIO *io = igGetIO();

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

    f32 font_size = 16.0f;
    f32 font_size_icons = 2.0f / 3.0f * font_size;
    io->FontDefault = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/0xproto.ttf", font_size, nil, nil);
    ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/fa-regular-400.ttf", font_size_icons, &icons, icons_ranges);
    ImFontAtlas_AddFontFromFileTTF(io->Fonts, "data/fonts/fa-solid-900.ttf", font_size_icons, &icons, icons_ranges);

    ImGuiStyle *style = igGetStyle();
    style->Colors[ImGuiCol_WindowBg] = (ImVec4){ 0.1f, 0.1f, 0.13f, 1.0f };
    style->Colors[ImGuiCol_MenuBarBg] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Child
    style->Colors[ImGuiCol_ChildBg] = (ImVec4){ 0.16f, 0.16f, 0.21f, 1.0f };

    // Border
    style->Colors[ImGuiCol_Border] = (ImVec4){ 0.44f, 0.37f, 0.61f, 0.29f };
    style->Colors[ImGuiCol_BorderShadow] = (ImVec4){ 0.0f, 0.0f, 0.0f, 0.24f };

    // Text
    style->Colors[ImGuiCol_Text] = (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f };
    style->Colors[ImGuiCol_TextDisabled] = (ImVec4){ 0.5f, 0.5f, 0.5f, 1.0f };
    style->Colors[ImGuiCol_TextSelectedBg] = (ImVec4){ 1.0f, 1.0f, 0.0f, 1.0f };

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

    ImNodesStyle *imnodes_style = imnodes_GetStyle();
    imnodes_style->Colors[ImNodesCol_Link] = UI_COLOR32(0.8f, 0.8f, 0.0f, 1.0f);
    imnodes_style->Colors[ImNodesCol_LinkHovered] = UI_COLOR32(1.0f, 1.0f, 0.0f, 1.0f);
    imnodes_style->Colors[ImNodesCol_LinkSelected] = UI_COLOR32(0.9f, 0.9f, 0.0f, 1.0f);
    imnodes_style->Colors[ImNodesCol_TitleBar] = UI_COLOR32(0.55f, 0.0f, 0.75f, 1.0f);
    imnodes_style->Colors[ImNodesCol_TitleBarHovered] = UI_COLOR32(0.60f, 0.0f, 0.8f, 1.0f);
    imnodes_style->Colors[ImNodesCol_TitleBarSelected] = UI_COLOR32(0.57f, 0.0f, 0.77f, 1.0f);
    imnodes_style->Flags |= ImNodesStyleFlags_GridLinesPrimary;
}

/// Initializes the ui
void ui_initialize(Display *display) {
    igCreateContext(nil);
    imnodes_CreateContext();
    ImPlot_CreateContext();

    ImGuiIO *io = igGetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ui_setup_style();

    ImGui_ImplGlfw_InitForOpenGL(display->handle, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    igClearIniSettings();
    igLoadIniSettingsFromDisk("data/setup.ini");

    imnodes_PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick |
                              ImNodesAttributeFlags_EnableLinkCreationOnSnap);

    ImNodesIO *nodes_io = imnodes_GetIO();
    nodes_io->LinkDetachWithModifierClick.Modifier = &io->KeyCtrl;
    nodes_io->MultipleSelectModifier.Modifier = &io->KeyCtrl;
}

/// Destroys the ui context
void ui_destroy(void) {
    imnodes_PopAttributeFlag();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot_DestroyContext(nil);
    imnodes_DestroyContext(nil);
    igDestroyContext(nil);
}

/// Begins a new ui draw pass
void ui_begin(void) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    ImGuiViewport *viewport = igGetMainViewport();
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
void ui_end(void) {
    igEnd();
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

    GLFWwindow *backup_context = glfwGetCurrentContext();
    igUpdatePlatformWindows();
    igRenderPlatformWindowsDefault(nil, nil);
    glfwMakeContextCurrent(backup_context);
}

/// Begins a new UI window
b8 ui_window_begin(const char *label, b8 *open) {
    if (open != nil && !*open) {
        return false;
    }

    b8 result = igBegin(label, (bool *) open, ImGuiWindowFlags_None);
    if (!result) {
        igEnd();
    }
    return result;
}

/// Ends the current window
void ui_window_end(void) {
    igEnd();
}

/// Begin the main menu
b8 ui_main_menu_begin(void) {
    return igBeginMainMenuBar();
}

/// End the main menu
void ui_main_menu_end(void) {
    igEndMainMenuBar();
}

/// Begin a menu
b8 ui_menu_begin(const char *label) {
    return igBeginMenu(label, true);
}

/// End the menu
void ui_menu_end(void) {
    igEndMenu();
}

/// Draw a menu item
b8 ui_menu_item(const char *title, const char *shortcut) {
    return igMenuItem_Bool(title, shortcut, false, true);
}

/// Draws text
void ui_text(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igTextV(fmt, list);
    va_end(list);
}

/// Draws wrapped text
void ui_text_wrapped(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igTextWrappedV(fmt, list);
    va_end(list);
}

/// Draw a note text
void ui_note(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    igBeginDisabled(true);
    igTextWrappedV(fmt, list);
    igEndDisabled();
    va_end(list);
}

/// Draws a tooltip
void ui_tooltip(const char *fmt, ...) {
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
void ui_tooltip_hovered(const char *fmt, ...) {
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
void ui_separator(void) {
    igSeparator();
}

/// Draws an icon at the end of the current line
static void ui_icon_end_of_line(const char *icon) {
    ImVec2 size;
    igGetContentRegionAvail(&size);
    igSameLine(size.x - igGetStyle()->FramePadding.x, -1.0f);
    ui_text("%s", icon);
}

/// Draw a selectable item
b8 ui_selectable(const char *label, const char *icon) {
    b8 selected = igSelectable_Bool(label, false, ImGuiSelectableFlags_None, (ImVec2){ 0.0f, 0.0f });
    if (icon) {
        ui_icon_end_of_line(icon);
    }
    return selected;
}

/// Begins setting the width of the next items
void ui_item_width_begin(f32 width) {
    igPushItemWidth(width);
}

/// Ends setting the width of the next items
void ui_item_width_end(void) {
    igPopItemWidth();
}

/// Draw a readonly text property
void ui_property_text_readonly(const char *property, const char *text) {
    igInputText(property, (char *) text, strlen(text), ImGuiInputTextFlags_ReadOnly, nil, nil);
}

/// Draw a number property
b8 ui_property_number(const char *property, s64 *x, const char *fmt) {
    return igInputScalar(property, ImGuiDataType_S64, x, nil, nil, fmt, ImGuiInputTextFlags_None);
}

/// Draw a readonly text property
void ui_property_number_readonly(const char *property, s64 x, const char *fmt) {
    igInputScalar(property, ImGuiDataType_S64, &x, nil, nil, fmt, ImGuiInputTextFlags_ReadOnly);
}

/// Draw a real property
b8 ui_property_real(const char *property, f64 *x, const char *fmt) {
    return igInputScalar(property, ImGuiDataType_Double, x, nil, nil, fmt, ImGuiInputTextFlags_None);
}

/// Draw a readonly real property
void ui_property_real_readonly(const char *property, f64 x, const char *fmt) {
    igInputScalar(property, ImGuiDataType_Double, &x, nil, nil, fmt, ImGuiInputTextFlags_ReadOnly);
}

/// Draw a searchbar
b8 ui_searchbar(StringBuffer *buffer, const char *label, const char *placeholder) {
    ImVec2 available;
    igGetContentRegionAvail(&available);
    return igInputTextEx(label, placeholder, buffer->data, (s32) buffer->size, (ImVec2){ available.x, 0.0f },
                         ImGuiInputTextFlags_None, nil, nil);
}

/// Draw a combobox
b8 ui_combobox(const char *title, s32 *selected, const char **items, usize length) {
    return igCombo_Str_arr(title, selected, items, length, -1);
}

/// Draws a tree node with an optional icon
b8 ui_tree_node_begin(const char *label, const char *icon, b8 selected) {
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
void ui_tree_node_end(void) {
    igTreePop();
}

/// Draws a tree node item with an optional icon
b8 ui_tree_item(const char *label, const char *icon, b8 selected) {
    ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Bullet;
    if (selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
        igPushStyleColor_Vec4(ImGuiCol_Text, igGetStyle()->Colors[ImGuiCol_TextSelectedBg]);
    }
    if (igTreeNodeBehavior(igGetID_Str(label), flags, label, nil)) {
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

/// Draws a tree node item that is draggable with an optional icon
b8 ui_tree_item_drag_drop_source(const char *label, const char *icon, b8 selected, void *data, usize size) {
    ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Bullet;
    if (selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
        igPushStyleColor_Vec4(ImGuiCol_Text, igGetStyle()->Colors[ImGuiCol_TextSelectedBg]);
    }
    if (igTreeNodeBehavior(igGetID_Str(label), flags, label, nil)) {
        igTreePop();
    }
    if (igBeginDragDropSource(ImGuiDragDropFlags_None)) {
        igSetDragDropPayload(object_browser_payload_id(), data, size, ImGuiCond_None);
        ui_text("%s", label);
        igEndDragDropSource();
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

/// Open the popup with the specified ID
void ui_popup_open(const char *id) {
    igOpenPopup_Str(id, ImGuiPopupFlags_None);
}

/// Begin a popup
b8 ui_popup_begin(const char *id) {
    return igBeginPopup(id, ImGuiWindowFlags_None);
}

/// Begin a node editor
void ui_node_editor_begin(void) {
    imnodes_BeginNodeEditor();
}

/// End a node editor
void ui_node_editor_end(void) {
    imnodes_EndNodeEditor();
}

/// Check whether a node editor ection is present (right click)
b8 ui_node_editor_action(void) {
    return imnodes_IsEditorHovered() && ui_click_right();
}

/// Begin a node item
void ui_node_begin(s32 id) {
    imnodes_BeginNode(id);
}

/// End a node item
void ui_node_end(void) {
    imnodes_EndNode();
}

/// Checks if the last ui item is selected
b8 ui_selected(void) {
    return igIsItemClicked(ImGuiMouseButton_Left);
}

/// Check if the left mouse button is clicked
b8 ui_click_left(void) {
    return igIsMouseClicked_Bool(ImGuiMouseButton_Left, false);
}

/// Check if the right mouse button is clicked
b8 ui_click_right(void) {
    return igIsMouseClicked_Bool(ImGuiMouseButton_Right, false);
}

/// Checks if the last ui item is hovered
b8 ui_hovered(void) {
    return igIsItemHovered(ImGuiHoveredFlags_None);
}

/// Keep the current line of the UI cursor
void ui_keep_line(void) {
    igSameLine(0.0f, -1.0f);
}
