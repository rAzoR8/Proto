#include "proto/AssemblyTextView.h"
#include "imgui.h"
#include "spvgentwo/Module.h"
#include "common/BinaryVectorWriter.h"

using namespace spvgentwo;

proto::AssemblyTextView::AssemblyTextView()
{
}

proto::AssemblyTextView::~AssemblyTextView()
{
}

void proto::AssemblyTextView::update(const spvgentwo::HeapVector<unsigned int>& _module)
{
    if (ImGui::Begin("Text View"))
    {
        ImGui::Checkbox("Indent", &m_indent);
        ImGui::SameLine();
        ImGui::Checkbox("Show offsets", &m_offsets);
        ImGui::SameLine();
        ImGui::Checkbox("Friendly names", &m_friendlyNames);
        ImGui::SameLine();
        ImGui::Checkbox("Scroll", &m_autoScroll);

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        // TODO get string

        //if (text != nullptr)
        //{
        //    ImGui::Text("%s", text->str);        
        //}

        if (m_autoScroll)
        {
            ImGui::SetScrollHereY();
        }

        ImGui::EndChild();
    }

    ImGui::End();
}
