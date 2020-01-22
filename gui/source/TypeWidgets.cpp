#include "proto/TypeWidgets.h"
#include "imgui.h"

proto::FundamentalTypeComboBox::FundamentalTypeComboBox(spvgentwo::IAllocator* _pAlloc, const char* _pTitle) :
    m_pTitle(_pTitle),
    m_type(_pAlloc)
{
}

proto::FundamentalTypeComboBox::~FundamentalTypeComboBox()
{
}

void proto::FundamentalTypeComboBox::update()
{
    struct Item
    {
        const char* name;
        spv::Op type;
        int width;
    };

    const Item items[] = {
        {"void", spv::Op::OpTypeVoid, 0},
        {"bool", spv::Op::OpTypeBool, 0},
        {"int32", spv::Op::OpTypeInt, -32},
        {"uint32", spv::Op::OpTypeInt, 32},
        {"float", spv::Op::OpTypeFloat, 32},
        {"double", spv::Op::OpTypeFloat, 64},
        {"int16", spv::Op::OpTypeInt, -16},
        {"uint16", spv::Op::OpTypeInt, 16},
    };

    m_selected = m_selected >= IM_ARRAYSIZE(items) ? 0 : m_selected;
    const char* current_item = items[m_selected].name;

    if (ImGui::BeginCombo(m_pTitle, current_item))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const Item& item = items[n];

            bool is_selected = (current_item == item.name);

            if (ImGui::Selectable(item.name, is_selected))
            {
                m_selected = n;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
                m_type.reset();

                m_type.setType(item.type);
                if (m_type.isInt())
                {
                    m_type.setIntWidth(labs(item.width));
                    m_type.setIntSign(item.width < 0);
                }
                else if (m_type.isFloat())
                {
                    m_type.setFloatWidth(labs(item.width));
                }
            }
        }
        ImGui::EndCombo();
    }
}
