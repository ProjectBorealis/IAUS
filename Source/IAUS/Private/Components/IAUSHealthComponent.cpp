// Copyright 2017-2018 Arran Walker

#include "IAUS/Public/Components/IAUSHealthComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UIAUSHealthComponent::UIAUSHealthComponent()
{
	MaxHealth = 100;
}

// Called when the game starts
void UIAUSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UIAUSHealthComponent::HandleTakeAnyDamage);
	}

	Health = MaxHealth;
}

void UIAUSHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
}

void UIAUSHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
}

float UIAUSHealthComponent::GetHealth() const
{
	return Health;
}

float UIAUSHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UIAUSHealthComponent::ReduceHealth(float Damage)
{
	Health = Health - Damage;

	if (Health <= KINDA_SMALL_NUMBER)
	{
		Health = 0.0f;
	}
}