#pragma once
#include <stdexcept>

enum class VbDotType
{
	Dot,
	Bang
};

inline const char* ToString(VbDotType value)
{
	switch (value)
	{
	case VbDotType::Dot: return "Dot";
	case VbDotType::Bang: return "Bang";
	}
	throw std::runtime_error("Invalid VbDotType.");
}
