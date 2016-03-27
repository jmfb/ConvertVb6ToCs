#pragma once
#include "VbCodeFunctionType.h"
#include "VbCodeFunctionAccess.h"
#include "VbCodeParameter.h"
#include "VbCodeType.h"
#include "VbCodeVariable.h"
#include "VbCodeStatement.h"
#include "optional.h"
#include <string>
#include <vector>
#include <iostream>

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

	void WriteXml(std::ostream& out) const
	{
		out << "<function name=\"" << name << "\">";
		for (auto& statement : statements)
			statement->WriteXml(out);
		out << "</function>";
	}

	VbCodeFunctionType type;
	VbCodeFunctionAccess access;
	bool isStatic = false;
	std::string name;
	std::vector<VbCodeParameter> parameters;
	optional<VbCodeType> returnValue;
	std::vector<VbCodeVariable> variables;
	std::vector<VbCodeVariable> statics;
	std::vector<VbCodeStatementPtr> statements;
};
