#pragma once
#include "VbCodeExpression.h"

class VbCodeMissingExpression : public VbCodeExpression
{
public:
	void WriteXml(std::ostream& out) const final
	{
		out << "<missing />";
	}
};
