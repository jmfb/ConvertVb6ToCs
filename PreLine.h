#pragma once
#include <memory>

class PreLineEvaluator;

class PreLine
{
public:
	virtual ~PreLine()
	{
	}

	virtual void Evaluate(PreLineEvaluator& evaluator) = 0;
};

using PreLinePtr = std::shared_ptr<PreLine>;
