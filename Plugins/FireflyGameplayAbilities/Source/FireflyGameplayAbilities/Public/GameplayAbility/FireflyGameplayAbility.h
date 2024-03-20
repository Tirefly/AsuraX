// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireflyGameplayAbility.generated.h"

class UFireflyGameplayAbilityAsset;
struct FInstancedStruct;

/**
 * 
 */
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region GameplayAbility

public:
	UFireflyGameplayAbility(const FObjectInitializer& ObjectInitializer);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#pragma endregion
	

#pragma region DataAsset

public:
	// 获取能力数据资产中的属性细则
	UFUNCTION(BlueprintPure, Category = Ability)
	bool GetParamSpecByAsset(FName ParamName, FInstancedStruct& OutParamSpec) const;

	UFUNCTION(BlueprintPure, Category = Ability, Meta = (AdvancedDisplay = "2"))
	bool GetParamSpecByAssetAdvanced(FName ParamName, FInstancedStruct& OutParamSpec, int32 AbilityLevel = 1, int32 CasterLevel = 1) const;

	// 获取能力使用者的等级
	UFUNCTION(BlueprintPure, Category = Ability)
	int32 GetAbilityCasterLevel() const;

protected:
	// 能力ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
	FName AbilityID = NAME_None;

	// 能力数据资产，能力ID被设置时从AssetManager中找到并绑定
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Ability)
	UFireflyGameplayAbilityAsset* AbilityAsset = nullptr;
	
#pragma endregion	
};
