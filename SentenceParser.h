#pragma once
#include "Token.h"
#include "Sentence.h"
#include "optional.h"
#include <string>
#include <map>
#include <set>
#include <stdexcept>
#include <cstdlib>
#include <tuple>
#include <initializer_list>

class SentenceParser
{
public:
	template <typename Enum>
	static Enum ToEnum(const Token& token, const std::map<std::string, Enum>& map)
	{
		for (auto& entry : map)
			if (token == entry.first)
				return entry.second;
		throw std::runtime_error("Invalid enum value.");
	}

	template <typename Enum>
	static Enum ToEnum(const optional<Token>& token, const std::map<std::string, Enum>& map, Enum defaultValue)
	{
		return token ? ToEnum(*token, map) : defaultValue;
	}

	template <typename... Accessors>
	static auto Parse(const Sentence& sentence, Accessors... accessors)
		-> std::tuple<decltype(accessors(sentence, 0))...>
	{
		return ParseSentencePart(sentence, 0, accessors...);
	}

private:
	template <typename Part>
	static std::size_t GetSentencePartSize(const Part& part)
	{
		return 1;
	}

	template <typename Part>
	static std::size_t GetSentencePartSize(const optional<Part>& optionalPart)
	{
		return optionalPart ? 1 : 0;
	}

	static std::tuple<> ParseSentencePart(const Sentence& sentence, std::size_t index)
	{
		if (sentence.GetNodes().size() > index)
			throw std::runtime_error("Sentence had more parts.");
		return{};
	}

	template <typename Accessor, typename... Accessors>
	static auto ParseSentencePart(const Sentence& sentence, std::size_t index, Accessor accessor, Accessors... accessors)
		-> std::tuple<decltype(accessor(sentence, 0)), decltype(accessors(sentence, 0))...>
	{
		auto part = accessor(sentence, index);
		auto nextIndex = index + GetSentencePartSize(part);
		auto parts = ParseSentencePart(sentence, nextIndex, accessors...);
		return std::tuple_cat(std::make_tuple(part), parts);
	}
};

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

class RequiredTokenAccessor
{
public:
	RequiredTokenAccessor(const std::string& value)
		: value(value)
	{
	}

	Token operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size())
			throw std::runtime_error("Index outside of sentence.");
		auto& token = sentence.GetNodes()[index]->AsToken();
		if (!value.empty() && token != value)
			throw std::runtime_error("Expected " + value);
		return token;
	}

private:
	std::string value;
};

inline RequiredTokenAccessor RequiredToken(const std::string& value = "")
{
	return{ value };
}

class OptionalSentenceAccessor
{
public:
	OptionalSentenceAccessor(const std::string& name)
		: name(name)
	{
	}

	optional<Sentence> operator()(const Sentence& sentence, std::size_t index) const
	{
		if (index >= sentence.GetNodes().size() || !sentence.GetNodes()[index]->IsSentence())
			return{};
		auto& child = sentence.GetNodes()[index]->AsSentence();
		if (child.GetName() == name)
			return child;
		return{};
	}

private:
	std::string name;
};

inline OptionalSentenceAccessor OptionalSentence(const std::string& name)
{
	return{ name };
}
