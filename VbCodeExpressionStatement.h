#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"

class VbCodeExpressionStatement : public VbCodeStatement
{
public:
	VbCodeExpressionStatement(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<expression-statement>";
		expression->WriteXml(out);
		out << "</expression-statement>";
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.StartLine();
		writer.out << "//TODO: expression-statement";
		writer.out << ";" << std::endl;
	}

	VbCodeExpressionPtr expression;
};
