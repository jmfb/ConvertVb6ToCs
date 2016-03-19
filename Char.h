#pragma once
#include "Position.h"

class Char
{
public:
	Char(char value, Position position)
		: value(value), position(position)
	{
	}
	Char(const Char& rhs) = default;
	~Char() = default;

	Char& operator=(const Char& rhs) = default;

	char GetValue() const
	{
		return value;
	}
	const Position& GetPosition() const
	{
		return position;
	}

	static Char EndOfFile()
	{
		return{ -1, { "", 0, 0 } };
	}

	bool operator==(const Char& rhs) const
	{
		return value == rhs.value;
	}
	bool operator!=(const Char& rhs) const
	{
		return !operator==(rhs);
	}

private:
	char value;
	Position position;
};
