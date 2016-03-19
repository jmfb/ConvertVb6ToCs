#pragma once
#include "SentenceItem.h"
#include "Token.h"

class SentenceToken : public SentenceItem
{
public:
	SentenceToken(const Token& token)
		: token(token)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<token>" << token.GetValue() << "</token>";
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
