#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"

class VbCodeLetStatement : public VbCodeStatement
{
public:
	VbCodeLetStatement(VbCodeExpressionPtr lValue, VbCodeExpressionPtr expression)
		: lValue(lValue), expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<let-statement>";
		out << "<l-value>";
		lValue->WriteXml(out);
		out << "</l-value>";
		out << "<r-value>";
		expression->WriteXml(out);
		out << "</r-value>";
		out << "</let-statement>";
	}

	VbCodeExpressionPtr lValue;
	VbCodeExpressionPtr expression;
};
