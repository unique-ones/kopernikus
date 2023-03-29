#include "display.h"
#include "render.h"
#include "planet.h"

int main(int argc, char **argv) {
    Display display = {};
    display_create(&display, "Kopernikus - Tracking Sequencer");

    render_create_context(&display);

    // Client initialization code
    ImNodes::SetNodeGridSpacePos(1, ImVec2(20.0f, 20.0f));

    while (display_running(&display)) {
        render_begin_frame();

        // Here goes the actual client rendering code
        ImGui::Begin("Tracking Sequencer");
        
        for (u32 i = 0; i < PLANET_COUNT; ++i) {
            Planet* planet = planets() + i;
            ImGui::Text("%s", planet->name);
            ImGui::Text("State: a=%f i=%f", planet->state.semi_major_axis, planet->state.inclination);
            ImGui::Text("Rate: a=%f i=%f", planet->rate.semi_major_axis, planet->rate.inclination);
        }

        ImNodes::BeginNodeEditor();
        ImNodes::BeginNode(1);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("planet node:)");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(2);
        ImGui::Text("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(3);
        ImGui::Indent(40);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::EndNodeEditor();

        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_end_frame();
        display_update(&display);
    }
    display_destroy(&display);
}