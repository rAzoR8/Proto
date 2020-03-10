#include "proto/EditorGraph.h"

#include "ImNodesEz.h"
#include "proto/TypeWidgets.h"

using namespace spvgentwo;

proto::EditorGraph::EditorGraph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_pAlloc(_pAlloc),
    m_module(_pAlloc, spv::Version, _pLogger),
    m_pName(_pName),
    m_newFunctionPopup(_pAlloc)
{
}

proto::EditorGraph::~EditorGraph()
{
    if (m_pCanvas != nullptr)
    {
        m_pAlloc->destruct(m_pCanvas);
        m_pCanvas = nullptr;
    }
}

void proto::EditorGraph::update()
{
    createCanvas();

    m_module.reset();
    // configure capabilities and extensions
    m_module.addCapability(spv::Capability::Shader);

    if (ImGui::Begin(m_pName, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImNodes::BeginCanvas(m_pCanvas);

        updateContextMenu();

        // nodes from module
        updateNodes();

        for (auto& node : m_nodes)
        {
            node.data().get().update();
        }

        ImNodes::EndCanvas();
    }
    ImGui::End();

    m_textView.update(m_module);
}

void proto::EditorGraph::clear()
{
    //m_module.clear();
}

void proto::EditorGraph::save()
{
}

void proto::EditorGraph::createCanvas()
{
    if (m_pCanvas == nullptr)
    {
        m_pCanvas = m_pAlloc->construct<ImNodes::CanvasState>();
    }
}

void proto::EditorGraph::updateNodes()
{
    ImVec2 pos = ImGui::GetCursorPos();

    //updateNodeFromContainer(m_module.getEntryPoints(), EditorNode::Type::EntryPoint,
    //    [&](EntryPoint& f) // add node func
    //{
    //    EditorNode& newNode = m_nodes.emplace_back(m_pAlloc, "EntryPoint", ImVec2{}, &f);
    //    newNode.update();
    //    ImNodes::AutoPositionNode(&newNode);
    //}, [&](EditorNode& n) // Remove node func
    //{

    //});

    //EditorNode& bbNode = m_nodes.emplace(&bb, EditorNode(m_pAlloc, "BBNode", pos, &bb)).first->second;
    //ImNodes::AutoPositionNode(&bbNode);

    //if (f.size() == 1u)
    //{
    //    EditorNode::connect({ &bbNode, "FuncEntry", getNode(&f), "EntryBlock" });
    //}
}

void proto::EditorGraph::updateContextMenu()
{
    if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
    {
        bool anySelected = false;

        for (const auto& node : m_nodes)
        {
            anySelected |= node.data();
        }

        if (anySelected == false)
        {
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
            ImGui::OpenPopup("NodesContextMenu");
        }
    }

    if (ImGui::BeginPopup("NodesContextMenu"))
    {
        if (ImGui::MenuItem("Add Function"))
        {
            m_newFunctionPopup.show(true);
        }

        if (ImGui::MenuItem("Add EntryPoint"))
        {
            //addEntryPoint();
        }

        if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    m_newFunctionPopup.update(m_module);
}