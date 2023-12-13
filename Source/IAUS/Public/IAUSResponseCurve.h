// Copyright Project Borealis

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

	UIAUSLinearResponseCurve()
	{
		Slope = 1.f;
		Exponent = 0.f;
		XShift = 0.f;
		YShift = 0.f;
	}
	
public:
	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize((Slope * (x - XShift)) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Polynomial"))
class IAUS_API UIAUSPolynomialResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

	UIAUSPolynomialResponseCurve()
	{
		Slope = 1.f;
		Exponent = 1.f;
		XShift = 0.f;
		YShift = 0.f;
	}

public:
	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize((Slope * FMath::Pow(x - XShift, Exponent)) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Logistic"))
class IAUS_API UIAUSLogisticResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

	UIAUSLogisticResponseCurve()
	{
		Slope = 1.f;
		Exponent = 1.f;
		XShift = 0.f;
		YShift = 0.f;
	}
	
public:
	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize((Slope / (1.f + FMath::Exp(-10.f * Exponent * (x - 0.5f - XShift)))) + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Logit"))
class IAUS_API UIAUSLogitResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()
	
	UIAUSLogitResponseCurve()
	{
		Slope = 1.f;
		Exponent = 0.f;
		XShift = 0.f;
		YShift = 0.f;
	}

public:
	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize(Slope * FMath::Loge((x - XShift) / (1.0 - (x - XShift))) / 5.0 + 0.5 + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Normal"))
class IAUS_API UIAUSNormalResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

	UIAUSNormalResponseCurve()
	{
		Slope = 0.f;
		Exponent = 1.f;
		XShift = 0.f;
		YShift = 0.f;
	}

public:
	FORCEINLINE virtual float ComputeValue(const float x) const override
	{
		return Sanitize(Slope * FMath::Exp(-30.0 * Exponent * (x - XShift - 0.5) * (x - XShift - 0.5)) + YShift);
	}
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Sine"))
class IAUS_API UIAUSSineResponseCurve : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize(0.5 * Slope * FMath::Sin(2.0 * UE_PI * (x - XShift)) + 0.5 + YShift); }
};

UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Constant"))
class IAUS_API UIAUSConstantResult : public UIAUSResponseCurve
{
	GENERATED_BODY()

public:
	/* Only Value considered */
	UPROPERTY(EditAnywhere, Category = "Settings")
	float StaticValue;

	FORCEINLINE virtual float ComputeValue(const float x) const override { return Sanitize(StaticValue); }
};