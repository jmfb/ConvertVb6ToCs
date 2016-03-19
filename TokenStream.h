#pragma once
#include "PreProcessor.h"
#include "Token.h"
#include <stack>

class TokenStream
{
public:
	TokenStream(PreProcessor& processor)
		: processor(processor)
	{
	}

	Token GetNextToken()
	{
		if (!tokens.empty())
		{
			auto result = tokens.top();
			tokens.pop();
			return result;
		}

		auto token = processor.GetNextToken();
		if (token == PreToken::EndOfFile())
			return Token::EndOfFile();

		return InternalCreateToken(token);
	}

	void PutBackToken(const Token& token)
	{
		if (token != Token::EndOfFile())
			tokens.push(token);
	}

protected:
	virtual Token InternalCreateToken(const PreToken& token) = 0;

private:
	PreProcessor& processor;
	std::stack<Token> tokens;
};
