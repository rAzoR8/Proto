#include "proto/TypeWidgets.h"
#include "imgui.h"

struct Item
{
    const char* name;
    spv::Op type;
    int width;
};

constexpr Item items[] = {
    {"void", spv::Op::OpTypeVoid, 0},
    {"bool", spv::Op::OpTypeBool, 0},
    {"int32", spv::Op::OpTypeInt, -32},
    {"uint32", spv::Op::OpTypeInt, 32},
    {"float", spv::Op::OpTypeFloat, 32},
    {"double", spv::Op::OpTypeFloat, 64},
    {"int16", spv::Op::OpTypeInt, -16},
    {"uint16", spv::Op::OpTypeInt, 16},
};

proto::FundamentalTypeComboBox::FundamentalTypeComboBox(const char* _pTitle) : ComboBox(_pTitle)
{
    for (auto item : items)
    {
        emplace_back(item.name);
    }
}

proto::FundamentalTypeComboBox::~FundamentalTypeComboBox()
{
    int i = 0;
}

void proto::FundamentalTypeComboBox::onSelect(unsigned int _index)
{
    auto item = items[_index];

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