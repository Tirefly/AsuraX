// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/BattleScene/ArenaRecyclingBin.h"

#include "Arena/Alliance/AllianceControllerInterface.h"


// Sets default values
AArenaRecyclingBin::AArenaRecyclingBin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AArenaRecyclingBin::BeginPlay()
{
	Super::BeginPlay();
	
}

UAllianceManagerComponent* AArenaRecyclingBin::GetAllianceManager() const
{
	if (const IAllianceControllerInterface* ACI = Cast<IAllianceControllerInterface>(GetOwner()))
	{
		return ACI->GetAllianceManager();
	}

	return nullptr;
}
