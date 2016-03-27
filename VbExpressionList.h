#pragma once
#include "SentenceParser.h"

//<expression-list> =
//	<pass-by-opt> id ':=' <expression> <expression-list'>
//	| <pass-by-opt> <expression> <expression-list'>
//	| <expression-list''> <expression-list'>;
//<expression-list'> =
//	<expression-list''> <expression-list'>
//	| @;
//<expression-list''> =
//	<expression-separator>
//	| <expression-separator> <pass-by-opt> id ':=' <expression>
//	| <expression-separator> <pass-by-opt> <expression>;
//<expression-separator> =
//	','
//	| ';';  #Tabbed column separator for Print statements
class VbExpressionList
{
public:
	VbExpressionList(const Sentence& sentence)
	{
		expressionItems.emplace_back("expression-item");
		for (auto index = 0u; index < sentence.GetNodes().size(); ++index)
		{
			auto nextSeparator = AsSeparator(sentence, index);
			if (!nextSeparator)
			{
				expressionItems.back().AddNode(sentence.GetNodes()[index]);
				continue;
			}
			if (!separator)
				separator = nextSeparator;
			else if (*separator != *nextSeparator)
				throw std::runtime_error("Cannot mix expression separators in an expression list.");
			expressionItems.emplace_back("expression-item");
		}
	}

	static optional<Token> AsSeparator(const Sentence& sentence, std::size_t index)
	{
		if (!sentence.GetNodes()[index]->IsSentence())
			return{};
		auto& separatorSentence = sentence.GetNodes()[index]->AsSentence();
		if (separatorSentence.GetName() != "expression-separator")
			return{};
		return separatorSentence.GetNodes()[0]->AsToken();
	}

	optional<Token> separator;
	std::vector<Sentence> expressionItems;
};
