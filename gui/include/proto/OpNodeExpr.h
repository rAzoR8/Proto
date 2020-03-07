#pragma once

#include "spvgentwo/List.h"
#include <stdint.h>

//forward decl
namespace spvgentwo
{
	class Instruction;
} // !spvgentwo

namespace proto
{
	enum class OpNodeType : uint32_t
	{
		Var = 0, // needs load
		Const,

		Equal,
		NotEqual,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,

		Add,
		Sub,
		Mul,
		Div,
		Dot,

		Select,
		Phi,

		Cast,

		NumOf
	};

	struct OpNodeDesc
	{
		OpNodeType type; 
		uint32_t numInputs;
		uint32_t numOutputs;
	};

	constexpr OpNodeDesc g_OpNodeDesc[uint32_t(OpNodeType::NumOf)] = { 
		{ OpNodeType::Var, 1u, 1u},
		{ OpNodeType::Const, 1u, 1u},
		{ OpNodeType::Equal, 2u, 1u},
		{ OpNodeType::NotEqual, 2u, 1u},
		{ OpNodeType::Less, 2u, 1u},
		{ OpNodeType::LessEqual, 2u, 1u},
		{ OpNodeType::Greater, 2u, 1u},
		{ OpNodeType::GreaterEqual, 2u, 1u},
		{ OpNodeType::Add, 2u, 1u},
		{ OpNodeType::Sub, 2u, 1u},
		{ OpNodeType::Mul, 2u, 1u},
		{ OpNodeType::Div, 2u, 1u},
		{ OpNodeType::Dot, 2u, 1u},
		{ OpNodeType::Select, 2u, 1u},
		{ OpNodeType::Phi, ~0u, 1u},
		{ OpNodeType::Cast, 1u, 1u},
	};

	class OpNodeExpr
	{
	public:
		OpNodeExpr(OpNodeType mtype = OpNodeType::NumOf);
		~OpNodeExpr();

		void operator()(const spvgentwo::List<OpNodeExpr*>& _inputs, const spvgentwo::List<OpNodeExpr*>& _outputs);		

		OpNodeType getType() const { return m_type; }

		OpNodeDesc getInfo() { return g_OpNodeDesc[uint32_t(m_type)]; }

	private:
		OpNodeType m_type = OpNodeType::NumOf;
		spvgentwo::Instruction* m_pResult = nullptr;
	};
} // !proto