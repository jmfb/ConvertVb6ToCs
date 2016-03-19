#pragma once
#include "TokenStream.h"
#include "VbPreProcessor.h"
#include "VbTokenFactory.h"

class VbTokenStream : public VbPreProcessor, public TokenStream
{
public:
	VbTokenStream(const std::string& name, const std::string& source)
		: VbPreProcessor{ name, source }, TokenStream{ static_cast<VbPreProcessor&>(*this) }
	{
	}

	using TokenStream::GetNextToken;

protected:
	Token InternalCreateToken(const PreToken& token) final
	{
		return VbTokenFactory::Create(token);
	}
};
