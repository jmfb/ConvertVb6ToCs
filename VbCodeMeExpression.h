#pragma once
#include "VbCodeExpression.h"

class VbCodeMeExpression : public VbCodeExpression
{
public:
	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.out << "this";
	}
};
