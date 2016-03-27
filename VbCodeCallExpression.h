#pragma once
#include "VbCodeExpression.h"
#include <vector>

class VbCodeCallExpression : public VbCodeExpression
{
public:
	VbCodeCallExpression(
		VbCodeExpressionPtr expression,
		const std::vector<VbCodeExpressionPtr>& arguments)
		: expression(expression), arguments(arguments)
	{
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeExpressionPtr> arguments;
};
