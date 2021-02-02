// Copyright 2017-2020 Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "IAUSBaseHealthComponent.generated.h"

UCLASS(ClassGroup = AI, meta = (BlueprintSpawnableComponent))
class IAUS_API UIAUSBaseHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	virtual void Heal(float HealAmount, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	virtual void ReduceHealth(float Damage);
};
