#pragma once
#include "SentenceParser.h"

//<translation-header> =
//	<class-header>
//	| <module-header>
//	| <form-header>;
class VbTranslationHeader
{
public:
	VbTranslationHeader(const Sentence& sentence)
		: VbTranslationHeader(SentenceParser::Parse(sentence,
			OptionalSentence("class-header"),
			OptionalSentence("module-header"),
			OptionalSentence("form-header")))
	{
	}

private:
	template <typename Tuple>
	VbTranslationHeader(const Tuple& result)
		: classHeader(std::get<0>(result)),
		moduleHeader(std::get<1>(result)),
		formHeader(std::get<2>(result))
	{
	}

public:
	optional<Sentence> classHeader;
	optional<Sentence> moduleHeader;
	optional<Sentence> formHeader;
};
