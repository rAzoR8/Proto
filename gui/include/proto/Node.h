#pragma once

#include "ImNodesEz.h"
#include "SpvGenTwo/List.h"
#include <vector>

// forward decls

namespace spvgentwo
{
	class Instruction;
	class BasicBlock;
	class Function;
	class EntryPoint;
}

namespace proto
{
	class Node
	{
	public:
		enum SlotKind : int
		{
			In,
			Out
		};

		enum class Type : int
		{
			Instruction,
			BasicBlock,
			Function,
			EntryPoint
		};

		struct SpvObj
		{
			union {
				spvgentwo::Instruction* instr;
				spvgentwo::BasicBlock* bb;
				spvgentwo::Function* func;
				spvgentwo::EntryPoint* ep;
			} obj{};

			Type type{};

			
			SpvObj(const SpvObj& _other) : obj(_other.obj), type(_other.type) {}
			SpvObj(spvgentwo::Instruction* _instr) : type(Type::Instruction) { obj.instr = _instr; }
			SpvObj(spvgentwo::BasicBlock* _bb) : type(Type::BasicBlock) { obj.bb = _bb; }
			SpvObj(spvgentwo::Function* _func) : type(Type::Function) { obj.func = _func; }
			SpvObj(spvgentwo::EntryPoint* _ep) : type(Type::EntryPoint) { obj.ep = _ep; }

			bool operator==(const spvgentwo::Instruction* _ptr) const { return type == Type::Instruction && obj.instr == _ptr; }
			bool operator==(const spvgentwo::BasicBlock* _ptr) const { return type == Type::BasicBlock && obj.bb == _ptr; }
			bool operator==(const spvgentwo::Function* _ptr) const { return type == Type::Function && obj.func == _ptr; }
			bool operator==(const spvgentwo::EntryPoint* _ptr) const { return type == Type::EntryPoint && obj.ep == _ptr; }
		};

		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, SpvObj _obj);

		~Node();

		const char* getTitle() const { return m_pTitle; }
		SpvObj getSpvObj() const { return m_spv; }

		void update();

		void addInputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pInput = nullptr);
		void addOutputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pOutput = nullptr);

		void clear();

	private:
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, Type _type);

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		const char* m_pTitle = "Node";

		SpvObj m_spv;

		ImVec2 m_pos{};
		bool m_selected = false;

		std::vector<ImNodes::Ez::SlotInfo> m_inputSlots;
		spvgentwo::List<Node*> m_inputs;

		std::vector<ImNodes::Ez::SlotInfo> m_outputSlots;
		spvgentwo::List<Node*> m_outputs;
	};
} // !proto