#include "proto/Node.h"

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, Type _type) :
	m_pAlloc(_pAlloc),
	m_pTitle(_pTitle),
	m_spv{nullptr},
	m_Type(_type),
	m_inputs(_pAlloc),
	m_outputs(_pAlloc),
	m_pos(_pos)
{
}

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::Instruction* _instr) :
	Node(_pAlloc, _pTitle, _pos, Type::Instruction)
{
	m_spv.instr = _instr;
}

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::BasicBlock* _bb) :
	Node(_pAlloc, _pTitle, _pos, Type::Instruction)
{
	m_spv.bb = _bb;
}

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::Function* _func) :
	Node(_pAlloc, _pTitle, _pos, Type::Instruction)
{
	m_spv.func = _func;
}

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, spvgentwo::EntryPoint* _ep) :
	Node(_pAlloc, _pTitle, _pos, Type::Instruction)
{
	m_spv.ep = _ep;
}

proto::Node::~Node()
{
}

void proto::Node::update()
{
	if (ImNodes::Ez::BeginNode(this, m_pTitle, &m_pos, &m_selected))
	{
		ImNodes::Ez::InputSlots(m_inputSlots.data(), (int)m_inputSlots.size());
		ImNodes::Ez::OutputSlots(m_outputSlots.data(), (int)m_outputSlots.size());
		ImNodes::Ez::EndNode();
	}

	for (Node* in : m_inputs)
	{
		ImNodes::Connection(in, "In", this, "This");
	}

	for (Node* out : m_outputs)
	{
		ImNodes::Connection(this, "This", out, "Out");
	}
}

void proto::Node::addInputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pInput)
{
	m_inputSlots.emplace_back(ImNodes::Ez::SlotInfo{ _pSlotTitle, (int)_kind });

	if (_pInput != nullptr)
	{
		m_inputs.emplace_back(_pInput);	
	}
}

void proto::Node::addOutputSlot(const char* _pSlotTitle, SlotKind _kind, Node* _pOutput)
{
	m_outputSlots.emplace_back(ImNodes::Ez::SlotInfo{ _pSlotTitle, (int)_kind });

	if (_pOutput != nullptr)
	{
		m_outputs.emplace_back(_pOutput);
	}
}

void proto::Node::clear()
{
	m_inputs.clear();
	m_inputSlots.clear();

	m_outputs.clear();
	m_outputSlots.clear();
}
