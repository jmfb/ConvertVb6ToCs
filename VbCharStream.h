#pragma once
#include "CharStream.h"

class VbCharStream : public CharStream
{
public:
	VbCharStream(const std::string& name, const std::string& value)
		: CharStream{ name, value }
	{
	}

protected:
	Char InternalGetNext() final
	{
		if (IsEndOfFile())
			return Char::EndOfFile();
		Char ch{ value[index], { name, line, column } };
		++index;
		++column;
		if (IsNewline(ch))
			Newline();
		else if (IsLineContinuation(ch))
			SkipLineContinuation();
		return ch;
	}

private:
	static bool IsNewline(const Char& ch)
	{
		return ch.GetValue() == '\n';
	}
	bool IsLineContinuation(const Char& ch)
	{
		return ch.GetValue() == ' ' &&
			(index + 1) < value.size() &&
			value[index] == '_' &&
			value[index + 1] == '\n';
	}
	void Newline()
	{
		++line;
		column = 1;
	}
	void SkipLineContinuation()
	{
		index += 2;
		Newline();
	}
};
