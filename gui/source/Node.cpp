#include "proto/Node.h"

#include "spvgentwo/EntryPoint.h"
#include "spvgentwo/Type.h"
#include "spvgentwo/String.h"

using namespace spvgentwo;

proto::Node::Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos, SpvObj _obj) :
	m_pAlloc(_pAlloc),
	m_pTitle(_pTitle),
	m_spv( _obj ),
	m_pos(_pos),
	m_inputSlots(_pAlloc),
	m_outputSlots(_pAlloc),
	m_connections(_pAlloc)
{
	switch (m_spv.type)
	{
	case Type::Instruction:
		break;
	case Type::BasicBlock:
		addInputSlot(Slot::FuncEntry, "FuncEntry");
		break;
	case Type::Function:
		addOutputSlot(Slot::FuncEntry, "EntryBlock");
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

		Connection con{};
		if (ImNodes::GetNewConnection(&con.input_node, &con.input_slot,
			&con.output_node, &con.output_slot) && allowedConnection(con))
		{
			connect(con);
		}

		// only render outputs
		for (auto it = m_connections.begin(); it != m_connections.end();)
		{
			const Connection& con = *it;
			if (con.output_node != this)
			{
				++it;
				continue;
			}

			if (ImNodes::Connection(con.input_node, con.input_slot,
				con.output_node, con.output_slot) == false && allowedDisconnection(con))
			{
				// Remove deleted connections
				((Node*)con.input_node)->remove(con);
				it = ((Node*)con.output_node)->remove(con); // // output node == this
			}
			else
			{
				++it;
			}
		}
	}
	ImNodes::Ez::EndNode();
}

void proto::Node::addInputSlot(Slot _kind, const char* _pTitle)
{
	m_inputSlots.emplace_back(_pTitle, (int)_kind);
}

void proto::Node::addOutputSlot(Slot _kind, const char* _pTitle)
{
	m_outputSlots.emplace_back(_pTitle, (int)_kind);
}

void proto::Node::clear()
{
	m_inputSlots.clear();
	m_outputSlots.clear();

	m_connections.clear();
}

bool proto::Node::allowedDisconnection(const Connection& _con)
{
	return strcmp(_con.input_slot, "FuncEntry") != 0;
	//return true;
}

bool proto::Node::allowedConnection(const Connection& _con)
{
	return true;
}

spvgentwo::List<proto::Connection>::Iterator proto::Node::remove(const Connection& _con)
{
	auto it = m_connections.find(_con);

	if (it != nullptr)
	{
		return m_connections.erase(it);
	}

	return it;
}

void proto::Node::connect(const Connection& _con)
{
	Node* in = (Node*)_con.input_node;
	Node* out = (Node*)_con.output_node;

	if (in->m_connections.contains(_con) == false && out->m_connections.contains(_con) == false)
	{
		in->m_connections.emplace_back(_con);
		out->m_connections.emplace_back(_con);
	}
}

void proto::Node::disconnect(const Connection& _con)
{
	Node* in = (Node*)_con.input_node;
	Node* out = (Node*)_con.output_node;

	in->remove(_con);
	out->remove(_con);
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