// Copyright 2017-2020 Project Borealis

#include "IAUS/Public/Components/IAUSBaseHealthComponent.h"

#include "GameFramework/Actor.h"

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