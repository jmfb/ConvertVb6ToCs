#pragma once
#include "VbCodeExpression.h"

class VbCodeMissingExpression : public VbCodeExpression
{
public:
	void WriteXml(std::ostream& out) const final
	{
		out << "<missing />";
	}

	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		//TODO: should this always just be null?
		writer.out << "null";
	}
};
