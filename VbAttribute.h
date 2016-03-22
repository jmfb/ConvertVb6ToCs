#pragma once
#include "SentenceParser.h"

//<attribute> =
//	<id.attribute> <qualified-id> '=' <expression> '\n';
class VbAttribute
{
public:
	VbAttribute(const Sentence& sentence)
		: VbAttribute(SentenceParser::Parse(sentence,
			RequiredToken("attribute"),
			RequiredSentence("qualified-id"),
			RequiredToken("="),
			RequiredSentence("expression"),
			RequiredToken("\\n")))
	{
	}

private:
	template <typename Tuple>
	VbAttribute(const Tuple& result)
		: name(std::get<1>(result)),
		value(std::get<3>(result))
	{
	}

public:
	Sentence name;
	Sentence value;
};
