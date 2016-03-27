#pragma once
#include "VbCodeType.h"
#include "VbCodeParameter.h"
#include "optional.h"
#include <vector>
#include <iostream>

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

	void WriteCs(std::ostream& out) const
	{
		out << "		[System.Runtime.InteropServices.DllImport(" << std::endl
			<< "			\"" << library << "\"";
		if (!alias.empty())
			out << "," << std::endl
				<< "			EntryPoint = \"" << alias << "\"";
		if (HasStringParameter())
			out << "," << std::endl
				<< "			CharSet = System.Runtime.InteropServices.CharSet." << (IsUnicodeApi() ? "Unicode" : "Ansi");
		out << ")]" << std::endl
			<< "		" << (isPublic ? "public" : "private") << " static extern ";
		if (returnType)
			returnType->WriteCs(out);
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
		out << ");" << std::endl
			<< std::endl;
	}

	bool HasStringParameter() const
	{
		return std::any_of(parameters.begin(), parameters.end(), [](auto p){ return p.type.type == VbCodeValueType::String; });
	}

	bool IsUnicodeApi() const
	{
		if (!alias.empty())
			return alias.back() == 'W';
		return name.back() == 'W';
	}

	bool isPublic;
	std::string name;
	std::string alias;
	std::string library;
	std::vector<VbCodeParameter> parameters;
	optional<VbCodeType> returnType;
};
