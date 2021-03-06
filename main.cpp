#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "VbTokenStream.h"
#include "VbConflictResolver.h"
#include "ParserLALR.h"

#include "VbCodeTranslationUnitFactory.h"

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

void ImportVba(TypeTable& table)
{
	auto library = table.DefineLibrary("VBA");
	auto collection = table.DefineClass(library, "Collection");
}

void ImportAdodb(TypeTable& table)
{
	auto library = table.DefineLibrary("ADODB");
	auto field = table.DefineClass(library, "Field");
	auto eDataFieldType = table.DefineEnum(library, "eDataFieldType");
}

void ProcessVisualBasicSource(const std::string& library, const std::string& fileName)
{
	TypeTable typeTable;
	//TODO: import external references
	ImportVba(typeTable);
	ImportAdodb(typeTable);

	typeTable.DefineLibrary(library);

	auto translationUnit = VbCodeTranslationUnitFactory{}.Create(library, ParseVisualBasic(fileName));

	translationUnit.DefineTypes(typeTable);
	//TODO: define types in all translation units
	translationUnit.DefineTypeMembers(typeTable);
	//TODO: define type members in all translation units

	//TODO: remove this once the type and symbol tables are completed
	translationUnit.ResolveUnqualifiedTypeNames();
	//translationUnit.WriteCs(std::cout);
	translationUnit.WriteCs(std::ofstream{ fileName + ".cs" });

	std::cout << fileName << std::endl;
	typeTable.Dump();
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	try
	{
		ProcessVisualBasicSource("DSHECommon", R"(C:\Save\Code\tests\DSHECommon\basGlobals.bas)");
		ProcessVisualBasicSource("DSHECommon", R"(C:\Save\Code\tests\DSHECommon\CDataField.cls)");
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}
