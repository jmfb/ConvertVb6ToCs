#pragma once
#include "SentenceParser.h"

//<attribute> =
//	<id.attribute> <qualified-id> '=' <expression> '\n';
class VbAttribute
{
public:
	VbAttribute(const Sentence& sentence)
	{
		std::tie(std::ignore, name, std::ignore, value, std::ignore) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("attribute"),
				RequiredSentence("qualified-id"),
				RequiredToken("="),
				RequiredSentence("expression"),
				RequiredToken("\\n"));
	}

	Sentence name;
	Sentence value;
};
