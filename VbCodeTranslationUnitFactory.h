#pragma once
#include "VbCodeTranslationUnit.h"
#include "VbCodeTranslationUnitType.h"
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
#include "VbCodeTypeDefinition.h"
#include "VbCodeEnumDefinition.h"
#include "VbCodeFunction.h"
#include "VbCodeFunctionFactory.h"
#include "Sentence.h"
#include <string>
#include <vector>

class VbCodeTranslationUnitFactory
{
public:
	VbCodeTranslationUnit Create(const std::string& library, const Sentence& sentence);

private:
	void LoadHeader(const Sentence& sentence);
	void LoadModuleHeader(const Sentence& sentence);
	void LoadClassHeader(const Sentence& sentence);
	void ProcessDeclaration(const Sentence& sentence);
	void ProcessStatement(const Sentence& sentence);
	void ProcessHeaderStatement(const Sentence& sentence);
	void ProcessBodyStatement(const Sentence& sentence);
	void ProcessConstStatement(const Sentence& sentence);
	void ProcessMemberStatement(bool isPublic, const Sentence& sentence);

private:
	VbCodeFunctionFactory functionFactory;

private:
	VbCodeTranslationUnitType type;
	std::string name;
	bool isOptionExplicit = false;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
	std::vector<VbCodeDeclare> declares;
	std::vector<VbCodeTypeDefinition> typeDefinitions;
	std::vector<VbCodeEnumDefinition> enumDefinitions;
	std::vector<VbCodeFunction> functions;
};
