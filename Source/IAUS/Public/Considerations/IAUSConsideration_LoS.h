// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSConsideration_LoS.generated.h"

/**
 *
 */
UCLASS(Meta = (DisplayName = "Line of Sight Consideration", Category = "Considerations"))
class IAUS_API UIAUSConsideration_LoS : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

public:
	UIAUSConsideration_LoS();

	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;
};
