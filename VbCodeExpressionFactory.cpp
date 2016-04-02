#include "VbCodeExpressionFactory.h"
#include "VbCodeUnaryExpression.h"
#include "VbCodeBinaryExpression.h"
#include "VbCodeConstantExpression.h"
#include "VbCodeWithExpression.h"
#include "VbCodeMemberExpression.h"
#include "VbCodeIdExpression.h"
#include "VbCodeCallExpression.h"
#include "VbCodeMissingExpression.h"
#include "VbCodeLenExpression.h"
#include "VbCodeMeExpression.h"
#include "VbCodeParenthesisExpression.h"
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
#include "VbDefaultValue.h"
#include "VbExpressionClause.h"
#include "VbExpressionList.h"
#include "VbExpressionItem.h"
#include "VbLValue.h"
#include "VbLValueTerminal.h"
#include "VbCallStatement.h"
#include "VbCallSuffix.h"
#include "VbCallTerminal.h"

VbCodeExpressionPtr VbCodeExpressionFactory::CreateDefaultValue(const optional<Sentence>& sentence)
{
	if (!sentence)
		return nullptr;
	VbDefaultValue defaultValue{ *sentence };
	return CreateExpression(defaultValue.expression);
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateExpression(const Sentence& sentence)
{
	VbExpression expression{ sentence };
	return CreateOrExpression(expression.orExpression);
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateLValue(const Sentence& sentence)
{
	VbLValue lValue{ sentence };
	VbCodeExpressionPtr expression;
	if (lValue.wsDot)
		expression = std::make_shared<VbCodeWithExpression>(ToDotType(*lValue.wsDot), lValue.id.GetValue());
	else if (lValue.id == "me")
		expression = std::make_shared<VbCodeMeExpression>();
	else
		expression = std::make_shared<VbCodeIdExpression>(lValue.id.GetValue());
	if (lValue.suffix)
		for (auto& terminalSentence : *lValue.suffix)
		{
			VbLValueTerminal terminal{ terminalSentence };
			if (terminal.dot)
				expression = std::make_shared<VbCodeMemberExpression>(
					expression,
					ToDotType(*terminal.dot),
					terminal.id->GetValue());
			else
				expression = std::make_shared<VbCodeCallExpression>(
					expression,
					CreateExpressionClause(*terminal.expressionClause));
		}
	return expression;
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateCallStatement(const Sentence& sentence)
{
	VbCallStatement callStatement{ sentence };
	if (callStatement.lValue)
	{
		auto lValue = CreateLValue(*callStatement.lValue);
		std::vector<VbCodeExpressionPtr> arguments;
		if (callStatement.expressionList)
			arguments = CreateExpressionList(*callStatement.expressionList);
		return std::make_shared<VbCodeCallExpression>(lValue, arguments);
	}
	VbCallSuffix callSuffix{ *callStatement.callSuffix };
	VbCallTerminal firstTerminal{ callSuffix.callTerminal };
	VbCodeExpressionPtr expression;
	if (callStatement.isMe)
		expression = std::make_shared<VbCodeMemberExpression>(
			std::make_shared<VbCodeMeExpression>(),
			VbCodeDotType::Dot,
			firstTerminal.name.GetValue());
	else if (callStatement.wsDot)
		expression = std::make_shared<VbCodeWithExpression>(
			ToDotType(*callStatement.wsDot),
			firstTerminal.name.GetValue());
	else
		expression = std::make_shared<VbCodeIdExpression>(firstTerminal.name.GetValue());
	if (firstTerminal.expressionClause)
		expression = std::make_shared<VbCodeCallExpression>(expression, CreateExpressionClause(*firstTerminal.expressionClause));
	for (auto& suffix : callSuffix.suffix)
	{
		VbCallTerminal nextTerminal{ suffix.second };
		expression = std::make_shared<VbCodeMemberExpression>(
			expression,
			ToDotType(suffix.first),
			nextTerminal.name.GetValue());
		if (nextTerminal.expressionClause)
			expression = std::make_shared<VbCodeCallExpression>(expression, CreateExpressionClause(*nextTerminal.expressionClause));
	}
	return expression;
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
	return std::make_shared<VbCodeBinaryExpression>(
		ToEqualityOperator(*expression.op),
		CreateEqualityExpression(*expression.equalityExpression),
		CreateRelationalExpression(expression.relationalExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateRelationalExpression(const Sentence& sentence)
{
	VbRelationalExpression expression{ sentence };
	if (!expression.relationalExpression)
		return CreateAdditiveExpression(expression.additiveExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		ToRelationalOperator(*expression.op),
		CreateRelationalExpression(*expression.relationalExpression),
		CreateAdditiveExpression(expression.additiveExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateAdditiveExpression(const Sentence& sentence)
{
	VbAdditiveExpression expression{ sentence };
	if (!expression.additiveExpression)
		return CreateMultiplicativeExpression(expression.multiplicativeExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		ToAdditiveOperator(*expression.op),
		CreateAdditiveExpression(*expression.additiveExpression),
		CreateMultiplicativeExpression(expression.multiplicativeExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateMultiplicativeExpression(const Sentence& sentence)
{
	VbMultiplicativeExpression expression{ sentence };
	if (!expression.multiplicativeExpression)
		return CreateUnaryExpression(expression.unaryExpression);
	return std::make_shared<VbCodeBinaryExpression>(
		ToMultiplicativeOperator(*expression.op),
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
	{
		auto expressions = CreateExpressionClause(*expression.expressionClause);
		if (expressions.size() != 1)
			throw std::runtime_error("Only one argument to Len is supported.");
		return std::make_shared<VbCodeLenExpression>(expressions[0]);
	}
	return std::make_shared<VbCodeUnaryExpression>(
		ToUnaryOperator(op),
		CreateUnaryExpression(*expression.unaryExpression));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreatePostfixExpression(const Sentence& sentence)
{
	VbPostfixExpression expression{ sentence };
	if (expression.primaryExpression)
		return CreatePrimaryExpression(*expression.primaryExpression);
	if (expression.dot)
		return std::make_shared<VbCodeMemberExpression>(
			CreatePostfixExpression(*expression.postfixExpression),
			ToDotType(*expression.dot),
			expression.id->GetValue());
	return std::make_shared<VbCodeCallExpression>(
		CreatePostfixExpression(*expression.postfixExpression),
		CreateExpressionClause(*expression.expressionClause));
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreatePrimaryExpression(const Sentence& sentence)
{
	VbPrimaryExpression expression{ sentence };
	if (expression.literal)
		return CreateConstantExpression(*expression.literal);
	if (expression.wsDot)
		return std::make_shared<VbCodeWithExpression>(ToDotType(*expression.wsDot), expression.id->GetValue());
	if (expression.expression2)
		throw std::runtime_error("Not implemented: (expression, expression)");
	if (expression.expression1)
		return std::make_shared<VbCodeParenthesisExpression>(CreateExpression(*expression.expression1));
	if (*expression.id == "me")
		return std::make_shared<VbCodeMeExpression>();
	return std::make_shared<VbCodeIdExpression>(expression.id->GetValue());
}

VbCodeExpressionPtr VbCodeExpressionFactory::CreateConstantExpression(const Sentence& sentence)
{
	if (sentence.GetNodes().size() != 1)
		throw std::runtime_error("Literal should contain exactly 1 token.");
	auto& value = VbCodeValueFactory::Create(sentence.GetNodes()[0]->AsToken());
	return std::make_shared<VbCodeConstantExpression>(value);
}

std::vector<VbCodeExpressionPtr> VbCodeExpressionFactory::CreateExpressionClause(const Sentence& sentence)
{
	VbExpressionClause expressionClause{ sentence };
	if (expressionClause.expressionList)
		return CreateExpressionList(*expressionClause.expressionList);
	return{};
}

std::vector<VbCodeExpressionPtr> VbCodeExpressionFactory::CreateExpressionList(const Sentence& sentence)
{
	std::vector<VbCodeExpressionPtr> expressions;
	VbExpressionList expressionList{ sentence };
	if (expressionList.separator && *expressionList.separator != ",")
		throw std::runtime_error("';' statement separator not yet implemented.");
	for (auto& expressionItemSentence : expressionList.expressionItems)
	{
		VbExpressionItem expressionItem{ expressionItemSentence };
		if (!expressionItem.expression)
			expressions.push_back(std::make_shared<VbCodeMissingExpression>());
		else
		{
			if (expressionItem.passBy)
				throw std::runtime_error("Pass-by on expression item not yet implemented.");
			if (expressionItem.name)
				throw std::runtime_error("Named expression items not yet implemented.");
			expressions.push_back(CreateExpression(*expressionItem.expression));
		}
	}
	return expressions;
}
