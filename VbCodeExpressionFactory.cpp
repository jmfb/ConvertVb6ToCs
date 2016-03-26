#include "VbCodeExpressionFactory.h"
#include "VbCodeUnaryExpression.h"
#include "VbCodeBinaryExpression.h"
#include "VbCodeConstantExpression.h"
#include "VbExpression.h"
#include "VbXorExpression.h"
#include "VbOrExpression.h"
#include "VbAndExpression.h"
#include "VbMultiplicativeExpression.h"
#include "VbAdditiveExpression.h"
#include "VbEqualityExpression.h"
#include "VbRelationalExpression.h"
#include "VbUnaryExpression.h"
#include "VbPostfixExpression.h"
#include "VbPrimaryExpression.h"
#include "VbCodeValueFactory.h"

VbCodeExpressionPtr VbCodeExpressionFactory::CreateExpression(const Sentence& sentence)
{
	VbExpression expression{ sentence };
	return CreateOrExpression(expression.orExpression);
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateOrExpression(const Sentence& sentence)
{
	VbOrExpression expression{ sentence };
	if (!expression.orExpression)
		return CreateXorExpression(expression.xorExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		VbCodeBinaryExpressionType::Or,
		CreateOrExpression(*expression.orExpression),
		CreateXorExpression(expression.xorExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateXorExpression(const Sentence& sentence)
{
	VbXorExpression expression{ sentence };
	if (!expression.xorExpression)
		return CreateAndExpression(expression.andExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		VbCodeBinaryExpressionType::Xor,
		CreateXorExpression(*expression.xorExpression),
		CreateAndExpression(expression.andExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateAndExpression(const Sentence& sentence)
{
	VbAndExpression expression{ sentence };
	if (!expression.andExpression)
		return CreateEqualityExpression(expression.equalityExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		VbCodeBinaryExpressionType::And,
		CreateAndExpression(*expression.andExpression),
		CreateEqualityExpression(expression.equalityExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateEqualityExpression(const Sentence& sentence)
{
	VbEqualityExpression expression{ sentence };
	if (!expression.equalityExpression)
		return CreateRelationalExpression(expression.relationalExpression);
	auto type = SentenceParser::ToEnum<VbCodeBinaryExpressionType>(
		*expression.op,
		{
			{ "=", VbCodeBinaryExpressionType::EqualTo },
			{ "<>", VbCodeBinaryExpressionType::NotEqualTo },
			{ "imp", VbCodeBinaryExpressionType::Implies },
			{ "eqv", VbCodeBinaryExpressionType::Equivalent },
			{ "is", VbCodeBinaryExpressionType::Is },
			{ "like", VbCodeBinaryExpressionType::Like }
		});
	return std::make_shared<VbCodeBinaryExpression>(
		type,
		CreateEqualityExpression(*expression.equalityExpression),
		CreateRelationalExpression(expression.relationalExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateRelationalExpression(const Sentence& sentence)
{
	VbRelationalExpression expression{ sentence };
	if (!expression.relationalExpression)
		return CreateAdditiveExpression(expression.additiveExpression);
	auto type = SentenceParser::ToEnum<VbCodeBinaryExpressionType>(
		*expression.op,
		{
			{ ">=", VbCodeBinaryExpressionType::GreaterThanOrEqual },
			{ ">", VbCodeBinaryExpressionType::GreaterThan },
			{ "<=", VbCodeBinaryExpressionType::LessThanOrEqual },
			{ "<", VbCodeBinaryExpressionType::LessThan }
		});
	return std::make_shared<VbCodeBinaryExpression>(
		type,
		CreateRelationalExpression(*expression.relationalExpression),
		CreateAdditiveExpression(expression.additiveExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateAdditiveExpression(const Sentence& sentence)
{
	VbAdditiveExpression expression{ sentence };
	if (!expression.additiveExpression)
		return CreateMultiplicativeExpression(expression.multiplicativeExpression);
	auto type = SentenceParser::ToEnum<VbCodeBinaryExpressionType>(
		*expression.op,
		{
			{ "+", VbCodeBinaryExpressionType::Addition },
			{ "-", VbCodeBinaryExpressionType::Subtraction },
			{ "&", VbCodeBinaryExpressionType::StringConcatenation }
		});
	return std::make_shared<VbCodeBinaryExpression>(
		type,
		CreateAdditiveExpression(*expression.additiveExpression),
		CreateMultiplicativeExpression(expression.multiplicativeExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateMultiplicativeExpression(const Sentence& sentence)
{
	VbMultiplicativeExpression expression{ sentence };
	if (!expression.multiplicativeExpression)
		return CreateUnaryExpression(expression.unaryExpression);
	auto type = SentenceParser::ToEnum<VbCodeBinaryExpressionType>(
		*expression.op,
		{
			{ "*", VbCodeBinaryExpressionType::Multiplication },
			{ "/", VbCodeBinaryExpressionType::Division },
			{ "\\", VbCodeBinaryExpressionType::IntegerDivision },
			{ "mod", VbCodeBinaryExpressionType::Modulus },
			{ "^", VbCodeBinaryExpressionType::Exponent }
		});
	return std::make_shared<VbCodeBinaryExpression>(
		type,
		CreateMultiplicativeExpression(*expression.multiplicativeExpression),
		CreateUnaryExpression(expression.unaryExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateUnaryExpression(const Sentence& sentence)
{
	VbUnaryExpression expression{ sentence };
	if (expression.postfixExpression)
		return CreatePostfixExpression(*expression.postfixExpression);
	auto& op = *expression.op;
	if (op == "new")
		throw std::runtime_error("Not implemented: new");
	if (op == "addressof")
		throw std::runtime_error("Not implemented: addressof");
	if (op == "typeof")
		throw std::runtime_error("Not implemented: typeof");
	if (op == "string")
		throw std::runtime_error("Not implemented: string");
	if (op == "len")
		throw std::runtime_error("Not implemented: len");
	auto type = SentenceParser::ToEnum<VbCodeUnaryExpressionType>(
		op,
		{
			{ "not", VbCodeUnaryExpressionType::Not },
			{ "-", VbCodeUnaryExpressionType::Negate },
			{ "#", VbCodeUnaryExpressionType::FileNumber }
		});
	return std::make_shared<VbCodeUnaryExpression>(
		type,
		CreateUnaryExpression(*expression.unaryExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreatePostfixExpression(const Sentence& sentence)
{
	VbPostfixExpression expression{ sentence };
	if (expression.primaryExpression)
		return CreatePrimaryExpression(*expression.primaryExpression);
	throw std::runtime_error("Not implemented: expression.id or expression(...)");
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreatePrimaryExpression(const Sentence& sentence)
{
	VbPrimaryExpression expression{ sentence };
	if (expression.literal)
		return CreateConstantExpression(*expression.literal);
	throw std::runtime_error("Not implemented: all other primary expressions");
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateConstantExpression(const Sentence& sentence)
{
	if (sentence.GetNodes().size() != 1)
		throw std::runtime_error("Literal should contain exactly 1 token.");
	auto& value = VbCodeValueFactory::Create(sentence.GetNodes()[0]->AsToken());
	return std::make_shared<VbCodeConstantExpression>(value);
}
