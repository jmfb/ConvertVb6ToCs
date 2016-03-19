#pragma once
#include "PreLineSource.h"
#include "PreLineDirective.h"
#include "PreLineIf.h"

class PreLineEvaluator
{
public:
	virtual void EvaluateSource(PreLineSource& line) = 0;
	virtual void EvaluateDirective(PreLineDirective& line) = 0;
	virtual void EvaluateIf(PreLineIf& line) = 0;
};

inline void PreLineSource::Evaluate(PreLineEvaluator& evaluator)
{
	evaluator.EvaluateSource(*this);
}

inline void PreLineDirective::Evaluate(PreLineEvaluator& evaluator)
{
	evaluator.EvaluateDirective(*this);
}

inline void PreLineIf::Evaluate(PreLineEvaluator& evaluator)
{
	evaluator.EvaluateIf(*this);
}
