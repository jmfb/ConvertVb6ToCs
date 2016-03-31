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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		//TODO: determine if member is a normal vs. dynamic member of current with element
		writer.With();
		if (dot != VbCodeDotType::Dot)
			throw std::runtime_error("Bang operator not supported on with expressions yet.");
		writer.out << "." << member;
	}

	VbCodeDotType dot;
	std::string member;
};
