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

#include "VbFunctionStatement.h"
#include "VbEndStatement.h"

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
		if-statement
		let-statement
		*/

		if (statementType.GetName() == "function-statement")
		{
			FunctionStatement function{ statementType };
			std::cout << "Function: " << ToString(function.access) << (function.isStatic ? " Static" : "") << " " << ToString(function.type) << " " << function.name << std::endl;
		}
		else if (statementType.GetName() == "end-statement")
		{
			EndStatement end{ statementType };
			std::cout << ToString(end.type) << std::endl;
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
