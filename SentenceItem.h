#pragma once
#include "Position.h"
#include <memory>
#include <deque>

class SentenceItem;
using SentenceItemPtr = std::shared_ptr<SentenceItem>;

class SentenceItem
{
public:
	virtual ~SentenceItem()
	{
	}

	virtual void WriteXml(std::ostream& out) const = 0;
	virtual Position GetPosition() const = 0;
	virtual bool CanBeReplacedWithChildren(const std::string& parentName) const
	{
		return false;
	}
	virtual std::deque<SentenceItemPtr> GetChildren() const
	{
		return{};
	}
	virtual void Simplify()
	{
	}
};
