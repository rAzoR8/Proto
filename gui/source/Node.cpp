#include "proto/Node.h"

#include "SpvGenTwo/EntryPoint.h"
#include "SpvGenTwo/Type.h"
#include "SpvGenTwo/String.h"

using namespace spvgentwo;

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, SpvObj _obj) :
	m_pAlloc(_pAlloc),
	m_pTitle(_pTitle),
	m_spv( _obj ),
	m_inputs(_pAlloc),
	m_inputSlots(_pAlloc),
	m_outputs(_pAlloc),
	m_outputSlots(_pAlloc),
	m_pos(_pos)
{
	switch (m_spv.type)
	{
	case Type::Instruction:
		break;
	case Type::BasicBlock:
		addInputSlot("EntryBlock",Slot::EntryBlock);
		break;
	case Type::Function:
		addOutputSlot("EntryBlock", Slot::EntryBlock);
		break;
	case Type::EntryPoint:
		break;
	default:
		break;
	}
}

proto::Node::~Node()
{
}

void proto::Node::update()
{
	if (ImNodes::Ez::BeginNode(this, m_pTitle, &m_pos, &m_selected))
	{
		ImNodes::Ez::InputSlots(m_inputSlots.data(), (int)m_inputSlots.size());
		
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
		
		ImNodes::Ez::OutputSlots(m_outputSlots.data(), (int)m_outputSlots.size());

		// only render outputs
		for (const Connection& out : m_outputs)
		{
			ImNodes::Connection(this, "EntryBlock", out.node, "EntryBlock");
		}

		for (const Connection& in : m_inputs)
		{
			ImNodes::Connection(in.node, "EntryBlock", this, "EntryBlock"); // todo replace slot
		}

		//for (const Connection& in : m_inputs)
		//{
		//	ImNodes::Connection(in.node, "EntryBlock", this, "EntryBlock");
		//}

	}
	ImNodes::Ez::EndNode();
}

void proto::Node::addInputSlot(const char* _pSlotTitle, Slot _kind)
{
	m_inputSlots.emplace_back( _pSlotTitle, (int)_kind);
}

void proto::Node::addOutputSlot(const char* _pSlotTitle, Slot _kind)
{
	m_outputSlots.emplace_back(_pSlotTitle, (int)_kind);
}

// connect this (source _ output) to input
void proto::Node::connect(const char* _pSrcSlot, Node* _pTarget, const char* _pDstSlot)
{
	m_outputs.emplace_back(_pTarget, _pSrcSlot, _pDstSlot);
	_pTarget->m_inputs.emplace_back(this, _pSrcSlot, _pDstSlot);
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

	String args(m_pAlloc);
	const char* ret =  func.getReturnType()->getType()->getString();
	const char* name =  func.getName();
	
	for (auto it = func.getParameters().begin(), end = func.getParameters().end(); it != end; ++it)
	{
		args += it->getType()->getString();
		if (it + 1 != end)
		{
			args += ", ";		
		}
	}

	ImGui::Text("%s %s(%s)", ret, name, args.c_str());

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

	//for (Instruction& i : bb) 
	//{
	//	const char* name = i.getName();
	//	ImGui::Text("%u = %u %s", i.getResultId(), (unsigned int)i.getOperation(), name);
	//}
}

void proto::Node::updateInstruction()
{
	Instruction& i = *m_spv.obj.instr;
	ImGui::Text("%u = %u %s", i.getResultId(), (unsigned int)i.getOperation(), i.getName());
}
