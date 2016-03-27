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

	bool MatchesEnd(VbCodeEndType end) const final
	{
		return end == VbCodeEndType::With;
	}

	VbCodeExpressionPtr expression;
	std::vector<VbCodeStatementPtr> statements;
};
