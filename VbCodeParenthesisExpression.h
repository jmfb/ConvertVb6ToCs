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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.out << "(";
		expression->WriteCs(writer);
		writer.out << ")";
	}

	VbCodeExpressionPtr expression;
};
