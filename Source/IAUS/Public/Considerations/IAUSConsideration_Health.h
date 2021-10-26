// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSConsideration_Health.generated.h"

/**
 *
 */
UCLASS(Meta = (DisplayName = "Health Consideration", Category = "Considerations"))
class IAUS_API UIAUSConsideration_Health : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

	UIAUSConsideration_Health();

public:
	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Utility")
	bool TargetsSelf;
};
