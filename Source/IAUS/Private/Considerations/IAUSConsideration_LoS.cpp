// Copyright Project Borealis

#include "Considerations/IAUSConsideration_LoS.h"

#include "AIController.h"

#include "IAUSEvaluator.h"

UIAUSConsideration_LoS::UIAUSConsideration_LoS() {}

float UIAUSConsideration_LoS::Score(const FIAUSBehaviorContext& Context) const
{
	bool HasLineOfSight = Context.AIController->LineOfSightTo(Context.Target, FVector::ZeroVector, false);

	return ResponseCurve->ComputeValue(static_cast<float>(HasLineOfSight));
}
