#pragma once
#include <cstddef>
#include <string>
#include <sstream>

class ItemXPropogate
{
public:
	ItemXPropogate() = delete;
	ItemXPropogate(std::size_t state, std::size_t subItem)
		: state(state), subItem(subItem)
	{
	}
	ItemXPropogate(const ItemXPropogate& rhs) = default;
	~ItemXPropogate() = default;

	ItemXPropogate& operator=(const ItemXPropogate& rhs) = default;

	std::string ToString() const
	{
		std::ostringstream out;
		out << state << "(" << subItem << ")";
		return out.str();
	}

	bool operator==(const ItemXPropogate& rhs) const
	{
		return state == rhs.state && subItem == rhs.subItem;
	}
	bool operator!=(const ItemXPropogate& rhs) const
	{
		return !operator==(rhs);
	}

public:
	std::size_t state;
	std::size_t subItem;
};
