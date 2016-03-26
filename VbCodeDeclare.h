#pragma once
#include "VbCodeType.h"
#include "VbCodeParameter.h"
#include "optional.h"
#include <vector>

class VbCodeDeclare
{
public:
	VbCodeDeclare(
		bool isPublic,
		const std::string& name,
		const std::string& alias,
		const std::string& library,
		const std::vector<VbCodeParameter>& parameters,
		const optional<VbCodeType>& returnType)
		: isPublic(isPublic),
		name(name),
		alias(alias),
		library(library),
		parameters(parameters),
		returnType(returnType)
	{
	}

	bool isPublic;
	std::string name;
	std::string alias;
	std::string library;
	std::vector<VbCodeParameter> parameters;
	optional<VbCodeType> returnType;
};
