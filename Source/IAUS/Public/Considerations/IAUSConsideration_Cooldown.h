// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSConsideration_Cooldown.generated.h"

/**
 *
 */
UCLASS()
class IAUS_API UIAUSConsideration_Cooldown : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

	UIAUSConsideration_Cooldown();

public:
	UPROPERTY(EditAnywhere, Category = "Utility")
	float CoolDownTime;

	UPROPERTY(EditAnywhere, Category = "Utility")
	FName Node;

	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;
};
