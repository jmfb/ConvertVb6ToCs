#pragma once
#include "SentenceParser.h"

//<translation-unit> =
//	<translation-header-opt>
//	<declaration-list-opt>;
//<declaration-list> =
//	<declaration> <declaration-list'>;
class VbTranslationUnit
{
public:
	VbTranslationUnit(const Sentence& sentence)
	{
		std::tie(translationHeader, declarationList) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("translation-header"),
				OptionalSentenceList("declaration"));
	}

	optional<Sentence> translationHeader;
	optional<std::vector<Sentence>> declarationList;
};
