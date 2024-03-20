// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FireflyGameplayAbilityAsset.generated.h"

struct FFireflyAbilityParameter;

/**
 * 
 */
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayAbilityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	UFireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	// 基本数据资产类型：能力数据资产
	static const FPrimaryAssetType GameplayAbilityAsset;

	// 能力的唯一标识，用于数据化驱动
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BasicSetting)
	FName AbilityID = NAME_None;

#pragma endregion


#pragma region BasicSetting

public:
	// 能力的图标
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BasicSetting)
	UTexture2D* AbilityIcon = nullptr;

	// 能力的名称
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = BasicSetting)
	FText AbilityName = FText();

	// 能力的描述（编辑使用）
	UPROPERTY(EditAnywhere, Category = BasicSetting, Meta = (MultiLine = true))
	FString DescriptionToEdit = FString();

	// 能力的描述（显示使用）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicSetting, Meta = (MultiLine = true))
	FText DescriptionToDisplay = FText();
	
	// 能力的描述（编辑使用）
	UPROPERTY(EditAnywhere, Category = BasicSetting, Meta = (GetOptions = GetAbilityParamNames))
	TArray<FName> DescriptionParams;

#pragma endregion


#pragma region CommitmentSetting
#pragma endregion


#pragma region AbilityParameter

public:
	// 编辑器使用，获取该能力数据资产中，所有参数的名称
	UFUNCTION(CallInEditor, BlueprintCallable, Category = AbilityParameter)
	TArray<FString> GetAbilityParamNames() const;

public:
	// 能力的参数
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = AbilityParameter)
	TMap<FName, FFireflyAbilityParameter> AbilityParameters;
	
#pragma endregion
};
