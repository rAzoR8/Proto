#include "proto/ProtoWindow.h"

#include "imgui.h"

proto::ProtoWindow::ProtoWindow() : Window(),
    m_graph(&m_alloc, &m_logger, "Code Graph")
{
}

proto::ProtoWindow::~ProtoWindow()
{
}

bool proto::ProtoWindow::updateUI()
{
    updateMenu();

    updateGraphs();

	return m_shouldClose == false;
}

void proto::ProtoWindow::updateMenu()
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
}

void proto::ProtoWindow::updateMenuFile()
{
    if (ImGui::MenuItem("New"))
    {
        m_graph.clear();
    }

    if (ImGui::MenuItem("Save"))
    {
        m_graph.save();
    }

    if (ImGui::MenuItem("Quit", "Alt+F4"))
    {
        m_shouldClose = true;
    }
}

void proto::ProtoWindow::updateGraphs()
{
    m_graph.update();
}
