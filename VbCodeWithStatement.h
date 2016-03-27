#pragma once
#include "VbCodeStatement.h"
#include "VbCodeExpression.h"
#include <vector>

class VbCodeWithStatement : public VbCodeStatement
{
public:
	VbCodeWithStatement(VbCodeExpressionPtr expression)
		: expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<with-statement>";
		out << "<with>";
		expression->WriteXml(out);
		out << "</with>";
		for (auto& statement : statements)
			statement->WriteXml(out);
		out << "</with-statement>";
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.StartLine();
		writer.out << "var ";
		writer.PushWith();
		writer.out << " = ";
		expression->WriteCs(writer);
		writer.out << ";" << std::endl;
		for (auto& statement : statements)
			statement->WriteCs(writer);
	}

	bool MatchesEnd(VbCodeEndType end) const final
	{
		return end == VbCodeEndType::With;
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeStatementPtr> statements;
};
