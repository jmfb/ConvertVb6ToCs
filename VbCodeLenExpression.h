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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.out << "Len(";
		expression->WriteCs(writer);
		writer.out << ")";
	}

	VbCodeExpressionPtr expression;
};
