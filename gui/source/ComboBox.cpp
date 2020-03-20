#include "proto/ComboBox.h"
#include "imgui.h"

using namespace spvgentwo;

proto::ComboBox::ComboBox(ComboBox&& _other) noexcept : HeapVector<HeapString>(stdrep::move(_other)),
    m_pTitle(_other.m_pTitle),
    m_onSelect(stdrep::move(_other.m_onSelect)),
    m_selected(_other.m_selected)
{
    _other.m_selected = 0u;
    _other.m_pTitle = nullptr;
}

proto::ComboBox::ComboBox(const ComboBox& _other) : HeapVector<HeapString>(_other),
    m_pTitle(_other.m_pTitle),
    m_onSelect(_other.m_onSelect),
    m_selected(_other.m_selected)
{
}

proto::ComboBox::ComboBox(const char* _pTitle) :
    HeapVector(),
	m_pTitle(_pTitle),
    m_onSelect()
{
}

proto::ComboBox::~ComboBox()
{
}

void proto::ComboBox::update()
{
    if (empty())
        return;

    m_selected = m_selected >= m_elements ? 0u : m_selected;
    const char* current_item = operator[](m_selected).c_str();

    if (ImGui::BeginCombo(m_pTitle, current_item))
    {
        for (int n = 0; n < m_elements; n++)
        {
            const char* item = operator[](n).c_str();

            bool is_selected = (current_item == item);

            if (ImGui::Selectable(item, is_selected /*,0, ImVec2(1.f, 2.f)*/))
            {
                m_selected = n;

				ImGui::SetItemDefaultFocus();
				if (m_onSelect)
				{
					m_onSelect(m_selected);
				}
				else
				{
					onSelect(m_selected);
				}
            }
        }
        ImGui::EndCombo();
    }
}
