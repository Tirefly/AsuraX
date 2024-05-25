// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "CombatUnit_Summon.generated.h"


// 召唤物单位的基类
UCLASS()
class LEGENDSTD_API ACombatUnit_Summon : public ACombatUnitBase
{
	GENERATED_BODY()

#pragma region CombatUnit_Summon

public:
	UFUNCTION(BlueprintCallable, Category = CombatUnit)
	virtual void InitOnSummoned(UAbilitySystemComponent* OwnerASC, float InitCoefficient);

public:
	// 召唤物主人的能力系统组件
	UPROPERTY(BlueprintReadOnly, Category = CombatUnit)
	TObjectPtr<UAbilitySystemComponent> SummonOwnerASC;
	
	// 属性初始化系数值
	UPROPERTY(BlueprintReadOnly, Category = CombatUnit)
	float AttributeInitCoefficient = 0.f;

#pragma endregion


#pragma region GameMode_GameStage

public:
	// 处理战场开战的逻辑
	virtual void HandleStartedBattle() override;

	// 处理战场结束战斗的逻辑
	virtual void HandleFinishedBattle() override;

protected:
	virtual void HandleGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage) override;

#pragma endregion


#pragma region FireflyObjectPool

public:
	// Actor从对象池中生成后执行的BeginPlay。
	virtual void PoolingBeginPlay_Implementation() override;

	virtual FName PoolingGetActorID_Implementation() const override { return NAME_None; }

#pragma endregion
};
