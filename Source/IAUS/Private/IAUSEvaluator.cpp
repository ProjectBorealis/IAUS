// Copyright 2017-2020 Project Borealis

#include "IAUSEvaluator.h"

#include "AIController.h"
#include "GameFramework/Character.h"

#include "Decorators/IAUSBTDecorator_Consideration.h"

void IAUSEvaluator::AddBehavior(const FIAUSBehavior& Behavior, const FString& Name)
{
	int32 Index = Behaviors.Add(Behavior);
	BehaviorNameIndex.Add(Name, Index);
}

FIAUSBehaviorContext IAUSEvaluator::ChooseBehavior(AAIController* Controller, const TArray<AActor*> Targets)
{
	FIAUSBehaviorContext BestContext = {};
	BestContext.TotalScore = 0;

	for (int32 Idx = 0; Idx < Behaviors.Num(); Idx++)
	{
		for (AActor* Actor : Targets)
		{
			if (Controller->GetPawn() == Actor)
			{
				if (!Behaviors[Idx].bTargetSelf)
				{
					continue;
				}
			}
			else
			{
				ETeamAttitude::Type Attitude = Controller->GetTeamAttitudeTowards(*Actor);
				if ((Attitude == ETeamAttitude::Friendly && !Behaviors[Idx].bTargetFriendly) ||
					(Attitude == ETeamAttitude::Neutral && !Behaviors[Idx].bTargetNeutral) ||
					(Attitude == ETeamAttitude::Hostile && !Behaviors[Idx].bTargetHostile))
				{
					continue;
				}
			}

			FIAUSBehaviorContext Context;
			Context.Evaluator = this;
			Context.BehaviorIndex = Idx;
			Context.AIController = Controller;
			Context.Actor = Actor;
			Context.TotalScore = Behaviors[Idx].InitialWeight;

			float CompensationFactor = 1.0 - (1.0 / Behaviors[Idx].Considerations.Num());

			for (auto Consideration : Behaviors[Idx].Considerations)
			{
				float Score = Consideration->Score(Context);
				if (Score <= 0)
				{
					Context.TotalScore = 0;
					break;
				}

				float Modification = (1.0 - Score) * CompensationFactor;
				Context.TotalScore *= Score + (Modification * Score);
			}

			if (Context.TotalScore == 1)
			{
				return Context;
			}

			if (Context.TotalScore > BestContext.TotalScore)
			{
				BestContext = Context;
			}
		}
	}
	return BestContext;
}