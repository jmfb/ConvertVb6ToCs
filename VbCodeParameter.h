#pragma once
#include "VbCodeType.h"
#include "VbCodeExpression.h"
#include <string>
#include <iostream>

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

	void WriteCs(std::ostream& out) const
	{
		out << std::endl
			<< "			";
		if (!isByVal)
			out << "ref ";
		if (isParamArray)
			out << "params ";
		if (isOptional)
			throw std::runtime_error("Optional parameters not yet implemented.");
		type.WriteCs(out);
		out << " " << name;
		if (defaultValue)
			throw std::runtime_error("Default value not yet implemented.");
	}

	bool isByVal;
	std::string name;
	VbCodeType type;
	bool isOptional;
	bool isParamArray;
	VbCodeExpressionPtr defaultValue;
};
