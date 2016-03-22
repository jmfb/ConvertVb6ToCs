#pragma once
#include "SentenceParser.h"

//<if-statement> =
//	"if" <expression> "then" <compound-statement> <else-statement-opt>
//	| "if" <expression> "then";
class VbIfStatement
{
public:
	VbIfStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, expression, std::ignore, compoundStatement, elseStatement) =
			SentenceParser::Parse(
				sentence,
				RequiredToken("if"),
				RequiredSentence("expression"),
				RequiredToken("then"),
				OptionalSentence("compound-statement"),
				OptionalSentence("else-statement"));
	}

	Sentence expression;
	optional<Sentence> compoundStatement;
	optional<Sentence> elseStatement;
};
