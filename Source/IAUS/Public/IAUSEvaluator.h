// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BehaviorTreeTypes.h"

struct FIAUSBehaviorContext
{
	class IAUSEvaluator* Evaluator = nullptr;
	AActor* Target = nullptr;
	class AAIController* AIController = nullptr;
	float TotalScore = 0.f;
	int32 BehaviorIndex = BTSpecialChild::NotInitialized;
};

struct FIAUSBehavior
{
	bool bTargetSelf = false;
	bool bTargetFriendly = false;
	bool bTargetNeutral = false;
	bool bTargetHostile = false;

	float InitialWeight = 0.f;
	float LastExecutionTime = 0.f;
	float CompensationFactor = 0.f;

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
