// Copyright Project Borealis

#include "Considerations/IAUSConsideration_Health.h"

#include "AIController.h"

#include "Components/IAUSBaseHealthComponent.h"
#include "IAUSEvaluator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IAUSConsideration_Health)

UIAUSConsideration_Health::UIAUSConsideration_Health()
{
	TargetsSelf = true;
}

float UIAUSConsideration_Health::Score(const FIAUSBehaviorContext& Context) const
{
	AActor* Target = Context.Target;

	if (TargetsSelf && Context.AIController)
	{
		Target = Context.AIController->GetPawn();
	}

	const UIAUSBaseHealthComponent* HealthComponent = Target ? Target->FindComponentByClass<UIAUSBaseHealthComponent>() : nullptr;

	if (!HealthComponent || HealthComponent->GetMaxHealth() <= 0.0f)
	{
		return 1.0f;
	}

	return ResponseCurve->ComputeValue(HealthComponent->GetHealth() / HealthComponent->GetMaxHealth());
}
