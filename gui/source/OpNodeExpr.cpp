#include "proto/OpNodeExpr.h"
#include "spvgentwo/Module.h"

using namespace proto;
using namespace spvgentwo;

OpNodeExpr::OpNodeExpr(spvgentwo::BasicBlock* _pBB, OpNodeType _type) :
	m_type(_type),
	m_pBB(_pBB)
{
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
