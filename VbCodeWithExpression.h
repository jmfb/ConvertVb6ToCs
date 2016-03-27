#pragma once
#include "VbCodeExpression.h"
#include "VbCodeDotType.h"
#include <string>

class VbCodeWithExpression : public VbCodeExpression
{
public:
	VbCodeWithExpression(VbCodeDotType dot, const std::string& member)
		: dot(dot), member(member)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<with-expression>" << ToString(dot) << member << "</with-expression>";
	}

	VbCodeDotType dot;
	std::string member;
};
