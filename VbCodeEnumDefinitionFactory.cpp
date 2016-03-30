#include "VbCodeEnumDefinitionFactory.h"
#include "VbEnumStatement.h"
#include "VbEnumMember.h"
#include "VbCodeExpressionFactory.h"

VbCodeEnumDefinition VbCodeEnumDefinitionFactory::Create(const Sentence& sentence)
{
	VbEnumStatement enumStatement{ sentence };
	std::vector<VbCodeEnumMember> members;
	if (enumStatement.members)
		for (auto& enumMemberSentence : *enumStatement.members)
		{
			VbEnumMember enumMember{ enumMemberSentence };
			if (enumMember.name)
				members.emplace_back(
					enumMember.name->GetValue(),
					enumMember.expression ?
						VbCodeExpressionFactory::CreateExpression(*enumMember.expression) :
						nullptr);
		}
	return
	{
		enumStatement.access && *enumStatement.access == "public",
		enumStatement.name.GetValue(),
		members
	};
}
