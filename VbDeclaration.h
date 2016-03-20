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
		: VbDeclaration(SentenceParser::Parse(sentence,
			OptionalSentence("attribute"),
			OptionalSentence("line-label"),
			OptionalSentence("vb-line"),
			OptionalToken("\\n")))
	{
	}

private:
	template <typename Tuple>
	VbDeclaration(const Tuple& result)
		: attribute(std::get<0>(result)),
		lineLabel(std::get<1>(result)),
		vbLine(std::get<2>(result))
	{
	}

public:
	optional<Sentence> attribute;
	optional<Sentence> lineLabel;
	optional<Sentence> vbLine;
};
