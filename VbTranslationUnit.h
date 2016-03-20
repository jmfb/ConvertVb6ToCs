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
		: VbTranslationUnit(SentenceParser::Parse(sentence,
			OptionalSentence("translation-header"),
			OptionalSentenceList("declaration")))
	{
	}

private:
	template <typename Tuple>
	VbTranslationUnit(const Tuple& result)
		: translationHeader(std::get<0>(result)),
		declarationList(std::get<1>(result))
	{
	}

public:
	optional<Sentence> translationHeader;
	optional<std::vector<Sentence>> declarationList;
};
