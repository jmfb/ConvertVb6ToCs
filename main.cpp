#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "VbTokenStream.h"
#include "VbConflictResolver.h"
#include "ParserLALR.h"

#include "VbCodeModuleFactory.h"

const auto visualBasicGrammarFileName = R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.txt)";
const auto visualBasicTransitionTableFileName = R"(c:\save\code\tests\ConvertVb6ToCs\VbGrammar.bin)";

template <typename LanguageConflictResolver>
void CompileGrammarToBinaryTransitionTable(const std::string& grammarFileName, const std::string& transitionTableFileName)
{
	std::stringstream buffer;
	std::ifstream{ grammarFileName.c_str() } >> buffer.rdbuf();

	auto transitionTable = ParserLALR::Generate(buffer.str(), "translation-unit", LanguageConflictResolver{});
	transitionTable.Write(BinaryWriter{ std::ofstream{ transitionTableFileName.c_str(), std::ios::binary } });
}

void CompileVisualBasicGrammarToTransitionTable()
{
	CompileGrammarToBinaryTransitionTable<VbConflictResolver>(visualBasicGrammarFileName, visualBasicTransitionTableFileName);
}

template <typename LanguageTokenStream>
Sentence Parse(const std::string& transitionTableFileName, const std::string& sourceFileName)
{
	TransitionTable transitionTable;
	transitionTable.Read(BinaryReader{ std::ifstream{ transitionTableFileName.c_str(), std::ios::binary } });

	std::stringstream buffer;
	std::ifstream{ sourceFileName.c_str() } >> buffer.rdbuf();
	return transitionTable.Parse(LanguageTokenStream{ sourceFileName, buffer.str() });
}

void ProcessVisualBasicModule(const std::string& library, const std::string& fileName)
{
	auto sentence = Parse<VbTokenStream>(visualBasicTransitionTableFileName, fileName);
	auto module = VbCodeModuleFactory{}.Create(library, sentence);
	module.ResolveUnqualifiedTypeNames();
	module.WriteCs(std::ofstream{ fileName + ".cs" });
}

int main(int argc, char** argv)
{
	try
	{
		ProcessVisualBasicModule("DSHECommon", R"(C:\Save\Code\tests\DSHECommon\basGlobals.bas)");
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
