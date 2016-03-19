#pragma once
#include "PreLineEvaluator.h"
#include "PreToken.h"
#include "PreLine.h"
#include <deque>

class PreProcessor : public PreLineEvaluator
{
public:
	PreProcessor(const std::deque<PreLinePtr>& lines)
		: lines(lines)
	{
	}

	PreToken GetNextToken()
	{
		return InternalGetNextToken();
	}

	const std::deque<PreLinePtr>& GetLines() const
	{
		return lines;
	}

protected:
	PreToken GetNextRawToken()
	{
		while (!tokens.empty() || !lines.empty())
		{
			if (!tokens.empty())
			{
				auto token = tokens.front();
				tokens.pop_front();
				return token;
			}

			auto line = lines.front();
			lines.pop_front();
			line->Evaluate(*this);
		}

		return PreToken::EndOfFile();
	}

	virtual PreToken InternalGetNextToken() = 0;

protected:
	std::deque<PreLinePtr> lines;
	std::deque<PreToken> tokens;
};
