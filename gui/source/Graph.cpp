#include "proto/Graph.h"

#include "ImNodesEz.h"

proto::Graph::Graph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_pAlloc(_pAlloc),
    m_module(_pAlloc, spv::Version, _pLogger),
    m_pName(_pName),
    m_nodes(_pAlloc)
{
}

proto::Graph::~Graph()
{
    if (m_pCanvas != nullptr)
    {
        m_pAlloc->destruct(m_pCanvas);
        m_pCanvas = nullptr;
    }
}

void proto::Graph::update()
{
    createCanvas();

    if (ImGui::Begin(m_pName, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImNodes::BeginCanvas(m_pCanvas);

        // re-create nodes from module
        createNodes();

        for (Node& n : m_nodes)
        {
            n.update();
        }

        /*struct Node
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
        ImNodes::Connection(&nodes[2], "In", &nodes[0], "Out");*/

        ImNodes::EndCanvas();
    }
    ImGui::End();
}

void proto::Graph::clear()
{
}

void proto::Graph::save()
{
    //m_module.clear();
}

void proto::Graph::createCanvas()
{
    if (m_pCanvas == nullptr)
    {
        m_pCanvas = m_pAlloc->construct<ImNodes::CanvasState>();
    }
}

void proto::Graph::createNodes()
{
    //m_nodes.clear();

    using namespace spvgentwo;
    for (Function& f : m_module.getFunctions())
    {
    
    }
}
