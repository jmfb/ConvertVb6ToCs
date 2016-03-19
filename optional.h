#pragma once
#include <stdexcept>

template <typename Type>
class optional
{
public:
	optional() = default;
	optional(const Type& value)
		: valid(true), value(value)
	{
	}
	optional(const optional<Type>& rhs) = default;
	~optional() = default;

	optional<Type>& operator=(const optional<Type>& rhs) = default;
	optional<Type>& operator=(const Type& rhs)
	{
		valid = true;
		value = rhs;
		return *this;
	}

	operator bool() const
	{
		return valid;
	}
	const Type& operator*() const
	{
		if (!valid)
			throw std::runtime_error("optional not valid");
		return value;
	}
	Type& operator*()
	{
		if (!valid)
			throw std::runtime_error("optional not valid");
		return value;
	}
	const Type* operator->() const
	{
		return &operator*();
	}
	Type* operator->()
	{
		return &operator*();
	}

private:
	bool valid = false;
	Type value;
};
