#pragma once
#include <string>

class VbCodeTypeName
{
public:
	VbCodeTypeName() = default;
	VbCodeTypeName(const std::string& library, const std::string& name)
		: library(library), name(name)
	{
	}

	std::string library;
	std::string name;
};
