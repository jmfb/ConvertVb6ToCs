#pragma once
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
