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

	void WriteXml(std::ostream& out) const final
	{
		out << "<id-expression>" << name << "</id-expression>";
	}

	std::string name;
};
