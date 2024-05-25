// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AttributeSet.h"
#include "FireflyAbilitySystemTypes.h"
#include "InputActionValue.h"

#include "FireflyAbilitySystemComponent.generated.h"


struct FFireflyGameplayEffectRuntimeConfig;
struct FFireflyMagnitude_MathExpression;

class UFireflyGameplayAbility;
class UFireflyAttributeSet;
class UFireflyAbilitySystemCustomExecution;
class UFireflyCustomExecutionContainer;

class UEnhancedInputComponent;
class UInputAction;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireflyGameplayAbilityDelegate, const FGameplayAbilitySpecHandle, AbilityHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFireflyGameplayAbilityCooldownDelegate, const FGameplayAbilitySpecHandle, AbiltyHandle, float, NewTimeRemaining, float, TotalDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireflyGameplayAbilityFailedDelegate, const FGameplayAbilitySpecHandle, AbilityHandle, const FGameplayTagContainer&, EventTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireflyGameplayAbilityEndedDelegate, const FGameplayAbilitySpecHandle, AbilityHandle, bool, bWasCanceled);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FFireflyGameplayAttributeDelegate, FGameplayAttribute, Attribute, float, NewValue, float, OldValue, const FGameplayTagContainer&, EventTags);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireflyActiveGameplayEffectDekegate, const FActiveGameplayEffectHandle&, ActiveEffectHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFireflyActiveGameplayEffectStackDelegate, const FActiveGameplayEffectHandle&, ActiveEffectHandle, int32, NewStackCount, int32, OldStackCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFireflyGameplayEffectTimeRemainingDelegate, const FActiveGameplayEffectHandle&, ActiveEffectHandle, float, NewStartTime, float, NewDuration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireflyGameplayTagStackChange, const FGameplayTag&, GameplayTag, int32, NewTagCount);


// 基于GAS扩展的能力系统组件，在AbilitySystemComponent的基础上进行了如下扩展：
// * 适应EnhancedInput的能力输入绑定
// * 更加友好的能力冷却处理和游戏性效果持续时间处理
// * 在GAS执行的各个时期都能够让开发者加入自定义执行逻辑
//
// AbilitySystemComponent extended from the GAS (Gameplay Ability System) that provides the following enhancements:
// * Adaptation to ability input bindings with EnhancedInput
// * Friendly ability cooldown handling and gameplay effect duration handling
// * Allows developers to add pluggable custom execution logic at various phases of the Gameplay Ability System execution
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region AbilitySystemComponent

public:
	UFireflyAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

#pragma endregion


#pragma region AbilityActorInfo

public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	// 初始化能力系统组件的ActorInfo，只能在本地客户端和服务端调用，在模拟端调用无效
	// Initialize AbilitySystemComponent's ActorInfo, can only be called on the local client or server, invalid when called on the simulated client.
	UFUNCTION(BlueprintCallable, Category = "Ability System", Meta = (DisplayName = "Init Ability Actor Info"))
	void K2_InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	// 获取施法者等级
	UFUNCTION(BlueprintPure, Category = Ability)
	virtual int32 GetAbilityCasterLevel() const { return 0; }

	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle) const;

#pragma endregion


#pragma region GameplayAbility_InputBinding

protected:
	UEnhancedInputComponent* GetOwnerEnhancedInputComponent() const;

public:
	// 将技能与增强输入的Action绑定，绑定操作应该在本地客户端执行，如果技能未被赋予或者技能已经与输入绑定，则绑定无法执行
	// Bind the ability to the enhanced input action, and the binding operation should be performed on the local client
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities", meta = (DisplayName = "Bind Ability to Input"))
	bool BindAbilityToInputAction(UInputAction* InputToBind, TSubclassOf<UFireflyGameplayAbility> AbilityToBind);

	// 将技能与增强输入的Action解绑，解绑操作应该在本地客户端执行
	// Unbind the ability from the enhanced input action, and the unbinding operation should be performed on the local client
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities", meta = (DisplayName = "Unbind Ability From Input"))
	void UnbindAbilityFromInputAction(UInputAction* InputToUnbind, TSubclassOf<UFireflyGameplayAbility> AbilityToUnbind);


	// 处理技能系统绑定的本地输入事件：开始
	// Handle local input event that ability system bound: Started
	virtual void AbilityLocalInputStarted(const FInputActionValue& InputValue, UInputAction* Input);
	// 处理技能系统绑定的本地输入事件：进行中
	// Handle local input event that ability system bound: Ongoing
	virtual void AbilityLocalInputOngoing(const FInputActionValue& InputValue, UInputAction* Input);
	// 处理技能系统绑定的本地输入事件：取消
	// Handle local input event that ability system bound: Canceled
	virtual void AbilityLocalInputCanceled(const FInputActionValue& InputValue, UInputAction* Input);
	// 处理技能系统绑定的本地输入事件：触发
	// Handle local input event that ability system bound: Triggered
	virtual void AbilityLocalInputTriggered(const FInputActionValue& InputValue, UInputAction* Input);
	// 处理技能系统绑定的本地输入事件：完成
	// Handle local input event that ability system bound: Completed
	virtual void AbilityLocalInputCompleted(const FInputActionValue& InputValue, UInputAction* Input);


	// 向服务器发送技能输入事件消息：开始
	// Send ability input event message to the server: Started
	UFUNCTION(Server, reliable, WithValidation)
	void Server_AbilityInputStarted(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual void Server_AbilityInputStarted_Implementation(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual bool Server_AbilityInputStarted_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	// 向服务器发送技能输入事件消息：进行中
	// Send ability input event message to the server: Ongoing
	UFUNCTION(Server, reliable, WithValidation)
	void Server_AbilityInputOngoing(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual void Server_AbilityInputOngoing_Implementation(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual bool Server_AbilityInputOngoing_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	// 向服务器发送技能输入事件消息：取消
	// Send ability input event message to the server: Canceled
	UFUNCTION(Server, reliable, WithValidation)
	void Server_AbilityInputCanceled(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual void Server_AbilityInputCanceled_Implementation(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual bool Server_AbilityInputCanceled_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	// 向服务器发送技能输入事件消息：触发
	// Send ability input event message to the server: Triggered
	UFUNCTION(Server, reliable, WithValidation)
	void Server_AbilityInputTriggered(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual void Server_AbilityInputTriggered_Implementation(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual bool Server_AbilityInputTriggered_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	// 向服务器发送技能输入事件消息：完成
	// Send ability input event message to the server: Completed
	UFUNCTION(Server, reliable, WithValidation)
	void Server_AbilityInputCompleted(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual void Server_AbilityInputCompleted_Implementation(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);
	virtual bool Server_AbilityInputCompleted_Validate(FGameplayAbilitySpecHandle AbilitySpecHandle, EInputActionValueType InputValueType, FVector InputValue);


	// 通知技能规范执行技能内部的输入事件：开始
	// Notify the ability spec to execute the internal input event: Started
	virtual void AbilitySpecInputStarted(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue);
	// 通知技能规范执行技能内部的输入事件：进行中
	// Notify the ability spec to execute the internal input event: Ongoing
	virtual void AbilitySpecInputOngoing(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue);
	// 通知技能规范执行技能内部的输入事件：取消
	// Notify the ability spec to execute the internal input event: Canceled
	virtual void AbilitySpecInputCanceled(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue);
	// 通知技能规范执行技能内部的输入事件：触发
	// Notify the ability spec to execute the internal input event: Triggered
	virtual void AbilitySpecInputTriggered(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue);
	// 通知技能规范执行技能内部的输入事件：完成
	// Notify the ability spec to execute the internal input event: Completed
	virtual void AbilitySpecInputCompleted(FGameplayAbilitySpec& Spec, const FInputActionValue& InputValue);


protected:
	// 技能和输入绑定的数据存储
	// Storage for data related to abilities and input bindings
	UPROPERTY()
	TMap<UInputAction*, FFireflyAbilityInputBindingData> AbilityInputBindings;

#pragma endregion


#pragma region GameplayAbility_Cooldown

protected:
	// 设置能力冷却时间剩余（内部函数）
	// Set the remaining cooldown time for an ability (internal function)
	virtual bool SetAbilityCooldownTimeRemaining_Internal(const UFireflyGameplayAbility* Ability, float NewTimeRemaining);

public:
	// 设置指定能力的冷却时间剩余
	// Set the remaining cooldown time for a specific ability
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool SetAbilityCooldownTimeRemaining(const FGameplayAbilitySpecHandle& Handle, float NewTimeRemaining);

	// 通过添加指定的值修改能力的冷却时间剩余
	// Modify the remaining cooldown time for a specific ability by adding a specified amount
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool ModifyAbilityCooldownTimeRemainingByAddition(const FGameplayAbilitySpecHandle& Handle, float AdditiveAmount);

	// 通过乘法因子修改能力的冷却时间剩余
	// Modify the remaining cooldown time for a specific ability by a multiplication factor
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool ModifyAbilityCooldownTimeRemainingByMultiplier(const FGameplayAbilitySpecHandle& Handle, float Multiplier);

#pragma endregion


#pragma region GameplayAbility_Delegates

public:
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;

	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;

	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

public:
	// 当能力被激活时触发的委托事件
	// Delegate event triggered when the ability is activated.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityDelegate OnAbilityActivated;

	// 当能力的消耗部分已提交时触发的委托事件
	// Delegate event triggered when the cost of the ability is committed.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityDelegate OnAbilityCostCommitted;

	// 当能力的冷却状态发生变化时触发的委托事件
	// Delegate event triggered when the cooldown of the ability is committed.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityCooldownDelegate OnAbilityCooldownCommitted;

	// 当能力结束时触发的委托事件
	// Delegate event triggered when the ability ends.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityEndedDelegate OnAbilityEnded;

	// 当激活能力失败时触发的委托事件
	// Delegate event triggered when activating the ability fails.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityFailedDelegate OnAbilityActivatingFailed;

	// 当能力的冷却状态发生变化时触发的委托事件
	// Delegate event triggered when the cooldown status of the ability changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Abilities")
	FFireflyGameplayAbilityCooldownDelegate OnAbilityCooldownChanged;

#pragma endregion


#pragma region GameplayAttribute_Initialization

public:
	// 添加游戏性属性集合
	// Add a Gameplay Attribute Set
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	const UFireflyAttributeSet* AddAttributeSet(TSubclassOf<UFireflyAttributeSet> AttributeSetClass);

	// 移除游戏性属性集合
	// Remove a Gameplay Attribute Set
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	void RemoveAttributeSet(TSubclassOf<UFireflyAttributeSet> AttributeSetClass);

	// 根据用户自定义的数据表行初始化属性集
	// @param AttributeSetClass 需要初始化的属性集类型
	// @param DataTable 用来初始化属性集的数据表
	// @param RowName 在数据表中搜索初始化属性集的数据的行名
	// @param bCompletelyOverride 如果为true，则初始化会覆盖掉所有活跃的效果，直接重置属性的基础值和当前值
	//
	// Initialize Gameplay Attribute Set by user custom Data Table row
	// @param AttributeSetClass Attribute Set 's type which need to be initialized
	// @param DataTable The data table used to initialize the attribute set
	// @param RowName The row name to search for data to initialize the attribute set in the data table
	// @param bCompletelyOverride If true, the initialization will override all active effects, directly resetting the base and current values of the attributes
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes")
	void InitAttributeSetFromCustomDataTable(TSubclassOf<UFireflyAttributeSet> AttributeSetClass
		, const UDataTable* DataTable, FName RowName, bool bCompletelyOverride = false);

#pragma endregion


#pragma region GameplayAttribute_Modification

public:
	// 该函数设置属性的基础值。需要注意的是，现有的活动修饰器不会被清除，它们将继续基于新的基础值影响属性。
	// Sets the base value of an attribute. Existing active modifiers are NOT cleared and will act upon the new base value.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes", Meta = (DisplayName = "Set Numeric Attribute Base"))
	void K2_SetNumericAttributeBase(FGameplayAttribute Attribute, float NewBaseValue);

	/**
	 *	Applies an in-place mod to the given attribute. This correctly update the attribute's aggregator, updates the attribute set property,
	 *	and invokes the OnDirty callbacks.
	 *	This does not invoke Pre/PostGameplayEffectExecute calls on the attribute set. This does no tag checking, application requirements, immunity, etc.
	 *	No GameplayEffectSpec is created or is applied!
	 *	This should only be used in cases where applying a real GameplayEffectSpec is too slow or not possible.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gameplay Attributes", Meta = (DisplayName = "Apply Mod To Attribute"))
	void K2_ApplyModToAttribute(FGameplayAttribute Attribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude);

#pragma endregion


#pragma region GameplayAttribute_Delegates

protected:
	// 注册游戏性属性委托事件
	// Register Gameplay Attribute Delegates
	virtual void RegisterGameplayAttributeDelegates(const UFireflyAttributeSet* FireflyAttributeSet);

	// 关闭游戏性属性委托事件
	// Shutdown Gameplay Attribute Delegates
	virtual void ShutdownGameplayAttributeDelegates(const UFireflyAttributeSet* FireflyAttributeSet);

	// 处理游戏性属性值变化的事件
	// Handle the event when a Gameplay attribute value changes
	virtual void HandleOnGameplayAttributeValueChanged(const FOnAttributeChangeData& Data);

public:
	// 当属性值发生变化时触发的委托事件
	// Delegate event triggered when the attribute value changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Attributes")
	FFireflyGameplayAttributeDelegate OnAttributeValueChanged;

	// 当属性的基础值发生变化时触发的委托事件
	// Delegate event triggered when the base value of the attribute changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Attributes")
	FFireflyGameplayAttributeDelegate OnAttributeBaseValueChanged;

#pragma endregion


#pragma region GameplayEffect_TimeRemaining

public:
	// 设置指定活动游戏效果的剩余时间
	// Set the remaining time for a specific active gameplay effect
	bool SetActiveGameplayEffectTimeRemaining(FActiveGameplayEffect* ActiveGameplayEffect, float NewTimeRemaining);

	// 通过句柄设置指定活动游戏效果的剩余时间
	// Set the remaining time for a specific active gameplay effect by handle
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool SetActiveGameplayEffectTimeRemainingByHandle(const FActiveGameplayEffectHandle Handle, float NewTimeRemaining);

	// 通过资产标签设置指定活动游戏效果的剩余时间
	// Set the remaining time for a specific active gameplay effect by asset tags
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool SetActiveGameplayEffectTimeRemainingByAssetTags(const FGameplayTagContainer& AssetTags, float NewTimeRemaining);

	// 通过拥有标签设置指定活动游戏效果的剩余时间
	// Set the remaining time for a specific active gameplay effect by owning tags.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = GameplayEffects)
	bool SetActiveGameplayEffectTimeRemainingByOwningTags(const FGameplayTagContainer& OwningTags, float NewTimeRemaining);

#pragma endregion


#pragma region GameplayEffect_Delegates

public:
	// 注册游戏性效果委托事件
	// Register Gameplay Effect Delegates
	virtual void RegisterGameplayEffectDelegates();

	// 关闭游戏性效果委托事件
	// Shutdown Gameplay Effect Delegates
	virtual void ShutdownGameplayEffectDelegates();

protected:
	// 处理添加的活跃 Gameplay Effect 事件
	// Handle the event of an added active Gameplay Effect
	virtual void HandleOnActiveGameplayEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	// 处理任何 Gameplay Effect 被移除的事件
	// Handle the event of any Gameplay Effect being removed
	virtual void HandleOnAnyGameplayEffectRemoved(const FActiveGameplayEffect& EffectRemoved);

	// 处理活跃 Gameplay Effect 堆栈变化的事件
	// Handle the event of a change in the stack count of an active Gameplay Effect
	virtual void HandleOnActiveGameplayEffectStackChanged(FActiveGameplayEffectHandle ActiveHandle, int32 NewStackCount, int32 PreviousStackCount);

	// 处理活跃 Gameplay Effect 时间变化的事件
	// Handle the event of a change in the time of an active Gameplay Effect
	virtual void HandleOnActiveGameplayEffectTimeChanged(FActiveGameplayEffectHandle ActiveHandle, float NewStartTime, float NewDuration);

public:
	// 当活跃的 Gameplay Effect 被应用时触发的委托事件
	// Delegate event triggered when an active Gameplay Effect is applied.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Effects")
	FFireflyActiveGameplayEffectDekegate OnActiveGameplayEffectApplied;

	// 当活跃的 Gameplay Effect 被移除时触发的委托事件
	// Delegate event triggered when an active Gameplay Effect is removed.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Effects")
	FFireflyActiveGameplayEffectDekegate OnActiveGameplayEffectRemoved;

	// 当活跃的 Gameplay Effect 堆栈发生变化时触发的委托事件
	// Delegate event triggered when the stack of an active Gameplay Effect changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Effects")
	FFireflyActiveGameplayEffectStackDelegate OnActiveGameplayEffectStackChanged;

	// 当活跃的 Gameplay Effect 剩余时间发生变化时触发的委托事件
	// Delegate event triggered when the remaining time of an active Gameplay Effect changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Effects")
	FFireflyGameplayEffectTimeRemainingDelegate OnActiveGameplayEffectTimeRemainingChanged;

#pragma endregion


#pragma region GameplayTag_Delegates

public:
	// 注册游戏性标签委托事件
	// Register Gameplay Tag Delegates
	virtual void RegisterGameplayTagDelegates();

	// 关闭游戏性标签委托事件
	// Shutdown Gameplay Effect Delegates
	virtual void ShutdownGameplayTagDelegates();

protected:
	virtual void OnAnyGameplayTagStackChanged(const FGameplayTag GameplayTag, const int32 NewCount);

public:
	// 当 Gameplay Tag 堆栈发生变化时触发的委托事件
	// Delegate event triggered when a Gameplay Tag stack changes.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Tags")
	FFireflyGameplayTagStackChange OnGameplayTagStackChanged;

#pragma endregion


#pragma region GameplayAbilitySystem_CustomExecution

public:
	// 添加自定义执行类
	// Add a custom execution class
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = CustomExecution)
	virtual void AddCustomExecution(TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution);

	// 移除自定义执行类
	// Remove a custom execution class
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = CustomExecution)
	virtual void RemoveCustomExecution(TSubclassOf<UFireflyAbilitySystemCustomExecution> CustomExecution);

	// 获取自定义执行类容器
	// Get the custom execution class container
	UFireflyCustomExecutionContainer* GetCustomExecutionContainer() const { return CustomExecutionContainer; }

protected:
	// 自定义执行类容器
	// Custom execution class container
	UPROPERTY()
	UFireflyCustomExecutionContainer* CustomExecutionContainer = nullptr;

#pragma endregion
};
