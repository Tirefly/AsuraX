// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AllianceManagerComponent.generated.h"


class UFireflyGridBase;
class ULegendBuilder_CombatUnit;
class AArenaRecyclingBin;
class AArenaBattlefield;
class AArenaAuditorium;
class ACombatUnit_Hero;
class ULegendConfig_GameMode;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEGENDSTD_API UAllianceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region ActorComponent

public:	
	// Sets default values for this component's properties
	UAllianceManagerComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	APawn* GetOwnerControlledPawn() const;

#pragma endregion


#pragma region GameMode

public:
	// 获取游戏模式的配置
	UFUNCTION(BlueprintCallable, Category = GameMode)
	ULegendConfig_GameMode* GetGameModeConfig() const { return GameModeConfig; }

protected:
	// 游戏模式的配置
	UPROPERTY(Transient)
	TObjectPtr<ULegendConfig_GameMode> GameModeConfig;

#pragma endregion


#pragma region PlayerGrade

protected:
	void SetCurrentGrade(int32 NewGrade);

public:
	// 执行升级操作
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Alliance)
	void PerformUpgrade();
	virtual void PerformUpgrade_Implementation();

	// 当前玩家角色能否升级
	UFUNCTION(BlueprintCallable, Category = Alliance)
	bool CanPerformUpgrade() const;

	// 当前玩家角色能否升级
	UFUNCTION(BlueprintCallable, Category = Alliance)
	float GetCurrentUpgradeCost() const;

	// 获取玩家角色的最大等级
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetMaxGrade() const;

	// 获取玩家角色的最大等级
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetCurrentGrade() const { return CurrentGrade; }

protected:
	// 玩家角色当前等级
	UPROPERTY(Transient)
	int32 CurrentGrade = 0;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerGradeDelegate, int32, NewGrade);
	// 玩家等级更新的委托
	UPROPERTY(BlueprintAssignable, Category = Alliance)
	FPlayerGradeDelegate OnPlayerGradeChanged;

#pragma endregion


#pragma region BattlePopulation

public:
	// 获取当前人口上限
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetCurrentMaxPopulation() const { return CurrentMaxPopulation; }

	// 获取当前人口
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetCurrentPopulation() const { return CurrentPopulation; }

	// 增加指定数量的人口
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void IncreaseMaxPopulation(int32 Addition = 1);

	// 处理英雄加入战场，占用人口的逻辑
	UFUNCTION(BlueprintCallable, Category = Alliance)
	bool HandleHeroOccupyPopulation(ACombatUnit_Hero* InHero);

	// 处理英雄离开战场，不占用人口的逻辑
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void HandleHeroLeavePopulation(ACombatUnit_Hero* HeroLeaved);

protected:
	// 当前人口上限
	UPROPERTY(Transient)
	int32 CurrentMaxPopulation = 0;

	// 当前人口
	UPROPERTY(Transient)
	int32 CurrentPopulation = 0;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerPopulationDelegate, int32, NewPopulation, int32, NewMaxPopulation);
	// 玩家人口更新的委托
	UPROPERTY(BlueprintAssignable, Category = Alliance)
	FPlayerPopulationDelegate OnPlayerPopulationChanged;

#pragma endregion


#pragma region Economic

public:
	// 获得新的金币
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void AcquireMoney(int32 MoneyAcquired);

	// 是否可以消耗金币
	UFUNCTION(BlueprintCallable, Category = Alliance)
	bool CanConsumeMoney(int32 MoneyToConsume) const { return CurrentMoney >= MoneyToConsume; }

	// 消耗金币
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void ConsumeMoney(int32 MoneyToConsume);

	// 获取当前的金币
	UFUNCTION(BlueprintCallable, Category = Alliance)
	int32 GetCurrentMoney() const { return CurrentMoney; }

protected:
	// 当前的金币
	UPROPERTY(Transient)
	int32 CurrentMoney = 0;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMoneyDelegate, int32, NewMoney);
	// 玩家金币更新的委托
	UPROPERTY(BlueprintAssignable, Category = Alliance)
	FPlayerMoneyDelegate OnPlayerMoneyChanged;

#pragma endregion


#pragma region Auditorium

public:
	// 设置备战席
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void SetCurrentAuditorium(AArenaAuditorium* InAuditorium);

	// 获取备战席
	UFUNCTION(BlueprintCallable, Category = Alliance)
	AArenaAuditorium* GetCurrentAuditorium() const { return Auditorium; }

protected:
	// 该玩家的备战席
	UPROPERTY(Transient)
	TObjectPtr<AArenaAuditorium> Auditorium;

#pragma endregion


#pragma region Battlefield

public:
	// 设置战场区域
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void SetCurrentBattlefield(AArenaBattlefield* InBattlefield);

	// 获取战场区域
	UFUNCTION(BlueprintCallable, Category = Alliance)
	AArenaBattlefield* GetCurrentBattlefield() const { return Battlefield; }

protected:
	// 该玩家的战场区域
	UPROPERTY(Transient)
	TObjectPtr<AArenaBattlefield> Battlefield;

#pragma endregion


#pragma region RecyclingBin

public:
	// 设置英雄回收站
	UFUNCTION(BlueprintCallable, Category = Alliance)
	void SetCurrentRecyclingBin(AArenaRecyclingBin* InRecyclingBin);

	// 获取英雄回收站
	UFUNCTION(BlueprintCallable, Category = Alliance)
	AArenaRecyclingBin* GetCurrentRecyclingBin() const;

protected:
	// 该玩家的英雄回收站
	UPROPERTY(Transient)
	TObjectPtr<AArenaRecyclingBin> RecyclingBin;

#pragma endregion


#pragma region HeroMgr

public:
	// 处理玩家尝试购买英雄的逻辑
	UFUNCTION(BlueprintCallable, Category = Alliance)
	bool HandleTryBuyHero(const ULegendBuilder_CombatUnit* CombatUnitBuilder);

	// 处理玩家尝试购买英雄的逻辑
	UFUNCTION(BlueprintCallable, Category = Alliance)
	bool HandleTrySellHero(ACombatUnit_Hero* Hero);

	// 处理英雄被放置在棋格座位的逻辑
	UFUNCTION(BlueprintCallable, Category = Game)
	bool HandleHeroPlacedInGrid(ACombatUnit_Hero* Hero, UFireflyGridBase* NewGrid);

protected:
	// 该玩家管理的英雄单位
	UPROPERTY(Transient)
	TArray<TObjectPtr<ACombatUnit_Hero>> Heroes;

#pragma endregion
};
