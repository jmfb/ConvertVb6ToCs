#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
#include "VbCodeTypeDefinition.h"
#include "VbCodeFunction.h"
#include <string>
#include <vector>

class VbCodeModule
{
public:
	VbCodeModule(
		const std::string& name,
		bool isOptionExplicit,
		const std::vector<VbCodeConstant>& constants,
		const std::vector<VbCodeMember>& members,
		const std::vector<VbCodeDeclare>& declares,
		const std::vector<VbCodeTypeDefinition>& typeDefinitions,
		const std::vector<VbCodeFunction>& functions)
		: name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants),
		members(members),
		declares(declares),
		typeDefinitions(typeDefinitions),
		functions(functions)
	{
	}

	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
	std::vector<VbCodeDeclare> declares;
	std::vector<VbCodeTypeDefinition> typeDefinitions;
	std::vector<VbCodeFunction> functions;
};
