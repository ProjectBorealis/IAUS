// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "IAUSAxisInput_Range.h"

#include "IAUSConsideration_Distance.generated.h"

/**
 *
 */
UCLASS(Meta = (DisplayName = "Distance Consideration", Category = "Considerations"))
class IAUS_API UIAUSConsideration_Distance : public UIAUSAxisInput_Range
{
	GENERATED_BODY()

	UIAUSConsideration_Distance();

public:
	virtual float Score(const struct FIAUSBehaviorContext& Context) const override;
};
