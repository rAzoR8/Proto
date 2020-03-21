#include "proto/TypedDataInput.h"
#include "spvgentwo/Type.h"
#include "imgui.h"

using namespace spvgentwo;

proto::TypedDataInput::TypedDataInput() :
	data{}
{
}

proto::TypedDataInput::~TypedDataInput()
{
}

void proto::TypedDataInput::update(const Type& _type)
{
	int dim = 0u;
	ImGuiDataType_ type = ImGuiDataType_COUNT;

	if (_type.isScalar())
	{
		dim = 1u;
	}
	else if (_type.isVector())
	{
		dim = _type.getVectorComponentCount();
	}
	else if (_type.isMatrix())
	{
		dim = _type.getMatrixColumnCount() * _type.front().getVectorComponentCount();
	}
	else
	{
		return;
	}

	const Type& base = _type.getBaseType();

	if (base.isS16())
	{		
		type = ImGuiDataType_S16;
	}
	else if (base.isS32())
	{
		type = ImGuiDataType_S32;
	}
	else if (base.isS64())
	{
		type = ImGuiDataType_S64;
	}
	if (base.isU16())
	{
		type = ImGuiDataType_U16;
	}
	else if (base.isU32())
	{
		type = ImGuiDataType_U32;
	}
	else if (base.isU64())
	{
		type = ImGuiDataType_U64;
	}
	else if (base.isF32())
	{
		type = ImGuiDataType_Float;
	}
	else if (base.isF64())
	{
		type = ImGuiDataType_Double;
	}
	else
	{
		return;
	}


	if (m_drag)
	{
		ImGui::DragScalarN(m_label, type, &data, dim, m_speed, &m_min, &m_max, m_format, m_power);
	}
}
