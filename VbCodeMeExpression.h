#pragma once
#include "VbCodeExpression.h"

class VbCodeMeExpression : public VbCodeExpression
{
public:
	void WriteXml(std::ostream& out) const final
	{
		out << "Me";
	}
};
