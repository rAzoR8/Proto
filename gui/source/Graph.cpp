#include "proto/Graph.h"

#include "ImNodesEz.h"
#include "proto/TypeWidgets.h"

using namespace spvgentwo;

proto::Graph::Graph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName) :
    m_pAlloc(_pAlloc),
    m_module(_pAlloc, spv::Version, _pLogger),
    m_pName(_pName)
{
    // configure capabilities and extensions
    m_module.addCapability(spv::Capability::Shader);

    return;

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
    updateNodeFromContainer(m_module.getFunctions(), Type::Function, 
        [&](Function& func) // add node func
    {
        Node& funcNode = m_nodes.emplace(&func, Node(m_pAlloc, "Func", ImVec2{}, &func)).first->second;
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
            Node& bbNode = m_nodes.emplace(&bb, Node(m_pAlloc, "BasicBlock", ImVec2{ 100, 100 }, &bb)).first->second;
            //ImNodes::AutoPositionNode(&bbNode);
            Node* func = getNode(&f);

            func->addOutputSlot("Entry", Slot::FunctionEntry);
            bbNode.addInputSlot("Function", Slot::FunctionEntry);
            func->connect("Function", &bbNode);

            bbNode.update();

        }, [&](Node& fNode) // Remove node func
        {

        });
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
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
        ImGui::OpenPopup("NodesContextMenu");
    }

    if (ImGui::BeginPopup("NodesContextMenu"))
    {
        if (ImGui::MenuItem("Add Function"))
        {
            m_addFunctionModal = true;
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

    if (m_addFunctionModal)
    {
        addFunction();
    }
}

void proto::Graph::addFunction()
{
   ImGui::OpenPopup("Create function signature");

    if (ImGui::BeginPopup("Create function signature"))
    {
        //static char name[256]{};
        //sprintf_s(name, "DefaultFuncName");

        //ImGui::InputText("Name", name, sizeof(name));

        static FundamentalTypeComboBox retCombo(m_pAlloc, "Return Type");
        retCombo.update();

        static List<FundamentalTypeComboBox> params(m_pAlloc);

        if (ImGui::Button("Add Parameter"))
        {
            params.emplace_back(m_pAlloc, "Parameter Type");
        }

        if (ImGui::Button("Remove Parameter"))
        {
            (void)params.pop_back();
        }

        for (FundamentalTypeComboBox& t : params)
        {
            ImGui::PushID(&t);
            t.update();
            ImGui::PopID();
        }

        if (ImGui::Button("Create"))
        {
            Function& fun = m_module.addFunction();
            Instruction* type = m_module.addType(retCombo.getType());

            fun.setReturnType(type);

            for (FundamentalTypeComboBox& t : params)
            {
                type = m_module.addType(t.getType());
                fun.addParameters(type);
            }
            params.clear();

            fun.finalize(spv::FunctionControlMask::Const/*, name*/);

            m_addFunctionModal = false;
        }

        ImGui::EndPopup();
    }
}

void proto::Graph::addEntryPoint()
{
    //m_module.addEntryPoint();
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
