#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
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
		const std::vector<VbCodeDeclare>& declares)
		: name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants),
		members(members),
		declares(declares)
	{
	}

	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
	std::vector<VbCodeDeclare> declares;
};
