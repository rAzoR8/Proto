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

		union SpvObj
		{
			spvgentwo::Instruction* instr;
			spvgentwo::BasicBlock* bb;
			spvgentwo::Function* func;
			spvgentwo::EntryPoint* ep;

			bool operator==(const void* _ptr) const { return reinterpret_cast<const void*>(instr) == _ptr; }
		};

		enum class Type : int 
		{
			Instruction,
			BasicBlock,
			Function, 
			EntryPoint
		};

		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::Instruction* _instr);
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::BasicBlock* _bb);
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::Function* _func);
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::EntryPoint* _ep);

		~Node();

		const char* getTitle() const { return m_pTitle; }
		SpvObj getSpvObj() const { return m_spv; }
		Type getType() const { return m_Type; }

		void update();

		void addInputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pInput = nullptr);
		void addOutputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pOutput = nullptr);

		void clear();

	private:
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, Type _type);

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		const char* m_pTitle = "Node";

		SpvObj m_spv{};

		Type m_Type;

		ImVec2 m_pos{};
		bool m_selected = false;

		std::vector<ImNodes::Ez::SlotInfo> m_inputSlots;
		spvgentwo::List<Node*> m_inputs;

		std::vector<ImNodes::Ez::SlotInfo> m_outputSlots;
		spvgentwo::List<Node*> m_outputs;
	};
} // !proto