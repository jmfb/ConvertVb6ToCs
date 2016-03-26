#include "VbCodeTypeDefinitionFactory.h"
#include "VbTypeStatement.h"
#include "VbTypeDefinition.h"
#include "VbCodeTypeFactory.h"

VbCodeTypeDefinition VbCodeTypeDefinitionFactory::Create(const Sentence& sentence)
{
	VbTypeStatement typeStatement{ sentence };
	std::vector<VbCodeVariable> members;
	for (auto& typeDefinitionSentence : typeStatement.typeDefinitions)
	{
		VbTypeDefinition typeDefinition{ typeDefinitionSentence };
		if (typeDefinition.arraySuffix)
			throw std::runtime_error("Type definition array suffix not yet implemented.");
		if (typeDefinition.name)
			members.emplace_back(typeDefinition.name->GetValue(), VbCodeTypeFactory::Create(typeDefinition.asSpecifier));
	}
	return
	{
		typeStatement.access && *typeStatement.access == "public",
		typeStatement.name.GetValue(),
		members
	};
}
