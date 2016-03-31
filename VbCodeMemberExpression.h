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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		source->WriteCs(writer);
		if (dot != VbCodeDotType::Dot)
			throw std::runtime_error("Bang operator not yet support in member exprssions.");
		//TODO: determine if member is a true member or if a dynamic invocation should occur
		writer.out << "." << member;
	}

	VbCodeExpressionPtr source;
	VbCodeDotType dot;
	std::string member;
};
