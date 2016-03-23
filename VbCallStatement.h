#pragma once
#include "SentenceParser.h"

//<call-statement> =
//	"call" "me" '.' <call-suffix>
//	| "call" <ws-dot> <call-suffix>
//	| "call" <call-suffix>
//	| <l-value> <expression-list-opt>;
class VbCallStatement
{
public:
	VbCallStatement(const Sentence& sentence)
	{
		std::tie(std::ignore, isMe, std::ignore, wsDot, callSuffix, lValue, expressionList) =
			SentenceParser::Parse(
				sentence,
				OptionalToken("call"),
				OptionalToken("me"),
				OptionalToken("."),
				OptionalSentence("ws-dot"),
				OptionalSentence("call-suffix"),
				OptionalSentence("l-value"),
				OptionalSentence("expression-list"));
	}

	bool isMe;
	optional<Sentence> wsDot;
	optional<Sentence> callSuffix;
	optional<Sentence> lValue;
	optional<Sentence> expressionList;
};
