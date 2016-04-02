#include "VbCodeFunctionFactory.h"
#include "VbStatement.h"
#include "VbFunctionStatement.h"
#include "VbPropertyStatement.h"
#include "VbEndStatement.h"
#include "VbDimStatement.h"
#include "VbConstStatement.h"
#include "VbWithStatement.h"
#include "VbLetStatement.h"
#include "VbSetStatement.h"
#include "VbIfStatement.h"
#include "VbElseIfStatement.h"
#include "VbElseStatement.h"
#include "VbEndKeyword.h"
#include "VbDimDefinition.h"
#include "VbConstantDefinition.h"
#include "VbCodeEndType.h"
#include "VbCodeParameterFactory.h"
#include "VbCodeTypeFactory.h"
#include "VbCodeWithStatement.h"
#include "VbCodeLetStatement.h"
#include "VbCodeIfStatement.h"
#include "VbCodeEndProgramStatement.h"
#include "VbCodeExpressionStatement.h"
#include "VbCodeExpressionFactory.h"
#include "VbCodeSelectStatement.h"
#include "VbSelectStatement.h"
#include "VbCaseStatement.h"
#include "VbCaseClause.h"
#include "VbCaseExpression.h"
#include "VbRelationalOp.h"
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
	else if (statement.constStatement)
		ProcessConstStatement(*statement.constStatement);
	else if (statement.withStatement)
		ProcessWithStatement(*statement.withStatement);
	else if (statement.letStatement)
		ProcessLetStatement(*statement.letStatement);
	else if (statement.setStatement)
		ProcessSetStatement(*statement.setStatement);
	else if (statement.ifStatement)
		ProcessIfStatement(*statement.ifStatement);
	else if (statement.elseIfStatement)
		ProcessElseIfStatement(*statement.elseIfStatement);
	else if (statement.elseStatement)
		ProcessElseStatement(*statement.elseStatement);
	else if (statement.callStatement)
		ProcessCallStatement(*statement.callStatement);
	else if (statement.selectStatement)
		ProcessSelectStatement(*statement.selectStatement);
	else if (statement.caseStatement)
		ProcessCaseStatement(*statement.caseStatement);
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
		ToFunctionType(functionStatement.type),
		ToFunctionAccess(functionStatement.access),
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
		ToFunctionType(propertyStatement.type),
		ToFunctionAccess(propertyStatement.access),
		propertyStatement.isStatic,
		propertyStatement.name.GetValue(),
		VbCodeParameterFactory::Create(propertyStatement.parameterClause),
		VbCodeTypeFactory::CreateOptional(propertyStatement.asArraySpecifier)
	};
	blocks.push(&function->statements);
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
	auto type = ToEndType(endKeyword.keyword);
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

void VbCodeFunctionFactory::ProcessConstStatement(const Sentence& sentence)
{
	VbConstStatement constStatement{ sentence };
	for (auto& constantDefinitionSentence : constStatement.constantDefinitions)
	{
		VbConstantDefinition constantDefinition{ constantDefinitionSentence };
		//TODO: add the constant to the function
		std::cout << "TODO: Const statement" << std::endl;
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

void VbCodeFunctionFactory::ProcessSetStatement(const Sentence& sentence)
{
	VbSetStatement setStatement{ sentence };
	blocks.top()->push_back(std::make_shared<VbCodeLetStatement>(	//TODO: Do I need to distinguish let/set?
		VbCodeExpressionFactory::CreateLValue(setStatement.lValue),
		VbCodeExpressionFactory::CreateExpression(setStatement.expression)));
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

void VbCodeFunctionFactory::ProcessSelectStatement(const Sentence& sentence)
{
	VbSelectStatement selectStatement{ sentence };
	auto statement = std::make_shared<VbCodeSelectStatement>(
		VbCodeExpressionFactory::CreateExpression(selectStatement.expression));
	compoundStatements.push(statement);
	blocks.push(nullptr);
}

void VbCodeFunctionFactory::ProcessCaseStatement(const Sentence& sentence)
{
	VbCaseStatement caseStatement{ sentence };
	VbCaseClause caseClause{ caseStatement.caseClause };
	if (compoundStatements.empty())
		throw std::runtime_error("Case could not find matching Select.");
	blocks.pop();
	if (!caseClause.caseExpressions)
		blocks.push(compoundStatements.top()->Else());
	else
	{
		std::vector<VbCodeCaseExpression> expressions;
		for (auto& caseExpressionSentence : *caseClause.caseExpressions)
		{
			VbCaseExpression caseExpression{ caseExpressionSentence };
			auto expression1 = VbCodeExpressionFactory::CreateExpression(caseExpression.expression1);
			if (caseExpression.expression2)
			{
				expressions.emplace_back(
					expression1,
					VbCodeExpressionFactory::CreateExpression(*caseExpression.expression2));
			}
			else if (!caseExpression.relationalOp)
			{
				expressions.emplace_back(expression1);
			}
			else
			{
				VbRelationalOp relationalOp{ *caseExpression.relationalOp };
				expressions.emplace_back(ToCaseType(relationalOp.op), expression1);
			}
		}
		blocks.push(compoundStatements.top()->Case(expressions));
	}
}
