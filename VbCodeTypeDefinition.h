#pragma once
#include "VbCodeVariable.h"
#include <string>
#include <vector>
#include <iostream>

class VbCodeTypeDefinition
{
public:
	VbCodeTypeDefinition(
		bool isPublic,
		const std::string& name,
		const std::vector<VbCodeVariable>& members)
		: isPublic(isPublic),
		name(name),
		members(members)
	{
	}

	void WriteCs(std::ostream& out) const
	{
		out << "		[StructLayout(LayoutKind.Sequential)]" << std::endl
			<< "		" << (isPublic ? "public" : "private") << " struct " << name << std::endl
			<< "		{" << std::endl;
		for (auto& member : members)
		{
			out << "			public ";
			member.type.WriteCs(out);
			out << " " << member.name << ";" << std::endl;
		}
		out << "		}" << std::endl
			<< std::endl;
	}

	bool isPublic;
	std::string name;
	std::vector<VbCodeVariable> members;
};
