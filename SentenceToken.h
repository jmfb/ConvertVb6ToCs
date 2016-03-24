#pragma once
#include "SentenceItem.h"
#include "Token.h"
#include <regex>

class SentenceToken : public SentenceItem
{
public:
	SentenceToken(const Token& token)
		: token(token)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		auto value = token.GetValue();
		value = std::regex_replace(value, std::regex{ "&" }, "&amp;");
		value = std::regex_replace(value, std::regex{ "<" }, "&lt;");
		value = std::regex_replace(value, std::regex{ ">" }, "&gt;");
		out << "<token>" << value << "</token>";
	}

	Position GetPosition() const final
	{
		return token.GetPosition();
	}

	bool IsToken() const final
	{
		return true;
	}

	const Token& AsToken() const final
	{
		return token;
	}

	const Token& GetToken() const
	{
		return token;
	}

private:
	Token token;
};
