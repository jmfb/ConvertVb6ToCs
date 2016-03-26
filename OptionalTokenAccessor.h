#pragma once
#include "optional.h"
#include "Token.h"
#include "Sentence.h"
#include <initializer_list>
#include <string>
#include <cstdlib>
#include <set>

class OptionalTokenAccessor
{
public:
	OptionalTokenAccessor(const std::initializer_list<std::string>& values)
		: values(values)
	{
	}

	optional<Token> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size() || !sentence.GetNodes()[index]->IsToken())
			return{};
		auto& token = sentence.GetNodes()[index]->AsToken();
		if (values.empty())
			return token;
		for (auto& value : values)
			if (token == value)
				return token;
		return{};
	}

private:
	std::set<std::string> values;
};

template <typename... String>
inline OptionalTokenAccessor OptionalToken(String... values)
{
	return{ values... };
}
