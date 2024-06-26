// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TireflyGameplayAbilityAsset.generated.h"


// GameplayAbility的数据资产
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset
	
public:
	UTireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	// 数据资产类型：能力资产
	static const FPrimaryAssetType GameplayAbilityAsset;

#pragma endregion


#pragma region EditorDataProcess

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

#pragma endregion


#pragma region AbilityBasics

public:
	// 能力的唯一标识Id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityBasics)
	FName AbilityId = NAME_None;

	// 能力的显示名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityBasics)
	FText AbilityName = FText::FromString("");

	// 能力的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityBasics)
	TSoftClassPtr<class UGameplayAbility> AbilityClass = nullptr;

	// 能力图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityBasics)
	TSoftObjectPtr<UTexture2D> AbilityIcon;

	// 能力的描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = AbilityBasics)
	TObjectPtr<class UTireflyAbilityParam_DescriptionBase> AbilityDescription = nullptr;

#pragma endregion


#pragma region AbiliytCommitment

public:
	// 能力的冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = AbiliytCommitment)
	TObjectPtr<class UTireflyAbilityParam_CooldownBase> CooldownTime = nullptr;

	// 能力的花费设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = AbiliytCommitment)
	TObjectPtr<class UTireflyAbilityParam_CostBase> CostSetting = nullptr;

#pragma endregion


#pragma region AbilityParameter

public:
	// 能力的参数设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = AbilityParameters)
	TMap<FName, TObjectPtr<class UTireflyGameplayAbilityParameter>> AbilityParameters;

#pragma endregion
};
