// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CurveTable.h"
#include "Engine/DataAsset.h"
#include "LegendConfig_GameMode.generated.h"


enum class EHeroRarity : uint8;

UCLASS()
class LEGENDSTD_API ULegendConfig_GameMode : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	ULegendConfig_GameMode(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region GameRound

public:
	// 一局游戏的最大回合数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameRound)
	int32 MaxRoundNumber = 3;

	// 战场上相同英雄的最大数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameRound)
	int32 SameHeroMaxNumOnBattlefield = 2;

#pragma endregion


#pragma region CombatUnit

public:
	// 该游戏模式下英雄的基类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnit)
	TSubclassOf<class ACombatUnit_Hero> HeroBaseClass;

	// 该游戏模式下敌人的基类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnit)
	TSubclassOf<class ACombatUnit_Enemy> EnemyBaseClass;

	// 该游戏模式下召唤物的基类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnit)
	TSubclassOf<class ACombatUnit_Summon> SummonBaseClass;

#pragma endregion


#pragma region Player

public:
	// 根据英雄的人口占用配置
	UFUNCTION(BlueprintPure, Category = Player)
	int32 GetHeroPopulationOccupancy(EHeroRarity Rarity) const;

public:
	// 玩家每次升级，获得的人口数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	FCurveTableRowHandle PopulationPerGrade;

	// 玩家升级需要消耗的金币数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	FCurveTableRowHandle PlayerUpgradeCost;

	// 玩家等级对战斗收益的影响系数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	FCurveTableRowHandle CoefficientOfPlayerGradeOnIncome;

	// 玩家最高等级
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	int32 PlayerMaxGrade = 15;

	// 玩家最大生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	int32 PlayerMaxHealth = 15;

	// 英雄占用的人口数配置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Player)
	TMap<EHeroRarity, int32> HeroPopulationOccupancy;

#pragma endregion


#pragma region Economic

public:
	// 蓝色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic")
	int32 InitialMoney = 15000;

#pragma endregion


#pragma region Economic_ShopRefresh

public:
	// 根据英雄的稀有度获取英雄的购买价格
	UFUNCTION(BlueprintPure, Category = Economic)
	int32 GetHeroShopRefreshCost(EHeroRarity Rarity) const;

	// 根据英雄的稀有度获取英雄的购买价格
	UFUNCTION(BlueprintPure, Category = Economic)
	const FVector&GetHeroShopRefreshWeight(EHeroRarity Rarity) const;

public:
	// 英雄上架数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	int32 HeroShopNumber = 5;

	// 蓝色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	int32 BlueRefreshCost = 150;

	// 紫色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	int32 PurpleRefreshCost = 200;

	// 金色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	int32 GoldRefreshCost = 300;

	// 蓝色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	FVector BlueRefreshWeight = FVector(0.7f, 0.3f, 0.f);

	// 紫色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	FVector PurpleRefreshWeight = FVector(0.35f, 0.5f, 0.15f);

	// 金色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|ShopRefresh")
	FVector GoldRefreshWeight = FVector(0.25f, 0.4f, 0.35f);

#pragma endregion


#pragma region Economic_HeroPrice

public:
	// 根据英雄的稀有度获取英雄的购买价格
	UFUNCTION(BlueprintPure, Category = Economic)
	int32 GetHeroPurchasePrice(EHeroRarity Rarity) const;

	// 根据英雄的稀有度和等级获取英雄的出售价格
	UFUNCTION(BlueprintPure, Category = Economic)
	int32 GetHeroSellingPrice(EHeroRarity Rarity, int32 Grade) const;

	// 蓝色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	int32 BlueHeroPurchasePrice = 150;

	// 紫色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	int32 PurpleHeroPurchasePrice = 200;

	// 金色英雄的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	int32 GoldHeroPurchasePrice = 300;

	// 蓝色英雄出售的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	TMap<int32, int32> BlueHeroSellingPrice;

	// 紫色英雄出售的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	TMap<int32, int32> PurpleHeroSellingPrice;

	// 金色英雄出售的价格
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economic|HeroPrice")
	TMap<int32, int32> GoldHeroSellingPrice;

#pragma endregion
};
