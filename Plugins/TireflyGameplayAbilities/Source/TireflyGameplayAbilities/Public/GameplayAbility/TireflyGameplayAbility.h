// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TireflyGameplayAbility.generated.h"


class UTireflyGameplayAbilityParameter;
class UTireflyGameplayAbilityAsset;


/**
 * 扩展了一些数据驱动的功能支持的GameplayAbility
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region GameplayAbility

public:
	UTireflyGameplayAbility(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region GameplayAbility_DataAsset

protected:
	UFUNCTION()
	TArray<FName> GetAbilityParamOptions() const;
	
public:
	UFUNCTION(BlueprintCallable, Category = Ability)
	UTireflyGameplayAbilityAsset* GetAbilityAsset() const { return AbilityAsset; }
	
	UFUNCTION(BlueprintCallable, Category = Ability)
	UTireflyGameplayAbilityParameter* GetAbilityAssetParameter(
		UPARAM(Meta = (GetParamOptions = "GetAbilityParamOptions"))FName ParamName) const;

protected:
	// 能力的唯一标识
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
	FName AbilityId;

	// 能力的数据资产路径，用于Debug确认能力是否绑定了能力数据资产
	UPROPERTY(VisibleAnywhere, Category = Ability)
	FString AbilityAssetPath;

	// 能力的数据资产，用于绑定能力数据
	UPROPERTY(BlueprintReadOnly, Category = Ability)
	TObjectPtr<UTireflyGameplayAbilityAsset> AbilityAsset;

#pragma endregion
};
