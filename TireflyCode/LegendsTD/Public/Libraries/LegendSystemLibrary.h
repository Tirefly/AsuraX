// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DataRegistrySubsystem.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "Common/LL_JsonDataManager.h"

#include "LegendSystemLibrary.generated.h"


class ULegendSynergyDataAsset;
class ULegendBuilder_CombatUnit;
struct FGameplayAttribute;
class ULegendGameplayAbilityAsset;


// 项目开发用的游戏性系统相关的函数库
UCLASS()
class LEGENDSTD_API ULegendSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region GameFramework

public:
	/** 获取核心游戏实例 */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject"))
	static class UCoreGameInstance* GetCoreGameInstance(const UObject* WorldContextObject);

	/** 获取局内游戏模式 */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject"))
	static class AArenaGameMode* GetArenaGameMode(const UObject* WorldContextObject);

	/** 获取局内游戏模式的配置 */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject"))
	static class ULegendConfig_GameMode* GetGameModeConfig(const UObject* WorldContextObject);

	/** 获取局内玩家控制器 */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class AArenaPlayerController* GetArenaPlayerController(const UObject* WorldContextObject, int32 PlayerIndex);

	/** 获取局内玩家状态管理器 */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class AArenaPlayerState* GetArenaPlayerState(const UObject* WorldContextObject, int32 PlayerIndex);

#pragma endregion


#pragma region DataTable

public:
	/** 获取特定类型的数据注册表的某一行数据 */
	template <class T>
	static const T* GetCachedTableRow(FName TableType, FName RowID);

	// 添加敌人阵型数据到数据表中
	UFUNCTION(BlueprintCallable, Category = DataTable)
	static void AddEnemyFormatToDataTable(UDataTable* DataTable, const FName& Row, const FLegendEnemyFormatTableRow& Format);

#pragma endregion


#pragma region DataAsset

public:
	// 获取项目的战斗单位配置资产
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static const class ULegendConfig_CombatUnit* GetCombatUnitConfig();

	// 获取项目的战斗单位配置资产
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static const class ULegendConfig_EnemyFormat* GetEnemyFormatConfig();

	/** 根据ID获取羁绊数据资产 */
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static const ULegendSynergyDataAsset* GetSynergyDataAsset(FName SynergyID);

	/** 获取数据配置：战斗单位某个受基础属性影响成长的额外属性的计算参数 */
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static bool GetDerivedAttributeCoefficient(const FGameplayAttribute& Attribute, float& OutCoefficient);

	/** 获取数据配置：战斗单位的实际攻击速度的计算结果值（单位：动画播放倍率） */
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static float CalculateAttackPlayRate(float AttackSpeed);

	/** 获取数据配置：战斗单位的实际攻击速度的计算结果值（单位：秒/次） */
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static float CalculateAttackPerSecond(float AttackSpeed);

#pragma endregion


#pragma region CombatUnitBuilder

public:
	/** 根据ID获取战斗单位构建器 */
	UFUNCTION(BlueprintPure, Category = DataAsset)
	static ULegendBuilder_CombatUnit* GetCombatUnitBuilder(FName CombatUnitID);

	/** 获取某个稀有度的所有英雄战斗单位构建器 */
	UFUNCTION(BlueprintPure, Category = DataAsset, Meta = (WorldContext = "WorldContext"))
	static void GetHeroBuildersByRarity(const UObject* WorldContext, EHeroRarity HeroRarity, TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders);

	/** 获取所有小兵战斗单位构建器 */
	UFUNCTION(BlueprintPure, Category = DataAsset, Meta = (WorldContext = "WorldContext"))
	static void GetMinionBuilders(const UObject* WorldContext, TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders);

	/** 获取所有首领战斗单位构建器 */
	UFUNCTION(BlueprintPure, Category = DataAsset, Meta = (WorldContext = "WorldContext"))
	static void GetBossBuilders(const UObject* WorldContext, TMap<FName, ULegendBuilder_CombatUnit*>& OutBuilders);

#pragma endregion


#pragma region Alliance

public:
	/** 在所有其他阵营中，获取一个距离给定战斗单位距离最近的目标单位 */
	UFUNCTION(BlueprintCallable, Category = Alliance, meta = (WorldContext = "WorldContextObject", DefaultToSelf = InActor))
	static AActor* GetNearestOtherAllianceTarget(const UObject* WorldContextObject, AActor* InActor);

	/** 在所有其他阵营中，获取一个距离给定战斗单位距离最近的目标单位 */
	UFUNCTION(BlueprintCallable, Category = Alliance, meta = (WorldContext = "WorldContextObject", DefaultToSelf = InActor))
	static TArray<AActor*> GetAliveAllies(const UObject* WorldContextObject, AActor* InActor);

#pragma endregion


#pragma region JsonConfig

public:
	/** 获取数据配置：基于输入的特定值*/
	template <class T>
	static T GetCachedJsonConfig(FName JsonKey, int32 ValueIndex = 0);

	/** 获取数据配置：交易英雄的价格数值 */
	UFUNCTION(BlueprintPure, Category = "LegendSystem|JsonConfig")
	static float GetPriceDealingInHero(EHeroRarity HeroRarity, bool IsBuy);

	/** 获取数据配置：交易英雄的价格数值 */
	UFUNCTION(BlueprintPure, Category = "LegendSystem|JsonConfig")
	static float GetPriceDealingInEquipment(bool IsBuy);

#pragma endregion
};

template <class T>
const T* ULegendSystemLibrary::GetCachedTableRow(FName TableType, FName RowID)
{
	const UDataRegistrySubsystem* SubsystemDR = UDataRegistrySubsystem::Get();
	if (!IsValid(SubsystemDR))
	{
		return nullptr;
	}
	return SubsystemDR->GetCachedItem<T>(FDataRegistryId(TableType, RowID));
}

template <class T>
T ULegendSystemLibrary::GetCachedJsonConfig(FName JsonKey, int32 ValueIndex)
{
	T OutResult = 0.f;
	TArray<T> OutResults;
	ULL_JsonDataManager::GetJsonDataManager()->GetJsonDataFromKey(JsonKey, OutResult, OutResults, ValueIndex);
	return OutResult;
}
