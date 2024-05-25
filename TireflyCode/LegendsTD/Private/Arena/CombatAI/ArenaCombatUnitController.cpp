// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CombatAI/ArenaCombatUnitController.h"

#include "Navigation/CrowdFollowingComponent.h"


AArenaCombatUnitController::AArenaCombatUnitController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AArenaCombatUnitController::BeginPlay()
{
	Super::BeginPlay();
}

void AArenaCombatUnitController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}