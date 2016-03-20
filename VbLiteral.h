#pragma once
#include "SentenceParser.h"

//<literal> =
//	val
//	| "true"
//	| "false"
//	| "empty"
//	| "nothing"
//	| "null";
class VbLiteral
{
public:
	VbLiteral(const Sentence& sentence)
		: VbLiteral(SentenceParser::Parse(sentence,
			RequiredToken()))
	{
	}

private:
	template <typename Tuple>
	VbLiteral(const Tuple& result)
		: value(std::get<0>(result))
	{
	}

public:
	Token value;
};
