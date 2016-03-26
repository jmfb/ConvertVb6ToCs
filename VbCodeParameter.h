#pragma once
#include "VbCodeType.h"
#include "VbCodeExpression.h"
#include <string>

class VbCodeParameter
{
public:
	VbCodeParameter(
		bool isByVal,
		const std::string& name,
		const VbCodeType& type,
		bool isOptional,
		bool isParamArray,
		VbCodeExpressionPtr defaultValue)
		: isByVal(isByVal),
		name(name),
		type(type),
		isOptional(isOptional),
		isParamArray(isParamArray),
		defaultValue(defaultValue)
	{
	}

	bool isByVal;
	std::string name;
	VbCodeType type;
	bool isOptional;
	bool isParamArray;
	VbCodeExpressionPtr defaultValue;
};
