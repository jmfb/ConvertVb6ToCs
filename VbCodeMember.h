#pragma once
#include "VbCodeType.h"
#include <string>
#include <iostream>

class VbCodeMember
{
public:
	VbCodeMember(bool isPublic, bool isWithEvents, const std::string& name, const VbCodeType& type)
		: isPublic(isPublic), isWithEvents(isWithEvents), name(name), type(type)
	{
	}

	void WriteCs(bool isStatic, std::ostream& out) const
	{
		if (isWithEvents)
			throw std::runtime_error("WithEvents not yet implemented.");
		out << "		" << (isPublic ? "public" : "private") << " " << (isStatic ? "static " : "");
		type.WriteCs(out);
		out << " " << name << ";" << std::endl;
	}

	bool isPublic;
	bool isWithEvents;
	std::string name;
	VbCodeType type;
};
