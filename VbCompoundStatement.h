#pragma once
#include "SentenceParser.h"

//<compound-statement> =
//	<statement> <compound-statement'>
//	| ':' <compound-statement'>;
//<compound-statement'> =
//	':' <statement> <compound-statement'>
//	| ':'
//	| @;
class VbCompoundStatement
{
public:
	VbCompoundStatement(const Sentence& sentence)
	{
		for (auto& node : sentence.GetNodes())
		{
			if (!node->IsSentence())
			{
				auto& token = node->AsToken();
				if (token != ":")
					throw std::runtime_error("Expected ':'.");
			}
			else
			{
				auto& statement = node->AsSentence();
				if (statement.GetName() != "statement")
					throw std::runtime_error("Expected statement.");
				statements.push_back(statement);
			}
		}
	}

public:
	std::vector<Sentence> statements;
};
