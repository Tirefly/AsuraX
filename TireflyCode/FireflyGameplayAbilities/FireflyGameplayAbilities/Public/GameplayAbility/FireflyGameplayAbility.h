// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireflyAbilitySystemTypes.h"
#include "FireflyGameplayAbilityAsset.h"
#include "GameplayEffectContainer/FireflyGameplayEffectContainer.h"
#include "InputActionValue.h"
#include "GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"

#include "FireflyGameplayAbility.generated.h"


class UTireflyGameplayAbilityParameter;
struct FFireflyMagnitude_MathExpression;
struct FFireflyGameplayEffectRuntimeConfig;
enum class ETriggerEvent : uint8;


// 基于GAS扩展的游戏性能力，在GameplayAbility的基础上进行了如下扩展：
// * 适应EnhancedInput的能力输入绑定
// * 更加友好的能力冷却处理和游戏性效果持续时间处理
// * 更加友好的能力花费处理
//
// GameplayAbility extended from the GAS (Gameplay Ability System) that provides the following enhancements:
// * Adaptation to ability input bindings with EnhancedInput
// * Friendly ability cooldown handling and gameplay effect duration handling
// * Friendly ability cost handling
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region GameplayAbility

public:
	UFireflyGameplayAbility(const FObjectInitializer& ObjectInitializer);

protected:
	friend class UFireflyAbilitySystemComponent;

public:
	// 如果一个能力被标记为'ActivateAbilityOnGranted'，在此处立即激活它们
	// Epic的注释：项目可能希望在此处初始化被动能力或执行其他"BeginPlay"类型的逻辑。
	// 
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion


#pragma region GameplayAbility_DataAsset

public:
	// 获取游戏性能力的数据资产
	// Get GameplayAbility's data asset
	UFUNCTION(BlueprintPure, Category = Ability, Meta = (DisplayName = "Get Ability Asset"))
	UFireflyGameplayAbilityAsset* K2_GetAbilityAsset() const { return AbilityAsset.Get(); }

	// 获取游戏性能力的数据资产
	// Get GameplayAbility's data asset
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
	void GetAbilityParamInfo(FTireflyAbilityParamInfo& ParamInfo);

protected:
	// 能力的唯一标识
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability)
	FName AbilityId;

	// 能力的数据资产路径，用于Debug确认能力是否绑定了能力数据资产
	UPROPERTY(VisibleAnywhere, Category = Ability)
	FString AbilityAssetPath;
	
	// 游戏性能力的数据资产
	// GameplayAbility's data asset
	UPROPERTY(Transient)
	TObjectPtr<UFireflyGameplayAbilityAsset> AbilityAsset;

#pragma endregion


#pragma region GameplayAbility_Granting

protected:
	// 当能力被赋予给一个AbilitySystemComponent时处理的逻辑，可以被当作能力的BeginPlay（实际上是在能力的AvatarActor被设置后触发）
	// Handle when an ability is granted to an AbilitySystemComponent, can be treated as the BeginPlay of the ability (actually triggered after the AvatarActor of the ability is set)
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void OnAbilityGranted();
	virtual void OnAbilityGranted_Implementation();

protected:
	// 是否在能力被成功赋予后立刻激活能力
	// Whether activate ability immediately on ability is granted to an AbilitySystemComponent
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	bool bActivateAbilityOnGranted;

#pragma endregion


#pragma region GameplayAbility_Activation

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// 是否在能力激活时提交能力的消耗和冷却 
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	bool bCommitAbilityOnActivated = true;

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


#pragma region GameplayAbility_InputBinding

protected:
	/**
	 * 当能力输入开始时触发的事件，若能力会被输入开始事件激活，则该事件不会在能力刚刚激活时触发。
	 * 若设置了输入激活能力策略，则 ActivateAbility() 有相同的作用
	 *
	 * Event triggered when the ability input is started, if the ability will be activated by input start event
	 * , this event will not be triggered when the ability has just been activated.
	 * If InputActivationPolicy is set, can use function ActivateAbility()
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Started"))
	void OnAbilityInputStarted(const FInputActionValue& Value);
	virtual void OnAbilityInputStarted_Implementation(const FInputActionValue& Value) {}

	// 当能力输入持续中时触发的事件
	// Event triggered when the ability input is ongoing
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Ongoing"))
	void OnAbilityInputOngoing(const FInputActionValue& Value);
	virtual void OnAbilityInputOngoing_Implementation(const FInputActionValue& Value) {}

	// 当能力输入被取消时触发的事件
	// Event triggered when the ability input is canceled
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Canceled"))
	void OnAbilityInputCanceled(const FInputActionValue& Value);
	virtual void OnAbilityInputCanceled_Implementation(const FInputActionValue& Value) {}

	/**
	 * 当能力输入被触发时触发的事件，若能力会被输入开始事件激活，则该事件不会在能力刚刚激活时触发
	 * 若设置了输入激活能力策略，则 ActivateAbility() 有相同的作用
	 *
	 * Event triggered when the ability input is triggered, if the ability will be activated by input start event
	 * , this event will not be triggered when the ability has just been activated.
	 * If InputActivationPolicy is set, can use function ActivateAbility()
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Triggered"))
	void OnAbilityInputTriggered(const FInputActionValue& Value);
	virtual void OnAbilityInputTriggered_Implementation(const FInputActionValue& Value) {}

	// 当能力输入完成时触发的事件
	// Event triggered when the ability input is completed
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Completed"))
	void OnAbilityInputCompleted(const FInputActionValue& Value);
	virtual void OnAbilityInputCompleted_Implementation(const FInputActionValue& Value) {}

	// 当能力输入事件被触发的事件
	// Event triggered when the ability input is completed
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Abilities", meta = (DisplayName = "Ability Input Event"))
	void OnAbilityInputEvent(ETriggerEvent InputEvent, const FInputActionValue& Value);
	virtual void OnAbilityInputEvent_Implementation(ETriggerEvent InputEvent, const FInputActionValue& Value) {}

protected:
	// 输入激活策略，用于激活能力的输入方式
	// Input activation policy for activating the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", Meta = (Bitmask, BitmaskEnum = "/Script/FireflyGameplayAbilities.EFireflyAbilityInputActivationPolicy"))
	uint8 InputActivationPolicy;

#pragma endregion


#pragma region GameplayEffect_EffectContainer

public:
	// 使用传递的容器来生成稍后应用的GameplayEffect容器Spec
	// Make gameplay effect container spec to be applied later, using the passed in container
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FFireflyGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FFireflyGameplayEffectContainer& Container
		, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	// 从EffectContainerMap中搜索并生成稍后应用的GameplayEffect容器Spec
	// Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FFireflyGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData
		, int32 OverrideGameplayLevel = -1);

	// 应用创建好的GameplayEffect容器Spec
	// Applies a gameplay effect container spec that was previously created
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FFireflyGameplayEffectContainerSpec& ContainerSpec);

	// 通过创建并应用容器Spec来应用GameplayEffect容器
	// Applies a gameplay effect container, by creating and then applying the container spec
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData
		, int32 OverrideGameplayLevel = -1);

protected:
	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FFireflyGameplayEffectContainer> EffectContainerMap;

#pragma endregion
};