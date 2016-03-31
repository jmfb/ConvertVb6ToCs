#pragma once
#include "VbCodeEnumMember.h"
#include "VbCodeWriter.h"
#include <string>
#include <vector>
#include <iostream>

class VbCodeEnumDefinition
{
public:
	VbCodeEnumDefinition(
		bool isPublic,
		const std::string& name,
		const std::vector<VbCodeEnumMember>& members)
		: isPublic(isPublic), name(name), members(members)
	{
	}

	void WriteCs(VbCodeWriter& writer) const
	{
		writer.StartLine();
		writer.out << (isPublic ? "public" : "internal") << " enum " << name << std::endl;
		writer.BeginBlock();
		auto first = true;
		for (auto& member : members)
		{
			if (!first)
				writer.out << "," << std::endl;
			first = false;
			writer.StartLine();
			writer.out << member.name;
			if (member.expression)
			{
				writer.out << " = ";
				member.expression->WriteCs(writer);
			}
		}
		writer.out << std::endl;
		writer.EndBlock();
		writer.out << std::endl;
	}

	bool isPublic;
	std::string name;
	std::vector<VbCodeEnumMember> members;
};
