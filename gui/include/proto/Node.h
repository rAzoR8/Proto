#pragma once

#include "ImNodesEz.h"
#include "SpvGenTwo/List.h"
#include <vector>
#include "SpvGenTwo/Hasher.h"
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

		SpvObj(const SpvObj& _other) noexcept : obj(_other.obj), type(_other.type) {}
		SpvObj(spvgentwo::Instruction* _instr) noexcept : type(Type::Instruction) { obj.instr = _instr; }
		SpvObj(spvgentwo::BasicBlock* _bb) noexcept : type(Type::BasicBlock) { obj.bb = _bb; }
		SpvObj(spvgentwo::Function* _func) noexcept : type(Type::Function) { obj.func = _func; }
		SpvObj(spvgentwo::EntryPoint* _ep) noexcept : type(Type::EntryPoint) { obj.ep = _ep; }

		bool operator==(const SpvObj& _other) const { return type == _other.type && obj.instr == _other.obj.instr; }
		bool operator==(const spvgentwo::Instruction* _ptr) const { return type == Type::Instruction && obj.instr == _ptr; }
		bool operator==(const spvgentwo::BasicBlock* _ptr) const { return type == Type::BasicBlock && obj.bb == _ptr; }
		bool operator==(const spvgentwo::Function* _ptr) const { return type == Type::Function && obj.func == _ptr; }
		bool operator==(const spvgentwo::EntryPoint* _ptr) const { return type == Type::EntryPoint && obj.ep == _ptr; }
	};

	class Node
	{
	public:
		enum SlotKind : int
		{
			In,
			Out
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
		void updateEntryPoint();
		void updateFunction();
		void updateBasicBlock();
		void updateInstruction();

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

template <>
struct spvgentwo::Hasher<proto::SpvObj>
{
	Hash64 operator()(const proto::SpvObj& obj, Hash64 _seed = detail::Offset) const
	{
		FNV1aHasher func;
		func(obj.obj, obj.type);
		return func;
	}
};