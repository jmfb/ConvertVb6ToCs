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

	void WriteCs(VbCodeWriter& writer) const final
	{
		expression->WriteCs(writer);
		writer.out << "(";
		auto first = true;
		for (auto& argument : arguments)
		{
			if (!first)
				writer.out << ", ";
			first = false;
			//TODO: need to inspect if target function parameter is byref to add "ref " keyword.
			argument->WriteCs(writer);
		}
		writer.out << ")";
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeExpressionPtr> arguments;
};
