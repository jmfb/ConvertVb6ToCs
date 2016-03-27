#pragma once
#include "VbCodeExpression.h"
#include "VbCodeDotType.h"
#include <string>

class VbCodeMemberExpression : public VbCodeExpression
{
public:
	VbCodeMemberExpression(
		VbCodeExpressionPtr source,
		VbCodeDotType dot,
		const std::string& member)
		: source(source), dot(dot), member(member)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<member-expression>";
		source->WriteXml(out);
		out << "<member>" << ToString(dot) << member << "</member>";
		out << "</member-expression>";
	}

	VbCodeExpressionPtr source;
	VbCodeDotType dot;
	std::string member;
};
