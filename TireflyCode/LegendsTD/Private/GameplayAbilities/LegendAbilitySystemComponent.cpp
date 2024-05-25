// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/LegendAbilitySystemComponent.h"

#include "Arena/CombatUnits/CombatUnitBase.h"
#include "GameplayAbilities/LegendAttributeSet.h"


ULegendAbilitySystemComponent::ULegendAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 ULegendAbilitySystemComponent::GetAbilityCasterLevel() const
{
	if (ACombatUnitBase* CombatUnitBase = Cast<ACombatUnitBase>(GetOwnerActor()))
	{
		return CombatUnitBase->GetCurrentGrade();
	}

	return Super::GetAbilityCasterLevel();
}
