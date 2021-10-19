// Copyright Project Borealis. All rights reserved.

#include "Decorators/IAUSBTDecorator_Utility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Composites/IAUSBTComposite_Behavior.h"
#include "Composites/IAUSBTComposite_Utility.h"
#include "Decorators/IAUSBTDecorator_Consideration.h"
#include "IAUS/IAUSCommon.h"

DECLARE_CYCLE_STAT(TEXT("IAUS Decorator Tick Node"), STAT_IAUSDecoratorTickNode, STATGROUP_IAUS);

UIAUSBTDecorator_Utility::UIAUSBTDecorator_Utility(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
	NodeName = TEXT("Utility Ticker");

	bNotifyTick = true;
	bNotifyActivation = true;

	SelectionCooldown = 0.25f;
}

void UIAUSBTDecorator_Utility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_IAUSDecoratorTickNode);

	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FIAUSBTUtilityDecoratorMemory>(NodeMemory);
	if (DecoratorMemory->ParentMemory == nullptr)
	{
		return;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return;
	}

	if ((DecoratorMemory->ParentMemory->LastBehaviorChangeTime + SelectionCooldown) >= GetWorld()->GetTimeSeconds())
	{
		return;
	}

	// Track current Context actor for later comparison
	AActor* CurrentActor = DecoratorMemory->ParentMemory->Context.Actor;

	TArray<AActor*> Targets;
	Controller->GetPerceptionComponent()->GetKnownPerceivedActors(nullptr, Targets);
	Targets.Add(Controller->GetPawn());

	DecoratorMemory->ParentMemory->Context = DecoratorMemory->ParentMemory->Evaluator.ChooseBehavior(Controller, Targets, false);

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

void UIAUSBTDecorator_Utility::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	const UIAUSBTComposite_Utility* UtilityComposite = Cast<UIAUSBTComposite_Utility>(GetMyNode());
	if (!UtilityComposite)
	{
		return;
	}

	if (FIAUSBTUtilityDecoratorMemory* MyMemory = GetNodeMemory<FIAUSBTUtilityDecoratorMemory>(SearchData))
	{
		if (FIAUSBTCompositeUtilityMemory* UtilityCompositeMemory = MyMemory->ParentMemory = UtilityComposite->GetNodeMemory<FIAUSBTCompositeUtilityMemory>(SearchData))
		{
			UtilityCompositeMemory->BehaviorMemories.Empty();

			for (int32 Idx = 0; Idx < UtilityComposite->Children.Num(); Idx++)
			{
				if (const UIAUSBTComposite_Behavior* BehaviorComposite = Cast<UIAUSBTComposite_Behavior>(UtilityComposite->Children[Idx].ChildComposite))
				{
					UtilityCompositeMemory->BehaviorMemories.Add(BehaviorComposite->GetNodeMemory<FIAUSBTCompositeBehaviorMemory>(SearchData));
				}
			}
		}
	}
}
bool UIAUSBTDecorator_Utility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FIAUSBTUtilityDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FIAUSBTUtilityDecoratorMemory>(NodeMemory);

	return !DecoratorMemory->IsInvalid;
}

uint16 UIAUSBTDecorator_Utility::GetInstanceMemorySize() const
{
	return sizeof(FIAUSBTUtilityDecoratorMemory);
}
