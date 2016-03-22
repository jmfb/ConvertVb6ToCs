#pragma once
#include "SentenceParser.h"

//<vb-line> =
//	<statement>
//	| <statement> ':' <compound-statement-opt>
//	| ':' <compound-statement-opt>;
class VbLine
{
public:
	VbLine(const Sentence& sentence)
	{
		std::tie(statement, std::ignore, compoundStatement) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("statement"),
				OptionalToken(":"),
				OptionalSentence("compound-statement"));
	}

	optional<Sentence> statement;
	optional<Sentence> compoundStatement;
};
