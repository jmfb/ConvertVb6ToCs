#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
#include "VbCodeTypeDefinition.h"
#include "VbCodeEnumDefinition.h"
#include "VbCodeFunction.h"
#include "VbCodeTranslationUnitType.h"
#include "VbCodeIdResolver.h"
#include "VbCodeStatementWriter.h"
#include <string>
#include <vector>
#include <iostream>

class VbCodeTranslationUnit : public VbCodeIdResolver
{
public:
	VbCodeTranslationUnit(
		VbCodeTranslationUnitType type,
		const std::string& library,
		const std::string& name,
		bool isOptionExplicit,
		const std::vector<VbCodeConstant>& constants,
		const std::vector<VbCodeMember>& members,
		const std::vector<VbCodeDeclare>& declares,
		const std::vector<VbCodeTypeDefinition>& typeDefinitions,
		const std::vector<VbCodeEnumDefinition>& enumDefinitions,
		const std::vector<VbCodeFunction>& functions)
		: type(type),
		library(library),
		name(name),
		isOptionExplicit(isOptionExplicit),
		constants(constants),
		members(members),
		declares(declares),
		typeDefinitions(typeDefinitions),
		enumDefinitions(enumDefinitions),
		functions(functions)
	{
	}

	std::string Resolve(const std::string& id) const final
	{
		if (currentFunction != nullptr)
		{
			if (id == currentFunction->name)
				return "__result__";
			for (auto& variable : currentFunction->statics)
				if (variable.name == id)
					return "__" + currentFunction->name + "_" + id;
			for (auto& variable : currentFunction->variables)
				if (variable.name == id)
					return id;
		}

		//TODO: module level constants/declares/members/type-definitions/etc.

		if (id == "Err")
			return "Err()";
		if (id == "CStr")
			return "Convert.ToString";
		if (id == "ScaleModeConstants")
			return "Microsoft.VisualBasic.PowerPacks.Printing.Compatibility.VB6.ScaleModeConstants";

		//No clue, just return it for now...
		return id;
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
		switch (type)
		{
		case VbCodeTranslationUnitType::Module:
			out << "	internal static class " << name << std::endl;
			break;
		case VbCodeTranslationUnitType::Class:
			out << "	[ComVisible(true)]" << std::endl
				<< "	[Guid(\"TODO: Read from type library\")]" << std::endl
				<< "	[ProgId(\"" << library << "." << name << "\")]" << std::endl
				<< "	[ComDefaultInterface(typeof(_" << name << "))]" << std::endl
				<< "	[ClassInterface(ClassInterfaceType.None)]" << std::endl
				<< "	public class " << name << " : MarshalByRefObject, _" << name << std::endl;
			break;
		default:
			throw std::runtime_error("Only support generating module and class files.");
		}
		out << "	{" << std::endl;
		VbCodeStatementWriter writer{ out, *this };
		for (auto& constant : constants)
			constant.WriteCs(out);
		if (!constants.empty())
			out << std::endl;
		for (auto& enumDefinition : enumDefinitions)
			enumDefinition.WriteCs(writer);
		for (auto& member : members)
			member.WriteCs(true, out);
		if (!members.empty())
			out << std::endl;
		for (auto& declare : declares)
			declare.WriteCs(out);
		for (auto& typeDefinition : typeDefinitions)
			typeDefinition.WriteCs(out);
		for (auto& function : functions)
		{
			currentFunction = &function;
			function.WriteCs(writer);
			currentFunction = nullptr;
		}
		out << "	}" << std::endl
			<< "}" << std::endl;
	}

	VbCodeTranslationUnitType type;
	std::string library;
	std::string name;
	bool isOptionExplicit;
	std::vector<VbCodeConstant> constants;
	std::vector<VbCodeMember> members;
	std::vector<VbCodeDeclare> declares;
	std::vector<VbCodeTypeDefinition> typeDefinitions;
	std::vector<VbCodeEnumDefinition> enumDefinitions;
	std::vector<VbCodeFunction> functions;

	mutable const VbCodeFunction* currentFunction = nullptr;
};
