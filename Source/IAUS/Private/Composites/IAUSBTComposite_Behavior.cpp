// Copyright Project Borealis. All rights reserved.

#include "Composites/IAUSBTComposite_Behavior.h"

UIAUSBTComposite_Behavior::UIAUSBTComposite_Behavior(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	bUseNodeActivationNotify = true;
	bUseNodeDeactivationNotify = true;
}

#if WITH_EDITOR
FString UIAUSBTComposite_Behavior::GetStaticDescription() const
{
	FString Description;

	if (bTargetSelf)
	{
		Description.Append(TEXT("Target Self\n"));
	}
	if (bTargetFriendly)
	{
		Description.Append(TEXT("Target Friendly\n"));
	}
	if (bTargetNeutral)
	{
		Description.Append(TEXT("Target Neutral\n"));
	}
	if (bTargetHostile)
	{
		Description.Append(TEXT("Target Hostile\n"));
	}

	return FString::Printf(TEXT("%s\n%s\nInitial Weight: %f"), *Super::GetStaticDescription(), *Description, InitialWeight);
}
#endif

void UIAUSBTComposite_Behavior::NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const
{
	if (FIAUSBTCompositeBehaviorMemory* Memory = GetNodeMemory<FIAUSBTCompositeBehaviorMemory>(SearchData))
	{
		Memory->bExecuting = true;
	}
}

void UIAUSBTComposite_Behavior::NotifyNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) const
{
	Super::NotifyNodeDeactivation(SearchData, NodeResult);

	if (FIAUSBTCompositeBehaviorMemory* Memory = GetNodeMemory<FIAUSBTCompositeBehaviorMemory>(SearchData))
	{
		Memory->bExecuting = false;
	}
}

uint16 UIAUSBTComposite_Behavior::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTCompositeBehaviorMemory);
}
