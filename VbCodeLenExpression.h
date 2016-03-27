#pragma once
#include "VbCodeExpression.h"

class VbCodeLenExpression : public VbCodeExpression
{
public:
	VbCodeLenExpression(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<len-expression>";
		expression->WriteXml(out);
		out << "</len-expression>";
	}

	VbCodeExpressionPtr expression;
};
