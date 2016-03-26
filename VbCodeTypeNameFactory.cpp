#include "VbCodeTypeNameFactory.h"
#include "VbQualifiedId.h"

VbCodeTypeName VbCodeTypeNameFactory::Create(const Sentence& sentence)
{
	VbQualifiedId qualifiedId{ sentence };
	if (qualifiedId.wsDot)
		throw std::runtime_error("Leading dot not allowed on type names.");
	if (qualifiedId.suffix.empty())
		return{ "", qualifiedId.id.GetValue() };
	if (qualifiedId.suffix.size() != 1)
		throw std::runtime_error("Only 'library.name' type names supported.");
	if (qualifiedId.suffix[0].first != VbCodeDotType::Dot)
		throw std::runtime_error("Bang not allowed in type names.");
	return{ qualifiedId.id.GetValue(), qualifiedId.suffix[0].second.GetValue() };
}
