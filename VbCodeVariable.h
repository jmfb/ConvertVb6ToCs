#pragma once
#include "VbCodeType.h"
#include <string>
#include <iostream>

class VbCodeVariable
{
public:
	VbCodeVariable(const std::string& name, const VbCodeType& type)
		: name(name), type(type)
	{
	}

	void WriteStaticCs(const std::string& functionName, std::ostream& out) const
	{
		out << "		private static ";
		type.WriteCs(out);
		out << " __" << functionName << "_" << name << ";" << std::endl;
	}

	void WriteCs(std::ostream& out) const
	{
		out << "			var " << name << " = default(";
		type.WriteCs(out);
		out << ");" << std::endl;
	}

	std::string name;
	VbCodeType type;
};
