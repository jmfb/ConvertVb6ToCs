#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"

class VbCodeExpressionStatement : public VbCodeStatement
{
public:
	VbCodeExpressionStatement(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<expression-statement>";
		expression->WriteXml(out);
		out << "</expression-statement>";
	}

	VbCodeExpressionPtr expression;
};
