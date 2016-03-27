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

	void WriteXml(std::ostream& out) const final
	{
		out << "<call-expression>";
		expression->WriteXml(out);
		out << "<arguments>";
		for (auto& argument : arguments)
			argument->WriteXml(out);
		out << "</arguments>";
		out << "</call-expression>";
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeExpressionPtr> arguments;
};
