#pragma once
#include "VbCodeFunctionType.h"
#include "VbCodeFunctionAccess.h"
#include "VbCodeParameter.h"
#include "VbCodeType.h"
#include "optional.h"
#include <string>
#include <vector>

class VbCodeFunction
{
public:
	VbCodeFunction() = default;
	VbCodeFunction(
		VbCodeFunctionType type,
		VbCodeFunctionAccess access,
		bool isStatic,
		const std::string& name,
		const std::vector<VbCodeParameter>& parameters,
		const optional<VbCodeType> returnValue)
		: type(type), access(access), isStatic(isStatic), name(name), parameters(parameters), returnValue(returnValue)
	{
	}

	VbCodeFunctionType type;
	VbCodeFunctionAccess access;
	bool isStatic = false;
	std::string name;
	std::vector<VbCodeParameter> parameters;
	optional<VbCodeType> returnValue;
	//TODO: statements
};
