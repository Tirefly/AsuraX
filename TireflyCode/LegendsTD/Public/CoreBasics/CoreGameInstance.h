// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"


class ULegendBuilder_CombatUnit;
class UFireflyEffect;
class UFireflyAbility;
class UFireflyAttribute;
class UDataTable;


// 项目的核心游戏实例
UCLASS()
class LEGENDSTD_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region GameInstance

public:
	UCoreGameInstance(const FObjectInitializer& ObjectInitializer);

	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;

#pragma endregion


#pragma region CombatUnitBuilders

protected:
	UFUNCTION()
	void HandleCombatUnitBuildersLoaded(TArray<FPrimaryAssetId> AssetsLoaded);

public:
	// 蓝色英雄构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> BlueHeroBuilders;

	// 紫色英雄构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> PurpleHeroBuilders;

	// 金色英雄构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> GoldHeroBuilders;

	// 首领单位构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> BossBuilders;

	// 小兵单位构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> MinionBuilders;

	// 召唤物单位构建器
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legend")
	TMap<FName, ULegendBuilder_CombatUnit*> SummonBuilders;

#pragma endregion


#pragma region Synergy

protected:
	UFUNCTION()
	void HandleSynergyDataAssetsLoaded(TArray<FPrimaryAssetId> AssetsLoaded);

#pragma endregion
};
