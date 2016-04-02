#pragma once
#include "VbCodeValue.h"
#include "VbCodeWriter.h"
#include <iostream>

class VbCodeConstant
{
public:
	VbCodeConstant(bool isPublic, const std::string& name, const VbCodeValue& value)
		: isPublic(isPublic), name(name), value(value)
	{
	}

	void WriteClassCs(std::ostream& out) const
	{
		out << "		" << (isPublic ? "public" : "private") << " const ";
		value.WriteTypeCs(out);
		out << " " << name << " = ";
		value.WriteValueCs(out);
		out << ";" << std::endl;
	}

	void WriteLocalCs(VbCodeWriter& writer) const
	{
		writer.StartLine();
		writer.out << "const ";
		value.WriteTypeCs(writer.out);
		writer.out << " " << name << " = ";
		value.WriteValueCs(writer.out);
		writer.out << ";" << std::endl;
	}

	bool isPublic;
	std::string name;
	VbCodeValue value;
};
