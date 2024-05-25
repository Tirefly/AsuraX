// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ArenaPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDSTD_API AArenaPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void UpdateRTSXY();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector CameraCurLocation = FVector(0.0f,0.0f,1000.0f);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRotator CameraCurRotation = FRotator(0.0f,-90.0f,0.0f);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CameraCurFOV = 90.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ViewportMoveSpeed = 50.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float EdgeRange = 0.01f;
};
