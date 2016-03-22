#pragma once
#include "SentenceParser.h"

//<declaration> =
//	<attribute>
//	| <line-label> <vb-line-opt> '\n'
//	| <vb-line> '\n'
//	| '\n';
class VbDeclaration
{
public:
	VbDeclaration(const Sentence& sentence)
	{
		std::tie(attribute, lineLabel, vbLine, std::ignore) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("attribute"),
				OptionalSentence("line-label"),
				OptionalSentence("vb-line"),
				OptionalToken("\\n"));
	}

	optional<Sentence> attribute;
	optional<Sentence> lineLabel;
	optional<Sentence> vbLine;
};
