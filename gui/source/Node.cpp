#include "proto/Node.h"

#include "SpvGenTwo/EntryPoint.h"

using namespace spvgentwo;

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, SpvObj _obj) :
	m_pAlloc(_pAlloc),
	m_pTitle(_pTitle),
	m_spv( _obj ),
	m_inputs(_pAlloc),
	m_outputs(_pAlloc),
	m_pos(_pos)
{
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

		switch (m_spv.type)
		{
		case Type::Instruction:
			updateInstruction();
			break;
		case Type::BasicBlock:
			updateBasicBlock();
			break;
		case Type::Function:
			updateFunction();
			break;
		case Type::EntryPoint:
			updateEntryPoint();
			break;
		default:
			break;
		}

		ImNodes::Ez::EndNode();
	}

	//for (Node* in : m_inputs)
	//{
	//	ImNodes::Connection(in, "In", this, "This");
	//}

	//for (Node* out : m_outputs)
	//{
	//	ImNodes::Connection(this, "This", out, "Out");
	//}
}

void proto::Node::addInputSlot(const char* _pSlotTitle, Slot _kind)
{
	m_inputSlots.emplace_back(ImNodes::Ez::SlotInfo{ _pSlotTitle, (int)_kind });
}

void proto::Node::addOutputSlot(const char* _pSlotTitle, Slot _kind)
{
	m_outputSlots.emplace_back(ImNodes::Ez::SlotInfo{ _pSlotTitle, (int)_kind });
}

void proto::Node::connect(const char* _pSlotTitle, Node* _pTarget)
{
	ImNodes::Connection(_pTarget, _pSlotTitle, this, _pSlotTitle);

	//m_outputs.emplace_back(_pTarget);
	//_pTarget->m_outputs.emplace_back(this);
}

void proto::Node::clear()
{
	m_inputs.clear();
	m_inputSlots.clear();

	m_outputs.clear();
	m_outputSlots.clear();
}

void proto::Node::updateEntryPoint()
{
	EntryPoint& ep = *m_spv.obj.ep;
	ImGui::Text("EntryPoint %s", ep.getName());
}

void proto::Node::updateFunction()
{
	Function& func = *m_spv.obj.func;
	ImGui::Text("%s", func.getName());

	if (m_selected && ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
		ImGui::OpenPopup("FunctionContextMenu");
	}

	if (ImGui::BeginPopup("FunctionContextMenu"))
	{
		if (ImGui::MenuItem("Add BasicBlock"))
		{
			func.addBasicBlock("Block");
		}

		if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	
}

void proto::Node::updateBasicBlock()
{
	BasicBlock& bb = *m_spv.obj.bb;
	//const char* label = bb.getName();
	//ImGui::Text("Label: %s", label);

	for (Instruction& i : bb) 
	{
		const char* name = i.getName();
		ImGui::Text("%u = %u %s", i.getResultId(), (unsigned int)i.getOperation(), name);
	}
}

void proto::Node::updateInstruction()
{
	Instruction& i = *m_spv.obj.instr;
	ImGui::Text("%u = %u %s", i.getResultId(), (unsigned int)i.getOperation(), i.getName());
}
