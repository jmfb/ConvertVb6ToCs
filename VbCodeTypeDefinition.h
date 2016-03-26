#pragma once
#include "VbCodeVariable.h"
#include <string>
#include <vector>

class VbCodeTypeDefinition
{
public:
	VbCodeTypeDefinition(
		bool isPublic,
		const std::string& name,
		const std::vector<VbCodeVariable>& members)
		: isPublic(isPublic),
		name(name),
		members(members)
	{
	}

	bool isPublic;
	std::string name;
	std::vector<VbCodeVariable> members;
};
