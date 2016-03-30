#pragma once
#include "VbCodeEnumMember.h"
#include "VbCodeStatementWriter.h"
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

	void WriteCs(VbCodeStatementWriter& writer) const
	{
		writer.out << "		" << (isPublic ? "public" : "internal") << " enum " << name << std::endl
			<< "		{";
		auto first = true;
		for (auto& member : members)
		{
			if (!first)
				writer.out << ",";
			writer.out << std::endl
				<< "			" << member.name;
			if (member.expression)
			{
				writer.out << " = ";
				member.expression->WriteCs(writer);
			}
		}
		writer.out << "		}" << std::endl
			<< std::endl;
	}

	bool isPublic;
	std::string name;
	std::vector<VbCodeEnumMember> members;
};
