#pragma once
#include "VbCodeStatement.h"

class VbCodeEndProgramStatement : public VbCodeStatement
{
public:
	void WriteCs(VbCodeStatementWriter& writer) const final
	{
		writer.StartLine();
		writer.out << "System.Diagnostics.Process.GetCurrentProcess().Close();" << std::endl;
	}
};
