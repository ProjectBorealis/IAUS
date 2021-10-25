// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "IAUS/Public/Components/IAUSBaseHealthComponent.h"

#include "IAUSHealthComponent.generated.h"

UCLASS(ClassGroup = AI, meta = (BlueprintSpawnableComponent))
class IAUS_API UIAUSHealthComponent : public UIAUSBaseHealthComponent
{
	GENERATED_BODY()

public:
	UIAUSHealthComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float MaxHealth;

public:
	virtual float GetHealth() const override;

	virtual float GetMaxHealth() const override;

	virtual void Heal(float HealAmount, bool bForce) override;

	virtual void ReduceHealth(float Damage) override;
};
