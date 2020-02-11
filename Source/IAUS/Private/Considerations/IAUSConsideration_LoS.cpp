// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Considerations/IAUSConsideration_LoS.h"

#include "AIController.h"

#include "IAUSEvaluator.h"

UIAUSConsideration_LoS::UIAUSConsideration_LoS()
{
}

float UIAUSConsideration_LoS::Score(const FIAUSBehaviorContext& Context) const
{
	bool HasLineOfSight = Context.AIController->LineOfSightTo(Context.Actor, FVector::ZeroVector, false);

	return ResponseCurve->ComputeValue(static_cast<float>(HasLineOfSight));
}
