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

	void WriteCs(std::ostream& out) const
	{
		for (auto& variable : statics)
			variable.WriteStaticCs(name, out);
		out << "		";
		switch (access)
		{
		case VbCodeFunctionAccess::Public:
			out << "public";
			break;
		case VbCodeFunctionAccess::Private:
			out << "private";
			break;
		case VbCodeFunctionAccess::Friend:
			out << "internal";
			break;
		}
		out << " " << (isStatic ? "static " : "");
		if (returnValue)
			returnValue->WriteCs(out);
		else
			out << "void";
		out << " " << name << "(";
		auto first = true;
		for (auto& parameter : parameters)
		{
			if (!first)
				out << ",";
			first = false;
			parameter.WriteCs(out);
		}
		out << ")" << std::endl
			<< "		{" << std::endl;
		if (returnValue)
		{
			out << "			var __result__ = default(";
			returnValue->WriteCs(out);
			out << ");" << std::endl;
		}
		for (auto& variable : variables)
			variable.WriteCs(out);
		//TODO: statements
		if (returnValue)
			out << "			return __result__;" << std::endl;
		out << "		}" << std::endl
			<< std::endl;
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
