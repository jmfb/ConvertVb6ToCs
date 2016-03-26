#pragma once
#include "VbCodeConstant.h"
#include <string>
#include <vector>

class VbCodeModule
{
public:
	VbCodeModule(
		const std::string& name,
		bool isOptionExplicit,
		const std::vector<VbCodeConstant>& constants)
		: name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants)
	{
	}

	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
};
