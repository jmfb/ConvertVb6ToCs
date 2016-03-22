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
	{
		std::tie(classHeader, moduleHeader, formHeader) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("class-header"),
				OptionalSentence("module-header"),
				OptionalSentence("form-header"));
	}

	optional<Sentence> classHeader;
	optional<Sentence> moduleHeader;
	optional<Sentence> formHeader;
};
