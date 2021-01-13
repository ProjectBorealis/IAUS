// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Components/IAUSHealthComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UIAUSHealthComponent::UIAUSHealthComponent()
{
	MaxHealth = 100.0f;
}

// Called when the game starts
void UIAUSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UIAUSHealthComponent::Heal(float HealAmount, bool bForce)
{
	if (!bForce && (HealAmount <= 0.0f || Health <= 0.0f))
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