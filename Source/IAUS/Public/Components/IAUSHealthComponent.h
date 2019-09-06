// Copyright 2017-2018 Arran Walker

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "IAUSHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class IAUS_API UIAUSHealthComponent : public UActorComponent
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

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	float GetHealth() const;

	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

	void ReduceHealth(float Damage);
};
