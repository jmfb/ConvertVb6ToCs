#pragma once
#include "SentenceParser.h"

//<module-header> =
//	<attribute-list>;
//<attribute-list> =
//	<attribute> <attribute-list'>;
//<attribute-list'> =
//	<attribute> <attribute-list'>
//	| @;
class VbModuleHeader
{
public:
	VbModuleHeader(const Sentence& sentence)
		: VbModuleHeader(SentenceParser::Parse(sentence,
			RequiredSentenceList("attribute")))
	{
	}

private:
	template <typename Tuple>
	VbModuleHeader(const Tuple& result)
		: attributes(std::get<0>(result))
	{
	}

public:
	std::vector<Sentence> attributes;
};
