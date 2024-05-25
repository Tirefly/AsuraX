// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "LegendBuilder_CombatUnit.generated.h"


class ULegendCombatBossConfig;
class ULegendGameplayAbilityAsset;


// 战斗单位构建器资产，包含一个战斗单位包括皮肤、能力、投射物、领域等所有相关数据
UCLASS(BlueprintType)
class LEGENDSTD_API ULegendBuilder_CombatUnit : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:
	// 战斗单位ID，同时作为构建器的ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CombatUnit")
	FName CombatUnitID;

public:
	/** 数据资产类型：战斗单位构建器 */
	static const FPrimaryAssetType CombatUnitBuilder;

#pragma endregion


#pragma region EditorDataProcess

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

#pragma endregion


#pragma region CombatUnitBuilder

public:
	// 战斗单位构建器数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatUnit, Meta = (BaseStruct = "/Script/LegendsTD.CombatUnitBuilderData", ExcludeBaseStruct))
	FInstancedStruct CoreBuilderData;
#pragma endregion


#pragma region CombatUnitAbility

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	ULegendGameplayAbilityAsset* GetSkillConfigAtGrade(int32 Grade) const;

public:
	// 战斗单位的普通攻击能力技能数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Ability")
	ULegendGameplayAbilityAsset* NormalAttackConfig;

	// 战斗单位的技能能力数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Ability")
	TArray<ULegendGameplayAbilityAsset*> SkillConfigs;

#pragma endregion


#pragma region CombatUnitBoss

public:
	// 战斗单位作为Boss的配置数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Boss")
	ULegendCombatBossConfig* BossConfig;

#pragma endregion


#pragma region CombatUnitSkin

public:
	// 战斗单位的默认皮肤数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skin")
	FLegendCombatUnitSkinData DefaultSkinData;

	// 战斗单位的皮肤数据表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skin")
	TMap<FName, FLegendCombatUnitSkinData> SkinsData;

#pragma endregion
};
