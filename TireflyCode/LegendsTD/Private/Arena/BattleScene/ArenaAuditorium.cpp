// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/BattleScene/ArenaAuditorium.h"

#include "ProceduralMeshComponent.h"
#include "Arena/Alliance/AllianceManagerComponent.h"
#include "Arena/Alliance/AllianceControllerInterface.h"


// Sets default values
AArenaAuditorium::AArenaAuditorium(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	MapRow = 1;
	MapColumn = 13;
}

// Called when the game starts or when spawned
void AArenaAuditorium::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaAuditorium::GenerateGridMap()
{
	Super::GenerateGridMap();

	for (const auto Grid : GridsOfMap)
	{
		Grid.Value->WorldTransform.SetRotation(SpawnRotation.Quaternion());
	}
}

UAllianceManagerComponent* AArenaAuditorium::GetAllianceManager() const
{
	if (const IAllianceControllerInterface* ACI = Cast<IAllianceControllerInterface>(GetOwner()))
	{
		return ACI->GetAllianceManager();
	}

	return nullptr;
}
