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

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		lhs->WriteCs(writer);
		switch (type)
		{
		case VbCodeBinaryExpressionType::Multiplication:
			writer.out << " * ";
			break;
		case VbCodeBinaryExpressionType::Division:
			writer.out << " / ";
			break;
		case VbCodeBinaryExpressionType::IntegerDivision:
			writer.out << " / "; //TODO: does this need to do something else?
			break;
		case VbCodeBinaryExpressionType::Modulus:
			writer.out << " % ";
			break;
		case VbCodeBinaryExpressionType::Exponent:
			throw std::runtime_error("Exponent operator not yet supported.");
		case VbCodeBinaryExpressionType::Addition:
			writer.out << " + ";
			break;
		case VbCodeBinaryExpressionType::Subtraction:
			writer.out << " - ";
			break;
		case VbCodeBinaryExpressionType::StringConcatenation:
			writer.out << " + "; //TODO: do I need to do automatic conversions to string?
			break;
		case VbCodeBinaryExpressionType::GreaterThan:
			writer.out << " > ";
			break;
		case VbCodeBinaryExpressionType::GreaterThanOrEqual:
			writer.out << " >= ";
			break;
		case VbCodeBinaryExpressionType::LessThan:
			writer.out << " < ";
			break;
		case VbCodeBinaryExpressionType::LessThanOrEqual:
			writer.out << " <= ";
			break;
		case VbCodeBinaryExpressionType::EqualTo:
			writer.out << " == ";
			break;
		case VbCodeBinaryExpressionType::NotEqualTo:
			writer.out << " != ";
			break;
		case VbCodeBinaryExpressionType::Implies:
			throw std::runtime_error("Implies operator not yet supported.");
		case VbCodeBinaryExpressionType::Equivalent:
			throw std::runtime_error("Equivalent operator not yet supported.");
		case VbCodeBinaryExpressionType::Is:
			writer.out << " is ";
			break;
		case VbCodeBinaryExpressionType::Like:
			throw std::runtime_error("Like operator not yet implemented.");
		case VbCodeBinaryExpressionType::And:
			writer.out << " && "; //TODO: determine if bitwise and is more appropriate
			break;
		case VbCodeBinaryExpressionType::Xor:
			writer.out << " ^ ";
			break;
		case VbCodeBinaryExpressionType::Or:
			writer.out << " || "; //TODO: determine if bitwise or is more appropriate
			break;
		}
		rhs->WriteCs(writer);
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
