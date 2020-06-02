#include "proto/FundamentalTypeComboBox.h"
#include "imgui.h"

struct Item
{
    const char* name;
    spvgentwo::spv::Op type;
    int width;
    int rows = 1;
    int columns = 0;
};

constexpr Item items[] = {
    {"u32", spvgentwo::spv::Op::OpTypeInt, 32},
    {"f32", spvgentwo::spv::Op::OpTypeFloat, 32},
    {"f32v2", spvgentwo::spv::Op::OpTypeFloat, 32, 2},
    {"f32v3", spvgentwo::spv::Op::OpTypeFloat, 32, 3},
    {"f32v4", spvgentwo::spv::Op::OpTypeFloat, 32, 4},
    {"mat2", spvgentwo::spv::Op::OpTypeFloat, 32, 2, 2},
    {"mat3", spvgentwo::spv::Op::OpTypeFloat, 32, 3, 3},
    {"mat4", spvgentwo::spv::Op::OpTypeFloat, 32, 4, 4},
    {"f64", spvgentwo::spv::Op::OpTypeFloat, 64},
    {"s16", spvgentwo::spv::Op::OpTypeInt, -16},
    {"u16", spvgentwo::spv::Op::OpTypeInt, 16},
    {"s32", spvgentwo::spv::Op::OpTypeInt, -32},
    {"bool", spvgentwo::spv::Op::OpTypeBool, 0},
    {"void", spvgentwo::spv::Op::OpTypeVoid, 0},
    {"bvec2", spvgentwo::spv::Op::OpTypeBool, 0, 2},
    {"bvec3", spvgentwo::spv::Op::OpTypeBool, 0, 3},
    {"bvec4", spvgentwo::spv::Op::OpTypeBool, 0, 4},
};

proto::FundamentalTypeComboBox::FundamentalTypeComboBox(const char* _pTitle) : ComboBox(_pTitle),
    m_type(spvgentwo::HeapAllocator::instance())
{
    for (auto item : items)
    {
        emplace_back(item.name);
    }
}

proto::FundamentalTypeComboBox::~FundamentalTypeComboBox()
{
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

    if (item.rows > 1)
    {
        m_type = m_type.wrapVector(item.rows);

        if (item.columns > 1)
        {
            m_type = m_type.wrapMatrix(item.columns);        
        }
    }
}