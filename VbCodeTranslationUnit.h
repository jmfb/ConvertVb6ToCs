#pragma once
#include "VbCodeConstant.h"
#include "VbCodeMember.h"
#include "VbCodeDeclare.h"
#include "VbCodeTypeDefinition.h"
#include "VbCodeEnumDefinition.h"
#include "VbCodeFunction.h"
#include "VbCodeTranslationUnitType.h"
#include "VbCodeIdResolver.h"
#include "VbCodeWriter.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "TypeTable.h"

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
		enumDefinitions(enumDefinitions)
	{
		for (auto& function : functions)
			AddFunction(function);
	}

	void DefineTypes(TypeTable& table) const
	{
		auto libraryHandle = table.GetLibrary(library);
		auto handle = table.DefineClass(libraryHandle, name);
		//TODO: add members in a second pass
		for (auto& typeDefinition : typeDefinitions)
			table.DefineClass(handle, typeDefinition.name);
		//TODO: add members in a second pass
		for (auto& enumDefinition : enumDefinitions)
		{
			auto enumHandle = table.DefineEnum(handle, enumDefinition.name);
			//TODO: define enum members...
		}
		//TODO: add declares as functions (2nd pass?)
		//TODO: add functions (2nd pass?)
	}

	std::string Resolve(const std::string& id) const final
	{
		if (currentFunction != nullptr)
		{
			if (currentFunction->IsGetter() && id == currentFunction->name)
				return "__result__";
			for (auto& variable : currentFunction->statics)
				if (variable.name == id)
					return "__" + currentFunction->name + "_" + id;
			for (auto& constant : currentFunction->constants)
				if (constant.name == id)
					return id;
			for (auto& variable : currentFunction->variables)
				if (variable.name == id)
					return id;
			if (currentFunction->IsSetter() && id == currentFunction->parameters[0].name)
				return "value";
			for (auto& parameter : currentFunction->parameters)
				if (parameter.name == id)
					return id;
			//TODO: check other local function definitions
		}

		//TODO: check other translation units
		if (id == "err_Value")
			return library + ".Globals.eError." + id;

		for (auto& constant : constants)
			if (constant.name == id)
				return id;
		for (auto& member : members)
			if (member.name == id)
				return id;
		for (auto& declare : declares)
			if (declare.name == id)
				return id;
		for (auto& typeDefinition : typeDefinitions)
			if (typeDefinition.name == id)
				return id;
		for (auto& enumDefinition : enumDefinitions)
		{
			if (enumDefinition.name == id)
				return id;
			for (auto& enumMember : enumDefinition.members)
				if (enumMember.name == id)
					return enumDefinition.name + "." + id;
		}
		for (auto& function : functions)
			if (function.name == id)
				return id;
		//TODO: other module level definitions

		if (id == "Err")
			return "Err()";
		if (id == "CStr")
			return "Convert.ToString";
		if (id == "Trim$")
			return "Trim";
		if (id == "ScaleModeConstants")
			return "Microsoft.VisualBasic.PowerPacks.Printing.Compatibility.VB6.ScaleModeConstants";

		//TODO: check referenced type libraries (properly)
		if (IsAdoFieldAttributeEnum(id))
			return "ADODB.FieldAttributeEnum." + id;
		if (IsAdoDataTypeEnum(id))
			return "ADODB.DataTypeEnum." + id;

		//No clue, just return it for now...
		return "TODO_Resolve(" + id + ")";
	}

	static bool IsAdoFieldAttributeEnum(const std::string& id)
	{
		return id == "adFldIsNullable" || id == "adFldLong";
	}

	static bool IsAdoDataTypeEnum(const std::string& id)
	{
		return id == "adBigInt" ||
			id == "adBinary" ||
			id == "adBoolean" ||
			id == "adChar" ||
			id == "adCurrency" ||
			id == "adDate" ||
			id == "adDBDate" ||
			id == "adDBTime" ||
			id == "adDBTimeStamp" ||
			id == "adDecimal" ||
			id == "adDouble" ||
			id == "adInteger" ||
			id == "adLongVarBinary" ||
			id == "adLongVarChar" ||
			id == "adNumeric" ||
			id == "adSingle" ||
			id == "adSmallInt" ||
			id == "adTinyInt" ||
			id == "adUnsignedBigInt" ||
			id == "adUnsignedInt" ||
			id == "adUnsignedSmallInt" ||
			id == "adUnsignedTinyInt" ||
			id == "adVarBinary" ||
			id == "adVarChar" ||
			id == "adVarNumeric" ||
			id == "adGUID";
	}

	void ResolveUnqualifiedTypeNames()
	{
		for (auto& member : members)
			ResolveUnqualifiedTypeName(member.type);
		for (auto& declare : declares)
			for (auto& parameter : declare.parameters)
				ResolveUnqualifiedTypeName(parameter.type);
		for (auto& function : functions)
			ResolveUnqualifiedTypeNamesInFunction(function);
		for (auto& property : properties)
			for (auto& function : property.second)
				ResolveUnqualifiedTypeNamesInFunction(function);
	}

	void ResolveUnqualifiedTypeNamesInFunction(VbCodeFunction& function)
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
		//TODO: check referenced libraries
		if (value == "Collection")
			return "VBA";
		if (value == name)
			return library;
		for (auto& typeDefinition : typeDefinitions)
			if (value == typeDefinition.name)
				return library + "." + name;
		for (auto& enumDefinition : enumDefinitions)
			if (value == enumDefinition.name)
				return library + "." + name;
		//TODO: check other translation units
		if (value == "eDataFieldType")
			return library + ".Globals";
		throw std::runtime_error("Unknown library for: " + value);
	}

	void WriteCs(std::ostream& out) const
	{
		out << "using System;" << std::endl
			<< "using System.Runtime.InteropServices;" << std::endl
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
		VbCodeWriter writer{ out, *this };
		for (auto& constant : constants)
			constant.WriteClassCs(out);
		if (!constants.empty())
			out << std::endl;
		for (auto& enumDefinition : enumDefinitions)
			enumDefinition.WriteCs(writer);
		for (auto& member : members)
			member.WriteCs(type == VbCodeTranslationUnitType::Module, out);
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
		for (auto& property : properties)
			WritePropertyCs(writer, property.second);
		out << "	}" << std::endl
			<< "}" << std::endl;
	}

	void AddFunction(const VbCodeFunction& function)
	{
		if (function.type == VbCodeFunctionType::Function ||
			function.type == VbCodeFunctionType::Sub)
			functions.push_back(function);
		else
			properties[function.name].push_back(function);
	}

	void WritePropertyCs(VbCodeWriter& writer, const std::vector<VbCodeFunction>& propertyFunctions) const
	{
		auto& name = propertyFunctions.front().name;
		optional<VbCodeType> type;
		auto access = VbCodeFunctionAccess::Private;
		for (auto& function : propertyFunctions)
		{
			if (function.access == VbCodeFunctionAccess::Public)
				access = VbCodeFunctionAccess::Public;
			else if (function.access == VbCodeFunctionAccess::Friend && access == VbCodeFunctionAccess::Private)
				access = VbCodeFunctionAccess::Friend;
			if (function.type == VbCodeFunctionType::PropertyGet)
				type = function.returnValue;
			if (function.isStatic)
				throw std::runtime_error("Static properties not yet supported.");
			if (!function.statics.empty())
				throw std::runtime_error("Static locals in properties not yet supported.");
		}
		if (!type)
			type = propertyFunctions[0].parameters[0].type;
		writer.StartLine();
		writer.out << ToCs(access) << " ";
		type->WriteCs(writer.out);
		writer.out << " " << name << std::endl;
		writer.BeginBlock();
		for (auto& function : propertyFunctions)
		{
			currentFunction = &function;
			function.WritePropertyCs(writer, access);
			currentFunction = nullptr;
		}
		writer.EndBlock();
		writer.out << std::endl;
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
	std::map<std::string, std::vector<VbCodeFunction>> properties;

	mutable const VbCodeFunction* currentFunction = nullptr;
};
