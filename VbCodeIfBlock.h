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

	void WriteXml(std::ostream& out) const
	{
		out << "<condition>";
		expression->WriteXml(out);
		out << "</condition>";
		for (auto& statement : statements)
			statement->WriteXml(out);
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeStatementPtr> statements;
};
