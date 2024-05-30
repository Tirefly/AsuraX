// Copyright Epic Games, Inc. All Rights Reserved.


#include "Arena/CombatUnits/CombatUnit_Enemy.h"

#include "Arena/GameFramework/ArenaGameMode.h"
#include "CoreBasics/CoreGameplayTag.h"
#include "Libraries/LegendSystemLibrary.h"


bool ACombatUnit_Enemy::CanBeTargeted(ACombatUnitBase* Opponent) const
{
	return Super::CanBeTargeted(Opponent) && bCanBeTargeted;
}

void ACombatUnit_Enemy::InitOnRespawned(APawn* InInstigator, UFireflyGridBase* Grid)
{
    Super::InitOnRespawned(InInstigator, Grid);
	if (const AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this))
	{
		if (ArenaGM->GetCurrentGameStage() == ELegendGameStage::InBattle)
		{
			HandleStartedBattle();
		}
	}
	
    // 定义一个lambda函数用于处理进入准备战斗状态
	auto RespawnLambda = [this]()
	{
		bCanBeTargeted = true;
		AbilitySystem->RemoveLooseGameplayTag(GAS_COMBATUNITSTATE_INVINCIBLE);
	};

	// 首先设置为无敌状态
	bCanBeTargeted = false;
    if (!AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_INVINCIBLE))
    {
    	AbilitySystem->AddLooseGameplayTag(GAS_COMBATUNITSTATE_INVINCIBLE);
    }
	
	// 设置一个计时器，1秒后再次调用RespawnLambda函数，设置为非无敌状态
	FTimerHandle RespawnTimer;
	const FTimerDelegate RespawnDelegate = FTimerDelegate::CreateLambda(RespawnLambda);
	GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, 1.f, false);
}


void ACombatUnit_Enemy::HandleStartedBattle()
{
	if (CombatUnitID == NAME_None)
	{
		return;
	}
	
	ToggleBrainLogic(true);
	ToggleCollision(true);
	ToggleVisibility(true);
}

void ACombatUnit_Enemy::HandleFinishedBattle()
{
	if (CombatUnitID == NAME_None)
	{
		return;
	}
	
	RemoveActiveCombatEffects();
	HandleRecycled();
}