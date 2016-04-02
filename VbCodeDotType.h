#pragma once
#include "Sentence.h"
#include "EnumParser.h"
#include <stdexcept>

enum class VbCodeDotType
{
	Dot,
	Bang
};

inline const char* ToString(VbCodeDotType value)
{
	switch (value)
	{
	case VbCodeDotType::Dot: return ".";
	case VbCodeDotType::Bang: return "!";
	}
	throw std::runtime_error("Invalid dot type.");
}

inline VbCodeDotType ToDotType(const Sentence& sentence)
{
	return EnumParser::Parse<VbCodeDotType>(
		sentence.GetNodes()[0]->AsToken(),
		{
			{ " !", VbCodeDotType::Bang },
			{ "!", VbCodeDotType::Bang },
			{ " .", VbCodeDotType::Dot },
			{ ".", VbCodeDotType::Dot }
		});
}
