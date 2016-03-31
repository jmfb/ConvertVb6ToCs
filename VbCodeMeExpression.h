#pragma once
#include "VbCodeExpression.h"

class VbCodeMeExpression : public VbCodeExpression
{
public:
	void WriteCs(VbCodeWriter& writer) const final
	{
		writer.out << "this";
	}
};
