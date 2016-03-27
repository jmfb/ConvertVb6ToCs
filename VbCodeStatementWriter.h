#pragma once
#include "VbCodeIdResolver.h"
#include <iostream>
#include <string>
#include <stack>

class VbCodeStatementWriter
{
public:
	VbCodeStatementWriter(std::ostream& out, const VbCodeIdResolver& resolver)
		: out(out), resolver(resolver), indent(3)
	{
	}

	void BeginBlock()
	{
		StartLine();
		out << "{" << std::endl;
		++indent;
	}
	void EndBlock()
	{
		--indent;
		StartLine();
		out << "}" << std::endl;
	}

	void PushIndent()
	{
		++indent;
	}
	void PopIndent()
	{
		--indent;
	}

	void StartLine()
	{
		out << std::string(indent, '\t');
	}

	std::string Resolve(const std::string& id) const
	{
		return resolver.Resolve(id);
	}

	void PushWith()
	{
		withs.push(nextWith);
		++nextWith;
		With();
	}
	void PopWith()
	{
		withs.pop();
	}
	void With()
	{
		if (withs.empty())
			throw std::runtime_error("Not currently in a with block.");
		out << "__with" << withs.top();
	}

public:
	std::ostream& out;

private:
	const VbCodeIdResolver& resolver;
	int indent;
	int nextWith = 1;
	std::stack<int> withs;
};
