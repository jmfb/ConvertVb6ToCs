#pragma once
#include "SentenceParser.h"

//<end-statement> =
//	"end" <end-keyword-opt>;
class VbEndStatement
{
public:
	VbEndStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, keyword) = SentenceParser::Parse(
			sentence,
			RequiredToken("end"),
			OptionalSentence("end-keyword"));
	}

	optional<Sentence> keyword;
};
