#include "proto/AssemblyTextView.h"
#include "imgui.h"
#include "spvgentwo/Module.h"
#include "common/BinaryVectorWriter.h"
#include "common/ModulePrinter.h"

using namespace spvgentwo;

proto::AssemblyTextView::AssemblyTextView(spvgentwo::IAllocator* _pAllocator) :
    m_grammar(_pAllocator),
    m_text(_pAllocator)
{
}

proto::AssemblyTextView::~AssemblyTextView()
{
}

void proto::AssemblyTextView::update(spvgentwo::Module& _module)
{
    if (ImGui::Begin("Text View"))
    {
        ImGui::Checkbox("Scroll", &m_autoScroll);

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        m_text.clear();
        m_text.reserve(2048u);
        ModulePrinter::ModuleStringPrinter printer(m_text);

        printModule(_module, m_grammar, printer);

		if (m_text.empty() == false)
		{
			ImGui::Text("%s", m_text.c_str());
		}

        if (m_autoScroll)
        {
            ImGui::SetScrollHereY();
        }

        ImGui::EndChild();
    }

    ImGui::End();
}
