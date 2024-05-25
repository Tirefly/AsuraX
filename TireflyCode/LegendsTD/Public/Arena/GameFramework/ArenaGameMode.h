// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "GameFramework/GameMode.h"
#include "ArenaGameMode.generated.h"


class ULegendConfig_GameMode;
class AArenaPlayerController;
class AArenaEnemyController;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAllianceEntranceDelegate, AController*, Alliance, AActor*, CombatUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameStageUpdateDelegate, ELegendGameStage, NewGameStage, ELegendGameStage, OldGameStage);


/** 阵营其所属的战斗单位的容器 */
USTRUCT(BlueprintType)
struct FArenaAllianceUnitContainer
{
	GENERATED_BODY()

	// 阵营中所有活着的战斗单位
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> AllianceAliveUnits = TArray<AActor*>{};

	// 阵营中所有死亡的战斗单位
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> AllianceDeadUnits = TArray<AActor*>{};

	FArenaAllianceUnitContainer() {}

	FArenaAllianceUnitContainer(const TArray<AActor*>& Alive, const TArray<AActor*>& Dead)
		: AllianceAliveUnits(Alive), AllianceDeadUnits(Dead){}

	bool operator==(const FArenaAllianceUnitContainer& Other) const
	{
		return AllianceAliveUnits == Other.AllianceAliveUnits
			&& AllianceDeadUnits == Other.AllianceDeadUnits;
	}
};


/** 游戏局内的GameMode */
UCLASS()
class LEGENDSTD_API AArenaGameMode : public AGameMode
{
	GENERATED_BODY()

#pragma region GameMode

public:
	AArenaGameMode(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartPlay() override;

#pragma endregion


#pragma region GameMode_Config

public:
	// 获取游戏模式的配置
	UFUNCTION(BlueprintCallable, Category = GameMode)
	ULegendConfig_GameMode* GetGameModeConfig() const { return GameModeConfig; }

protected:
	// 游戏模式的配置
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	TObjectPtr<ULegendConfig_GameMode> GameModeConfig;

#pragma endregion


#pragma region GameStage

public:
	// 获取当前的游戏阶段
	UFUNCTION(BlueprintCallable, Category = GameMode)
	ELegendGameStage GetCurrentGameStage() const { return CurrentStage; }

	// 设置当前的游戏阶段
	UFUNCTION(BlueprintCallable, Category = GameMode)
	void SetCurrentGameStage(ELegendGameStage InGameStage);

	// 当前游戏阶段更新之前的逻辑
	UFUNCTION(BlueprintNativeEvent, Category = GameMode)
	void PreGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage);
	virtual void PreGameStageUpdated_Implementation(ELegendGameStage NewGameStage , ELegendGameStage OldGameStage) {}

	// 当前游戏阶段更新之后的逻辑
	UFUNCTION(BlueprintNativeEvent, Category = GameMode)
	void PostGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage);
	virtual void PostGameStageUpdated_Implementation(ELegendGameStage NewGameStage , ELegendGameStage OldGameStage) {}

protected:
	// 当前的游戏阶段
	UPROPERTY()
	ELegendGameStage CurrentStage = ELegendGameStage::Loading;

public:
	// 当游戏阶段更新时触发的委托
	UPROPERTY(BlueprintAssignable, Category = GameMode)
	FGameStageUpdateDelegate OnGameStageUpdated;

#pragma endregion


#pragma region Alliance_Management

public:
	/** 处理战斗单位加入或更换阵营的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleCombatUnitJoinAlliance(AActor* CombatUnit, AController* NewAlliance, AController* OldAlliance);

	/** 处理战斗单位脱离阵营的逻辑，当且仅当战斗单位被销毁时调用 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleCombatUnitQuitAlliance(AActor* CombatUnit, AController* Alliance);

	/** 获取所有阵营和该阵营当前的战斗单位的容器集合 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	TMap<AController*, FArenaAllianceUnitContainer> GetAllianceUnitContainers() const { return AllianceUnitContainers; }

	/** 获取所有阵营和该阵营当前的战斗单位的容器集合 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	bool IsCombatUnitInAlliance(AActor* CombatUnit, AController* Alliance) const;
	
	/** 处理战斗单位复活时的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleCombatUnitReborn(AController* Alliance, AActor* CombatUnit);

	/** 蓝图实现：处理战斗单位复活时的逻辑 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode", Meta = (DisplayName = "Handle Combat Unit Reborn"))
	void ReceiveHandleCombatUnitReborn(AController* Alliance, AActor* CombatUnit);

	/** 处理战斗单位死亡时的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void HandleCombatUnitDead(AController* Alliance, AActor* CombatUnit);

	/** 蓝图实现：处理战斗单位死亡时的逻辑 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode", Meta = (DisplayName = "Handle Combat Unit Dead"))
	void ReceiveHandleCombatUnitDead(AController* Alliance, AActor* CombatUnit);

protected:
	/** 所有阵营和该阵营当前的战斗单位的容器集合 */
	UPROPERTY()
	TMap<AController*, FArenaAllianceUnitContainer> AllianceUnitContainers;

public:
	/** 战斗单位加入阵营触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FAllianceEntranceDelegate OnCombatUnitEnterAlliance;

	/** 战斗单位离开阵营触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FAllianceEntranceDelegate OnCombatUnitQuitAlliance;

	/** 在某个阵营的战斗单位死亡时触发的代理 */
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FAllianceEntranceDelegate OnCombatUnitDead;

#pragma endregion


#pragma region PlayerAlliance

public:
	/** 当有玩家阵营初始化结束时调用的函数 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameMode")
	void HandlePlayerAllianceInitialized(AArenaPlayerController* Player);
	virtual void HandlePlayerAllianceInitialized_Implementation(AArenaPlayerController* Player);

protected:
	UPROPERTY()
	TArray<TObjectPtr<AArenaPlayerController>> PlayerAlliances;

#pragma endregion


#pragma region EnemyAlliance

public:
	/** 当有敌人AI阵营初始化结束时调用的函数 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameMode")
	void HandleEnemyAllianceInitialized(AArenaEnemyController* Enemy);
	virtual void HandleEnemyAllianceInitialized_Implementation(AArenaEnemyController* Enemy);

protected:
	UPROPERTY()
	TArray<TObjectPtr<AArenaEnemyController>> EnemyAlliances;

#pragma endregion


#pragma region Alliance_Settings

protected:
	// 查看网络重播时使用的EnemyController类。
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Alliance)
	TSubclassOf<AArenaEnemyController> EnemyControllerClass;

	// 敌人AI阵营的数量
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Alliance)
	int32 PlayerAllianceNum = 1;

	// 敌人AI阵营的数量
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Alliance)
	int32 EnemyAllianceNum = 1;

#pragma endregion
};