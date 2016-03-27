#pragma once
#include <string>

class VbCodeIdResolver
{
public:
	virtual std::string Resolve(const std::string& id) const = 0;
};
