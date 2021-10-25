// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

struct FIAUSBehaviorContext
{
	class IAUSEvaluator* Evaluator;
	class AActor* Target;
	class AAIController* AIController;
	float TotalScore;
	int32 BehaviorIndex;
};

struct FIAUSBehavior
{
	bool bTargetSelf;
	bool bTargetFriendly;
	bool bTargetNeutral;
	bool bTargetHostile;

	float InitialWeight;
	float LastExecutionTime;

	TArray<class UIAUSBTDecorator_Consideration*> Considerations;
};

/**
 *
 */
class IAUS_API IAUSEvaluator
{
public:
	TArray<FIAUSBehavior> Behaviors;
	TMap<FString, int32> BehaviorNameIndexMap;

	void AddBehavior(const FIAUSBehavior& Behavior, const FString& Name);
	FIAUSBehaviorContext ChooseBehavior(class AAIController* Controller, const TArray<class AActor*> Targets, bool bCheckTeamAttitude = true);
};
