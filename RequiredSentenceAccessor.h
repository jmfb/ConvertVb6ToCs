#pragma once
#include "Sentence.h"
#include <string>
#include <cstdlib>
#include <stdexcept>

class RequiredSentenceAccessor
{
public:
	RequiredSentenceAccessor(const std::string& name)
		: name(name)
	{
	}

	Sentence operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index out of sentence.");
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (name.empty() || child.GetName() == name)
			return child;
		throw std::runtime_error("Expected: " + name);
	}

private:
	std::string name;
};

inline RequiredSentenceAccessor RequiredSentence(const std::string& name = "")
{
	return{ name };
}
