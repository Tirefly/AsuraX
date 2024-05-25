// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoreBasics/CoreGameplayTypes.h"
#include "LegendSynergyDataAsset.generated.h"


class ULegendGameplayAbilityAsset;


UCLASS()
class LEGENDSTD_API ULegendSynergyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

protected:
	// 羁绊ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Synergy")
	FName SynergyID;

public:
	// 数据资产类型：羁绊数据资产
	static const FPrimaryAssetType SynergyDataAsset;

#pragma endregion


#pragma region SynergyData

public:
	/** 该羁绊的名称 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Synergy")
	FText SynergyName = FText();

	/** 该羁绊的图标 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Synergy")
	UTexture2D* SynergyIcon;

	/** 该羁绊的游戏性描述 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Synergy")
	FText SynergyGameplayDescription;

	/** 该羁绊的文学性描述 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Synergy")
	FText SynergyLiteratureDescription;

	/** 该羁绊所有相关的单位ID */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Synergy")
	TArray<FName> SynergyUnitIDs = TArray<FName>{};

	/** 该羁绊激活后对战斗单位增强属性的幅度值 */
	UPROPERTY(BlueprintReadOnly, VIsibleAnywhere, Category = "Synergy")
	float SynergyAttributeMagnitude;

#pragma endregion


#pragma region SynergyAbility

public:
	// 羁绊对应阵营的玩家角色的技能能力数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Ability")
	TArray<ULegendGameplayAbilityAsset*> SynergyAbilitySystemConfigs;

	// 羁绊对应阵营的玩家角色的技能能力数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FSynergyAbilitySystemData PlayerAbilitySystemConfigs;

	// 羁绊对应阵营的玩家角色的技能能力数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TMap<FName, FSynergyAbilitySystemData> CombatUnitAbilitySystemConfigs;

#pragma endregion
};
