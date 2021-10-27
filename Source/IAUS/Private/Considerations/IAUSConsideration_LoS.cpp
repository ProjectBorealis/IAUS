// Copyright Project Borealis

#include "Considerations/IAUSConsideration_LoS.h"

#include "AIController.h"

#include "IAUSEvaluator.h"

UIAUSConsideration_LoS::UIAUSConsideration_LoS()
{
	NodeName = TEXT("Line of Sight to Target");
}

float UIAUSConsideration_LoS::Score(const FIAUSBehaviorContext& Context) const
{
	if (Context.AIController)
	{
		const bool bHasLineOfSight = Context.AIController->LineOfSightTo(Context.Target, FVector::ZeroVector, false);
		return ResponseCurve->ComputeValue(static_cast<float>(bHasLineOfSight));
	}

	return 0.0f;
}
