#pragma once
#include "VbCodeExpression.h"

class VbCodeLenExpression : public VbCodeExpression
{
public:
	VbCodeLenExpression(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.out << "Len(";
		expression->WriteCs(writer);
		writer.out << ")";
	}

	VbCodeExpressionPtr expression;
};
