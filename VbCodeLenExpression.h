#pragma once
#include "VbCodeExpression.h"

class VbCodeLenExpression : public VbCodeExpression
{
public:
	VbCodeLenExpression(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	VbCodeExpressionPtr expression;
};
