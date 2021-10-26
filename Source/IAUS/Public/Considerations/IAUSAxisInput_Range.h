// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

#include "IAUSAxisInput_Range.generated.h"

/**
 *
 */
UCLASS(Abstract)
class IAUS_API UIAUSAxisInput_Range : public UIAUSBTDecorator_Consideration
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Utility|Range")
	float Minimum;

	UPROPERTY(EditAnywhere, Category = "Utility|Range")
	float Maximum;
};
