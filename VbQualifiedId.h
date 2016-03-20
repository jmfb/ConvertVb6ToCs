#pragma once
#include "SentenceParser.h"
#include "VbDotType.h"
#include <vector>
#include <utility>

//<qualified-id> =
//	id <qualified-suffix-opt>
//	| <ws-dot> id <qualified-suffix-opt>;
//<qualified-suffix> =
//	<dot> id <qualified-suffix'>;
//<qualified-suffix'> =
//	<dot> id <qualified-suffix'>
//	| @;
//<dot> =
//	'.'
//	| '!';  #Special ADO recordset operator "rs!name" -> "rs.Fields.Items("name").Value"
//<ws-dot> =
//	' .'
//	| ' !';
class VbQualifiedId
{
public:
	VbQualifiedId(const Sentence& sentence)
		: VbQualifiedId(SentenceParser::Parse(sentence,
			OptionalSentence("ws-dot"),
			RequiredToken(),
			OptionalSentence("qualified-suffix")))
	{
	}

private:
	template <typename Tuple>
	VbQualifiedId(const Tuple& result)
		: id(std::get<1>(result))
	{
		auto wsDotSentence = std::get<0>(result);
		if (wsDotSentence)
			wsDot = SentenceParser::ToEnum<VbDotType>(
				wsDotSentence->GetNodes()[0]->AsToken(),
				{
					{ " .", VbDotType::Dot },
					{ " !", VbDotType::Bang }
				});
		auto qualifiedSuffix = std::get<2>(result);
		if (!qualifiedSuffix)
			return;
		for (auto index = 0u; index < qualifiedSuffix->GetNodes().size(); index += 2)
		{
			auto dot = SentenceParser::ToEnum<VbDotType>(
				qualifiedSuffix->GetNodes()[index]->AsSentence().GetNodes()[0]->AsToken(),
				{
					{ ".", VbDotType::Dot },
					{ "!", VbDotType::Bang }
				});
			if ((index + 1) >= qualifiedSuffix->GetNodes().size())
				throw std::runtime_error("Qualified suffix must be in pairs.");
			auto part = qualifiedSuffix->GetNodes()[index + 1]->AsToken();
			suffix.emplace_back(dot, part);
		}
	}

public:
	optional<VbDotType> wsDot;
	Token id;
	std::vector<std::pair<VbDotType, Token>> suffix;
};
