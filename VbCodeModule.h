#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
#include "VbCodeTypeDefinition.h"
#include "VbCodeFunction.h"
#include <string>
#include <vector>
#include <iostream>

class VbCodeModule
{
public:
	VbCodeModule(
		const std::string& library,
		const std::string& name,
		bool isOptionExplicit,
		const std::vector<VbCodeConstant>& constants,
		const std::vector<VbCodeMember>& members,
		const std::vector<VbCodeDeclare>& declares,
		const std::vector<VbCodeTypeDefinition>& typeDefinitions,
		const std::vector<VbCodeFunction>& functions)
		: library(library),
		name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants),
		members(members),
		declares(declares),
		typeDefinitions(typeDefinitions),
		functions(functions)
	{
	}

	void ResolveUnqualifiedTypeNames()
	{
		for (auto& member : members)
			ResolveUnqualifiedTypeName(member.type);
		for (auto& declare : declares)
			for (auto& parameter : declare.parameters)
				ResolveUnqualifiedTypeName(parameter.type);
		for (auto& function : functions)
		{
			if (function.returnValue)
				ResolveUnqualifiedTypeName(*function.returnValue);
			for (auto& parameter : function.parameters)
				ResolveUnqualifiedTypeName(parameter.type);
			for (auto& variable : function.statics)
				ResolveUnqualifiedTypeName(variable.type);
			for (auto& variable : function.variables)
				ResolveUnqualifiedTypeName(variable.type);
		}
	}

	void ResolveUnqualifiedTypeName(VbCodeType& type)
	{
		if (IsUnqualified(type))
			Qualify(type);
	}

	static bool IsUnqualified(const VbCodeType& type)
	{
		return type.type == VbCodeValueType::UserObject && type.userObject.library.empty();
	}

	void Qualify(VbCodeType& type)
	{
		type.userObject.library = ResolveLibrary(type.userObject.name);
	}

	std::string ResolveLibrary(const std::string& value) const
	{
		if (value == "Collection")
			return "VBA";
		if (value == name)
			return library;
		for (auto& typeDefinition : typeDefinitions)
			if (value == typeDefinition.name)
				return library + "." + name;
		throw std::runtime_error("Unknown library for: " + value);
	}

	void WriteCs(std::ostream& out) const
	{
		out << "using System;" << std::endl
			<< "using System.Runtime.InteropServices; " << std::endl
			<< "using static Microsoft.VisualBasic.Constants;" << std::endl
			<< "using static Microsoft.VisualBasic.Information;" << std::endl
			<< "using static Microsoft.VisualBasic.PowerPacks.Printing.Compatibility.VB6.ScaleModeConstants;" << std::endl
			<< "using static Microsoft.VisualBasic.Strings;" << std::endl
			<< std::endl
			<< "namespace " << library << std::endl
			<< "{" << std::endl;
		out << "	internal static class " << name << std::endl
			<< "	{" << std::endl;
		for (auto& constant : constants)
			constant.WriteCs(out);
		if (!constants.empty())
			out << std::endl;
		for (auto& member : members)
			member.WriteCs(true, out);
		if (!members.empty())
			out << std::endl;
		for (auto& declare : declares)
			declare.WriteCs(out);
		for (auto& typeDefinition : typeDefinitions)
			typeDefinition.WriteCs(out);
		for (auto& function : functions)
			function.WriteCs(out);
		out << "	}" << std::endl
			<< "}" << std::endl;
	}

	std::string library;
	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
	std::vector<VbCodeDeclare> declares;
	std::vector<VbCodeTypeDefinition> typeDefinitions;
	std::vector<VbCodeFunction> functions;
};
