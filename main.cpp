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

	std::ofstream outFunctions{ R"(c:\temp\functions.xml)" };
	auto module = VbCodeModuleFactory{}.Create("DSHECommon", sentence);
	module.ResolveUnqualifiedTypeNames();
	std::cout << "namespace " << module.library << std::endl
		<< "{" << std::endl;
	std::cout << "	internal static class " << module.name << std::endl
		<< "	{" << std::endl;
	for (auto& constant : module.constants)
		constant.WriteCs(std::cout);
	for (auto& member : module.members)
		member.WriteCs(true, std::cout);
	for (auto& declare : module.declares)
		declare.WriteCs(std::cout);
	for (auto& typeDefinition : module.typeDefinitions)
		typeDefinition.WriteCs(std::cout);
	for (auto& function : module.functions)
		function.WriteCs(std::cout);
	std::cout << "	}" << std::endl
		<< "}" << std::endl;
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
