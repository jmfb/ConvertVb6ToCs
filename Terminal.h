#pragma once
#include "TokenType.h"
#include "Token.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include <string>
#include <sstream>

class Terminal
{
public:
	Terminal() = default;
	Terminal(TokenType type, const std::string& value)
		: type(type), value(value)
	{
	}

	TokenType GetType() const
	{
		return type;
	}
	const std::string& GetValue() const
	{
		return value;
	}

	bool operator==(const Token& token) const
	{
		if (type == TokenType::Keyword && value == "$")
			return token == Token::EndOfFile();
		return type == token.GetType() && (value.empty() || token == value);
	}
	bool operator!=(const Token& token) const
	{
		return !operator==(token);
	}

	bool operator==(const Terminal& rhs) const
	{
		return type == rhs.type && value == rhs.value;
	}
	bool operator!=(const Terminal& rhs) const
	{
		return !operator==(rhs);
	}
	bool operator<(const Terminal& rhs) const
	{
		return type < rhs.type || (type == rhs.type && value < rhs.value);
	}

	void Write(BinaryWriter& writer) const
	{
		writer.Write(static_cast<std::size_t>(type));
		writer.Write(value);
	}

	void Read(BinaryReader& reader)
	{
		type = static_cast<TokenType>(reader.ReadSize());
		value = reader.ReadString();
	}

	std::string ToString() const
	{
		std::ostringstream out;
		switch (type)
		{
		case TokenType::Identifier:
			if (value.empty())
				out << "id";
			else
				out << "<id.," << value << ">";
			break;

		case TokenType::Constant:
			if (value.empty())
				out << "val";
			else
				out << value;
			break;

		case TokenType::Keyword:
			if (value == "$" || value == "#" || value == "@")
				out << value;
			else
				out << "\"" << value << "\"";
			break;

		case TokenType::Operator:
			out << "'" << value << "'";
			break;
		}
		return out.str();
	}

	static Terminal Keyword(const std::string& value)
	{
		return{ TokenType::Keyword, value };
	}
	static Terminal Operator(const std::string& value)
	{
		return{ TokenType::Operator, value };
	}
	static Terminal Identifier(const std::string& value = "")
	{
		return{ TokenType::Identifier, value };
	}
	static Terminal Constant()
	{
		return{ TokenType::Constant, "" };
	}
	static Terminal EndOfFile()
	{
		return{ TokenType::Keyword, "$" };
	}
	static Terminal LookAhead()
	{
		return{ TokenType::Keyword, "#" };
	}

private:
	TokenType type = TokenType::EndOfFile;
	std::string value;
};
