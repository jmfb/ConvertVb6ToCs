#pragma once
#include "VbCodeExpression.h"
#include "VbCodeStatement.h"
#include <vector>
#include <iostream>

class VbCodeIfBlock
{
public:
	VbCodeIfBlock(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeStatementPtr> statements;
};
