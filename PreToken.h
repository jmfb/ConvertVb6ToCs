#pragma once
#include "PreTokenType.h"
#include "Position.h"
#include <string>

class PreToken
{
public:
	PreToken(PreTokenType type, const Position& position, const std::string& value)
		: type(type), position(position), value(value)
	{
	}

	PreTokenType GetType() const
	{
		return type;
	}
	const Position& GetPosition() const
	{
		return position;
	}
	const std::string& GetValue() const
	{
		return value;
	}
	void SetValue(const std::string& value)
	{
		this->value = value;
	}

	bool operator==(const PreToken& rhs) const
	{
		return type == rhs.type && value == rhs.value;
	}
	bool operator!=(const PreToken& rhs) const
	{
		return !operator==(rhs);
	}

	static PreToken EndOfFile()
	{
		return{ PreTokenType::EndOfFile, { "", 0, 0 }, "" };
	}

protected:
	void ReplaceToken(PreTokenType type, const std::string& value)
	{
		this->type = type;
		this->value = value;
	}

private:
	PreTokenType type;
	Position position;
	std::string value;
};
