// Copyright 2017-2018 Arran Walker

#pragma once

#include "CoreMinimal.h"

#include "IAUSResponseCurve.generated.h"

/**
 *
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class IAUS_API UIAUSResponseCurve : public UObject
{
	GENERATED_BODY()

protected:
	static float Sanitize(const float y);

public:
	const float Pi = 3.14159265358979323846;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Slope;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Exponent;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float XShift;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float YShift;

	virtual float ComputeValue(const float x) const;
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Linear"))
class IAUS_API UIAUSLinearResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const { return Sanitize((Slope * (x - XShift)) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Polynomial"))
class IAUS_API UIAUSPolynomialResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const { return Sanitize((Slope * FMath::Pow(x - XShift, Exponent)) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Logistic"))
class IAUS_API UIAUSLogisticResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const { return Sanitize((Slope / (1 + FMath::Exp(-10.0 * Exponent * (x - 0.5 - XShift)))) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Logit"))
class IAUS_API UIAUSLogitResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const { return Sanitize(Slope * FMath::Loge((x - XShift) / (1.0 - (x - XShift))) / 5.0 + 0.5 + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Normal"))
class IAUS_API UIAUSNormalResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const
	{
		return Sanitize(Slope * FMath::Exp(-30.0 * Exponent * (x - XShift - 0.5) * (x - XShift - 0.5)) + YShift);
	}
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Sine"))
class IAUS_API UIAUSSineResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE float ComputeValue(const float x) const { return Sanitize(0.5 * Slope * FMath::Sin(2.0 * Pi * (x - XShift)) + 0.5 + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Constant"))
class IAUS_API UIAUSConstantResult : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	/* Only Value considered */
	UPROPERTY(EditAnywhere, Category = "Settings")
	float StaticValue;

	FORCEINLINE float ComputeValue(const float x) const { return Sanitize(StaticValue); }
};