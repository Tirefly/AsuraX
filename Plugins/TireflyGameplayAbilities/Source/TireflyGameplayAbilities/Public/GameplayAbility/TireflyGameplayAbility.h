// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TireflyGameplayAbilityAsset.h"
#include "TireflyGameplayAbility.generated.h"


struct FTireflyAbilityParamInfo;
class UTireflyGameplayAbilityParameter;
class UTireflyAbilityParam_CostBase;


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
	UFUNCTION(BlueprintCallable, Category = Ability, Meta = (DisplayName = "Get Ability Asset"))
	UTireflyGameplayAbilityAsset* K2_GetAbilityAsset() const { return AbilityAsset.Get(); }

	// 获取能力资产
	template<class T>
	T* GetAbilityAsset() const { return Cast<T>(AbilityAsset.Get()); }

	template<class T>
	T* GetAbilityParameter(FName ParamName) const
	{
		if (AbilityAsset)
		{
			return Cast<T>(AbilityAsset->AbilityParameters.FindRef(ParamName));
		}

		return nullptr;
	}

	// 获取该能力的Caster数据
	UFUNCTION(BlueprintPure, Category = Ability)
	virtual FTireflyAbilityParamInfo GetAbilityParamInfo() const;

	UFUNCTION(BlueprintPure, Category = Ability)
	virtual int32 GetCasterLevel() const;

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
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle,
						   const FGameplayAbilityActorInfo* ActorInfo,
						   OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle,
								   const FGameplayAbilityActorInfo* ActorInfo,
								   const FGameplayAbilityActivationInfo ActivationInfo,
								   OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle,
						   const FGameplayAbilityActorInfo* ActorInfo,
						   const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// 允许在应用能力属性消耗之前执行一次属性消耗的计算
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Costs)
	UTireflyAbilityParam_CostBase* GetCostSetting() const;
	virtual UTireflyAbilityParam_CostBase* GetCostSetting_Implementation() const;

#pragma endregion
};
