#pragma once
#include "Grammar.h"
#include "String.h"
#include <cctype>
#include <stdexcept>

class GrammarParser
{
public:
	Grammar Parse(const std::string& str)
	{
		enum class ParseState
		{
			Initial,
			LeftSide,
			Middle,
			RightSide,
			NonTerminal,
			Operator,
			Keyword,
			Identifier,
			Constant,
			Comment,
			FirstComment,
			BlockComment,
			BlockCommentEnd
		};

		std::string name;
		NonTerminal nonTerminal;
		Production production;
		auto productionValid = false;
		auto state = ParseState::Initial;
		auto return_state = ParseState::Initial;
		auto line_num = 1;
		auto column = 1;
		for (auto ch : str)
		{
			if (ch == '\n')
			{
				++line_num;
				column = 1;
			}
			else
				++column;

			switch (state)
			{
			case ParseState::Initial:
				if (std::isspace(ch))
					continue;
				else if (ch == '#')
				{
					return_state = state;
					state = ParseState::FirstComment;
				}
				else if (ch == '<')
					state = ParseState::LeftSide;
				else
					throw std::runtime_error("Invalid character at beginning of production, expected <");
				break;

			case ParseState::LeftSide:
				if (ch == '>')
				{
					if (name.empty())
						throw std::runtime_error("Cannot define empty non-terminal names.");
					nonTerminal = { name };
					name.clear();
					state = ParseState::Middle;
				}
				else
					name += ch;
				break;

			case ParseState::Middle:
				if (std::isspace(ch))
					continue;
				else if (ch == '#')
				{
					return_state = state;
					state = ParseState::FirstComment;
				}
				else if (ch == '=')
				{
					state = ParseState::RightSide;
					production = {};
					productionValid = true;
				}
				else
					throw std::runtime_error("Missing = after non-terminal name");
				break;

			case ParseState::RightSide:
				if (std::isspace(ch))
					continue;
				else if (ch == '#')
				{
					return_state = state;
					state = ParseState::FirstComment;
				}
				else if (ch == ';')
				{
					if (productionValid)
					{
						if (production.GetItems().empty())
							throw std::runtime_error("Cannot define production with no items.");
						nonTerminal.AddItem(production);
					}
					items[nonTerminal.GetName()] = nonTerminal;
					productionValid = false;
					state = ParseState::Initial;
				}
				else if (ch == '|')
				{
					if (productionValid)
					{
						if (production.GetItems().empty())
							throw std::runtime_error("Cannot define production with no items (before |)");
						nonTerminal.AddItem(production);
					}
					production = {};
					productionValid = true;
				}
				else if (ch == '@')
				{
					if (!production.GetItems().empty())
						throw std::runtime_error("Cannot have a production containing anything other than @.");
					productionValid = false;
					nonTerminal.AddItem({});
				}
				else if (ch == '"')
					state = ParseState::Keyword;
				else if (ch == '\'')
					state = ParseState::Operator;
				else if (ch == '<')
					state = ParseState::NonTerminal;
				else if (ch == 'i')
					state = ParseState::Identifier;
				else if (ch == 'v')
					state = ParseState::Constant;
				else
					throw std::runtime_error("Invalid character in grammar parse string");
				break;

			case ParseState::Keyword:
				if (ch == '"')
				{
					if (name.empty())
						throw std::runtime_error("Cannot use empty keywords.");
					production.AddItem(Terminal::Keyword(name));
					name.clear();
					state = ParseState::RightSide;
				}
				else
					name += ch;
				break;

			case ParseState::Operator:
				if (ch == '\'')
				{
					if (name.empty())
						throw std::runtime_error("Cannot use empty operators.");
					production.AddItem(Terminal::Operator(name));
					name.clear();
					state = ParseState::RightSide;
				}
				else
				{
					name += ch;
					if (name.size() > 5)
						throw std::runtime_error("Operators longer than 5 characters are invalid");
				}
				break;

			case ParseState::NonTerminal:
				if (ch == '>')
				{
					AddNonTerminalToProduction(name, production);
					name.clear();
					state = ParseState::RightSide;
				}
				else
					name += ch;
				break;

			case ParseState::Identifier:
				if (ch == 'd')
				{
					production.AddItem(Terminal::Identifier());
					state = ParseState::RightSide;
				}
				break;

			case ParseState::Constant:
				if (ch == 'l')
				{
					production.AddItem(Terminal::Constant());
					state = ParseState::RightSide;
				}
				break;

			case ParseState::FirstComment:
				if (ch == '#')
					state = ParseState::BlockComment;
				else if (ch == '\n')
					state = return_state;
				else
					state = ParseState::Comment;
				break;

			case ParseState::Comment:
				if (ch == '\n')
					state = return_state;
				break;

			case ParseState::BlockComment:
				if (ch == '#')
					state = ParseState::BlockCommentEnd;
				break;

			case ParseState::BlockCommentEnd:
				if (ch == '#')
					state = return_state;
				else
					state = ParseState::BlockComment;
				break;
			}
		}
		if (state != ParseState::Initial)
			throw std::runtime_error("Invalid state after parsing.");

		return{ items };
	}

private:
	void AddNonTerminalToProduction(std::string name, Production& production)
	{
		if (name.empty())
			throw std::runtime_error("Cannot use empty non-terminal names.");

		//Parse out optional assignment
		bool IsOptional = String::EndsWith(name, "-opt");
		name = IsOptional ? name.substr(0, name.size() - 4) : name;

		//Check name again after parsing out optional suffixes
		if (name.empty())
			throw std::runtime_error("Cannot use empty non-terminal names (after suffix removal).");

		//Check for special identifier command prefixes
		if (name.find("id.") == 0)
		{
			if (IsOptional)
				throw std::runtime_error("Cannot apply optional parameter to id. definition.");
			production.AddItem(Terminal::Identifier(name.substr(3)));
		}
		else if (IsOptional)
		{
			//Add the name+opt non-terminal to the production and make sure the proper optional item is defined
			auto optname = name + "-opt";
			production.AddItem(optname);
			if (items.find(optname) == items.end())
			{
				NonTerminal optional{ optname };
				if (name.find("op-") == 0)
					optional.AddItem(Production{}.AddItem(Terminal::Operator(name.substr(3))));
				else if (name.find("key-") == 0)
					optional.AddItem(Production{}.AddItem(Terminal::Keyword(name.substr(4))));
				else if (name == "id")
					optional.AddItem(Production{}.AddItem(Terminal::Identifier()));
				else
					optional.AddItem(Production{}.AddItem(name));
				optional.AddItem({});
				items[optname] = optional;
			}
		}
		else
		{
			//Handle special types that were created for the purpose of adding grammar actions
			if (name.find("op-") == 0)
				production.AddItem(Terminal::Operator(name.substr(3)));
			else if (name.find("key-") == 0)
				production.AddItem(Terminal::Keyword(name.substr(4)));
			else if (name == "id")
				production.AddItem(Terminal::Identifier());
			else
				production.AddItem(name);
		}
	}

private:
	std::map<std::string, NonTerminal> items;
};
