#include "VbCodeTypeFactory.h"
#include "VbAsSpecifier.h"
#include "VbTypeSpecifier.h"
#include "VbSimpleType.h"
#include "VbCodeExpressionFactory.h"
#include "VbCodeValueFactory.h"
#include "VbCodeTypeNameFactory.h"
#include "SentenceParser.h"

VbCodeType VbCodeTypeFactory::Create(const Sentence& sentence)
{
	VbAsSpecifier asSpecifier{ sentence };
	if (asSpecifier.arraySpecifier)
		throw std::runtime_error("Array specifier not implemented yet.");
	VbTypeSpecifier typeSpecifier{ asSpecifier.typeSpecifier };
	if (typeSpecifier.qualifiedId)
		return{ asSpecifier.isNew, VbCodeValueType::UserObject, VbCodeTypeNameFactory::Create(*typeSpecifier.qualifiedId) };
	if (asSpecifier.isNew)
		throw new std::runtime_error("New specifier not allowed on non-user object types.");
	VbSimpleType simpleType{ *typeSpecifier.simpleType };
	if (simpleType.constantSize)
	{
		if (simpleType.type.GetValue() != "string")
			throw std::runtime_error("Constant length string is only valid option.");
		return{ VbCodeValueType::String, VbCodeValueFactory::Create(*simpleType.constantSize) };
	}
	if (simpleType.variableSize)
	{
		if (simpleType.type.GetValue() != "string")
			throw std::runtime_error("Variable length string is only valid option.");
		return{ VbCodeValueType::String, VbCodeTypeNameFactory::Create(*simpleType.variableSize) };
	}
	return
	{
		SentenceParser::ToEnum<VbCodeValueType>(
			simpleType.type,
			{
				{ "byte", VbCodeValueType::Byte },
				{ "boolean", VbCodeValueType::Boolean },
				{ "integer", VbCodeValueType::Integer },
				{ "long", VbCodeValueType::Long },
				{ "currency", VbCodeValueType::Currency },
				{ "single", VbCodeValueType::Single },
				{ "double", VbCodeValueType::Double },
				{ "date", VbCodeValueType::Date },
				{ "string", VbCodeValueType::String },
				{ "object", VbCodeValueType::Object },
				{ "variant", VbCodeValueType::Variant }
			})
	};
}
