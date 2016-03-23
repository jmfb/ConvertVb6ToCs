#pragma once
#include "SentenceParser.h"

//<l-value> =
//	id <l-value-suffix-opt>
//	| <ws-dot> id <l-value-suffix-opt>
//	| "me" <l-value-suffix-opt>;
//<l-value-suffix> =
//	<l-value-terminal> <l-value-suffix'>;
//<l-value-suffix'> =
//	<l-value-terminal> <l-value-suffix'>
//	| @;
class VbLValue
{
public:
	VbLValue(const Sentence& sentence)
	{
		std::tie(wsDot, id, suffix) = SentenceParser::Parse(
			sentence,
			OptionalSentence("ws-dot"),
			RequiredToken(),
			OptionalSentenceList("l-value-suffix", "l-value-terminal"));
	}

	optional<Sentence> wsDot;
	Token id;
	optional<std::vector<Sentence>> suffix;
};
