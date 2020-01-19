#include "proto/ProtoWindow.h"

#include "imgui.h"
#include "ImNodes.h"
#include "ImNodesEz.h"

proto::ProtoWindow::ProtoWindow() : Window(),
    m_module(&m_alloc, spv::Version, &m_logger)
{
}

proto::ProtoWindow::~ProtoWindow()
{
}

bool proto::ProtoWindow::updateUI()
{
    if (updateMenu() == false)
        return false;

    static ImNodes::CanvasState canvas;

    if (ImGui::Begin("ImNodes", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImNodes::BeginCanvas(&canvas);

        struct Node
        {
            ImVec2 pos{};
            bool selected{};
            ImNodes::Ez::SlotInfo inputs[1];
            ImNodes::Ez::SlotInfo outputs[1];
        };

        static Node nodes[3] = {
            {{50, 100}, false, {{"In", 1}}, {{"Out", 1}}},
            {{250, 50}, false, {{"In", 1}}, {{"Out", 1}}},
            {{250, 100}, false, {{"In", 1}}, {{"Out", 1}}},
        };

        for (Node& node : nodes)
        {
            if (ImNodes::Ez::BeginNode(&node, "Node Title", &node.pos, &node.selected))
            {
                ImNodes::Ez::InputSlots(node.inputs, 1);
                ImNodes::Ez::OutputSlots(node.outputs, 1);
                ImNodes::Ez::EndNode();
            }
        }

        ImNodes::Connection(&nodes[1], "In", &nodes[0], "Out");
        ImNodes::Connection(&nodes[2], "In", &nodes[0], "Out");

        ImNodes::EndCanvas();
    }
    ImGui::End();

    //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    //bool show_demo_window;
    //bool show_another_window;
    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    //float f;
    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ////ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //static int counter = 0;

    //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //    counter++;
    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);

    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::End();

	return true;
}

bool proto::ProtoWindow::updateMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            updateMenuFile();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    return true;
}

void proto::ProtoWindow::updateMenuFile()
{
    
}
