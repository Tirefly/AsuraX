// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TireflyAbilitySystemComponent.generated.h"


class UTireflyAttributeSet;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTireflyGameplayAbilityDelegate, const FGameplayAbilitySpecHandle, AbilityHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTireflyGameplayAbilityCooldownDelegate, const FGameplayAbilitySpecHandle, AbiltyHandle, float, NewTimeRemaining, float, TotalDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTireflyGameplayAbilityFailedDelegate, const FGameplayAbilitySpecHandle, AbilityHandle, const FGameplayTagContainer&, EventTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTireflyGameplayAbilityEndedDelegate, const FGameplayAbilitySpecHandle, AbilityHandle, bool, bWasCanceled);


/**
 * 
 */
UCLASS(ClassGroup = AbilitySystem, Meta = (BlueprintSpawnableComponent))
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region AbilitySystemComponent

public:
	UTireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region AbilityActorInfo

public:
	// 初始化能力系统组件的ActorInfo，只能在本地客户端和服务端调用，在模拟端调用无效
	UFUNCTION(BlueprintCallable, Category = Ability, Meta = (DisplayName = "Init Ability Actor Info"))
	void K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	// 获取施法者等级
	UFUNCTION(BlueprintPure, Category = Ability)
	virtual int32 GetAbilityCasterLevel() const { return 0; }

#pragma endregion


#pragma region GameplayAttribute_Initialization

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	const UTireflyAttributeSet* AddAttributeSet(TSubclassOf<UTireflyAttributeSet> AttributeSetClass);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	void RemoveAttributeSet(TSubclassOf<UTireflyAttributeSet> AttributeSetClass);
	
	/**
	 * 利用数据表的指定行初始化属性数据，数据表行结构可以在蓝图创建
	 * ，数据表行结构中的变量名必须与属性集中要被初始化的属性名一致
	 * @param AttributeSetClass 要用来初始化的属性集的类
	 * @param DataTable 要用来初始化属性集的数据表引用
	 * @param RowName 数据表中用来初始化属性集的行名
	 * @param bIgnoreGameplayEffect 如果bIgnoreGameplayEffect为true，则完全无视GameplayEffect，覆盖属性的BaseValue和CurrentValue；
	 * 若为false，则仅覆盖BaseValue且不影响GameplayEffect对属性的修改
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	void InitAttributeSetFromDataTableRow(TSubclassOf<UTireflyAttributeSet> AttributeSetClass
		, const UDataTable* DataTable, FName RowName, bool bIgnoreGameplayEffect = false);

#pragma endregion


#pragma region GameplayAttribute_Modification

public:
	// 该函数设置属性的基础值。需要注意的是，现有的活动GameplayEffectModifier不会被清除，它们将继续基于新的基础值影响属性。
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes", Meta = (DisplayName = "Set Numeric Attribute Base"))
	void K2_SetNumericAttributeBase(FGameplayAttribute Attribute, float NewBaseValue);

	/**
	 * 对给定属性应用就地修改。这将正确更新属性的聚合器，更新属性集属性，并调用OnDirty回调。
	 * 这不会在属性集上调用Pre/PostGameplayEffectExecute。这不进行任何标记检查、应用要求、免疫等。
	 * 不创建或应用任何GameplayEffectSpec！
	 * 仅在应用真实GameplayEffectSpec过于缓慢或不可能的情况下使用。
	 *
	 * @param Attribute 要应用修改的属性。
	 * @param ModifierOp 修改操作的类型，例如加法、减法等。
	 * @param ModifierMagnitude 修改操作的量级或幅度。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes", Meta = (DisplayName = "Apply Mod To Attribute"))
	void K2_ApplyModToAttribute(FGameplayAttribute Attribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude);

#pragma endregion


#pragma region GameplayAttribute_Delegates

protected:
	virtual void RegisterGameplayAttributeDelegates(const UTireflyAttributeSet* TireflyAttributeSet);

	virtual void ShutdownGameplayAttributeDelegates(const UTireflyAttributeSet* TireflyAttributeSet);

	virtual void HandleOnGameplayAttributeValueChanged(const FOnAttributeChangeData& Data);

public:
	// 生命属性值更新的动态多播委托事件
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTireflyGameplayAttributeDelegate, FGameplayAttribute, Attribute
		, float, NewValue, float, OldValue, const FGameplayTagContainer&, EventTags);

	// 当属性值发生变化时触发的委托事件
	// Delegate event triggered when the attribute value changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Attributes")
	FTireflyGameplayAttributeDelegate OnAttributeValueChanged;

	// 当属性的基础值发生变化时触发的委托事件
	// Delegate event triggered when the base value of the attribute changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Attributes")
	FTireflyGameplayAttributeDelegate OnAttributeBaseValueChanged;

#pragma endregion


#pragma region GameplayAbility_Delegates

public:
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;

	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;

	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

public:
	// 当能力被激活时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityDelegate OnAbilityActivated;

	// 当能力的消耗部分已提交时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityDelegate OnAbilityCostCommitted;

	// 当能力的冷却状态发生变化时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityCooldownDelegate OnAbilityCooldownCommitted;

	// 当能力结束时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityEndedDelegate OnAbilityEnded;

	// 当激活能力失败时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityFailedDelegate OnAbilityActivatingFailed;

	// 当能力的冷却状态发生变化时触发的委托事件
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FTireflyGameplayAbilityCooldownDelegate OnAbilityCooldownChanged;

#pragma endregion
};
