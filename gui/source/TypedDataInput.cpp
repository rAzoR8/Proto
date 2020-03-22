#include "proto/TypedDataInput.h"
#include "spvgentwo/Constant.h"
#include "imgui.h"

using namespace spvgentwo;

proto::TypedDataInput::TypedDataInput(const char* _label) :
	m_label(_label),
	m_min{},
	m_max{},
	m_step{},
	m_fastStep{},
	m_data{}
{
}

proto::TypedDataInput::~TypedDataInput()
{
}

bool proto::TypedDataInput::update(const Type& _type)
{
	uint32_t rows = 0;
	uint32_t cols = 1;
	ImGuiDataType_ type = ImGuiDataType_COUNT;

	if (_type.isScalar())
	{
		rows = 1u;
	}
	else if (_type.isVector())
	{
		rows = _type.getVectorComponentCount();
	}
	else if (_type.isMatrix())
	{
		rows = _type.getMatrixRowCount();
		cols = _type.getMatrixColumnCount();
	}
	else
	{
		return false;
	}

	const Type& base = _type.getBaseType();

	if (base.isS16())
	{		
		type = ImGuiDataType_S16;
		m_step.s16 = m_step.s16 == 0 ? 1 : m_step.s16;
	}
	else if (base.isS32())
	{
		type = ImGuiDataType_S32;
		m_step.s32 = m_step.s32 == 0 ? 1 : m_step.s32;
	}
	else if (base.isS64())
	{
		type = ImGuiDataType_S64;
		m_step.s64 = m_step.s64 == 0 ? 1 : m_step.s64;
	}
	else if (base.isU16())
	{
		type = ImGuiDataType_U16;
		m_step.u16 = m_step.u16 == 0 ? 1 : m_step.u16;
	}
	else if (base.isU32())
	{
		type = ImGuiDataType_U32;
		m_step.u32 = m_step.u32 == 0 ? 1 : m_step.u32;
	}
	else if (base.isU64())
	{
		type = ImGuiDataType_U64;
		m_step.u64 = m_step.u64 == 0 ? 1 : m_step.u64;
	}
	else if (base.isF32())
	{
		type = ImGuiDataType_Float;
		m_step.f32 = m_step.s32 == 0 || m_step.s32 == 1 ? 1.f : m_step.f32;
	}
	else if (base.isF64())
	{
		type = ImGuiDataType_Double;
		m_step.f64 = m_step.s64 == 0 || m_step.s64 == 1 ? 1.0 : m_step.f64;
	}
	else
	{
		return false;
	}

	const uint32_t rowLength = (base.getIntWidth() / 4) * rows;

	if (ImGui::RadioButton("Input Popup", m_inputInModal))
	{
		m_inputInModal = !m_inputInModal;
	}
	ImGui::SameLine();
	if (ImGui::Button("Input Options"))
	{
		m_showOptions = !m_showOptions;
	}

	if (m_inputInModal)
	{
		ImGui::OpenPopup("Input Value");
		m_inputModalOpen = ImGui::BeginPopupModal("Input Value", &m_inputInModal);
	}	

	if (m_inputDrag)
	{
		for (auto i = 0u; i < cols; ++i)
		{
			ImGui::PushID(i);
			ImGui::DragScalarN("##Value", type, &m_data.u8 + (uint64_t)i * rowLength, rows, m_speed, &m_min, &m_max, m_format, m_power);
			ImGui::PopID();
		}
	}
	if (m_inputScalar)
	{
		for (auto i = 0u; i < cols; ++i)
		{
			ImGui::PushID(i);
			ImGui::InputScalarN("##Value", type, &m_data.u8 + (uint64_t)i * rowLength, rows, &m_step, &m_fastStep);
			ImGui::PopID();
		}
	}
	if (m_inputColor && type == ImGuiDataType_Float && cols < 2)
	{
		if (rows == 3)
		{
			ImGui::ColorEdit3("##Value", m_data.f32v3);
		}
		else if (rows == 4)
		{
			ImGui::ColorEdit4("##Value", m_data.f32v4);
		}
	}

	if (m_inputModalOpen)
	{
		ImGui::EndPopup();
	}

	if (m_showOptions)
	{
		ImGui::OpenPopup("Options");
	}

	if (ImGui::BeginPopupModal("Options", &m_showOptions))
	{
		ImGui::RadioButton("Drag", &m_activeOption, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Scalar", &m_activeOption, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Color", &m_activeOption, 2);

		m_inputDrag = m_activeOption == 0;
		m_inputScalar = m_activeOption == 1;
		m_inputColor = m_activeOption == 2;

		if (m_inputDrag)
		{
			ImGui::DragScalar("Min", type, &m_min, 0.01f);
			ImGui::DragScalar("Max", type, &m_max, 0.01f);
			ImGui::DragFloat("Speed", &m_speed, 0.01f);
			ImGui::DragFloat("Power", &m_power, 0.01f);
		}

		if (m_inputScalar)
		{
			ImGui::DragScalar("Step", type, &m_step, 0.01f);
			ImGui::DragScalar("FastStep", type, &m_fastStep, 0.01f);
		}
		ImGui::EndPopup();
	}

	return true;
}

bool proto::TypedDataInput::update(const spvgentwo::Type& _type, spvgentwo::Constant& _outConstant)
{
	if (update(_type) == false)
	{
		return false;	
	}

	// integral scalar types

	if (_type.isS16())
	{
		_outConstant.make(m_data.s16);
	}
	else if (_type.isS32())
	{
		_outConstant.make(m_data.s32);
	}
	else if (_type.isS64())
	{
		_outConstant.make(m_data.s64);
	}
	else if (_type.isU16())
	{
		_outConstant.make(m_data.u16);
	}
	else if (_type.isU32())
	{
		_outConstant.make(m_data.u32);
	}
	else if (_type.isU64())
	{
		_outConstant.make(m_data.u64);
	}

	// floats
	else if (_type.isF32())
	{
		_outConstant.make(m_data.f32);
	}
	else if (_type.isF64())
	{
		_outConstant.make(m_data.f64);
	}

	// signed int 16 vec
	else if (_type.isVectorOfSInt(2u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v2));
	}
	else if (_type.isVectorOfSInt(3u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v3));
	}
	else if (_type.isVectorOfSInt(4u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v4));
	}

	// unsinged int 16 vec
	else if (_type.isVectorOfUInt(2u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v2));
	}
	else if (_type.isVectorOfUInt(3u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v3));
	}
	else if (_type.isVectorOfUInt(4u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v4));
	}

	// signed int 32 vec
	else if (_type.isVectorOfSInt(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v2));
	}
	else if (_type.isVectorOfSInt(3u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v3));
	}
	else if (_type.isVectorOfSInt(4u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v4));
	}

	// unsinged int 32 vec
	else if (_type.isVectorOfUInt(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v2));
	}
	else if (_type.isVectorOfUInt(3u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v3));
	}
	else if (_type.isVectorOfUInt(4u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v4));
	}

	// signed int 64 vec
	else if (_type.isVectorOfSInt(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v2));
	}
	else if (_type.isVectorOfSInt(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v3));
	}
	else if (_type.isVectorOfSInt(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v4));
	}

	// unsinged int 64 vec
	else if (_type.isVectorOfUInt(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v2));
	}
	else if (_type.isVectorOfUInt(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v3));
	}
	else if (_type.isVectorOfUInt(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v4));
	}

	// float vec 32
	else if (_type.isVectorOfFloat(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.f32v2));
	}
	else if (_type.isVectorOfFloat(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.f32v3));
	}
	else if (_type.isVectorOfFloat(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.f32v4));
	}

	// float vec 64
	else if (_type.isVectorOfFloat(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v2));
	}
	else if (_type.isVectorOfFloat(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v3));
	}
	else if (_type.isVectorOfFloat(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v4));
	}

	// s16 mat
	else if (_type.isMatrixOfInt(2u, 2u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m44));
	}

	// u16 mat
	else if (_type.isMatrixOfInt(2u, 2u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m44));
	}


	// s32 mat
	else if (_type.isMatrixOfInt(2u, 2u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m44));
	}

	// u32 mat
	else if (_type.isMatrixOfInt(2u, 2u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m44));
	}

	// s64 mat
	else if (_type.isMatrixOfInt(2u, 2u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m44));
	}

	// u64 mat
	else if (_type.isMatrixOfInt(2u, 2u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m44));
	}

	// f32mat
	else if (_type.isMatrixOfFloat(2u, 2u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m22));
	}
	else if (_type.isMatrixOfFloat(3u, 3u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m33));
	}
	else if (_type.isMatrixOfFloat(4u, 4u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m44));
	}

	// f64 mat
	else if (_type.isMatrixOfFloat(2u, 2u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m22));
	}
	else if (_type.isMatrixOfFloat(3u, 3u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m33));
	}
	else if (_type.isMatrixOfFloat(4u, 4u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m44));
	}

	else // could not match type
	{
		return false;
	}

	return _outConstant.getType() == _type;
}
