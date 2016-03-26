#pragma once
#include "SentenceParser.h"

//<declare-alias> =
//	<id.alias> val;
class VbDeclareAlias
{
public:
	VbDeclareAlias(const Sentence& sentence)
	{
		std::tie(std::ignore, alias) = SentenceParser::Parse(
			sentence,
			RequiredToken("alias"),
			RequiredToken());
	}

	Token alias;
};
