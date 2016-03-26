#pragma once
#include "VbCodeValue.h"

class VbCodeConstant
{
public:
	VbCodeConstant(bool isPublic, const std::string& name, const VbCodeValue& value)
		: isPublic(isPublic), name(name), value(value)
	{
	}

	bool isPublic;
	std::string name;
	VbCodeValue value;
};
