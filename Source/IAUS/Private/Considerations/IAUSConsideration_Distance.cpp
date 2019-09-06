// Copyright 2017-2018 Arran Walker

#include "IAUS/Public/Considerations/IAUSConsideration_Distance.h"

#include "IAUS/Public/Composites/IAUSBTComposite_Utility.h"

#include "AIController.h"

UIAUSConsideration_Distance::UIAUSConsideration_Distance()
{
	Maximum = 2000;
}

float UIAUSConsideration_Distance::Score(const FIAUSBehaviorContext& Context) const
{
	const float Input = FVector::Distance(Context.AIController->GetPawn()->GetActorLocation(), Context.Actor->GetActorLocation());
	const float Normalized = (Input - Minimum) / (Maximum - Minimum);

	return ResponseCurve->ComputeValue(Normalized);
}
