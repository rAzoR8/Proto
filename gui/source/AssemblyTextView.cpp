#include "proto/AssemblyTextView.h"
#include "imgui.h"
#include "spvgentwo/Module.h"

#include <spirv-tools/libspirv.hpp>
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

        uint32_t options = SPV_BINARY_TO_TEXT_OPTION_NONE;
        if (m_indent) options |= SPV_BINARY_TO_TEXT_OPTION_INDENT;
        if (m_offsets) options |= SPV_BINARY_TO_TEXT_OPTION_SHOW_BYTE_OFFSET;
        if (m_friendlyNames) options |= SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES;

        spv_text text = nullptr;
        spv_diagnostic diagnostic = nullptr;
        spv_context context = spvContextCreate(SPV_ENV_UNIVERSAL_1_5);
        spv_result_t error = spvBinaryToText(context, _module.data(), _module.size(), options, &text, &diagnostic);
        spvContextDestroy(context);

        if (error)
        {
            spvDiagnosticPrint(diagnostic);
            spvDiagnosticDestroy(diagnostic);
        }

        ImGui::Text("%s", text->str);

        spvTextDestroy(text);
    }

    ImGui::End();
}
