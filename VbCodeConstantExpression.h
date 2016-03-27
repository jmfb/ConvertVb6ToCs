#pragma once
#include "VbCodeExpression.h"
#include "VbCodeValue.h"

class VbCodeConstantExpression : public VbCodeExpression
{
public:
	VbCodeConstantExpression(VbCodeValue value)
		: value(value)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<constant-expression>";
		value.WriteXml(out);
		out << "</constant-expression>";
	}
	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		value.WriteValueCs(writer.out);
	}

	VbCodeValue EvaluateConstant() const final
	{
		return value;
	}

public:
	VbCodeValue value;
};
