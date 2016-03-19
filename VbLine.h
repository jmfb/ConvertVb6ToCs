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
		: VbLine(SentenceParser::Parse(sentence,
			OptionalSentence("statement"),
			OptionalToken(":"),
			OptionalSentence("compound-statement")))
	{
	}

private:
	template <typename Tuple>
	VbLine(const Tuple& result)
		: statement(std::get<0>(result)),
		compoundStatement(std::get<2>(result))
	{
	}

public:
	optional<Sentence> statement;
	optional<Sentence> compoundStatement;
};
