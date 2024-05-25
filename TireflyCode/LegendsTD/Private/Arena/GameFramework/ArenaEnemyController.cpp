// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/GameFramework/ArenaEnemyController.h"

#include "Arena/Alliance/AllianceManagerComponent.h"
#include "Arena/GameFramework/ArenaGameMode.h"
#include "Libraries/LegendSystemLibrary.h"
#include "Synergy/SynergyManagerComponent.h"


AArenaEnemyController::AArenaEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SynergyManager = CreateDefaultSubobject<USynergyManagerComponent>(TEXT("SynergyManager"));
	if (IsValid(SynergyManager))
	{}

	AllianceManager = CreateDefaultSubobject<UAllianceManagerComponent>(TEXT("AllianceManager"));
	if (IsValid(AllianceManager))
	{}

	for (UActorComponent* Component : GetComponents())
	{
		Component->PrimaryComponentTick.bCanEverTick = false;
		Component->PrimaryComponentTick.bStartWithTickEnabled = false;
	}
}

void AArenaEnemyController::BeginPlay()
{
	Super::BeginPlay();

	AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this);
	if (IsValid(ArenaGM))
	{
		ArenaGM->HandleEnemyAllianceInitialized(this);
	}
}

USynergyManagerComponent* AArenaEnemyController::GetSynergyManager() const
{
	return ISynergyControllerInterface::GetSynergyManager();
}

APawn* AArenaEnemyController::GetSynergyDirector() const
{
	return GetPawn();
}

void AArenaEnemyController::HandleCombatUnitJoined(AActor* InCombatUnit)
{
	SynergyManager->HandleCombatUnitJoined(InCombatUnit);
}

void AArenaEnemyController::HandleCombatUnitQuit(AActor* InCombatUnit)
{
	SynergyManager->HandleCombatUnitQuit(InCombatUnit);
}

void AArenaEnemyController::HandleEquipmentJoined(AActor* InCombatUnit, FName InEquipmentUnitID)
{
	SynergyManager->HandleEquipmentJoined(InCombatUnit, InEquipmentUnitID);
}

void AArenaEnemyController::HandleEquipmentQuit(APawn* InCombatUnit, FName InEquipmentUnitID)
{
	SynergyManager->HandleEquipmentQuit(InCombatUnit, InEquipmentUnitID);
}