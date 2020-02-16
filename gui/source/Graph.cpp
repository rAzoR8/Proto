#include "proto/Graph.h"

#include "ImNodesEz.h"
#include "proto/TypeWidgets.h"

using namespace spvgentwo;

proto::Graph::Graph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_pAlloc(_pAlloc),
    m_module(_pAlloc, spv::Version, _pLogger),
    m_pName(_pName),
    m_newFunctionPopup(_pAlloc)
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

    m_module.reset();
    // configure capabilities and extensions
    m_module.addCapability(spv::Capability::Shader);

    if (ImGui::Begin(m_pName, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImNodes::BeginCanvas(m_pCanvas);

        updateContextMenu();

        // nodes from module
        updateNodes();

        for (auto& [spv, node] : m_nodes)
        {
            node.update();
        }

        ImNodes::EndCanvas();
    }
    ImGui::End();

    m_textView.update(m_module);
}

void proto::Graph::clear()
{
    //m_module.clear();
}

void proto::Graph::save()
{
}

void proto::Graph::createCanvas()
{
    if (m_pCanvas == nullptr)
    {
        m_pCanvas = m_pAlloc->construct<ImNodes::CanvasState>();
    }
}

void proto::Graph::updateNodes()
{
    ImVec2 pos = ImGui::GetCursorPos();

    updateNodeFromContainer(m_module.getFunctions(), Type::Function, 
        [&](Function& func) // add node func
    {
        Node& funcNode = m_nodes.emplace(&func, Node(m_pAlloc, "FuncNode", pos, &func)).first->second;
        ImNodes::AutoPositionNode(&funcNode);
    },  [&](Node& fNode) // Remove node func
    {
        for (BasicBlock& bb : *fNode.getSpvObj().obj.func)
        {
            //auto it = m_nodes.find_if([&bb](const Node& n) {return n.getSpvObj() == &bb; });
            //if (it != nullptr)
            //{
            //    m_nodes.erase(it);
            //}
        }
    });

    for (Function& f : m_module.getFunctions())
    {
        updateNodeFromContainer(f, Type::BasicBlock,
            [&](BasicBlock& bb) // add node func
        {
            Node& bbNode = m_nodes.emplace(&bb, Node(m_pAlloc, "BBNode", pos, &bb)).first->second;
            ImNodes::AutoPositionNode(&bbNode);

            if (f.size() == 1u)
            {
                Node::connect({ &bbNode, "FuncEntry", getNode(&f), "EntryBlock" });
            }

        }, [&](Node& fNode) // Remove node func
        {

        });

		for (BasicBlock& bb : f)
		{
			updateNodeFromContainer(bb, Type::Instruction,
				[&](Instruction& instr) // add node func
			{
				Node& instrNode = m_nodes.emplace(&instr, Node(m_pAlloc, "IntrsNode", pos, &instr)).first->second;
				ImNodes::AutoPositionNode(&instrNode);

				//Node::connect({ &instrNode, "FuncEntry", getNode(&bb), "EntryBlock" });

			}, [&](Node& fNode) // Remove node func
			{

			});
		}

    }

    //updateNodeFromContainer(m_module.getEntryPoints(), Node::Type::EntryPoint,
    //    [&](EntryPoint& f) // add node func
    //{
    //    Node& newNode = m_nodes.emplace_back(m_pAlloc, "EntryPoint", ImVec2{}, &f);
    //    newNode.update();
    //    ImNodes::AutoPositionNode(&newNode);
    //}, [&](Node& n) // Remove node func
    //{

    //});
}

void proto::Graph::updateContextMenu()
{
    if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
    {
        bool anySelected = false;

        for (const auto& [spv, node] : m_nodes)
        {
            anySelected |= node.isSelected();
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

proto::Node* proto::Graph::getNode(SpvObj _obj)
{
    auto it = m_nodes.find(_obj);
    if (it != m_nodes.end())
    {
        return &it->second;
    }

    return nullptr;
}
