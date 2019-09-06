// Copyright 2019 Zachary Swoyer, Arran Walker

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSConsideration_Direction.generated.h"

/**
 *
 */
UCLASS()
class IAUS_API UIAUSConsideration_Direction : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

	UIAUSConsideration_Direction();

public:
	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;
};
