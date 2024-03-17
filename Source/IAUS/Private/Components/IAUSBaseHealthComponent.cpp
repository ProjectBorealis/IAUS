// Copyright Project Borealis

#include "Components/IAUSBaseHealthComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IAUSBaseHealthComponent)

void UIAUSBaseHealthComponent::Heal(float HealAmount, bool bForce) {}

float UIAUSBaseHealthComponent::GetHealth() const
{
	return 0.0f;
}

float UIAUSBaseHealthComponent::GetMaxHealth() const
{
	return 100.0f;
}

void UIAUSBaseHealthComponent::ReduceHealth(float Damage) {}
