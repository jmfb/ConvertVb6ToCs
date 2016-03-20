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
	Done:
		Foo = CStr(index) : index = index + 1
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

#include "VbTranslationUnit.h"
#include "VbDeclaration.h"
#include "VbLine.h"
#include "VbLineLabel.h"
#include "VbStatement.h"
#include "VbCompoundStatement.h"
#include "VbFunctionStatement.h"
#include "VbEndStatement.h"
#include "VbIfStatement.h"
#include "VbLetStatement.h"
#include "VbLiteral.h"
#include "VbQualifiedId.h"

void DumpStatement(const Sentence& sentence)
{
	VbStatement statement{ sentence };
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

void Dump(const Sentence& sentence)
{
	VbTranslationUnit translationUnit{ sentence };
	//TODO: translation-header
	if (!translationUnit.declarationList)
		return;
	for (auto& declarationSentence : *translationUnit.declarationList)
	{
		VbDeclaration declaration{ declarationSentence };
		//TODO: attribute
		if (declaration.lineLabel)
		{
			VbLineLabel lineLabel{ *declaration.lineLabel };
			std::cout << "Label: " << lineLabel.label.GetValue() << std::endl;
		}
		if (!declaration.vbLine)
			continue;
		VbLine vbLine{ *declaration.vbLine };
		if (vbLine.statement)
			DumpStatement(*vbLine.statement);
		if (!vbLine.compoundStatement)
			continue;
		VbCompoundStatement compoundStatement{ *vbLine.compoundStatement };
		for (auto& statement : compoundStatement.statements)
			DumpStatement(statement);
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
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
