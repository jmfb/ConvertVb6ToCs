#pragma once
#include "VbCodeFunctionType.h"
#include "VbCodeFunctionAccess.h"
#include "VbCodeParameter.h"
#include "VbCodeType.h"
#include "VbCodeVariable.h"
#include "VbCodeStatement.h"
#include "VbCodeStatementWriter.h"
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

	void WriteCs(VbCodeStatementWriter& writer) const
	{
		for (auto& variable : statics)
			variable.WriteStaticCs(name, writer.out);
		writer.out << "		";
		switch (access)
		{
		case VbCodeFunctionAccess::Public:
			writer.out << "public";
			break;
		case VbCodeFunctionAccess::Private:
			writer.out << "private";
			break;
		case VbCodeFunctionAccess::Friend:
			writer.out << "internal";
			break;
		}
		writer.out << " " << (isStatic ? "static " : "");
		if (returnValue)
			returnValue->WriteCs(writer.out);
		else
			writer.out << "void";
		writer.out << " " << name << "(";
		auto first = true;
		for (auto& parameter : parameters)
		{
			if (!first)
				writer.out << ",";
			first = false;
			parameter.WriteCs(writer.out);
		}
		writer.out << ")" << std::endl
			<< "		{" << std::endl;
		if (returnValue)
		{
			writer.out << "			var __result__ = default(";
			returnValue->WriteCs(writer.out);
			writer.out << ");" << std::endl;
		}
		for (auto& variable : variables)
			variable.WriteCs(writer.out);
		for (auto& statement : statements)
			statement->WriteCs(writer);
		if (returnValue)
			writer.out << "			return __result__;" << std::endl;
		writer.out << "		}" << std::endl
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
