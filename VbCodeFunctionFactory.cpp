#include "VbCodeFunctionFactory.h"
#include "VbStatement.h"
#include "VbFunctionStatement.h"
#include "VbPropertyStatement.h"
#include "VbEndStatement.h"
#include "VbEndKeyword.h"
#include "VbCodeEndType.h"
#include "VbCodeParameterFactory.h"
#include "VbCodeTypeFactory.h"
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
		throw std::runtime_error("TODO: Support 'End <program>' statement.");
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
		std::cout << "TODO: Support End If/Select/With statement." << std::endl;
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
}
