#pragma once
#include "VbCodeFunctionType.h"
#include "VbCodeFunctionAccess.h"
#include "VbCodeParameter.h"
#include "VbCodeType.h"
#include "VbCodeVariable.h"
#include "VbCodeStatement.h"
#include "VbCodeWriter.h"
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

	void WriteCs(VbCodeWriter& writer) const
	{
		for (auto& variable : statics)
			variable.WriteStaticCs(name, writer.out);
		writer.StartLine();
		writer.out << ToCs(access) << " " << (isStatic ? "static " : "");
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
		writer.out << ")" << std::endl;
		writer.BeginBlock();
		if (returnValue)
		{
			writer.StartLine();
			writer.out << "var __result__ = default(";
			returnValue->WriteCs(writer.out);
			writer.out << ");" << std::endl;
		}
		for (auto& variable : variables)
			variable.WriteCs(writer.out);
		for (auto& statement : statements)
			statement->WriteCs(writer);
		if (returnValue)
		{
			writer.StartLine();
			writer.out << "return __result__;" << std::endl;
		}
		writer.EndBlock();
		writer.out << std::endl;
	}

	void WritePropertyCs(VbCodeWriter& writer, VbCodeFunctionAccess propertyAccess) const
	{
		writer.StartLine();
		if (access != propertyAccess)
			writer.out << ToCs(access) << " ";
		writer.out << (type == VbCodeFunctionType::PropertyGet ? "get" : "set") << std::endl;
		writer.BeginBlock();
		if (type == VbCodeFunctionType::PropertyGet)
		{
			if (!parameters.empty())
				throw std::runtime_error("Indexed property get not yet supported.");
			writer.StartLine();
			writer.out << "var __result__ = default(";
			returnValue->WriteCs(writer.out);
			writer.out << ");" << std::endl;
		}
		else
		{
			if (parameters.size() > 1)
				throw std::runtime_error("Indexed property set not yet supported.");
		}
		for (auto& statement : statements)
			statement->WriteCs(writer);
		if (type == VbCodeFunctionType::PropertyGet)
		{
			writer.StartLine();
			writer.out << "return __result__;" << std::endl;
		}
		writer.EndBlock();
	}

	bool IsSetter() const
	{
		return type == VbCodeFunctionType::PropertyLet || type == VbCodeFunctionType::PropertySet;
	}
	bool IsGetter() const
	{
		return type == VbCodeFunctionType::PropertyGet || type == VbCodeFunctionType::Function;
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
