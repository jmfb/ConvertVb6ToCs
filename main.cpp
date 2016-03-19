#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "VbTokenStream.h"
#include "VbConflictResolver.h"
#include "ParserLALR.h"

static const auto source = R"x(
	Public Function Foo( _
		ByVal index As Long, _
		ByRef found As Boolean _
	) As Variant
		If (index) Then
			found = True
		End If
		Foo = CStr(index)
	End Sub
)x";

void CompileGrammarToBinaryTransitionTable()
{
	std::cout << "Loading grammar text...";
	std::ifstream in{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.txt)" };
	std::stringstream buffer;
	in >> buffer.rdbuf();
	std::cout << "done." << std::endl;

	std::cout << "Starting to load transition table" << std::endl;
	auto transitionTable = ParserLALR::Generate(buffer.str(), "translation-unit", VbConflictResolver{});
	std::cout << "Done loading transition table" << std::endl;

	std::cout << "Saving binary...";
	transitionTable.Write(BinaryWriter{ std::ofstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
	std::cout << "done." << std::endl;
}

enum class EndType
{
	If,
	Select,
	With,
	Program,
	Sub,
	Function,
	Property
};

const char* ToString(EndType value)
{
	switch (value)
	{
	case EndType::If: return "End If";
	case EndType::Select: return "End Select";
	case EndType::With: return "End With";
	case EndType::Program: return "End";
	case EndType::Sub: return "End Sub";
	case EndType::Function: return "End Function";
	case EndType::Property: return "End Property";
	}
	throw std::runtime_error("Invalid EndType");
}

void IsSentence(const Sentence& sentence, const std::string& name, std::size_t minSize, std::size_t maxSize)
{
	if (sentence.GetName() != name)
		throw std::runtime_error("Should be: " + name);
	if (sentence.GetNodes().size() < minSize)
		throw std::runtime_error("Not enough nodes.");
	if (sentence.GetNodes().size() > maxSize)
		throw std::runtime_error("Too few nodes.");
}

void IsToken(const Sentence& sentence, std::size_t index, const std::string& value)
{
	if (index >= sentence.GetNodes().size())
		throw std::runtime_error("Index past end of sentence.");
	auto& token = sentence.GetNodes()[index]->AsToken();
	if (token != value)
		throw std::runtime_error("Expected: " + value);
}

template <typename Enum>
Enum ToEnum(const Token& token, const std::map<std::string, Enum>& map)
{
	for (auto& entry : map)
		if (token == entry.first)
			return entry.second;
	throw std::runtime_error("Invalid enum value.");
}

EndType ToEndType(const Sentence& sentence)
{
	//<end-statement> =
	//	"end" <end-keyword-opt>;
	//<end-keyword> =
	//	#<id.type> and "enum" handled specially by begin statement
	//	"sub"
	//	| "function"
	//	| <id.property>
	//	| "if"
	//	| <id.select>
	//	| "with";
	IsSentence(sentence, "end-statement", 1, 2);
	IsToken(sentence, 0, "end");
	if (sentence.GetNodes().size() == 1)
		return EndType::Program;
	auto& endKeyword = sentence.GetNodes().back()->AsSentence();
	IsSentence(endKeyword, "end-keyword", 1, 1);
	auto& keyword = endKeyword.GetNodes().front()->AsToken();
	return ToEnum<EndType>(keyword,
	{
		{ "sub", EndType::Sub },
		{ "function", EndType::Function },
		{ "property", EndType::Property },
		{ "if", EndType::If },
		{ "select", EndType::Select },
		{ "with", EndType::With }
	});
}

void Dump(const Sentence& translationUnit)
{
	//<translation-unit> =
	//	<translation-header-opt>
	//	<declaration-list-opt>;
	if (translationUnit.GetName() != "translation-unit")
		throw std::runtime_error("Top level item should be translation-unit");
	if (translationUnit.GetNodes().empty())
		return; //There is no declaration list
	//TODO: support optional translation-header
	if (translationUnit.GetNodes().size() > 1)
		throw std::runtime_error("Translation unit should contain a single declaration list.");
	auto& declarationList = translationUnit.GetNodes().front()->AsSentence();
	if (declarationList.GetName() != "declaration-list")
		throw std::runtime_error("Should be a declaration-list.");
	//<declaration-list> =
	//	<declaration> <declaration-list'>;
	for (auto& node : declarationList.GetNodes())
	{
		auto& declaration = node->AsSentence();
		if (declaration.GetName() != "declaration")
			throw std::runtime_error("Should be a declaration.");
		//<declaration> =
		//	<attribute>
		//	| <line-label> <vb-line-opt> '\n'
		//	| <vb-line> '\n'
		//	| '\n';
		if (declaration.GetNodes().size() == 1)
			continue; //Every declaration must end with a newline.  Single token declarations are therefore empty and can be skipped.
		//TODO: Scan past line-label (only valid within a function definition, statement may exist after label on the same line)
		auto& vbLine = declaration.GetNodes().front()->AsSentence();
		if (vbLine.GetName() != "vb-line")
			throw std::runtime_error("Should be a vb-line.");
		//<vb-line> =
		//	<statement>
		//	| <statement> ':' <compound-statement-opt>
		//	| ':' <compound-statement-opt>;
		if (vbLine.GetNodes().size() != 1)
			throw std::runtime_error("Compound statements not yet supported.");
		auto& statement = vbLine.GetNodes().front()->AsSentence();
		if (statement.GetName() != "statement")
			throw std::runtime_error("Should be statement.");
		if (statement.GetNodes().size() != 1)
			throw std::runtime_error("Statement should have exactly one child.");
		auto& statementType = statement.GetNodes().front()->AsSentence();

		/*
		function-statement
		if-statement
		let-statement
		end-statement
		let-statement
		end-statement
		*/

		if (statementType.GetName() == "end-statement")
		{
			auto endType = ToEndType(statementType);
			std::cout << ToString(endType) << std::endl;
		}
	}
}

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Loading binary...";
		TransitionTable transitionTable;
		transitionTable.Read(BinaryReader{ std::ifstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
		std::cout << "done." << std::endl;

		std::cout << "Parsing source...";
		auto sentence = transitionTable.Parse(VbTokenStream{ "source", source });
		std::cout << "done." << std::endl;

		Dump(sentence);

		//std::cout << "Saving to xml...";
		//std::ofstream out{ R"(c:\temp\output.xml)" };
		//sentence.WriteXml(out);
		//std::cout << "done." << std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
