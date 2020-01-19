#include "proto/Graph.h"

#include "ImNodesEz.h"

using namespace spvgentwo;

proto::Graph::Graph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_pAlloc(_pAlloc),
    m_module(_pAlloc, spv::Version, _pLogger),
    m_pName(_pName),
    m_nodes(_pAlloc)
{
    // configure capabilities and extensions
    m_module.addCapability(spv::Capability::Shader);

    // global variables
    Instruction* uniformVar = m_module.uniform<vector_t<float, 3>>("u_Position");

    // float add(float x, float y)
    Function& funcAdd = m_module.addFunction<float, float, float>("add", spv::FunctionControlMask::Const);
    {
        BasicBlock& bb = *funcAdd; // get entry block to this function

        Instruction* x = funcAdd.getParameter(0);
        Instruction* y = funcAdd.getParameter(1);

        Instruction* z = bb.Add(x, y);
        bb.returnValue(z);
    }

    // void entryPoint();
    {
        EntryPoint& entry = m_module.addEntryPoint(spv::ExecutionModel::Fragment, "main");
        entry.addExecutionMode(spv::ExecutionMode::OriginUpperLeft);
        BasicBlock& bb = *entry; // get entry block to this function

        Instruction* uniVec = bb->opLoad(uniformVar);
        Instruction* s = bb->opDot(uniVec, uniVec);
        entry->call(&funcAdd, s, s); // call add(s, s)
        entry->opReturn();
    }
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

        // nodes from module
        updateNodes();

        for (Node& n : m_nodes)
        {
            n.update();
        }

        ImNodes::EndCanvas();
    }
    ImGui::End();
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
    ImVec2 pos = { 50, 50 };

    updateNodeFromContainer(m_module.getFunctions(), 
        [&](Function& f) // add node func
    {
        // add function
        pos.x += 25;
        m_nodes.emplace_back(m_pAlloc, "Func", pos, &f);
    },  [&](Node& n) // Remove node func
    {
    
    });
}
