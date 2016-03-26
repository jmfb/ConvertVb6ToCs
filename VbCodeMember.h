#pragma once
#include "VbCodeType.h"
#include <string>

class VbCodeMember
{
public:
	VbCodeMember(bool isPublic, bool isWithEvents, const std::string& name, const VbCodeType& type)
		: isPublic(isPublic), isWithEvents(isWithEvents), name(name), type(type)
	{
	}

	bool isPublic;
	bool isWithEvents;
	std::string name;
	VbCodeType type;
};
