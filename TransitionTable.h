#pragma once
#include "TransitionList.h"
#include "Sentence.h"
#include "SentenceToken.h"
#include "TokenStream.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include <vector>
#include <stack>
#include <stdexcept>

class TransitionTable
{
public:
	const std::vector<TransitionList>& GetTable() const
	{
		return table;
	}

	Sentence Parse(TokenStream& tokens)
	{
		std::stack<int> state;
		state.push(0);
		std::stack<SentenceItemPtr> stack;
		std::stack<Sentence*> sentences;
		auto token = tokens.GetNextToken();
		for (auto accepted = false; !accepted; )
		{
			auto action = table[state.top()].GetAction(token);
			switch (action.GetType())
			{
			case TransitionType::Accept:
				accepted = true;
				break;

			case TransitionType::Shift:
				stack.push(std::make_shared<SentenceToken>(token));
				sentences.push(nullptr);
				state.push(action.GetState());
				token = tokens.GetNextToken();
				break;

			case TransitionType::Reduce:
				auto sentence = std::make_shared<Sentence>(action.GetNonTerminal());
				for (auto index = 0u; index < action.GetProductionItemCount(); ++index)
				{
					sentence->AddNodeToFront(stack.top());
					stack.pop();
					sentences.pop();
					state.pop();
				}
				stack.push(sentence);
				sentences.push(sentence.get());
				state.push(table[state.top()].GetGoto(action.GetNonTerminal()));
				break;
			}
		}
		if (sentences.size() != 1)
			throw std::runtime_error("Stack should contain single sentence on acceptance.");
		if (sentences.top() == nullptr)
			throw std::runtime_error("Top of stack should be sentence.");
		sentences.top()->Simplify();
		return *sentences.top();
	}

	void Write(BinaryWriter& writer) const
	{
		writer.Write(table.size());
		for (auto& item : table)
			item.Write(writer);
	}

	void Read(BinaryReader& reader)
	{
		table.resize(reader.ReadSize());
		for (auto& item : table)
			item.Read(reader);
	}

public:
	std::vector<TransitionList> table;
};
