// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Decorators/IAUSBTDecorator_Utility.h"

#include "IAUS/Public/Composites/IAUSBTComposite_Utility.h"
#include "IAUS/Public/Decorators/IAUSBTDecorator_Consideration.h"

#include "AIController.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IAUS/IAUSCommon.h"
#include "Perception/AIPerceptionComponent.h"

DEFINE_LOG_CATEGORY(LogIAUS);

UIAUSBTDecorator_Utility::UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Utility Ticker";

	bNotifyTick = true;
	bNotifyActivation = true;

	IntertiaWeight = 1.0f;
}

void UIAUSBTDecorator_Utility::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	const UIAUSBTComposite_Utility* Parent = Cast<const UIAUSBTComposite_Utility>(this->GetMyNode());
	if (Parent == nullptr)
	{
		return;
	}

	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = GetNodeMemory<FIAUSBTUtilityDecoratorMemory>(SearchData);
	DecoratorMemory->ParentMemory = Parent->GetNodeMemory<FIAUSBTCompositeUtilityMemory>(SearchData);
}

uint16 UIAUSBTDecorator_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTUtilityDecoratorMemory);
}

bool UIAUSBTDecorator_Utility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = reinterpret_cast<FIAUSBTUtilityDecoratorMemory*>(NodeMemory);

	return !DecoratorMemory->IsInvalid;
}

void UIAUSBTDecorator_Utility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = reinterpret_cast<FIAUSBTUtilityDecoratorMemory*>(NodeMemory);
	if (DecoratorMemory->ParentMemory == nullptr)
	{
		return;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return;
	}

	// Intertia check - Give behavior a moment to stabilize before we start looking for other things to do
	if ((DecoratorMemory->ParentMemory->LastBehaviorChangeTime + IntertiaWeight) >= GetWorld()->GetTimeSeconds())
	{
		return;
	}

	// Track current Context actor for later comparison
	AActor* CurrentActor = DecoratorMemory->ParentMemory->Context.Actor;

	TArray<AActor*> Targets;
	Controller->GetPerceptionComponent()->GetKnownPerceivedActors(nullptr, Targets);
	Targets.Add(Controller->GetPawn());

	DecoratorMemory->ParentMemory->Context = DecoratorMemory->ParentMemory->Evaluator.ChooseBehavior(Controller, Targets);

	if (DecoratorMemory->ParentMemory->Context.TotalScore == 0)
	{
		DecoratorMemory->IsInvalid = true;
		return;
	}

	DecoratorMemory->IsInvalid = false;
	if (DecoratorMemory->ParentMemory->Context.BehaviorIndex != static_cast<int32>(DecoratorMemory->ParentMemory->CurrentChild) ||
		CurrentActor != DecoratorMemory->ParentMemory->Context.Actor)
	{
		DecoratorMemory->ParentMemory->Evaluator.Behaviors[DecoratorMemory->ParentMemory->Context.BehaviorIndex].LastExecutionTime = GetWorld()->GetTimeSeconds();

		// Set last behavior time for inertia check
		DecoratorMemory->ParentMemory->LastBehaviorChangeTime = GetWorld()->GetTimeSeconds();
		if (DecoratorMemory->ParentMemory->OwnerComp.IsValid())
		{
			DecoratorMemory->ParentMemory->OwnerComp->RequestExecution(EBTNodeResult::Failed);
		}
		else
		{
			UE_LOG(LogIAUS, Warning, TEXT("Behavior Tree Component pointer is not valid for the request execution call."));
		}
	}
}
