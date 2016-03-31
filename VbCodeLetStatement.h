#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"

class VbCodeLetStatement : public VbCodeStatement
{
public:
	VbCodeLetStatement(VbCodeExpressionPtr lValue, VbCodeExpressionPtr expression)
		: lValue(lValue), expression(expression)
	{
	}

	void WriteCs(VbCodeWriter& writer) const final
	{
		writer.StartLine();
		lValue->WriteCs(writer);
		writer.out << " = ";
		expression->WriteCs(writer);
		writer.out << ";" << std::endl;
	}

	VbCodeExpressionPtr lValue;
	VbCodeExpressionPtr expression;
};
