#pragma once
#include "VbCodeExpression.h"

class VbCodeParenthesisExpression : public VbCodeExpression
{
public:
	VbCodeParenthesisExpression(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<parenthesis-expression>";
		expression->WriteXml(out);
		out << "</parenthesis-expression>";
	}

	VbCodeExpressionPtr expression;
};
