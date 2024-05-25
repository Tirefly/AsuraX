// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/GameFramework/ArenaPlayerState.h"


AArenaPlayerState::AArenaPlayerState(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AArenaPlayerState::BeginPlay()
{
	Super::BeginPlay();
}