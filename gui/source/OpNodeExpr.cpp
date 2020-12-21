#include "proto/OpNodeExpr.h"
#include "spvgentwo/Module.h"
#include "spvgentwo/Templates.h"

#include "proto/Logger.h"

using namespace proto;
using namespace spvgentwo;

OpNodeExpr::OpNodeExpr(OpNodeExpr&& _other) noexcept:
	m_type(_other.m_type),
	m_pBB(_other.m_pBB),
	m_pResult(_other.m_pResult),
	m_pVar(_other.m_pVar),
	m_pos(_other.m_pos),
	m_selected(_other.m_selected),
	m_varDesc(stdrep::move(_other.m_varDesc)),
	m_constDesc(stdrep::move(_other.m_constDesc)),
	m_inputSlots(stdrep::move(_other.m_inputSlots)),
	m_outputSlots(stdrep::move(_other.m_outputSlots)),
	m_connections(stdrep::move(_other.m_connections)),
	m_pGraph(_other.m_pGraph),
	m_pParent(_other.m_pParent),
	m_typeComboBox(stdrep::move(_other.m_typeComboBox)),
	m_storageClassCombobox(stdrep::move(_other.m_storageClassCombobox))
{
	_other.m_pBB = nullptr;
	_other.m_pResult = nullptr;
	_other.m_pVar = nullptr;
	_other.m_pGraph = nullptr;
	_other.m_pGraph = nullptr;
}

OpNodeExpr::OpNodeExpr(ImVec2 _pos, OpNodeType _type) :
	m_type(_type),
    m_pos(_pos),
	m_varDesc{ HeapAllocator::instance() },
	m_constDesc{ HeapAllocator::instance() },
	m_inputSlots(),
	m_outputSlots(),
	m_connections(),
	m_typeComboBox("Type"),
	m_storageClassCombobox("StorageClass",
		"Input", spv::StorageClass::Input,
		"Output", spv::StorageClass::Output,
		"Uniform", spv::StorageClass::Uniform,
		"UniformConstant", spv::StorageClass::UniformConstant,
		"StorageBuffer", spv::StorageClass::StorageBuffer)
{
	const auto& info = getInfo();
	for (const char* in : info.inputs)
	{
		m_inputSlots.emplace_back(in, 1);
	}
	for (const char* out : info.outputs)
	{
		m_outputSlots.emplace_back(out, 1);
	}

	if (_type == OpNodeType::InVar)
	{
		m_storageClassCombobox.setSelectedIndex(0);
	}
	else if (_type == OpNodeType::OutVar)
	{
		m_storageClassCombobox.setSelectedIndex(1);
	}
}

OpNodeExpr::~OpNodeExpr()
{
}

void OpNodeExpr::operator()(const List<OpNodeExpr*>& _inputs, const List<OpNodeExpr*>& _outputs)
{
	Instruction* lhs = _inputs.empty() ? nullptr : _inputs.front()->m_pResult;
	Instruction* rhs = _inputs.size() > 1u ? (*(_inputs.begin()+1u))->m_pResult : nullptr;

	if ((lhs != nullptr && lhs->getOperation() == spv::Op::OpNop) || (rhs != nullptr && rhs->getOperation() == spv::Op::OpNop))
	{
		return;
	}

	switch (m_type)
	{
    case OpNodeType::InVar: // turn var desc into opVar & load
		if (makeVar())
		{
			m_pResult = (*m_pBB)->opLoad(m_pVar);		
		}
        break;
    case OpNodeType::OutVar: // turn var desc into opVar & store
		if(makeVar() && lhs != nullptr)
		{
			(*m_pBB)->opStore(m_pVar, lhs);
		}
        break;
    case OpNodeType::Const:
        makeConst();
        break;
    case OpNodeType::Equal:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Equal(lhs, rhs);
		}
        break;
    case OpNodeType::NotEqual:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->NotEqual(lhs, rhs);
		}
        break;
    case OpNodeType::Less:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Less(lhs, rhs);
		}
        break;
    case OpNodeType::LessEqual:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->LessEqual(lhs, rhs);
		}
        break;
    case OpNodeType::Greater:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Greater(lhs, rhs);
		}
        break;
    case OpNodeType::GreaterEqual:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->GreaterEqual(lhs, rhs);
		}
        break;
    case OpNodeType::Add:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Add(lhs, rhs);
		}
        break;
    case OpNodeType::Sub:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Sub(lhs, rhs);
		}
        break;
    case OpNodeType::Mul:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Mul(lhs, rhs);
		}
        break;
    case OpNodeType::Div:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Div(lhs, rhs);
		}
        break;
    case OpNodeType::Dot:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->opDot(lhs, rhs);
		}
        break;
    case OpNodeType::Select:
		if (_inputs.size() == 3u && _inputs.back()->m_pResult != nullptr)
		{
			m_pResult = (*m_pBB)->opSelect(lhs, rhs, _inputs.back()->m_pResult);
		}
        break;
    case OpNodeType::Cast:
        break;
    default:
        break;
	}
}

void OpNodeExpr::setBasicBlock(spvgentwo::BasicBlock* _pBB)
{
	m_pBB = _pBB;
}

void OpNodeExpr::setParent(spvgentwo::ExprGraph<OpNodeExpr>* _pGraph, typename spvgentwo::ExprGraph<OpNodeExpr>::NodeType* _pParent)
{
	m_pGraph = _pGraph;
	m_pParent = _pParent;
}

bool OpNodeExpr::makeVar()
{
	if (m_varDesc.type.isVoid())
		return false;

    Module* pModule = m_pBB->getModule();
    Instruction* pType = pModule->addType(m_varDesc.type); // type needs to be a pointer with storage class
    if (m_varDesc.storageClass == spv::StorageClass::Function)
    {
        m_pVar = m_pBB->getFunction()->variable(pType, m_varDesc.name);
    }
    else
    {
        m_pVar = pModule->variable(pType, m_varDesc.storageClass, m_varDesc.name, nullptr);
    }

	return m_pVar != nullptr;
}

bool OpNodeExpr::makeConst()
{
    m_pResult = m_pBB->getModule()->addConstant(m_constDesc.constant, m_constDesc.name);
	return m_pResult != nullptr;
}

void OpNodeExpr::updateOpDesc()
{
	switch (m_type)
	{
	case OpNodeType::InVar:
	case OpNodeType::OutVar:
		updateVarDesc();
		break;
	case OpNodeType::Const:
		updateConstDesc();
		break;
	default:
		break;
	}
}

void OpNodeExpr::updateVarDesc()
{
	m_typeComboBox.update();
	m_storageClassCombobox.update();
	m_varDesc.storageClass = m_storageClassCombobox.getSelectedItem();
	m_varDesc.type = m_typeComboBox.getType().wrapPointer(m_varDesc.storageClass);
}

void OpNodeExpr::updateConstDesc()
{
	m_constDesc.constant.reset();
	m_typeComboBox.update();

	const Type& t = m_typeComboBox.getType();
	m_dataInput.update(t, m_constDesc.constant);
}

bool OpNodeExpr::update()
{
	const char* name = getInfo().name;
	if (ImNodes::Ez::BeginNode(this, name, &m_pos, &m_selected))
	{
		ImNodes::Ez::InputSlots(m_inputSlots.data(), (int)m_inputSlots.size());

		ImGui::BeginGroup();

		ImGui::PushItemWidth(400/*ImGui::GetContentRegionAvail().x * 0.25f*/);

		// draw body here
		updateOpDesc();

		ImGui::PopItemWidth();

		ImGui::EndGroup();

		ImNodes::Ez::OutputSlots(m_outputSlots.data(), (int)m_outputSlots.size());

		Connection con{};
		if (ImNodes::GetNewConnection((void**)&con.input_node, &con.input_slot,
			(void**)&con.output_node, &con.output_slot) && allowedConnection(con))
		{
			logInfo("Connected %s -> %s", con.output_node->getInfo().name, con.input_node->getInfo().name);

			con.input_node->m_connections.emplace_back(con);
			con.output_node->m_connections.emplace_back(con);
			
			con.output_node->m_pParent->connect(con.input_node->m_pParent);
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
				logInfo("Disconnected %s -> %s", con.output_node->getInfo().name, con.input_node->getInfo().name);

				con.output_node->m_pParent->disconnect(con.input_node->m_pParent);

				// Remove deleted connections
				con.input_node->remove(con);
				it = con.output_node->remove(con); // output node == this
			}
			else
			{
				++it;
			}
		}
	}
	ImNodes::Ez::EndNode();

	if (m_selected && ImGui::IsKeyPressedMap(ImGuiKey_Delete))
	{
		for (auto& con : m_connections)
		{
			logInfo("Disconnected %s -> %s", con.output_node->getInfo().name, con.input_node->getInfo().name);
			con.output_node->m_pParent->disconnect(con.input_node->m_pParent);

			if (con.output_node == this)
			{
				con.input_node->remove(con);
			}
			else
			{
				con.output_node->remove(con);
			}
		}

		m_connections.clear();
		return true; // remove node
	}

	return false;
}

bool OpNodeExpr::allowedDisconnection(const Connection& _con)
{
	return true;
}

bool OpNodeExpr::allowedConnection(const Connection& _con)
{
	// check if the input slot is taken
	for (const Connection& con : m_connections)
	{
		if (strcmp(con.input_slot, _con.input_slot) == 0)
		{
			return false;
		}
	}

	return true;
}

spvgentwo::List<proto::Connection>::Iterator OpNodeExpr::remove(const Connection& _con)
{
	auto it = m_connections.find(_con);

	if (it != nullptr)
	{
		return m_connections.erase(it);
	}

	return it;
}