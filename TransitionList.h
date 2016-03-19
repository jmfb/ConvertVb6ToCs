#pragma once
#include "Terminal.h"
#include "TransitionAction.h"
#include "ConflictResolver.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <stdexcept>

class TransitionList
{
public:
	TransitionList() = default;
	TransitionList(std::size_t index)
		: index(index)
	{
	}

	void AddAction(const Terminal& terminal, const TransitionAction& action, ConflictResolver& resolver)
	{
		auto iter = actions.find(terminal);
		if (iter != actions.end())
		{
			if (iter->second != action)
			{
				switch (resolver.Resolve(terminal, actions[terminal], action))
				{
				case ConflictResolution::KeepNew:
					actions[terminal] = action;
					break;

				case ConflictResolution::KeepOld:
					//nothing, keeping old action and throwing away new action
					break;

				case ConflictResolution::None:
					throw std::runtime_error("Conflicting actions generated.");
				}
			}
		}
		else
			actions[terminal]  = action;
	}

	void AddGoto(const std::string& nonTerminal, std::size_t state)
	{
		auto iter = gotos.find(nonTerminal);
		if (iter == gotos.end())
			gotos[nonTerminal] = state;
		else if (iter->second != state)
			throw std::runtime_error("Conflicting gotos generated.");
	}

	TransitionAction GetAction(const Token& token)
	{
		std::vector<std::pair<Terminal, TransitionAction>> matchingActions;
		std::copy_if(actions.begin(), actions.end(), std::back_inserter(matchingActions), [&](auto& p){ return p.first == token; });
		if (matchingActions.empty())
			throw std::runtime_error("Error: missing action for terminal: " + token.ToString());
		if (matchingActions.size() == 1)
			return matchingActions.front().second;
		return ResolveMultipleTransitions(token, matchingActions);
	}

	std::size_t GetGoto(const std::string& nonTerminal)
	{
		auto iter = gotos.find(nonTerminal);
		if (iter == gotos.end())
			throw std::runtime_error("Error: missing goto for non-terminal.");
		return iter->second;
	}

	void Write(BinaryWriter& writer) const
	{
		writer.Write(index);
		writer.Write(actions.size());
		for (auto& item : actions)
		{
			item.first.Write(writer);
			item.second.Write(writer);
		}
		writer.Write(gotos.size());
		for (auto& item : gotos)
		{
			writer.Write(item.first);
			writer.Write(item.second);
		}
	}

	void Read(BinaryReader& reader)
	{
		index = reader.ReadSize();
		auto actionCount = reader.ReadSize();
		for (auto index = 0u; index < actionCount; ++index)
		{
			Terminal terminal;
			terminal.Read(reader);
			TransitionAction action;
			action.Read(reader);
			actions.insert({ terminal, action });
		}
		auto gotoCount = reader.ReadSize();
		for (auto index = 0u; index < gotoCount; ++index)
		{
			auto nonTerminal = reader.ReadString();
			auto state = reader.ReadSize();
			gotos.insert({ nonTerminal, state });
		}
	}

private:
	TransitionAction ResolveMultipleTransitions(const Token& token, const std::vector<std::pair<Terminal, TransitionAction>>& matchingActions)
	{
		//Multiple matching identifiers: id vs. id.value, choose id.value
		if (matchingActions.front().first.GetType() == TokenType::Identifier && matchingActions.size() == 2)
		{
			auto firstIsId = matchingActions.front().first.GetValue().empty();
			auto secondIsId = matchingActions.back().first.GetValue().empty();
			if (firstIsId != secondIsId)
				return firstIsId ?
					matchingActions.back().second :
					matchingActions.front().second;
		}
		throw std::runtime_error("Multiple actions found for token");
	}

private:
	std::size_t index = 0;
	std::map<Terminal, TransitionAction> actions;
	std::map<std::string, std::size_t> gotos;
};
