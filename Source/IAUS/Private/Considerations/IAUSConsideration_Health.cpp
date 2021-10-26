// Copyright Project Borealis

#include "Considerations/IAUSConsideration_Health.h"

#include "AIController.h"

#include "Components/IAUSBaseHealthComponent.h"
#include "IAUSEvaluator.h"

UIAUSConsideration_Health::UIAUSConsideration_Health()
{
	TargetsSelf = true;
}

float UIAUSConsideration_Health::Score(const FIAUSBehaviorContext& Context) const
{
	AActor* Target = Context.Target;

	if (TargetsSelf)
	{
		Target = Context.AIController->GetPawn();
	}

	const UIAUSBaseHealthComponent* HealthComponent = Target->FindComponentByClass<UIAUSBaseHealthComponent>();

	if (!HealthComponent || HealthComponent->GetMaxHealth() <= 0.0f)
	{
		return 1.0f;
	}

	return ResponseCurve->ComputeValue(HealthComponent->GetHealth() / HealthComponent->GetMaxHealth());
}
