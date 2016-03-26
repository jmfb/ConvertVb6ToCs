#pragma once
#include "VbCodeType.h"
#include <string>

class VbCodeVariable
{
public:
	VbCodeVariable(const std::string& name, const VbCodeType& type)
		: name(name), type(type)
	{
	}

	std::string name;
	VbCodeType type;
};
