#include "proto/EditorGraph.h"

#include "ImNodesEz.h"
#include "common/BinaryFileWriter.h"
#include "common/HeapAllocator.h"

using namespace spvgentwo;

proto::EditorGraph::EditorGraph(spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_module(HeapAllocator::instance(), spv::Version, _pLogger),
    m_pName(_pName),
    m_newFunctionPopup(),
    m_nodes(HeapAllocator::instance())
{
}

proto::EditorGraph::~EditorGraph()
{
    if (m_pCanvas != nullptr)
    {
        HeapAllocator::instance()->destruct(m_pCanvas);
        m_pCanvas = nullptr;
    }
}

void proto::EditorGraph::update()
{
    createCanvas();

    m_module.reset();
    // configure capabilities and extensions
    m_module.addCapability(spv::Capability::Shader);
    Function& main = m_module.addEntryPoint<void>(spv::ExecutionModel::Vertex, "main");
    m_pBB = &main.front();

    if (ImGui::Begin(m_pName, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImNodes::BeginCanvas(m_pCanvas);

        updateContextMenu();

        for (auto& node : m_nodes)
        {
            OpNodeExpr& expr = node.data().get();
            expr.setBasicBlock(m_pBB);

            expr.update();
        }

        ImNodes::EndCanvas();
    }
    ImGui::End();

    evaluateExprGraph();

    m_textView.update(m_module);
}

void proto::EditorGraph::clear()
{
    m_nodes.clear();
}

void proto::EditorGraph::save()
{
    BinaryFileWriter writer("proto.spv");
    m_module.write(&writer);
    system("spirv-dis proto.spv");
    assert(system("spirv-val proto.spv") == 0);
}

void proto::EditorGraph::createCanvas()
{
    if (m_pCanvas == nullptr)
    {
        m_pCanvas = HeapAllocator::instance()->construct<ImNodes::CanvasState>();
    }
}

void proto::EditorGraph::updateContextMenu()
{
    ImVec2 pos = ImGui::GetCursorPos();

    if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
    {
        bool anySelected = false;

        //for (const auto& node : m_nodes)
        //{
        //    anySelected |= node.data().get().isSelected();
        //}

        if (anySelected == false)
        {
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
            ImGui::OpenPopup("NodesContextMenu");
        }
    }

    if (ImGui::BeginPopup("NodesContextMenu"))
    {
        spvgentwo::ExprGraph<OpNodeExpr>::NodeType* pNode = nullptr;

        if (ImGui::MenuItem("Constant"))
        {
            pNode = m_nodes.emplace(OpNodeExpr{ pos, OpNodeType::Const });
        }

        if (ImGui::MenuItem("InputVar"))
        {
            pNode = m_nodes.emplace(OpNodeExpr{ pos, OpNodeType::InVar });
        }

        if (ImGui::MenuItem("OutputVar"))
        {
            pNode = m_nodes.emplace(OpNodeExpr{ pos, OpNodeType::OutVar });
        }

        if (ImGui::MenuItem("Add"))
        {
            pNode = m_nodes.emplace(OpNodeExpr{ pos, OpNodeType::Add });
        }

        if (pNode != nullptr)
        {
            auto& editorNode = pNode->data().get();
            editorNode.setBasicBlock(m_pBB);
            editorNode.setParent(&m_nodes, pNode);
            ImNodes::AutoPositionNode(&editorNode);
        }

        if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void proto::EditorGraph::evaluateExprGraph()
{
    m_nodes.resetEvaluationState();

    for(auto& node : m_nodes)
    {
        // output vars are cfg sinks
        if (node.data()->getType() == OpNodeType::OutVar)
        {
            spvgentwo::ExprGraph<OpNodeExpr>::evaluateRecursive<ExprArgs::FunctionPtrLists>(&node);
        }
    }
}
