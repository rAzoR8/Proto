#include "proto/TypeWidgets.h"
#include "imgui.h"

/*static*/ spvgentwo::Type proto::createFundamentalTypeComboBox(spvgentwo::IAllocator* _pAlloc, const char* _pTitle)
{
	spvgentwo::Type t(_pAlloc);

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

	static const char* current_item = items[0].name;

    if (ImGui::BeginCombo(_pTitle, current_item))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const Item& item = items[n];

            bool is_selected = (current_item == item.name);

            if (ImGui::Selectable(item.name, is_selected))
            {
                current_item = item.name;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                t.reset();

                t.setType(item.type);
                if (t.isInt())
                {
                    t.setIntWidth(labs(item.width));
                    t.setIntSign(item.width < 0);
                }
                else if (t.isFloat())
                {
                    t.setFloatWidth(labs(item.width));
                }
            }
        }
        ImGui::EndCombo();
    }

	return t;
}
