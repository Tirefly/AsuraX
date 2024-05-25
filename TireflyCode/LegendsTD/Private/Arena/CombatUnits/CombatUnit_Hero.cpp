// Copyright Epic Games, Inc. All Rights Reserved.


#include "Arena/CombatUnits/CombatUnit_Hero.h"

#include "FireflyGridMovementComponent.h"
#include "Arena/BattleScene/ArenaAuditorium.h"
#include "Arena/BattleScene/ArenaBattlefield.h"
#include "Arena/GameFramework/ArenaGameMode.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"
#include "DataAsset/LegendConfig_GameMode.h"
#include "Libraries/LegendSystemLibrary.h"


EHeroRarity ACombatUnit_Hero::GetHeroRarity() const
{
	const FHeroBuilderData* HeroBuilderData = CombatUnitBuilder->CoreBuilderData.GetPtr<FHeroBuilderData>();
	if (!HeroBuilderData)
	{
		return EHeroRarity::Max;
	}
	return HeroBuilderData->Rarity;
}

int32 ACombatUnit_Hero::GetHeroPopulation() const
{
	if (const ULegendConfig_GameMode* ConfigGM = ULegendSystemLibrary::GetGameModeConfig(this))
	{
		return ConfigGM->GetHeroPopulationOccupancy(GetHeroRarity());
	}

	return (int32)GetHeroRarity() + 1;
}

void ACombatUnit_Hero::BackToBattleGridAfterBattleFinished()
{
	if (IsValid(BattleGrid))
	{
		GridMovement->StopMovementImmediately();
		SetCurrentMovementGrid(BattleGrid);
	}
}

void ACombatUnit_Hero::HandlePlacedInNewGrid()
{
	UFireflyGridBase* Grid = GetCurrentMovementGrid();
	if (!IsValid(Grid))
	{
		return;
	}

	if (AArenaAuditorium* Auditorium = Cast<AArenaAuditorium>(Grid->GridMap))
	{
		BattleGrid = nullptr;
		ToggleSynergy(false);
		SetCurrentAlliance(nullptr);
	}

	if (AArenaBattlefield* Battlefield = Cast<AArenaBattlefield>(Grid->GridMap))
	{
		BattleGrid = Grid;
		BackToOriginAlliance();
		ToggleSynergy(true);
	}
}

void ACombatUnit_Hero::HandleIsSold()
{
	if (IsValid(GridMovement))
	{
		GridMovement->SetCurrentGrid(nullptr);
	}
		
	// TODO: 所有武器回归武器库
	HandleRecycled();
}

void ACombatUnit_Hero::SetCurrentMovementGrid(UFireflyGridBase* InGrid)
{
	Super::SetCurrentMovementGrid(InGrid);

	HandlePlacedInNewGrid();
}

void ACombatUnit_Hero::HandleStartedBattle()
{
	if (!IsValid(GetCurrentMovementGrid()) || !IsValid(GetCurrentAlliance()))
	{
		return;
	}

	if (!GetCurrentMovementGrid()->GridMap->IsA(AArenaBattlefield::StaticClass()))
	{
		return;
	}

	//ToggleCollideWithPawn(ECR_Block);
	ToggleBrainLogic(true);
}

void ACombatUnit_Hero::HandleFinishedBattle()
{
	ToggleBrainLogic(false);
	ToggleAliveState(true);
	ToggleVisibility(true);
	ToggleCollision(true);

	// TODO: 英雄法力值按照技能、装备、羁绊效果回复
	
	RemoveActiveCombatEffects();
	RecoverHealthByPercent(1.f);
	RecoverManaByPercent(0.f);
	BackToBattleGridAfterBattleFinished();
}
