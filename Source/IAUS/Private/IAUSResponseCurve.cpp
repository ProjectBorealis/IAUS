// Copyright 2017-2020 Project Borealis

#include "IAUSResponseCurve.h"

#include "Math/UnrealMathUtility.h"

float UIAUSResponseCurve::ComputeValue(const float x) const
{
	return 0.0;
}

float UIAUSResponseCurve::Sanitize(const float y)
{
	if (!FMath::IsFinite(y))
	{
		return 0.0;
	}

	if (y < 0.0)
	{
		return 0.0;
	}

	if (y > 1.0)
	{
		return 1.0;
	}

	return y;
}
