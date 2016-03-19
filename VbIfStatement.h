#pragma once
#include "SentenceParser.h"

//<if-statement> =
//	"if" <expression> "then" <compound-statement> <else-statement-opt>
//	| "if" <expression> "then";
class VbIfStatement
{
public:
	VbIfStatement(const Sentence& sentence)
		: VbIfStatement(SentenceParser::Parse(sentence,
			RequiredToken("if"),
			RequiredSentence("expression"),
			RequiredToken("then"),
			OptionalSentence("compound-statement"),
			OptionalSentence("else-statement")))
	{
	}

private:
	template <typename Tuple>
	VbIfStatement(const Tuple& result)
		: expression(std::get<1>(result)),
		compoundStatement(std::get<3>(result)),
		elseStatement(std::get<4>(result))
	{
	}

public:
	Sentence expression;
	optional<Sentence> compoundStatement;
	optional<Sentence> elseStatement;
};
