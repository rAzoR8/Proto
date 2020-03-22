#include "proto/OpNodeExpr.h"
#include "spvgentwo/Module.h"

using namespace proto;
using namespace spvgentwo;

OpNodeExpr::OpNodeExpr(OpNodeExpr&& _other) noexcept:
	m_type(_other.m_type),
	m_pBB(_other.m_pBB),
	m_pResult(_other.m_pResult),
	m_pVar(_other.m_pVar),
	m_pos(_other.m_pos),
	m_selected(_other.m_selected),
	m_toBeRemoved(_other.m_toBeRemoved),
	m_varDesc(stdrep::move(_other.m_varDesc)),
	m_constDesc(stdrep::move(_other.m_constDesc)),
	m_inputSlots(stdrep::move(_other.m_inputSlots)),
	m_outputSlots(stdrep::move(_other.m_outputSlots)),
	m_connections(stdrep::move(_other.m_connections)),
	m_pGraph(_other.m_pGraph),
	m_pParent(_other.m_pParent),
	m_typeComboBox(stdrep::move(_other.m_typeComboBox))
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
	m_typeComboBox("Type")
{
	for (auto i = 0u; i < getInfo().numInputs; ++i)
	{
		m_inputSlots.emplace_back(g_OpNodeInputName[i], 1);
	}
	for (auto i = 0u; i < getInfo().numOutputs; ++i)
	{
		m_outputSlots.emplace_back(g_OpNodeOutputName[i], 1);
	}
}

OpNodeExpr::~OpNodeExpr()
{
}

void OpNodeExpr::operator()(const List<OpNodeExpr*>& _inputs, const List<OpNodeExpr*>& _outputs)
{
	Instruction* lhs = _inputs.empty() ? nullptr : _inputs.front()->m_pResult;
	Instruction* rhs = _inputs.size() == 2u ? _inputs.back()->m_pResult : nullptr;

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
        break;
    case OpNodeType::NotEqual:
        break;
    case OpNodeType::Less:
        break;
    case OpNodeType::LessEqual:
        break;
    case OpNodeType::Greater:
        break;
    case OpNodeType::GreaterEqual:
        break;
    case OpNodeType::Add:
		if (lhs != nullptr && rhs != nullptr)
		{
			m_pResult = (*m_pBB)->Add(lhs, rhs);
		}
        break;
    case OpNodeType::Sub:
        break;
    case OpNodeType::Mul:
        break;
    case OpNodeType::Div:
        break;
    case OpNodeType::Dot:
        break;
    case OpNodeType::Select:
        break;
    case OpNodeType::Cast:
        break;
    case OpNodeType::NumOf:
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
        m_pVar = m_pBB->getFunction()->variable(pType, nullptr, m_varDesc.name);
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
	m_varDesc.type = m_typeComboBox.getType();
}

void OpNodeExpr::updateConstDesc()
{
	m_constDesc.constant.reset();
	m_typeComboBox.update();

	const Type& t = m_typeComboBox.getType();
	m_dataInput.update(t, m_constDesc.constant);
}

void OpNodeExpr::update()
{
	const char* name = getInfo().name;
	if (ImNodes::Ez::BeginNode(this, name, &m_pos, &m_selected))
	{
		ImNodes::Ez::InputSlots(m_inputSlots.data(), (int)m_inputSlots.size());

		ImGui::BeginGroup();

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);

		// draw body here
		updateOpDesc();

		ImGui::PopItemWidth();

		ImGui::EndGroup();

		ImNodes::Ez::OutputSlots(m_outputSlots.data(), (int)m_outputSlots.size());

		Connection con{};
		if (ImNodes::GetNewConnection((void**)&con.input_node, &con.input_slot,
			(void**)&con.output_node, &con.output_slot) && allowedConnection(con))
		{
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
				// mark fore removal
				m_toBeRemoved = true;

				con.output_node->m_pParent->connect(con.input_node->m_pParent);

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
}

void OpNodeExpr::clear()
{
	m_inputSlots.clear();
	m_outputSlots.clear();

	m_connections.clear();
}

bool OpNodeExpr::allowedDisconnection(const Connection& _con)
{
	//return strcmp(_con.input_slot, "FuncEntry") != 0;
	return true;
}

bool OpNodeExpr::allowedConnection(const Connection& _con)
{
	return (_con.input_node->m_connections.contains(_con) == false && _con.output_node->m_connections.contains(_con) == false);
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