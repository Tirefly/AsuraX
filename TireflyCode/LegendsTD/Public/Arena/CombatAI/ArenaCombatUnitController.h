// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ArenaCombatUnitController.generated.h"

class UStateTreeComponent;

/** 所有战斗单位使用的控制器的基类 */
UCLASS()
class LEGENDSTD_API AArenaCombatUnitController : public AAIController
{
	GENERATED_BODY()

#pragma region AIController

public:
	AArenaCombatUnitController(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

#pragma endregion
};
