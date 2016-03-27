#pragma once
#include "VbCodeStatement.h"

class VbCodeEndProgramStatement : public VbCodeStatement
{
public:
	void WriteXml(std::ostream& out) const final
	{
		out << "<end-program-statement />";
	}
};
