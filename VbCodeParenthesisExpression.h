#pragma once
#include "VbCodeExpression.h"

class VbCodeParenthesisExpression : public VbCodeExpression
{
public:
	VbCodeParenthesisExpression(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteCs(VbCodeWriter& writer) const final
	{
		writer.out << "(";
		expression->WriteCs(writer);
		writer.out << ")";
	}

	VbCodeExpressionPtr expression;
};
