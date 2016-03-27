#pragma once
#include "VbCodeValue.h"
#include <iostream>

class VbCodeConstant
{
public:
	VbCodeConstant(bool isPublic, const std::string& name, const VbCodeValue& value)
		: isPublic(isPublic), name(name), value(value)
	{
	}

	void WriteCs(std::ostream& out) const
	{
		out << "		" << (isPublic ? "public" : "private") << " const ";
		value.WriteTypeCs(out);
		out << " " << name << " = ";
		value.WriteValueCs(out);
		out << ";" << std::endl;
	}

	bool isPublic;
	std::string name;
	VbCodeValue value;
};
