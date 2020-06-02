#pragma once

#include "common/HeapList.h"
#include "common/HeapVector.h"
#include "common/HeapString.h"
#include "common/ExprGraph.h"
#include "spvgentwo/Constant.h"

#include "proto/FundamentalTypeComboBox.h"
#include "proto/TypedDataInput.h"

#include <stdint.h>
#include "ImNodesEz.h"

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
		const char* name; 
		spvgentwo::HeapVector<const char*> inputs;
		spvgentwo::HeapVector<const char*> outputs;
	};

	static const OpNodeDesc g_OpNodeDesc[uint32_t(OpNodeType::NumOf)] = { 
		{ "InVar", {}, {"Value"}},
		{ "OutVar", "Value", {}},
		{ "Const", {}, "Value"},
		{ "Equal", {"X", "Y"}, {"bool"}},
		{ "NotEqual", {"X", "Y"}, {"bool"}},
		{ "Less", {"X", "Y"}, {"bool"}},
		{ "LessEqual", {"X", "Y"}, {"bool"}},
		{ "Greater", {"X", "Y"}, {"bool"}},
		{ "GreaterEqual", {"X", "Y"}, {"bool"}},
		{ "Add", {"X", "Y"}, {"Value"}},
		{ "Sub",{"X", "Y"}, {"Value"}},
		{ "Mul", {"X", "Y"}, {"Value"}},
		{ "Div", {"X", "Y"}, {"Value"}},
		{ "Dot", {"X", "Y"}, {"Value"}},
		{ "Select",{"bool", "X", "Y"}, {"Value"}},
		//{ OpNodeType::Phi, ~0u, 1u},
		{ "Cast", {"Type1"}, {"Type2"}},
	};

	struct VarDesc
	{
		spvgentwo::Type type;
		spvgentwo::spv::StorageClass storageClass = spvgentwo::spv::StorageClass::UniformConstant;
		const char* name = nullptr;
	};

	struct ConstDesc
	{
		spvgentwo::Constant constant;
		const char* name = nullptr;
	};

	// forward decl
	class OpNodeExpr;

	/// A structure defining a connection between two slots of two nodes.
	struct Connection
	{
		/// `id` that was passed to BeginNode() of input node.
		OpNodeExpr* input_node = nullptr;
		/// Descriptor of input slot.
		const char* input_slot = nullptr;
		/// `id` that was passed to BeginNode() of output node.
		OpNodeExpr* output_node = nullptr;
		/// Descriptor of output slot.
		const char* output_slot = nullptr;

		bool operator==(const Connection& other) const
		{
			return input_node == other.input_node &&
				input_slot == other.input_slot &&
				output_node == other.output_node &&
				output_slot == other.output_slot;
		}

		bool operator!=(const Connection& other) const
		{
			return !operator ==(other);
		}
	};

	class OpNodeExpr
	{
	public:
		OpNodeExpr(OpNodeExpr&& _other) noexcept;
		OpNodeExpr(ImVec2 _pos = ImVec2(), OpNodeType _type = OpNodeType::NumOf);
		~OpNodeExpr();

		void operator()(const spvgentwo::List<OpNodeExpr*>& _inputs, const spvgentwo::List<OpNodeExpr*>& _outputs);		

		OpNodeType getType() const { return m_type; }
		OpNodeDesc getInfo() { return g_OpNodeDesc[uint32_t(m_type)]; }

		void setBasicBlock(spvgentwo::BasicBlock* _pBB);
		void setParent(spvgentwo::ExprGraph<OpNodeExpr>* _pGraph, typename spvgentwo::ExprGraph<OpNodeExpr>::NodeType* _pParent);
		
		// returns true if nodes is to be removed
		bool update();

		bool isSelected() const { return m_selected; }

		ImVec2 getPosition() const { return m_pos; }

	private:
		bool allowedDisconnection(const Connection& _con);
		bool allowedConnection(const Connection& _con);
		spvgentwo::List<Connection>::Iterator remove(const Connection& _con);

	// expr evaluation
		bool makeVar();
		bool makeConst();

	// ui update:
		void updateOpDesc();
		void updateVarDesc();
		void updateConstDesc();
		
	private:
		OpNodeType m_type = OpNodeType::NumOf;
		spvgentwo::BasicBlock* m_pBB = nullptr;
		spvgentwo::Instruction* m_pResult = nullptr;
		spvgentwo::Instruction* m_pVar = nullptr;

		ImVec2 m_pos{};
		bool m_selected = false;

		VarDesc m_varDesc{};
		ConstDesc m_constDesc{};

		spvgentwo::HeapVector<ImNodes::Ez::SlotInfo> m_inputSlots;
		spvgentwo::HeapVector<ImNodes::Ez::SlotInfo> m_outputSlots;
		spvgentwo::HeapList<Connection> m_connections;

		spvgentwo::ExprGraph<OpNodeExpr>* m_pGraph = nullptr; 
		typename spvgentwo::ExprGraph<OpNodeExpr>::NodeType* m_pParent = nullptr;

	private:
		FundamentalTypeComboBox m_typeComboBox;
		ComboBox<spvgentwo::spv::StorageClass> m_storageClassCombobox;
		TypedDataInput m_dataInput;
	};
} // !proto