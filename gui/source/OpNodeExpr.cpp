#include "proto/OpNodeExpr.h"
#include "spvgentwo/Module.h"

using namespace proto;
using namespace spvgentwo;

OpNodeExpr::OpNodeExpr(spvgentwo::IAllocator* _pAlloc,ImVec2 _pos, OpNodeType _type) :
    m_pos(_pos),
	m_type(_type),
	m_connections(_pAlloc),
	m_inputSlots(_pAlloc),
	m_outputSlots(_pAlloc)
{
	for (auto i = 0u; i < getInfo().numInputs; ++i)
	{
		m_inputSlots.emplace_back("In", /*(int)i + */1);
	}
	for (auto i = 0u; i < getInfo().numOutputs; ++i)
	{
		m_outputSlots.emplace_back("Out", /*(int)i +*/ 1);
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
        makeVar();
        m_pResult = (*m_pBB)->opLoad(m_pVar);
        break;
    case OpNodeType::OutVar: // turn var desc into opVar & store
        makeVar();
        (*m_pBB)->opStore(m_pVar, lhs);
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
        m_pResult = (*m_pBB)->Add(lhs, rhs);
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

void OpNodeExpr::makeVar()
{
    Module* pModule = m_pBB->getModule();
    Instruction* pType = pModule->addType(m_pVarDesc->type); // type needs to be a pointer with storage class
    if (m_pVarDesc->storageClass == spv::StorageClass::Function)
    {
        m_pVar = m_pBB->getFunction()->variable(pType, nullptr, m_pVarDesc->name);
    }
    else
    {
        m_pVar = pModule->variable(pType, m_pVarDesc->storageClass, m_pVarDesc->name, nullptr);
    }
}

void  OpNodeExpr::makeConst()
{
    m_pResult = m_pBB->getModule()->addConstant(m_pConstDesc->constant, m_pConstDesc->name);
}

void OpNodeExpr::update()
{
	const char* name = getInfo().name;
	if (ImNodes::Ez::BeginNode(this, name, &m_pos, &m_selected))
	{
		ImNodes::Ez::InputSlots(m_inputSlots.data(), (int)m_inputSlots.size());

		// draw body here

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
				// mark fore removal
				m_toBeRemoved = true;

				// Remove deleted connections
				((OpNodeExpr*)con.input_node)->remove(con);
				it = ((OpNodeExpr*)con.output_node)->remove(con); // output node == this
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

void OpNodeExpr::connect(const Connection& _con)
{
	OpNodeExpr* in = (OpNodeExpr*)_con.input_node;
	OpNodeExpr* out = (OpNodeExpr*)_con.output_node;

	if (in->m_connections.contains(_con) == false && out->m_connections.contains(_con) == false)
	{
		in->m_connections.emplace_back(_con);
		out->m_connections.emplace_back(_con);
	}
}

void OpNodeExpr::disconnect(const Connection& _con)
{
	OpNodeExpr* in = (OpNodeExpr*)_con.input_node;
	OpNodeExpr* out = (OpNodeExpr*)_con.output_node;

	in->remove(_con);
	out->remove(_con);
}