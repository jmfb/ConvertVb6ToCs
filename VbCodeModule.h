#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include <string>
#include <vector>

class VbCodeModule
{
public:
	VbCodeModule(
		const std::string& name,
		bool isOptionExplicit,
		const std::vector<VbCodeConstant>& constants,
		const std::vector<VbCodeMember>& members)
		: name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants),
		members(members)
	{
	}

	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
};
