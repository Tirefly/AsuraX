// Copyright Epic Games, Inc. All Rights Reserved.


#include "Arena/CombatUnits/CombatUnit_Summon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Arena/GameFramework/ArenaGameMode.h"
#include "CoreBasics/CoreGameplayTag.h"
#include "GameplayAbilities/LegendAbilitySystemComponent.h"
#include "GameplayAbilities/GameplayEffects/LegendGameplayEffect_SummonInitializer.h"
#include "Libraries/LegendSystemLibrary.h"


void ACombatUnit_Summon::InitOnSummoned(UAbilitySystemComponent* OwnerASC, float InitCoefficient, float Lifetime)
{
	if (!IsValid(OwnerASC) || InitCoefficient == 0.f)
	{
		return;
	}
	
	SummonOwnerASC = OwnerASC;
	AttributeInitCoefficient = InitCoefficient;

	const FGameplayEffectSpecHandle SpecHandle = SummonOwnerASC->MakeOutgoingSpec(
		ULegendGameplayEffect_SummonInitializer::StaticClass()
		, 1.f, SummonOwnerASC->MakeEffectContext());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle
		, GAS_SETBYCALLER_ATTRIBUTEINITIALIZER, AttributeInitCoefficient);
	SummonOwnerASC->BP_ApplyGameplayEffectSpecToTarget(SpecHandle, AbilitySystem);

	SetCurrentGrade(Cast<ACombatUnitBase>(SummonOwnerASC->GetAvatarActor())->GetCurrentGrade());

	// 设置一个计时器，1秒后再次调用RespawnLambda函数，设置为非无敌状态
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ACombatUnit_Summon::HandleStartedBattle, 0.5f, false);

	if (Lifetime > 0.f)
	{
		FTimerHandle LifetimeTimer;
		GetWorldTimerManager().SetTimer(LifetimeTimer, this, &ACombatUnit_Summon::HandleRecycled, Lifetime, false);
	}
}

void ACombatUnit_Summon::HandleStartedBattle()
{
	if (CombatUnitID == NAME_None)
	{
		return;
	}
	
	ToggleBrainLogic(true);
	ToggleCollision(true);
	ToggleVisibility(true);
}

void ACombatUnit_Summon::HandleFinishedBattle()
{
	if (CombatUnitID == NAME_None)
	{
		return;
	}
	
	RemoveActiveCombatEffects();
	HandleRecycled();
}

void ACombatUnit_Summon::HandleGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage)
{
	if (NewGameStage == ELegendGameStage::InBattle)
	{
		HandleStartedBattle();
	}
	else
	{
		HandleRecycled();
	}
}

void ACombatUnit_Summon:: PoolingBeginPlay_Implementation()
{
	if (const AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this))
	{
		if (ArenaGM->GetCurrentGameStage() != ELegendGameStage::InBattle)
		{
			HandleRecycled();
			return;
		}
	}

	Super::PoolingBeginPlay_Implementation();
}