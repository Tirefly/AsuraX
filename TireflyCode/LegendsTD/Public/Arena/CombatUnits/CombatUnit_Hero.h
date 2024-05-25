// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "CombatUnit_Hero.generated.h"


class UFireflyGridBase;
class AArenaHeroSeat;


// 英雄单位的基类
UCLASS()
class LEGENDSTD_API ACombatUnit_Hero : public ACombatUnitBase
{
	GENERATED_BODY()

#pragma region HeroRarity

public:
	// 获取英雄的品阶
	UFUNCTION(BlueprintPure, Category = CombatUnit)
	EHeroRarity GetHeroRarity() const;

	// 获取英雄的品阶计算后的人口占用
    UFUNCTION(BlueprintPure, Category = CombatUnit)
    int32 GetHeroPopulation() const;

#pragma endregion


#pragma region GridMap

public:
	/** 将英雄设置到在战斗准备阶段的棋格位置 */
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void BackToBattleGridAfterBattleFinished();

	// 处理英雄单位被放置在新的棋格的逻辑
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void HandlePlacedInNewGrid();

	// 处理英雄单位被卖掉的逻辑
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	void HandleIsSold();

	// 设置战斗单位当前所属的棋格位置
	virtual void SetCurrentMovementGrid(UFireflyGridBase* InGrid) override;

	// 处理战场开战的逻辑
	virtual void HandleStartedBattle() override;

	// 处理战场结束战斗的逻辑
	virtual void HandleFinishedBattle() override;

protected:
	UPROPERTY(Transient)
	UFireflyGridBase* BattleGrid = nullptr;

#pragma endregion
};
