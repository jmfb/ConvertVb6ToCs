#pragma once
#include "PreToken.h"
#include <vector>
#include <stdexcept>

class PreTokenLine
{
public:
	PreTokenLine(const std::vector<PreToken>& tokens)
		: tokens(tokens)
	{
	}

	bool IsDirective() const
	{
		for (auto& token : tokens)
			if (IsDirectiveOperator(token))
				return true;
			else if (!CanSkip(token))
				return false;
		return false;
	}

	std::pair<PreToken, std::vector<PreToken>> ParseDirective() const
	{
		auto operatorIndex = FindDirectiveOperator();
		auto identifierIndex = FindDirectiveIdentifier(operatorIndex);
		if (identifierIndex == -1)
			return{ tokens[operatorIndex], { tokens.begin() + operatorIndex + 1, tokens.end() } };
		return{ tokens[identifierIndex], { tokens.begin() + identifierIndex + 1, tokens.end() } };
	}

	const std::vector<PreToken>& GetTokens() const
	{
		return tokens;
	}

private:
	static bool IsDirectiveOperator(const PreToken& token)
	{
		return token.GetType() == PreTokenType::Operator && token.GetValue() == "#";
	}
	static bool CanSkip(const PreToken& token)
	{
		return token.GetType() == PreTokenType::Whitespace || token.GetType() == PreTokenType::Comment;
	}

	std::size_t FindDirectiveOperator() const
	{
		for (auto index = 0u; index < tokens.size(); ++index)
			if (IsDirectiveOperator(tokens[index]))
				return index;
			else if (!CanSkip(tokens[index]))
				break;
		throw std::runtime_error("Line is not a preprocessor directive (missing #).");
	}

	std::size_t FindDirectiveIdentifier(std::size_t operatorIndex) const
	{
		for (auto index = operatorIndex + 1; index < tokens.size(); ++index)
			if (tokens[index].GetType() == PreTokenType::Identifier)
				return index;
			else if (!CanSkip(tokens[index]))
				throw std::runtime_error("Line is not a preprocessor directive (missing command after #).");
		return -1;
	}

private:
	std::vector<PreToken> tokens;
};
