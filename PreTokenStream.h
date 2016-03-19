#pragma once
#include "CharStream.h"
#include "PreTokenLine.h"

class PreTokenStream
{
public:
	PreTokenStream(CharStream& stream)
		: stream(stream)
	{
	}

	bool IsEndOfFile() const
	{
		return stream.IsEndOfFile();
	}
	void MoveFirst()
	{
		stream.MoveFirst();
	}
	const std::string& GetName() const
	{
		return stream.GetName();
	}

	PreTokenLine GetNextLine()
	{
		std::vector<PreToken> tokens;
		while (!IsEndOfFile())
		{
			auto token = InternalGetNextToken();
			auto isNewline = token.GetType() == PreTokenType::Newline;
			if (!isNewline || IncludeNewline())
				tokens.push_back(token);
			if (isNewline)
				break;
		}
		return{ tokens };
	}

	PreToken GetNextToken()
	{
		return InternalGetNextToken();
	}

protected:
	virtual bool IncludeNewline() const
	{
		return false;
	}

	virtual PreToken InternalGetNextToken() = 0;

protected:
	CharStream& stream;
};
