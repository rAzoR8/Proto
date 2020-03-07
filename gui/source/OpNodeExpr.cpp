#include "proto/OpNodeExpr.h"

using namespace proto;
using namespace spvgentwo;

OpNodeExpr::OpNodeExpr(OpNodeType _type) :
	m_type(_type)
{
}

OpNodeExpr::~OpNodeExpr()
{
}

void OpNodeExpr::operator()(const List<OpNodeExpr*>& _inputs, const List<OpNodeExpr*>& _outputs)
{
	Instruction* lhs = _inputs.empty() ? nullptr : _inputs.front()->m_pResult;
	Instruction* rhs = _inputs.size() == 2u ? _inputs.back()->m_pResult : nullptr;
}
