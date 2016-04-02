#pragma once
#include "VbCodeCaseType.h"
#include "VbCodeExpression.h"
#include "VbCodeWriter.h"

class VbCodeCaseExpression
{
public:
	VbCodeCaseExpression(VbCodeExpressionPtr expression)
		: type(VbCodeCaseType::EqualTo), expression1(expression), expression2(nullptr)
	{
	}
	VbCodeCaseExpression(
		VbCodeCaseType type,
		VbCodeExpressionPtr expression)
		: type(type), expression1(expression), expression2(nullptr)
	{
	}
	VbCodeCaseExpression(
		VbCodeExpressionPtr expression1,
		VbCodeExpressionPtr expression2)
		: type(VbCodeCaseType::Between), expression1(expression1), expression2(expression2)
	{
	}

	void WriteIfExpression(VbCodeWriter& writer, int select) const
	{
		if (type == VbCodeCaseType::Between)
		{
			writer.out << "__select" << select << " >= ";
			expression1->WriteCs(writer);
			writer.out << " && __select" << select << " <= ";
			expression2->WriteCs(writer);
		}
		else
		{
			writer.out << "__select" << select << " " << ToCs(type) << " ";
			expression1->WriteCs(writer);
		}
	}

	VbCodeCaseType type;
	VbCodeExpressionPtr expression1;
	VbCodeExpressionPtr expression2;
};
