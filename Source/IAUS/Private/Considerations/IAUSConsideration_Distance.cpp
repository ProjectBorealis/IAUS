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
	const float Input = FVector::Distance(Context.AIController->GetPawn()->GetActorLocation(), Context.Target->GetActorLocation());
	const float Normalized = (Input - Minimum) / (Maximum - Minimum);

	return ResponseCurve->ComputeValue(Normalized);
}
