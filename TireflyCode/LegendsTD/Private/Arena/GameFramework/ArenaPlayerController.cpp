// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/GameFramework/ArenaPlayerController.h"

#include "Arena/Alliance/AllianceManagerComponent.h"
#include "Synergy/SynergyManagerComponent.h"


AArenaPlayerController::AArenaPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SynergyManager = CreateDefaultSubobject<USynergyManagerComponent>(TEXT("SynergyManager"));
	if (IsValid(SynergyManager))
	{
	}

	AllianceManager = CreateDefaultSubobject<UAllianceManagerComponent>(TEXT("AllianceManager"));
	if (IsValid(AllianceManager))
	{
	}
}

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
}

USynergyManagerComponent* AArenaPlayerController::GetSynergyManager() const
{
	return SynergyManager;
}

APawn* AArenaPlayerController::GetSynergyDirector() const
{
	return GetPawn();
}

void AArenaPlayerController::HandleCombatUnitJoined(AActor* InCombatUnit)
{
	SynergyManager->HandleCombatUnitJoined(InCombatUnit);
}

void AArenaPlayerController::HandleCombatUnitQuit(AActor* InCombatUnit)
{
	SynergyManager->HandleCombatUnitQuit(InCombatUnit);
}

void AArenaPlayerController::HandleEquipmentJoined(AActor* InCombatUnit, FName InEquipmentUnitID)
{
	SynergyManager->HandleEquipmentJoined(InCombatUnit, InEquipmentUnitID);
}

void AArenaPlayerController::HandleEquipmentQuit(APawn* InCombatUnit, FName InEquipmentUnitID)
{
	SynergyManager->HandleEquipmentQuit(InCombatUnit, InEquipmentUnitID);
}