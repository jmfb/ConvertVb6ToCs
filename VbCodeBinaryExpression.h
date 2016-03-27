#pragma once
#include "VbCodeExpression.h"
#include "VbCodeBinaryExpressionType.h"

class VbCodeBinaryExpression : public VbCodeExpression
{
public:
	VbCodeBinaryExpression(
		VbCodeBinaryExpressionType type,
		VbCodeExpressionPtr lhs,
		VbCodeExpressionPtr rhs)
		: type(type), lhs(lhs), rhs(rhs)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		auto name = GetXmlName();
		out << "<" << name << ">";
		lhs->WriteXml(out);
		rhs->WriteXml(out);
		out << "</" << name << ">";
	}

	const char* GetXmlName() const
	{
		switch (type)
		{
		case VbCodeBinaryExpressionType::Multiplication: return "multiplication-expression";
		case VbCodeBinaryExpressionType::Division: return "division-expression";
		case VbCodeBinaryExpressionType::IntegerDivision: return "integer-division-expression";
		case VbCodeBinaryExpressionType::Modulus: return "modulus-expression";
		case VbCodeBinaryExpressionType::Exponent: return "exponent-expression";
		case VbCodeBinaryExpressionType::Addition: return "addition-expression";
		case VbCodeBinaryExpressionType::Subtraction: return "subtraction-expression";
		case VbCodeBinaryExpressionType::StringConcatenation: return "string-concatenation-expression";
		case VbCodeBinaryExpressionType::GreaterThan: return "greater-than-expresion";
		case VbCodeBinaryExpressionType::GreaterThanOrEqual: return "greater-than-or-equal-expression";
		case VbCodeBinaryExpressionType::LessThan: return "less-than-expression";
		case VbCodeBinaryExpressionType::LessThanOrEqual: return "less-than-or-equal-expression";
		case VbCodeBinaryExpressionType::EqualTo: return "equal-to-expression";
		case VbCodeBinaryExpressionType::NotEqualTo: return "not-equal-to-expression";
		case VbCodeBinaryExpressionType::Implies: return "implies-expression";
		case VbCodeBinaryExpressionType::Equivalent: return "equivalent-expression";
		case VbCodeBinaryExpressionType::Is: return "is-expression";
		case VbCodeBinaryExpressionType::Like: return "like-expression";
		case VbCodeBinaryExpressionType::And: return "and-expression";
		case VbCodeBinaryExpressionType::Xor: return "xor-expression";
		case VbCodeBinaryExpressionType::Or: return "or-expression";
		}
		throw std::runtime_error("Invalid binary operator.");
	}

public:
	VbCodeBinaryExpressionType type;
	VbCodeExpressionPtr lhs;
	VbCodeExpressionPtr rhs;
};
