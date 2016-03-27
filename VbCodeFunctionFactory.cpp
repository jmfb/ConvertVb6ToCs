#include "VbCodeFunctionFactory.h"
#include "VbStatement.h"
#include "VbFunctionStatement.h"
#include "VbPropertyStatement.h"
#include "VbEndStatement.h"
#include "VbDimStatement.h"
#include "VbWithStatement.h"
#include "VbLetStatement.h"
#include "VbIfStatement.h"
#include "VbElseIfStatement.h"
#include "VbElseStatement.h"
#include "VbEndKeyword.h"
#include "VbDimDefinition.h"
#include "VbCodeEndType.h"
#include "VbCodeParameterFactory.h"
#include "VbCodeTypeFactory.h"
#include "VbCodeWithStatement.h"
#include "VbCodeLetStatement.h"
#include "VbCodeIfStatement.h"
#include "VbCodeEndProgramStatement.h"
#include "VbCodeExpressionStatement.h"
#include "VbCodeExpressionFactory.h"
#include <iostream>

void VbCodeFunctionFactory::AddStatement(const Sentence& sentence)
{
	if (IsInFunction())
		ProcessStatement(sentence);
	else
		ProcessNextFunction(sentence);
}

bool VbCodeFunctionFactory::IsInFunction() const
{
	return function;
}

bool VbCodeFunctionFactory::IsEndOfFunction() const
{
	return endOfFunction;
}

VbCodeFunction VbCodeFunctionFactory::Create() const
{
	return *function;
}

void VbCodeFunctionFactory::ProcessStatement(const Sentence& sentence)
{
	VbStatement statement{ sentence };
	if (statement.endStatement)
		ProcessEndStatement(*statement.endStatement);
	else if (statement.dimStatement)
		ProcessDimStatement(false, *statement.dimStatement);
	else if (statement.staticStatement)
		ProcessDimStatement(true, *statement.staticStatement);
	else if (statement.withStatement)
		ProcessWithStatement(*statement.withStatement);
	else if (statement.letStatement)
		ProcessLetStatement(*statement.letStatement);
	else if (statement.ifStatement)
		ProcessIfStatement(*statement.ifStatement);
	else if (statement.elseIfStatement)
		ProcessElseIfStatement(*statement.elseIfStatement);
	else if (statement.elseStatement)
		ProcessElseStatement(*statement.elseStatement);
	else if (statement.callStatement)
		ProcessCallStatement(*statement.callStatement);
	else
		std::cout << "TODO: misc. function statement" << std::endl;
}

void VbCodeFunctionFactory::ProcessNextFunction(const Sentence& sentence)
{
	VbStatement statement{ sentence };
	if (statement.functionStatement)
		BeginFunction(*statement.functionStatement);
	else if (statement.subStatement)
		BeginFunction(*statement.subStatement);
	else if (statement.propertyGetStatement)
		BeginProperty(*statement.propertyGetStatement);
	else if (statement.propertyLetStatement)
		BeginProperty(*statement.propertyLetStatement);
	else if (statement.propertySetStatement)
		BeginProperty(*statement.propertySetStatement);
	else
		throw std::runtime_error("Expected next statement to begin a function.");
}

void VbCodeFunctionFactory::BeginFunction(const Sentence& sentence)
{
	VbFunctionStatement functionStatement{ sentence };
	function = VbCodeFunction
	{
		ParseType(functionStatement.type),
		ParseAccess(functionStatement.access),
		functionStatement.isStatic,
		functionStatement.name.GetValue(),
		VbCodeParameterFactory::Create(functionStatement.parameterClause),
		VbCodeTypeFactory::CreateOptional(functionStatement.asArraySpecifier)
	};
	blocks.push(&function->statements);
}

void VbCodeFunctionFactory::BeginProperty(const Sentence& sentence)
{
	VbPropertyStatement propertyStatement{ sentence };
	function = VbCodeFunction
	{
		ParseType(propertyStatement.type),
		ParseAccess(propertyStatement.access),
		propertyStatement.isStatic,
		propertyStatement.name.GetValue(),
		VbCodeParameterFactory::Create(propertyStatement.parameterClause),
		VbCodeTypeFactory::CreateOptional(propertyStatement.asArraySpecifier)
	};
	blocks.push(&function->statements);
}

VbCodeFunctionType VbCodeFunctionFactory::ParseType(const Token& token)
{
	return SentenceParser::ToEnum<VbCodeFunctionType>(
		token,
		{
			{ "function", VbCodeFunctionType::Function },
			{ "sub", VbCodeFunctionType::Sub },
			{ "get", VbCodeFunctionType::PropertyGet },
			{ "let", VbCodeFunctionType::PropertyLet },
			{ "set", VbCodeFunctionType::PropertySet }
		});
}

VbCodeFunctionAccess VbCodeFunctionFactory::ParseAccess(const optional<Token>& token)
{
	return SentenceParser::ToEnum(
		token,
		{
			{ "public", VbCodeFunctionAccess::Public },
			{ "private", VbCodeFunctionAccess::Private },
			{ "friend", VbCodeFunctionAccess::Friend }
		},
		VbCodeFunctionAccess::Private);
}

void VbCodeFunctionFactory::ProcessEndStatement(const Sentence& sentence)
{
	VbEndStatement endStatement{ sentence };
	if (!endStatement.keyword)
	{
		blocks.top()->push_back(std::make_shared<VbCodeEndProgramStatement>());
		return;
	}
	VbEndKeyword endKeyword{ *endStatement.keyword };
	auto type = SentenceParser::ToEnum<VbCodeEndType>(
		endKeyword.keyword,
		{
			{ "if", VbCodeEndType::If },
			{ "select", VbCodeEndType::Select },
			{ "with", VbCodeEndType::With },
			{ "sub", VbCodeEndType::Sub },
			{ "function", VbCodeEndType::Function },
			{ "property", VbCodeEndType::Property }
		});
	switch (type)
	{
	case VbCodeEndType::If:
	case VbCodeEndType::Select:
	case VbCodeEndType::With:
		if (compoundStatements.empty())
			throw std::runtime_error("End If/Select/With found without any preceding block.");
		compoundStatements.top()->MatchEnd(type);
		blocks.pop();
		blocks.top()->push_back(compoundStatements.top());
		compoundStatements.pop();
		break;

	case VbCodeEndType::Sub:
		if (function->type != VbCodeFunctionType::Sub)
			throw std::runtime_error("Mismatched End Sub.");
		endOfFunction = true;
		break;

	case VbCodeEndType::Function:
		if (function->type != VbCodeFunctionType::Function)
			throw std::runtime_error("Mismatched End Function.");
		endOfFunction = true;
		break;

	case VbCodeEndType::Property:
		if (function->type == VbCodeFunctionType::Function || function->type == VbCodeFunctionType::Sub)
			throw std::runtime_error("Mismatched End Property.");
		endOfFunction = true;
		break;
	}

	if (endOfFunction && blocks.size() != 1)
		throw std::runtime_error("Missing end block before end of function.");
}

void VbCodeFunctionFactory::ProcessDimStatement(bool isStatic, const Sentence& sentence)
{
	VbDimStatement dimStatement{ sentence };
	for (auto& dimDefinitionSentence : dimStatement.dimDefinitions)
	{
		VbDimDefinition dimDefinition{ dimDefinitionSentence };
		if (dimDefinition.arraySuffix)
			throw std::runtime_error("Dim array suffix not yet implemented.");
		if (dimDefinition.isWithEvents)
			throw std::runtime_error("Dim definition may not be with events.");
		VbCodeVariable variable{ dimDefinition.name.GetValue(), VbCodeTypeFactory::Create(dimDefinition.asNewSpecifier) };
		if (isStatic)
			function->statics.push_back(variable);
		else
			function->variables.push_back(variable);
	}
}

void VbCodeFunctionFactory::ProcessWithStatement(const Sentence& sentence)
{
	VbWithStatement withStatement{ sentence };
	auto statement = std::make_shared<VbCodeWithStatement>(VbCodeExpressionFactory::CreateLValue(withStatement.lValue));
	compoundStatements.push(statement);
	blocks.push(&statement->statements);
}

void VbCodeFunctionFactory::ProcessLetStatement(const Sentence& sentence)
{
	VbLetStatement letStatement{ sentence };
	blocks.top()->push_back(std::make_shared<VbCodeLetStatement>(
		VbCodeExpressionFactory::CreateLValue(letStatement.lValue),
		VbCodeExpressionFactory::CreateExpression(letStatement.expression)));
}

void VbCodeFunctionFactory::ProcessIfStatement(const Sentence& sentence)
{
	VbIfStatement ifStatement{ sentence };
	if (ifStatement.compoundStatement)
		throw std::runtime_error("If followed by compound-statement not yet implemented.");
	if (ifStatement.elseStatement)
		throw std::runtime_error("If followed by else not yet implemented.");
	auto expression = VbCodeExpressionFactory::CreateExpression(ifStatement.expression);
	auto statement = std::make_shared<VbCodeIfStatement>(expression);
	compoundStatements.push(statement);
	blocks.push(&statement->ifBlocks.back().statements);
}

void VbCodeFunctionFactory::ProcessElseIfStatement(const Sentence& sentence)
{
	VbElseIfStatement elseIfStatement{ sentence };
	if (compoundStatements.empty())
		throw std::runtime_error("ElseIf could not find matching If");
	auto expression = VbCodeExpressionFactory::CreateExpression(elseIfStatement.expression);
	blocks.pop();
	blocks.push(compoundStatements.top()->ElseIf(expression));
}

void VbCodeFunctionFactory::ProcessElseStatement(const Sentence& sentence)
{
	VbElseStatement elseStatement{ sentence };
	if (compoundStatements.empty())
		throw std::runtime_error("Else could not find matching If");
	if (elseStatement.compoundStatement)
		throw std::runtime_error("Else followed by compound-statement not yet implemented.");
	blocks.pop();
	blocks.push(compoundStatements.top()->Else());
}

void VbCodeFunctionFactory::ProcessCallStatement(const Sentence& sentence)
{
	auto expression = VbCodeExpressionFactory::CreateCallStatement(sentence);
	blocks.top()->push_back(std::make_shared<VbCodeExpressionStatement>(expression));
}
