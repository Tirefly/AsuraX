// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"


UCLASS()
class LEGENDSTD_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region Actor

public:
	AArenaPlayerState(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

#pragma endregion
};
