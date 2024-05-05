// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TireflyGameplayAbility.generated.h"


class UTireflyGameplayAbilityParameter;
class UTireflyAbilityParam_CostBase;
class UTireflyGameplayAbilityAsset;


/**
 * 扩展了一些功能支持的GameplayAbility，扩展的功能如下：
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


#pragma region AbilityAsset

protected:
	// 获取能力参数选项
	UFUNCTION()
	TArray<FName> GetAbilityParamOptions() const;
	
public:
	// 获取能力资产
	UFUNCTION(BlueprintCallable, Category = Ability)
	UTireflyGameplayAbilityAsset* GetAbilityAsset() const { return AbilityAsset; }

	// 获取指定名称的能力参数
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


#pragma region AbilitySpec

public:
	// 获取该能力的SpecHandle
	UFUNCTION(BlueprintPure, Category = Ability)
	FGameplayAbilitySpecHandle GetAbilitySpecHandle() const { return GetCurrentAbilitySpecHandle(); }

#pragma endregion


#pragma region Commitment_Cooldown

public:
	/** Returns all tags that are currently on cooldown */
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual bool CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo,
									   const bool ForceCooldown,
									   OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	/** Applies CooldownGameplayEffect to the target */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
							   const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo) const override;	

	// 允许在应用能力冷却时间之前执行一次冷却时间的计算，如果你的游戏角色含有类似于“技能冷却缩减”之类的属性，可以考虑使用该函数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Cooldowns)
	float GetCooldownDuration() const;
	virtual float GetCooldownDuration_Implementation() const;

#pragma endregion


#pragma region Commitment_Cost

public:
	/** Returns the gameplay effect used to apply cost */
	virtual UGameplayEffect* GetCostGameplayEffect() const override;

	/** Checks cost. returns true if we can pay for the ability. False if not */
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// 允许在应用能力属性消耗之前执行一次属性消耗的计算
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Costs)
	TArray<UTireflyAbilityParam_CostBase*> GetCostSettings() const;
	virtual TArray<UTireflyAbilityParam_CostBase*> GetCostSettings_Implementation() const;

#pragma endregion
};
