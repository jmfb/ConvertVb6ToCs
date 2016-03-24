#pragma once
#include "Sentence.h"
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <initializer_list>
#include <set>

class RequiredSentenceAccessor
{
public:
	RequiredSentenceAccessor(const std::initializer_list<std::string>& values)
		: values(values)
	{
	}

	Sentence operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index out of sentence.");
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (!values.empty() && values.find(child.GetName()) == values.end())
			throw std::runtime_error("Unexpected sentence: " + child.GetName());
		return child;
	}

private:
	std::set<std::string> values;
};

template <typename... Strings>
inline RequiredSentenceAccessor RequiredSentence(Strings... values)
{
	return{ values... };
}
