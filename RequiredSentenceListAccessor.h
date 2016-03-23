#pragma once
#include "Sentence.h"
#include <string>
#include <vector>
#include <stdexcept>

class RequiredSentenceListAccessor
{
public:
	RequiredSentenceListAccessor(
		const std::string& listName,
		const std::string& itemName,
		const std::string& separator)
		: listName(listName), itemName(itemName), separator(separator)
	{
	}

	std::vector<Sentence> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index out of sentence.");
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (child.GetName() != listName)
			throw std::runtime_error("Expected: " + listName);
		std::vector<Sentence> list;
		for (auto& node : child.GetNodes())
		{
			if (node->IsSentence())
			{
				auto& item = node->AsSentence();
				if (item.GetName() != itemName)
					throw std::runtime_error("Expected: " + itemName);
				list.push_back(item);
			}
			else
			{
				auto& token = node->AsToken();
				if (separator.empty() || token != separator)
					throw std::runtime_error("Invalid sentence list separator.");
			}
		}
		return list;
	}

private:
	std::string listName;
	std::string itemName;
	std::string separator;
};

inline RequiredSentenceListAccessor RequiredSentenceList(const std::string& name)
{
	return{ name + "-list", name, "" };
}

inline RequiredSentenceListAccessor RequiredSentenceList(
	const std::string& listName,
	const std::string& itemName,
	const std::string& separator = "")
{
	return{ listName, itemName, separator };
}
