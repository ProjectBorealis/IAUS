// Copyright Project Borealis

#include "Considerations/IAUSConsideration_Distance.h"

#include "AIController.h"

#include "IAUSEvaluator.h"

UIAUSConsideration_Distance::UIAUSConsideration_Distance()
{
	Maximum = 2000;
}

float UIAUSConsideration_Distance::Score(const FIAUSBehaviorContext& Context) const
{
	if (!Context.Target || !Context.AIController || !Context.AIController->GetPawn())
	{
		// return a result like if the target were at max distance
		return ResponseCurve->ComputeValue(Maximum);
	}
	
	const float Input = FVector::Distance(Context.AIController->GetPawn()->GetActorLocation(), Context.Target->GetActorLocation());
	const float Normalized = (Input - Minimum) / (Maximum - Minimum);

	return ResponseCurve->ComputeValue(Normalized);
}
