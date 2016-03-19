#pragma once
#include "TransitionType.h"
#include "Production.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include <string>
#include <cstdlib>

class TransitionAction
{
public:
	TransitionAction()
		: type(TransitionType::Accept)
	{
	}
	TransitionAction(std::size_t state)
		: type(TransitionType::Shift), state(state)
	{
	}
	TransitionAction(const std::string& nonTerminal, const Production& production)
		: type(TransitionType::Reduce),
		nonTerminal(nonTerminal),
		production(&production),
		productionItemCount(production.GetItems().size())
	{
	}

	TransitionType GetType() const
	{
		return type;
	}
	std::size_t GetState() const
	{
		return state;
	}
	const std::string& GetNonTerminal() const
	{
		return nonTerminal;
	}
	const void* GetProduction() const
	{
		return production;
	}
	std::size_t GetProductionItemCount() const
	{
		return productionItemCount;
	}

	bool operator==(const TransitionAction& rhs) const
	{
		if (type != rhs.type)
			return false;
		switch (type)
		{
		case TransitionType::Shift:
			return state == rhs.state;
		case TransitionType::Reduce:
			return nonTerminal == rhs.nonTerminal && production == rhs.production;
		}
		return true;
	}
	bool operator!=(const TransitionAction& rhs) const
	{
		return !operator==(rhs);
	}

	void Write(BinaryWriter& writer) const
	{
		writer.Write(static_cast<std::size_t>(type));
		writer.Write(state);
		writer.Write(nonTerminal);
		writer.Write(productionItemCount);
	}

	void Read(BinaryReader& reader)
	{
		type = static_cast<TransitionType>(reader.ReadSize());
		state = reader.ReadSize();
		nonTerminal = reader.ReadString();
		productionItemCount = reader.ReadSize();
	}

private:
	TransitionType type;
	std::size_t state = 0;
	std::string nonTerminal;
	const void* production = nullptr;
	std::size_t productionItemCount = 0;
};
