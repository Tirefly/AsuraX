// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "CombatUnit_Enemy.generated.h"


// 敌人单位的基类
UCLASS()
class LEGENDSTD_API ACombatUnit_Enemy : public ACombatUnitBase
{
	GENERATED_BODY()

#pragma region CombatUnit

public:
	virtual bool CanBeTargeted(ACombatUnitBase* Opponent) const override;

	virtual void InitOnRespawned(APawn* InInstigator, UFireflyGridBase* Grid) override;

protected:
	UPROPERTY()
	bool bCanBeTargeted = false;

#pragma endregion
	

#pragma region GameMode_GameStage

public:
	// 处理战场开战的逻辑
	virtual void HandleStartedBattle() override;

	// 处理战场结束战斗的逻辑
	virtual void HandleFinishedBattle() override;

#pragma endregion
};
