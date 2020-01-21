#include "proto/NewFunctionPopup.h"
#include "imgui.h"
#include "SpvGenTwo/Module.h"

using namespace spvgentwo;

proto::NewFunctionPopup::NewFunctionPopup(spvgentwo::IAllocator* _pAllocator) :
	m_pAllocator(_pAllocator),
	m_name(_pAllocator),
	m_returnType(_pAllocator, "Return Type"),
	m_parameterTypes(_pAllocator)
{
    m_name.resize(64, "\0");
}

proto::NewFunctionPopup::~NewFunctionPopup()
{
}

void proto::NewFunctionPopup::update(spvgentwo::Module& _module)
{
    if (m_visble == false)
        return;

    ImGui::OpenPopup("Create function signature");

    if (ImGui::BeginPopup("Create function signature"))
    {
        ImGui::InputText("Name", m_name.c_str(), m_name.size());

        m_returnType.update();

        if (ImGui::Button("Add Parameter"))
        {
            m_parameterTypes.emplace_back(m_pAllocator, "Parameter Type");
        }

        for (auto it = m_parameterTypes.begin(); it != m_parameterTypes.end();)
        {
            auto& ComboBox = *it;
            ImGui::PushID(&ComboBox);
            ComboBox.update();
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(&ComboBox);
            if (ImGui::Button("Remove"))
            {
                it = m_parameterTypes.erase(it);
            }
            else
            {
                ++it;
            }
            ImGui::PopID();
        }

        if (ImGui::Button("Create"))
        {
            Function& fun = _module.addFunction();
            Instruction* type = _module.addType(m_returnType.getType());

            fun.setReturnType(type);

            for (FundamentalTypeComboBox& t : m_parameterTypes)
            {
                type = _module.addType(t.getType());
                fun.addParameters(type);
            }

            fun.finalize(spv::FunctionControlMask::Const, m_name.c_str());

            m_parameterTypes.clear();
            m_visble = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            m_parameterTypes.clear();
            m_visble = false;
        }
    }
    ImGui::EndPopup();
}
