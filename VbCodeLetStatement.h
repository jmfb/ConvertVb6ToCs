#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"

class VbCodeLetStatement : public VbCodeStatement
{
public:
	VbCodeLetStatement(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	//TODO: l-value
	VbCodeExpressionPtr expression;
};
