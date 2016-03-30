#pragma once
#include "SentenceParser.h"

//<relational-op> =
//	'>=' | '<=' | '>' | '<' | '<>' | '=';
class VbRelationalOp
{
public:
	VbRelationalOp(const Sentence& sentence)
	{
		std::tie(op) = SentenceParser::Parse(
			sentence,
			RequiredToken(">=", "<=", ">", "<", "<>", "="));
	}

	Token op;
};
