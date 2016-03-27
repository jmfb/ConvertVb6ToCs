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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		switch (type)
		{
		case VbCodeUnaryExpressionType::FileNumber:
			throw std::runtime_error("File number expressions not yet supported.");
		case VbCodeUnaryExpressionType::Negate:
			writer.out << "-";
			expression->WriteCs(writer);
			break;
		case VbCodeUnaryExpressionType::Not:
			writer.out << "!";
			expression->WriteCs(writer);
			break;
		}
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
