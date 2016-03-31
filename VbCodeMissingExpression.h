#pragma once
#include "VbCodeExpression.h"

class VbCodeMissingExpression : public VbCodeExpression
{
public:
	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		//TODO: should this always just be null?
		writer.out << "null";
	}
};
