#pragma once
#include "VbCodeType.h"
#include "VbCodeWriter.h"
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

	void WriteCs(VbCodeWriter& writer) const
	{
		writer.StartLine();
		writer.out << "var " << name << " = default(";
		type.WriteCs(writer.out);
		writer.out << ");" << std::endl;
	}

	std::string name;
	VbCodeType type;
};
