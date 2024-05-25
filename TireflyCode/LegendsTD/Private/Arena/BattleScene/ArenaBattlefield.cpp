// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/BattleScene/ArenaBattlefield.h"

#include "ProceduralMeshComponent.h"
#include "Arena/Alliance/AllianceControllerInterface.h"
#include "Arena/Alliance/AllianceManagerComponent.h"


// Sets default values
AArenaBattlefield::AArenaBattlefield(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MapColumn = 11;
	MapRow = PlayerSideRow + EnemySideRow + BattlefieldRow;
}

// Called when the game starts or when spawned
void AArenaBattlefield::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaBattlefield::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//当行、列、大小改变时重新生成棋盘
	if (PropertyChangedEvent.Property->GetName() == "MapRow"
		|| PropertyChangedEvent.Property->GetName() == "MapColumn"
		|| PropertyChangedEvent.Property->GetName() == "GridRadius"
		|| PropertyChangedEvent.Property->GetName() == "PlayerSideRow"
		|| PropertyChangedEvent.Property->GetName() == "BattlefieldRow"
		|| PropertyChangedEvent.Property->GetName() == "EnemySideRow")
	{
		//重新生成棋盘
		GenerateGridMap();
	}
}

UAllianceManagerComponent* AArenaBattlefield::GetAllianceManager() const
{
	if (const IAllianceControllerInterface* ACI = Cast<IAllianceControllerInterface>(GetOwner()))
	{
		return ACI->GetAllianceManager();
	}

	return nullptr;
}

void AArenaBattlefield::GenerateGridMap()
{
	MapRow = PlayerSideRow + EnemySideRow + BattlefieldRow;
	
	Super::GenerateGridMap();

	for (const auto Grid : GridsOfMap)
	{
		if (Grid.Key.CoordY < PlayerSideRow)
		{
			PlayerFormationGrids.Add(Grid.Key);
			Grid.Value->WorldTransform.SetRotation(PlayerSideRotation.Quaternion());
			SetGridMaterial(Grid.Value, DebugMaterial2);
		}
		else if (Grid.Key.CoordY >= PlayerSideRow && Grid.Key.CoordY < PlayerSideRow + BattlefieldRow)
		{
			BattlefieldGrids.Add(Grid.Key);
			SetGridMaterial(Grid.Value, DebugMaterial1);
		}
		else
		{
			EnemyFormationGrids.Add(Grid.Key);
			Grid.Value->WorldTransform.SetRotation(EnemySideRotation.Quaternion());
			SetGridMaterial(Grid.Value, BlockMaterial);
		}
	}
}

bool AArenaBattlefield::IsGridInPlayerFormation(const FFireflyGridCoordinate& Grid) const
{
	return PlayerFormationGrids.Contains(Grid);
}

bool AArenaBattlefield::IsGridInBattlefield(const FFireflyGridCoordinate& Grid) const
{
	return BattlefieldGrids.Contains(Grid);
}

bool AArenaBattlefield::IsGridInEnemyFormation(const FFireflyGridCoordinate& Grid) const
{
	return EnemyFormationGrids.Contains(Grid);
}