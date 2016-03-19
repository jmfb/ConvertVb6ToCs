#pragma once
#include "TokenType.h"
#include "Position.h"
#include "String.h"
#include <string>
#include <sstream>

class Token
{
public:
	Token() = default;
	Token(const Position& position, TokenType type, const std::string& value, bool caseSensitive)
		: position(position), type(type), value(value), caseSensitive(caseSensitive)
	{
	}

	const Position& GetPosition() const
	{
		return position;
	}
	TokenType GetType() const
	{
		return type;
	}
	const std::string& GetValue() const
	{
		return value;
	}

	std::string ToString() const
	{
		std::ostringstream out;
		out << position.ToString() << ": <" << ::ToString(type) << "> " << value;
		return out.str();
	}

	bool operator==(const std::string& rhs) const
	{
		return String::Equals(value, rhs, caseSensitive);
	}
	bool operator!=(const std::string& rhs) const
	{
		return !operator==(rhs);
	}

	bool operator==(const Token& rhs) const
	{
		return type == rhs.type && String::Equals(value, rhs.value, caseSensitive);
	}
	bool operator!=(const Token& rhs) const
	{
		return !operator==(rhs);
	}

	static Token EndOfFile()
	{
		return{ { "", 0, 0 }, TokenType::EndOfFile, "", true };
	}

private:
	Position position;
	TokenType type = TokenType::EndOfFile;
	std::string value;
	bool caseSensitive = true;
};
