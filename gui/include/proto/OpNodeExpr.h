#pragma once

#include "spvgentwo/List.h"
#include "spvgentwo/Constant.h"
#include <stdint.h>

//forward decl
namespace spvgentwo
{
	class Instruction;
	class BasicBlock;
} // !spvgentwo

namespace proto
{
	enum class OpNodeType : uint32_t
	{
		InVar = 0, // needs load
		OutVar, // needs store
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
		//Phi,

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
		{ OpNodeType::InVar, 0u, 1u},
		{ OpNodeType::OutVar, 1u, 0u},
		{ OpNodeType::Const, 0u, 1u},
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
		//{ OpNodeType::Phi, ~0u, 1u},
		{ OpNodeType::Cast, 1u, 1u},
	};

	struct VarDesc
	{
		spvgentwo::Type type;
		spv::StorageClass storageClass = spv::StorageClass::UniformConstant;
		const char* name = nullptr;
	};

	struct ConstDesc
	{
		spvgentwo::Constant constant;
		const char* name = nullptr;
	};

	class OpNodeExpr
	{
	public:
		OpNodeExpr(spvgentwo::BasicBlock* _pBB = nullptr, OpNodeType mtype = OpNodeType::NumOf);
		~OpNodeExpr();

		void operator()(const spvgentwo::List<OpNodeExpr*>& _inputs, const spvgentwo::List<OpNodeExpr*>& _outputs);		

		OpNodeType getType() const { return m_type; }

		OpNodeDesc getInfo() { return g_OpNodeDesc[uint32_t(m_type)]; }

		void setVarDesc(const VarDesc* _pVarDesc) { m_pVarDesc = _pVarDesc; }
		void setConstDesc(const ConstDesc* _pConstDesc) { m_pConstDesc = _pConstDesc; }

	private:
		void makeVar();
		void makeConst();

	private:
		OpNodeType m_type = OpNodeType::NumOf;
		spvgentwo::BasicBlock* m_pBB = nullptr;
		spvgentwo::Instruction* m_pResult = nullptr;
		spvgentwo::Instruction* m_pVar = nullptr;
		const VarDesc* m_pVarDesc = nullptr;
		const ConstDesc* m_pConstDesc = nullptr;
	};
} // !proto