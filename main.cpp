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

#include "VbCodeModuleFactory.h"

void Process(const std::string& name, const std::string& source)
{
	TransitionTable transitionTable;
	transitionTable.Read(BinaryReader{ std::ifstream{ R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)", std::ios::binary } });
	auto sentence = transitionTable.Parse(VbTokenStream{ name, source });
	sentence.WriteXml(std::ofstream{ R"(c:\temp\output.xml)" });

	auto module = VbCodeModuleFactory{}.Create(sentence);
	std::cout << module.name << std::endl;
	for (auto& constant : module.constants)
		std::cout << (constant.isPublic ? "public" : "private") << " const " << constant.name << std::endl;
	//TODO: more
}

int main(int argc, char** argv)
{
	try
	{
		//Process("source", source);
		auto fileName = R"(c:\save\code\tests\DSHECommon\basGlobals.bas)";
		std::ifstream in{ fileName };
		std::stringstream buffer;
		in >> buffer.rdbuf();
		Process(fileName, buffer.str());
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
