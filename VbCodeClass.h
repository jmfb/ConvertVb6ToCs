#pragma once
#include "VbCodeType.h"
#include <string>
#include <iostream>
#include <stdexcept>

class VbCodeClass
{
public:
	VbCodeClass(
		const std::string& library,
		const std::string& name)
		: library(library),
		name(name)
	{
	}

	void ResolveUnqualifiedTypeNames()
	{
		//TODO:
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
		//for (auto& typeDefinition : typeDefinitions)
		//	if (value == typeDefinition.name)
		//		return library + "." + name;
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
			<< "{" << std::endl
			<< "	[ComVisible(true)]" << std::endl
			<< "	[Guid(\"TODO: Read from type library\")]" << std::endl
			<< "	[ProgId(\"" << library << "." << name << "\")]" << std::endl
			<< "	[ComDefaultInterface(typeof(_" << name << "))]" << std::endl
			<< "	[ClassInterface(ClassInterfaceType.None)]" << std::endl
			<< "	public class " << name << " : MarshalByRefObject, _" << name << std::endl
			<< "	{" << std::endl;
		//TODO:
		out << "	}" << std::endl
			<< "}" << std::endl;
	}

	std::string library;
	std::string name;
};
