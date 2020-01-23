#pragma once

#include "ImNodesEz.h"
#include "SpvGenTwo/List.h"
#include "spvgentwo/Vector.h"
#include "spvgentwo/Hasher.h"
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

	enum class Slot : int
	{
		Unknown = 0,
		EntryBlock = 1, // functionEntryBlock
		Instruction,
		BasicBlock
	};

	static const char* g_slotTitles[] = { "EntryBlock", "Instruction", "BasicBlock" };
	inline const char* getSlotTitle(Slot _slot) { return g_slotTitles[(int)_slot - 1]; }
	Slot getSlot(const char* _pSlot);

	/// A structure defining a connection between two slots of two nodes.
	struct Connection
	{
		/// `id` that was passed to BeginNode() of input node.
		void* input_node = nullptr;
		/// Descriptor of input slot.
		const char* input_slot = nullptr;
		/// `id` that was passed to BeginNode() of output node.
		void* output_node = nullptr;
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

	class Node
	{
	public:
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, SpvObj _obj);

		~Node();

		const char* getTitle() const { return m_pTitle; }
		SpvObj getSpvObj() const { return m_spv; }

		void update();

		void addInputSlot(Slot _kind);
		void addOutputSlot(Slot _kind);

		void clear();

	private:
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, Type _type);

		void disconnect(const Connection& _con);
	
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

		spvgentwo::Vector<ImNodes::Ez::SlotInfo> m_inputSlots;
		spvgentwo::Vector<ImNodes::Ez::SlotInfo> m_outputSlots;
		spvgentwo::List<Connection> m_connections;
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