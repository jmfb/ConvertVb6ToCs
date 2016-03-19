#pragma once
#include "PreToken.h"
#include "Token.h"
#include "String.h"
#include <set>
#include <string>
#include <stdexcept>

class VbTokenFactory
{
public:
	static Token Create(const PreToken& token)
	{
		switch (token.GetType())
		{
		case PreTokenType::Identifier:
			return{ token.GetPosition(), IsKeyword(token.GetValue()) ? TokenType::Keyword : TokenType::Identifier, token.GetValue(), false };

		case PreTokenType::Operator:
			if (IsOperator(token.GetValue()))
				return{ token.GetPosition(), TokenType::Operator, token.GetValue(), true };
			throw std::runtime_error("Invalid operator token");

		case PreTokenType::Number:
		case PreTokenType::StringLiteral:
		case PreTokenType::CharacterLiteral:
			return{ token.GetPosition(), TokenType::Constant, token.GetValue(), true };

		case PreTokenType::Newline:
			return{ token.GetPosition(), TokenType::Operator, R"(\n)", true };
		}
		throw std::runtime_error("Invalid token");
	}

private:
	static bool IsKeyword(const std::string& value)
	{
		static const std::set<std::string> keywords
		{
			"addressof", "and", "as",
			"boolean", "byref",
			"byte", "byval", "call", "case",
			"const", "currency",
			"declare", "defbool", "defbyte", "defcur",
			"defdate", "defdbl", "defint", "deflng",
			"defobj", "defsng", "defstr", "defvar",
			"dim", "do", "double",
			"each", "else", "elseif", "empty",
			"end", "enum", "eqv", "erase",
			"error",
			"event", "exit", "false",
			"for", "friend", "function",
			"get", "gosub", "if",
			"imp", "implements", "in",
			"integer", "is", "len",
			"let", "like",
			"line input",
			"long", "loop",
			"lset", "me",
			"mod", "new",
			"not", "nothing", "null",
			"on", "option",
			"optional", "or", "paramarray",
			"preserve", "private",
			"public", "put", "raiseevent",
			"redim",
			"resume", "return", "rset",
			"seek",
			"set", "shared",
			"single", "static", "stop",
			"string", "sub", "then",
			"to", "true", "typeof",
			"unlock", "until", "variant",
			"wend", "while", "with", "withevents",
			"write", "xor"
		};
		return keywords.find(String::ToLower(value)) != keywords.end();
	}

	static bool IsOperator(const std::string& value)
	{
		static const std::set<std::string> operators
		{
			"[", "]", "(", ")", ":", ".", ",", "#",
			"+", "-", "*", "/", "\\", "^", "&", "$", "!",
			"=", "<", ">", "<>", "<=", ">=", ";", ":=",
			" .", " !", " ("
		};
		return operators.find(value) != operators.end();
	}
};
