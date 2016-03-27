#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "VbTokenStream.h"
#include "VbConflictResolver.h"
#include "ParserLALR.h"

#include "VbCodeModuleFactory.h"
#include "VbCodeClassFactory.h"

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

Sentence ParseVisualBasic(const std::string& fileName)
{
	return Parse<VbTokenStream>(visualBasicTransitionTableFileName, fileName);
}

void ProcessVisualBasicModule(const std::string& library, const std::string& fileName)
{
	auto module = VbCodeModuleFactory{}.Create(library, ParseVisualBasic(fileName));
	module.ResolveUnqualifiedTypeNames();
	module.WriteCs(std::ofstream{ fileName + ".cs" });
}

void ProcessVisualBasicClass(const std::string& library, const std::string& fileName)
{
	auto cls = VbCodeClassFactory{}.Create(library, ParseVisualBasic(fileName));
	cls.ResolveUnqualifiedTypeNames();
	cls.WriteCs(std::cout);
}

int main(int argc, char** argv)
{
	try
	{
		//ProcessVisualBasicModule("DSHECommon", R"(C:\Save\Code\tests\DSHECommon\basGlobals.bas)");
		ProcessVisualBasicClass("DSHECommon", R"(C:\Save\Code\tests\DSHECommon\CDataField.cls)");
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
