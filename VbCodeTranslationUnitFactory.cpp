#include "VbCodeTranslationUnitFactory.h"
#include "VbTranslationUnit.h"
#include "VbTranslationHeader.h"
#include "VbModuleHeader.h"
#include "VbClassHeader.h"
#include "VbSettingBlock.h"
#include "VbSetting.h"
#include "VbSettingValue.h"
#include "VbLValue.h"
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
#include "VbCodeTypeDefinitionFactory.h"
#include "VbCodeEnumDefinitionFactory.h"

VbCodeTranslationUnit VbCodeTranslationUnitFactory::Create(const std::string& library, const Sentence& sentence)
{
	VbTranslationUnit translationUnit{ sentence };
	if (!translationUnit.translationHeader)
		throw std::runtime_error("Missing translation header.");
	LoadHeader(*translationUnit.translationHeader);
	if (translationUnit.declarationList)
		for (auto& declarationSentence : *translationUnit.declarationList)
			ProcessDeclaration(declarationSentence);
	if (functionFactory.IsInFunction())
		throw std::runtime_error("Missing end of function before end of file.");
	return
	{
		type,
		library,
		name,
		isOptionExplicit,
		constants,
		members,
		declares,
		typeDefinitions,
		enumDefinitions,
		functions
	};
}

void VbCodeTranslationUnitFactory::LoadHeader(const Sentence& sentence)
{
	VbTranslationHeader translationHeader{ sentence };
	if (translationHeader.moduleHeader)
		LoadModuleHeader(*translationHeader.moduleHeader);
	else if (translationHeader.classHeader)
		LoadClassHeader(*translationHeader.classHeader);
	else
		throw std::runtime_error("Only module and class headers are supported.");
}

void VbCodeTranslationUnitFactory::LoadModuleHeader(const Sentence& sentence)
{
	type = VbCodeTranslationUnitType::Module;
	VbModuleHeader moduleHeader{ sentence };
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

void VbCodeTranslationUnitFactory::LoadClassHeader(const Sentence& sentence)
{
	type = VbCodeTranslationUnitType::Class;
	VbClassHeader classHeader{ sentence };
	VbSettingBlock settingBlock{ classHeader.settingBlock };
	if (settingBlock.settings)
		for (auto& settingSentence : *settingBlock.settings)
		{
			VbSetting setting{ settingSentence };
			VbLValue lValue{ setting.lValue };
			VbSettingValue settingValue{ setting.settingValue };
			//TODO: MultiUse, Persistable, DataBindingBehavior, DataSourceBehavior, MTSTransactionMode
		}
	for (auto& attributeSentence : classHeader.attributes)
	{
		VbAttribute attribute{ attributeSentence };
		auto attributeName = VbQualifiedId{ attribute.name }.ToSimpleName();
		auto value = VbCodeExpressionFactory::CreateExpression(attribute.value)->EvaluateConstant();
		if (attributeName == "VB_Name")
		{
			if (value.type != VbCodeValueType::String)
				throw std::runtime_error("VB_Name should be a string.");
			name = value.stringValue;
		}
		//TODO: VB_GlobalNameSpace, VB_Creatable, VB_PredeclaredId, VB_Exposed
	}
}

void VbCodeTranslationUnitFactory::ProcessDeclaration(const Sentence& sentence)
{
	VbDeclaration declaration{ sentence };
	if (declaration.attribute)
		throw std::runtime_error("Attribute not supported in module definition outside of header.");
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

void VbCodeTranslationUnitFactory::ProcessStatement(const Sentence& sentence)
{
	if (functions.empty() && !functionFactory.IsInFunction())
		ProcessHeaderStatement(sentence);
	else
		ProcessBodyStatement(sentence);
}

void VbCodeTranslationUnitFactory::ProcessHeaderStatement(const Sentence& sentence)
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
		typeDefinitions.push_back(VbCodeTypeDefinitionFactory::Create(*statement.typeStatement));
	else if (statement.enumStatement)
		enumDefinitions.push_back(VbCodeEnumDefinitionFactory::Create(*statement.enumStatement));
	else
		functionFactory.AddStatement(sentence);
}

void VbCodeTranslationUnitFactory::ProcessBodyStatement(const Sentence& sentence)
{
	functionFactory.AddStatement(sentence);
	if (functionFactory.IsEndOfFunction())
	{
		functions.push_back(functionFactory.Create());
		if (type == VbCodeTranslationUnitType::Module)
			functions.back().isStatic = true;
		functionFactory = {};
	}
}

void VbCodeTranslationUnitFactory::ProcessConstStatement(const Sentence& sentence)
{
	VbConstStatement constStatement{ sentence };
	auto isPublic = constStatement.access &&
		(*constStatement.access == "public" || *constStatement.access == "global");
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

void VbCodeTranslationUnitFactory::ProcessMemberStatement(bool isPublic, const Sentence& sentence)
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
