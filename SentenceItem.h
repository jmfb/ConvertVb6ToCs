#pragma once
#include "Position.h"
#include "Token.h"
#include <memory>
#include <deque>

class SentenceItem;
using SentenceItemPtr = std::shared_ptr<SentenceItem>;

class Sentence;

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
	virtual bool IsSentence() const
	{
		return false;
	}
	virtual const Sentence& AsSentence() const
	{
		throw std::runtime_error("Not a sentence.");
	}
	virtual bool IsToken() const
	{
		return false;
	}
	virtual const Token& AsToken() const
	{
		throw std::runtime_error("Not a token.");
	}
};
