// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/GameFramework/ArenaPlayerCameraManager.h"
#include <Common/LL_CameraFunctionLibrary.h>

void AArenaPlayerCameraManager::UpdateRTSXY()
{
	FVector2D XY = ULL_CameraFunctionLibrary::GetRTSCameraMove(this, ViewportMoveSpeed, EdgeRange);
	CameraCurLocation.X += XY.X;
	CameraCurLocation.Y += XY.Y;
}
