#pragma once
#include "VbCodeExpression.h"
#include <string>

class VbCodeIdExpression : public VbCodeExpression
{
public:
	VbCodeIdExpression(const std::string& name)
		: name(name)
	{
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.out << writer.Resolve(name);
	}

	std::string name;
};
