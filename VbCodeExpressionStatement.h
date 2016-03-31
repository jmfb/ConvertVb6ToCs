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

	void WriteCs(VbCodeWriter& writer) const final
	{
		writer.StartLine();
		expression->WriteCs(writer);
		writer.out << ";" << std::endl;
	}

	VbCodeExpressionPtr expression;
};
