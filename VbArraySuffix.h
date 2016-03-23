#pragma once
#include "SentenceParser.h"

//<array-suffix> =
//	'(' <subscript-list-opt> ')';
class VbArraySuffix
{
public:
	VbArraySuffix(const Sentence& sentence)
	{
		std::tie(std::ignore, subscripts, std::ignore) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("("),
				OptionalSentenceList("subscript-list", "subscript", ","),
				RequiredToken(")"));
	}

	optional<std::vector<Sentence>> subscripts;
};
