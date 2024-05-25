// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FireflyAbilitySystemTypes.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "FireflyGameplayAbilityAsset.generated.h"


struct FFireflyGameplayEffectRuntimeConfig;


// 游戏性能里的数据资产
// Data asset of gameplay ability
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayAbilityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

#pragma region PrimaryDataAsset

public:
	UFireflyGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintNativeEvent, Meta = (ForceAsFunction, DisplayName = "Post Init Properties"), Category = "TireflyGameplayAbilityParameter")
	void K2_PostInitProperties();
	virtual void K2_PostInitProperties_Implementation() {}

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	FName AbilityID = NAME_None;

	// 能力的显示名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	FText AbilityName = FText::FromString("");

	// 能力的类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	TSubclassOf<class UGameplayAbility> AbilityClass = nullptr;

	// 能力图标
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	TSoftObjectPtr<UTexture2D> AbilityIcon;

	// 能力的描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityBasics)
	class UTireflyAbilityParam_DescriptionBase* AbilityDescription = nullptr;

#pragma endregion


#pragma region AbiliytCommitment

public:
	// 能力的冷却时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbiliytCommitment)
	class UTireflyAbilityParam_CooldownBase* CooldownTime = nullptr;

	// 能力的花费设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbiliytCommitment)
	TArray<class UTireflyAbilityParam_CostBase*> CostSettings;

#pragma endregion


#pragma region AbilityParameter

public:
	// 能力的参数设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AbilityParameters)
	TMap<FName, class UTireflyGameplayAbilityParameter*> AbilityParameters;

#pragma endregion
};
