#pragma once
#include "VbCodeStatement.h"
#include "VbCodeCaseExpression.h"
#include "VbCodeWriter.h"
#include <vector>

class VbCodeCaseBlock
{
public:
	VbCodeCaseBlock() = default;
	VbCodeCaseBlock(const std::vector<VbCodeCaseExpression>& expressions)
		: expressions(expressions)
	{
	}

	void WriteIfExpression(VbCodeWriter& writer, int select) const
	{
		expressions[0].WriteIfExpression(writer, select);
		for (auto index = 1u; index < expressions.size(); ++index)
		{
			writer.out << " || ";
			expressions[index].WriteIfExpression(writer, select);
		}
	}

	std::vector<VbCodeCaseExpression> expressions;
	std::vector<VbCodeStatementPtr> statements;
};
