#pragma once
#include <stdexcept>

enum class TokenType
{
	EndOfFile,
	Keyword,
	Identifier,
	Operator,
	Constant
};

inline const char* ToString(TokenType value)
{
	switch (value)
	{
	case TokenType::EndOfFile: return "EndOfFile";
	case TokenType::Keyword: return "Keyword";
	case TokenType::Identifier: return "Identifier";
	case TokenType::Operator: return "Operator";
	case TokenType::Constant: return "Constant";
	}
	throw std::runtime_error("Invalid token type.");
}
