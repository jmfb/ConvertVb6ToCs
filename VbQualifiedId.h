#pragma once
#include "SentenceParser.h"
#include "VbCodeDotType.h"
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
	{
		optional<Sentence> wsDotSentence;
		optional<Sentence> qualifiedSuffix;
		std::tie(wsDotSentence, id, qualifiedSuffix) =
			SentenceParser::Parse(
				sentence,
				OptionalSentence("ws-dot"),
				RequiredToken(),
				OptionalSentence("qualified-suffix"));
		if (wsDotSentence)
			wsDot = ToDotType(*wsDotSentence);
		if (!qualifiedSuffix)
			return;
		for (auto index = 0u; index < qualifiedSuffix->GetNodes().size(); index += 2)
		{
			auto dot = ToDotType(qualifiedSuffix->GetNodes()[index]->AsSentence());
			if ((index + 1) >= qualifiedSuffix->GetNodes().size())
				throw std::runtime_error("Qualified suffix must be in pairs.");
			auto part = qualifiedSuffix->GetNodes()[index + 1]->AsToken();
			suffix.emplace_back(dot, part);
		}
	}

	std::string ToSimpleName() const
	{
		if (wsDot)
			throw std::runtime_error("Simple name may not be within With block.");
		std::ostringstream out;
		out << id.GetValue();
		for (auto& part : suffix)
		{
			if (part.first != VbCodeDotType::Dot)
				throw std::runtime_error("Bang separator not allowed in simple names.");
			out << "." << part.second.GetValue();
		}
		return out.str();
	}

	optional<VbCodeDotType> wsDot;
	Token id;
	std::vector<std::pair<VbCodeDotType, Token>> suffix;
};
