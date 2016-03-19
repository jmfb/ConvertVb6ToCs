#pragma once
#include "PreProcessor.h"
#include "VbCharStream.h"
#include "VbPreTokenStream.h"
#include "String.h"
#include <stack>
#include <list>
#include <stdexcept>

class VbPreProcessor : public PreProcessor
{
public:
	VbPreProcessor(const std::string& name, const std::string& source)
		: PreProcessor{ Parse(VbPreTokenStream{ VbCharStream{ name, source } }) }
	{
	}

protected:
	PreToken InternalGetNextToken() final
	{
		return GetNextRawToken();
	}

private:
	static std::deque<PreLinePtr> Parse(PreTokenStream& stream)
	{
		std::deque<PreLinePtr> result;
		std::stack<std::deque<PreLinePtr>*> groups;
		std::list<PreLineIf> ifs;
		groups.push(&result);
		while (!stream.IsEndOfFile())
		{
			auto line = stream.GetNextLine();
			if (line.IsDirective())
			{
				auto directive = line.ParseDirective();
				auto& command = directive.first;
				auto& tokens = directive.second;
				auto name = String::ToLower(command.GetValue());
				if (name == "const")
				{
					groups.top()->push_back(std::make_shared<PreLineDirective>(PreDirectiveType::Define, command.GetPosition(), tokens));
				}
				else if (name == "if")
				{
					ifs.emplace_back(PreIfBlock{ PreIfType::If, command.GetPosition(), tokens });
					groups.push(&ifs.back().GetBlocks().back().GetIfGroup());
				}
				else if (name == "elseif")
				{
					if (ifs.empty())
						throw std::runtime_error("#elif encountered without corresponding #if/def/ndef.");
					if (ifs.back().GetBlocks().back().GetType() == PreIfType::Else)
						throw std::runtime_error("#elif encountered after #else is invalid.");
					groups.pop();
					ifs.back().GetBlocks().emplace_back(PreIfType::ElseIf, command.GetPosition(), tokens);
					groups.push(&ifs.back().GetBlocks().back().GetIfGroup());
				}
				else if (name == "else")
				{
					if (ifs.empty())
						throw std::runtime_error("#else encountered without corresponding #if/def/ndef.");
					//TODO: check that tokens after #endif are whitespace or comments
					groups.pop();
					ifs.back().GetBlocks().emplace_back(PreIfType::Else, command.GetPosition(), tokens);
					groups.push(&ifs.back().GetBlocks().back().GetIfGroup());
				}
				else if (name == "end")
				{
					if (ifs.empty())
						throw std::runtime_error("#endif encountered without corresponding #if/def/ndef.");
					//TODO: check that tokens after #end are just "if", whitespace, and comments
					groups.pop();
					groups.top()->push_back(std::make_shared<PreLineIf>(ifs.back()));
				}
				else
					throw std::runtime_error("Invalid preprocessor command");
			}
			else
				groups.top()->push_back(std::make_shared<PreLineSource>(line.GetTokens()));
		}
		if (!ifs.empty())
			throw std::runtime_error("Missing #end if before end of file");
		if (groups.size() != 1)
			throw std::runtime_error("Mismatched preprocessor control directives.");
		return result;
	}

protected:
	void EvaluateSource(PreLineSource& line) final
	{
		auto prev = PreToken::EndOfFile();
		auto firstToken = true;
		for (auto index = 0u; index < line.GetTokens().size(); ++index)
		{
			auto token = line.GetTokens()[index];
			if (token.GetType() != PreTokenType::Whitespace && token.GetType() != PreTokenType::Comment)
			{
				//Check for "Line Input ..." statement to form the special "line input" keyword.
				if (firstToken &&
					token.GetType() == PreTokenType::Identifier &&
					String::ToLower(token.GetValue()) == "line" &&
					(index + 2) < line.GetTokens().size() &&
					line.GetTokens()[index + 1].GetType() == PreTokenType::Whitespace &&
					line.GetTokens()[index + 2].GetType() == PreTokenType::Identifier &&
					String::ToLower(line.GetTokens()[index + 2].GetValue()) == "input")
				{
					index += 2;
					token.SetValue("line input");
				}
				//Convert <non-identifier> <dot> into <ws-spot> token (for "with" block scoped identifiers)
				if ((token.GetValue() == "." || token.GetValue() == "!") && (prev == PreToken::EndOfFile() || ((prev.GetValue() != ")") && (prev.GetType() != PreTokenType::Identifier))))
					token.SetValue(" " + token.GetValue());
				//Convert <non-identifier> <l-paren> into <ws-l-paren> token (for [call] syntax disambiguation)
				if (token.GetValue() == "(" && prev != PreToken::EndOfFile() && prev.GetType() != PreTokenType::Identifier)
					token.SetValue(" (");

				tokens.push_back(token);
				firstToken = false;
			}
			prev = token;
		}
	}

	void EvaluateDirective(PreLineDirective& line) final
	{
		switch (line.GetType())
		{
		case PreDirectiveType::Define:
			//TODO: evaluate token sequence of form "id = expression"
			throw std::runtime_error("#Const directive not yet implemented.");
		}
	}

	void EvaluateIf(PreLineIf& line) final
	{
		for (auto& block : line.GetBlocks())
		{
			switch (block.GetType())
			{
			case PreIfType::If:
			case PreIfType::ElseIf:
				//TODO: evaluate if; when true insert IfGroup at beginning of lines and break from loop
				throw std::runtime_error("#If/#ElseIf directive not yet implemented.");

			case PreIfType::Else:
				lines.insert(lines.begin(), block.GetIfGroup().begin(), block.GetIfGroup().end());
				break;
			}
		}
	}
};
