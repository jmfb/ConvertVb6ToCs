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

#include "VbLine.h"
#include "VbStatement.h"
#include "VbFunctionStatement.h"
#include "VbEndStatement.h"
#include "VbIfStatement.h"
#include "VbLetStatement.h"

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

		VbLine vbLine{ declaration.GetNodes().front()->AsSentence() };
		if (vbLine.statement)
		{
			VbStatement statement{ *vbLine.statement };
			if (statement.statement.GetName() == "function-statement")
			{
				VbFunctionStatement function{ statement.statement };
				std::cout << "Function: " << ToString(function.access) << (function.isStatic ? " Static" : "") << " " << ToString(function.type) << " " << function.name << std::endl;
			}
			else if (statement.statement.GetName() == "if-statement")
			{
				VbIfStatement ifStatement{ statement.statement };
				std::cout << "If" << std::endl;
			}
			else if (statement.statement.GetName() == "end-statement")
			{
				VbEndStatement end{ statement.statement };
				std::cout << ToString(end.type) << std::endl;
			}
			else if (statement.statement.GetName() == "let-statement")
			{
				VbLetStatement let{ statement.statement };
				std::cout << "Let" << std::endl;
			}
		}
		//TODO: something with compound statement
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
