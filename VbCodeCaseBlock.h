#pragma once
#include "VbCodeStatement.h"
#include "VbCodeCaseExpression.h"
#include <vector>

class VbCodeCaseBlock
{
public:
	VbCodeCaseBlock() = default;
	VbCodeCaseBlock(const std::vector<VbCodeCaseExpression>& expressions)
		: expressions(expressions)
	{
	}

	std::vector<VbCodeCaseExpression> expressions;
	std::vector<VbCodeStatementPtr> statements;
};
