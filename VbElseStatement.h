#pragma once
#include "SentenceParser.h"

//<else-statement> =
//	"else" <compound-statement-opt>;
class VbElseStatement
{
public:
	VbElseStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, compoundStatement) = SentenceParser::Parse(
			sentence,
			RequiredToken("else"),
			OptionalSentence("compound-statement"));
	}

	optional<Sentence> compoundStatement;
};
