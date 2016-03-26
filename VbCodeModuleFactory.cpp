#include "VbCodeModuleFactory.h"
#include "VbTranslationUnit.h"
#include "VbTranslationHeader.h"
#include "VbModuleHeader.h"
#include "VbAttribute.h"
#include "VbQualifiedId.h"
#include "VbCodeExpressionFactory.h"
#include "VbDeclaration.h"
#include "VbLine.h"
#include "VbCompoundStatement.h"
#include "VbStatement.h"
#include "VbConstStatement.h"
#include "VbConstantDefinition.h"
#include "VbCodeTypeFactory.h"
#include "VbCodeConstant.h"
#include "VbDimStatement.h"
#include "VbDimDefinition.h"
#include "VbCodeDeclareFactory.h"
#include <iostream>

VbCodeModule VbCodeModuleFactory::Create(const Sentence& sentence)
{
	VbTranslationUnit translationUnit{ sentence };
	if (!translationUnit.translationHeader)
		throw std::runtime_error("Missing translation header.");
	LoadHeader(*translationUnit.translationHeader);
	try
	{
		if (translationUnit.declarationList)
			for (auto& declarationSentence : *translationUnit.declarationList)
				ProcessDeclaration(declarationSentence);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return
	{
		name,
		isOptionExplicit,
		constants,
		members,
		declares
	};
}

void VbCodeModuleFactory::LoadHeader(const Sentence& sentence)
{
	VbTranslationHeader translationHeader{ sentence };
	if (!translationHeader.moduleHeader)
		throw std::runtime_error("Missing module header.");
	VbModuleHeader moduleHeader{ *translationHeader.moduleHeader };
	if (moduleHeader.attributes.size() != 1)
		throw std::runtime_error("Should be exactly one attribute.");
	VbAttribute attribute{ moduleHeader.attributes[0] };
	auto attributeName = VbQualifiedId{ attribute.name }.ToSimpleName();
	if (attributeName != "VB_Name")
		throw std::runtime_error("Only supported module attribute is VB_Name");
	auto value = VbCodeExpressionFactory::CreateExpression(attribute.value)->EvaluateConstant();
	if (value.type != VbCodeValueType::String)
		throw std::runtime_error("VB_Name should be a string.");
	name = value.stringValue;
}

void VbCodeModuleFactory::ProcessDeclaration(const Sentence& sentence)
{
	VbDeclaration declaration{ sentence };
	if (declaration.attribute)
		throw std::runtime_error("Declaration not supported in module definition outside of header.");
	if (declaration.lineLabel)
		throw std::runtime_error("Line label not yet implemented.");
	if (!declaration.vbLine)
		return;
	VbLine line{ *declaration.vbLine };
	if (line.statement)
		ProcessStatement(*line.statement);
	if (!line.compoundStatement)
		return;
	VbCompoundStatement compoundStatement{ *line.compoundStatement };
	for (auto& statementSentence : compoundStatement.statements)
		ProcessStatement(statementSentence);
}

void VbCodeModuleFactory::ProcessStatement(const Sentence& sentence)
{
	if (isBeforeFirstFunction)
		ProcessHeaderStatement(sentence);
	else
		ProcessBodyStatement(sentence);
}

void VbCodeModuleFactory::ProcessHeaderStatement(const Sentence& sentence)
{
	VbStatement statement{ sentence };
	if (statement.optionExplicitStatement)
		isOptionExplicit = true;
	else if (statement.constStatement)
		ProcessConstStatement(*statement.constStatement);
	else if (statement.publicStatement)
		ProcessMemberStatement(true, *statement.publicStatement);
	else if (statement.privateStatement)
		ProcessMemberStatement(false, *statement.privateStatement);
	else if (statement.declareStatement)
		declares.push_back(VbCodeDeclareFactory::Create(*statement.declareStatement));
	else if (statement.typeStatement)
		std::cout << "TODO: type-statement" << std::endl;
	else if (statement.functionStatement)
		throw std::runtime_error("TODO: Function statement!");
	else
		throw std::runtime_error("Unsupported module header level statement.");
}

void VbCodeModuleFactory::ProcessBodyStatement(const Sentence& sentence)
{
	VbStatement statement{ sentence };
	throw std::runtime_error("Unsupported module body level statement.");
}

void VbCodeModuleFactory::ProcessConstStatement(const Sentence& sentence)
{
	VbConstStatement constStatement{ sentence };
	auto isPublic = constStatement.access &&
		*constStatement.access == "public" ||
		*constStatement.access == "global";
	for (auto& constantDefinitionSentence : constStatement.constantDefinitions)
	{
		VbConstantDefinition constantDefinition{ constantDefinitionSentence };
		auto& name = constantDefinition.name.GetValue();
		auto value = VbCodeExpressionFactory::CreateExpression(constantDefinition.expression)->EvaluateConstant();
		auto type = constantDefinition.asSpecifier ?
			VbCodeTypeFactory::Create(*constantDefinition.asSpecifier) :
			VbCodeType{ value.type };
		if (type.type != value.type)
			throw std::runtime_error("Coercing constant type value is not yet implemented.");
		constants.emplace_back(isPublic, name, value);
	}
}

void VbCodeModuleFactory::ProcessMemberStatement(bool isPublic, const Sentence& sentence)
{
	VbDimStatement dimStatement{ sentence };
	for (auto& dimDefinitionStatement : dimStatement.dimDefinitions)
	{
		VbDimDefinition dimDefinition{ dimDefinitionStatement };
		auto& name = dimDefinition.name.GetValue();
		if (dimDefinition.arraySuffix)
			throw std::runtime_error("Array suffix not yet supported.");
		members.emplace_back(
			isPublic,
			dimDefinition.isWithEvents,
			name,
			VbCodeTypeFactory::Create(dimDefinition.asNewSpecifier));
	}
}
