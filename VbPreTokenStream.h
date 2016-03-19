#pragma once
#include "PreTokenStream.h"
#include "VbPreTokenFactory.h"
#include <cctype>
#include <stdexcept>

class VbPreTokenStream : public PreTokenStream
{
public:
	VbPreTokenStream(CharStream& stream)
		: PreTokenStream{ stream }
	{
	}

	bool IncludeNewline() const final
	{
		return true;
	}

protected:
	PreToken InternalGetNextToken() final
	{
		enum class State
		{
			Initial,
			Whitespace,
			Comment,
			IdentifierR,
			IdentifierRE,
			IdentifierREM,
			Identifier,
			DateString,
			String,
			StringClose,
			OperatorAmpersand,
			OperatorLessThan,
			OperatorGreaterThan,
			OperatorColon,
			NumberHex,
			NumberOctal,
			Number,
			NumberDecimal,
			Guid
		};
		State state = State::Initial;
		Position pos{ "", 0, 0 };
		std::string value;
		while (!IsEndOfFile())
		{
			Char ch = stream.GetNext();
			switch (state)
			{
			case State::Initial:
				pos = ch.GetPosition();
				value += ch.GetValue();
				if (ch.GetValue() == '\n')
					return VbPreTokenFactory::Create(PreTokenType::Newline, pos, value);
				else if (std::isspace(ch.GetValue()))
					state = State::Whitespace;
				else if (ch.GetValue() == '\'')
					state = State::Comment;
				else if (ch.GetValue() == 'r' || ch.GetValue() == 'R')
					state = State::IdentifierR;
				else if (std::isalpha(ch.GetValue()) || ch.GetValue() == '_')
					state = State::Identifier;
				else if (ch.GetValue() == '"')
					state = State::String;
				else if (ch.GetValue() == '&')
					state = State::OperatorAmpersand;
				else if (ch.GetValue() == '#')
				{
					//Backtracking required to determine if # is beginnig of date
					//string or just an operator.  Stupid language.
					Char next = stream.ScanPast("0123456789/: AMP");
					if (next != Char::EndOfFile() && next.GetValue() == '#')
						state = State::DateString;
					else
						return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);
				}
				else if (std::isdigit(ch.GetValue()))
					state = State::Number;
				else if (ch.GetValue() == ':')
					state = State::OperatorColon;
				else if (std::string{ "$^*()[],./\\-+=;!" }.find(ch.GetValue()) != std::string::npos)
					return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);
				else if (ch.GetValue() == '<')
					state = State::OperatorLessThan;
				else if (ch.GetValue() == '>')
					state = State::OperatorGreaterThan;
				else if (ch.GetValue() == '{')
					state = State::Guid;
				else
					return VbPreTokenFactory::Create(PreTokenType::OtherChar, pos, value);
				break;

			case State::Whitespace:
				if (ch.GetValue() == '\n' || !std::isspace(ch.GetValue()))
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Whitespace, pos, value);
				}
				value += ch.GetValue();
				break;

			case State::Comment:
				if (ch.GetValue() == '\n')
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Comment, pos, value);
				}
				value += ch.GetValue();
				break;

			case State::IdentifierR:
				if (ch.GetValue() == 'e' || ch.GetValue() == 'E')
				{
					value += ch.GetValue();
					state = State::IdentifierRE;
				}
				else
				{
					stream.PutBack(ch);
					state = State::Identifier;
				}
				break;

			case State::IdentifierRE:
				if (ch.GetValue() == 'm' || ch.GetValue() == 'M')
				{
					value += ch.GetValue();
					state = State::IdentifierREM;
				}
				else
				{
					stream.PutBack(ch);
					state = State::Identifier;
				}
				break;

			case State::IdentifierREM:
				if (ch.GetValue() == '\n')
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Comment, pos, value);
				}
				else if (std::isspace(ch.GetValue()))
				{
					value += ch.GetValue();
					state = State::Comment;
				}
				else
				{
					stream.PutBack(ch);
					state = State::Identifier;
				}
				break;

			case State::Identifier:
				if (ch.GetValue() == '_' || std::isalnum(ch.GetValue()))
					value += ch.GetValue();
				else if (std::string{ "%&#@$" }.find(ch.GetValue()) != std::string::npos)
					return VbPreTokenFactory::Create(PreTokenType::Identifier, pos, value + ch.GetValue());
				else
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Identifier, pos, value);
				}
				break;

			case State::DateString:
				if (ch.GetValue() == '\n')
				{
					stream.PutBack(ch);
					//Warning: Date string not closed before end of line...
					return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value);
				}
				else if (ch.GetValue() == '#')
					return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value + ch.GetValue());
				value += ch.GetValue();
				break;

			case State::String:
				if (ch.GetValue() == '\n')
				{
					stream.PutBack(ch);
					//Warning: String not closed before end of line...
					return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value);
				}
				else if (ch.GetValue() == '"')
					state = State::StringClose;
				value += ch.GetValue();
				break;

			case State::StringClose:
				if (ch.GetValue() == '"')
				{
					value += ch.GetValue();
					state = State::String;
				}
				else
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value);
				}
				break;

			case State::OperatorAmpersand:
				if (ch.GetValue() == 'h' || ch.GetValue() == 'H')
				{
					value += ch.GetValue();
					state = State::NumberHex;
				}
				else if (ch.GetValue() == 'o' || ch.GetValue() == 'O')
				{
					value += ch.GetValue();
					state = State::NumberOctal;
				}
				else
				{
					stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);
				}
				break;

			case State::OperatorLessThan:
				if (ch.GetValue() == '>' || ch.GetValue() == '=')
					value += ch.GetValue();
				else
					stream.PutBack(ch);
				return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);

			case State::OperatorGreaterThan:
				if (ch.GetValue() == '=')
					value += ch.GetValue();
				else
					stream.PutBack(ch);
				return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);

			case State::OperatorColon:
				if (ch.GetValue() == '=')
					return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value + ch.GetValue());
				stream.PutBack(ch);
				return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);

			case State::NumberHex:
				if (std::string{ "0123456789abcdefABCDEF" }.find(ch.GetValue()) != std::string::npos)
					value += ch.GetValue();
				else
				{
					//Hex literals may be suffixed with & as well (don't need it in token)
					if (ch.GetValue() != '&')
						stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Number, pos, value);
				}
				break;

			case State::NumberOctal:
				if (std::string{ "01234567" }.find(ch.GetValue()) != std::string::npos)
					value += ch.GetValue();
				else
				{
					//Octal literals may be suffixed with & as well (don't need it in token)
					if (ch.GetValue() != '&')
						stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Number, pos, value);
				}
				break;

			case State::Number:
				if (std::isalnum(ch.GetValue()))
					value += ch.GetValue();
				else if (ch.GetValue() == '.')
				{
					value += ch.GetValue();
					state = State::NumberDecimal;
				}
				else
				{
					if (std::string{ "#!@&" }.find(ch.GetValue()) != std::string::npos)
						value += ch.GetValue();
					else
						stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Number, pos, value);
				}
				break;

			case State::NumberDecimal:
				if (std::isdigit(ch.GetValue()))
					value += ch.GetValue();
				else
				{
					if (std::string{ "#!@&" }.find(ch.GetValue()) != std::string::npos)
						value += ch.GetValue();
					else
						stream.PutBack(ch);
					return VbPreTokenFactory::Create(PreTokenType::Number, pos, value);
				}
				break;

			case State::Guid:
				value += ch.GetValue();
				if (ch.GetValue() == '}')
					return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value);
				break;
			}
		}

		switch (state)
		{
		case State::Whitespace:
			return VbPreTokenFactory::Create(PreTokenType::Whitespace, pos, value);
		case State::Comment:
		case State::IdentifierREM:
			return VbPreTokenFactory::Create(PreTokenType::Comment, pos, value);
		case State::Identifier:
		case State::IdentifierR:
		case State::IdentifierRE:
			return VbPreTokenFactory::Create(PreTokenType::Identifier, pos, value);
		case State::DateString:
		case State::String:
		case State::Guid:
		//Warning: DateString or String not closed before end of file...
		case State::StringClose:
			return VbPreTokenFactory::Create(PreTokenType::StringLiteral, pos, value);
		case State::OperatorAmpersand:
		case State::OperatorLessThan:
		case State::OperatorGreaterThan:
		case State::OperatorColon:
			return VbPreTokenFactory::Create(PreTokenType::Operator, pos, value);
		case State::NumberHex:
		case State::NumberOctal:
		case State::Number:
		case State::NumberDecimal:
			return VbPreTokenFactory::Create(PreTokenType::Number, pos, value);
		case State::Initial:
		default:
			throw std::runtime_error("Invalid state after getting next token (reached end of file).");
		}
	}
};
