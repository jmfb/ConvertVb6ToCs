#pragma once
#include "VbCodeExpression.h"
#include "VbCodeUnaryExpressionType.h"

class VbCodeUnaryExpression : public VbCodeExpression
{
public:
	VbCodeUnaryExpression(
		VbCodeUnaryExpressionType type,
		VbCodeExpressionPtr expression)
		: type(type), expression(expression)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		auto name = GetXmlName();
		out << "<" << name << ">";
		expression->WriteXml(out);
		out << "</" << name << ">";
	}

	const char* GetXmlName() const
	{
		switch (type)
		{
		case VbCodeUnaryExpressionType::FileNumber: return "file-number-expression";
		case VbCodeUnaryExpressionType::Negate: return "negate-expression";
		case VbCodeUnaryExpressionType::Not: return "not-expression";
		}
		throw std::runtime_error("Invalid unary type.");
	}

public:
	VbCodeUnaryExpressionType type;
	VbCodeExpressionPtr expression;
};
