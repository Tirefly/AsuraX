// Copyright tzlFirefly, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FireflyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// 基于GAS扩展的游戏性属性集，在AttributeSet的基础上进行了如下扩展：
// * 在属性更改的各个阶段加入跟随GameplayEffect的，可灵活插拔的自定义执行逻辑
// * 添加了基于开发者自定义数据表的属性集初始化方式
//
// AttributeSet extended from the GAS (Gameplay Ability System) that provides the following enhancements:
// * Pluggable custom execution logic for various stages of attribute changes
// * Initialization of attribute set based on developer-defined data tables.
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	UFireflyAttributeSet(const FObjectInitializer& ObjectInitializer);

	/** 打印调试信息到日志中 */
	/** Print debug information to the log */
	virtual void PrintDebug() override { }

#pragma endregion


#pragma region AttributeSet_GameplayEffectExecute

public:
	// 在修改属性值之前调用。AttributeSet可以在此处进行附加修改。返回true以继续，或返回false以丢弃修改。
	// 注意，这仅在“执行”期间调用。例如，对属性的“基础值”进行修改。它不会在应用GameplayEffect时调用，例如5秒+10移动速度增益。
	// * 游戏性效果的执行：GameplayEffect 的 DurationPolicy 是 Instant，或者 GameplayEffect 是周期性跳动的。
	// * 游戏性效果的执行，会对属性的“基础值”造成不可逆的修改。
	// 
	// Called just before modifying the value of an attribute. AttributeSet can make additional modifications here. Return true to continue, or false to throw out the modification.
	// Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	// * Execution of Gameplay Effects: When the DurationPolicy of the GameplayEffect is Instant, or when the GameplayEffect is periodically ticking.
	// * Execution of Gameplay Effects results in irreversible modifications to the "base value" of attributes.
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	// 在执行GameplayEffect之前调用以修改属性的基础值。不能再进行更多的修改。
	// 注意，这仅在“执行”期间调用。例如，对属性的“基础值”进行修改。它不会在应用GameplayEffect时调用，例如5秒+10移动速度增益。
	// * 游戏性效果的执行：GameplayEffect 的 DurationPolicy 是 Instant，或者 GameplayEffect 是周期性跳动的。
	// * 游戏性效果的执行，会对属性的“基础值”造成不可逆的修改。
	// 
	// Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	// Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	// * Execution of Gameplay Effects: When the DurationPolicy of the GameplayEffect is Instant, or when the GameplayEffect is periodically ticking.
	// * Execution of Gameplay Effects results in irreversible modifications to the "base value" of attributes.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

#pragma endregion


#pragma region AttributeSet_AttributeChange

public:
	/**
	 * 在对属性进行任何修改之前调用。这比PreAttributeModify/PostAttributeModify更低级。
	 * 因为任何事情都可能触发这个函数，所以这里不提供额外的上下文。执行的效果、持续时间效果、正在移除的效果、应用免疫、堆叠规则变化等等。
	 * 此函数的目的是强制执行像“Health = Clamp(Health, 0, MaxHealth)”这样的操作，而不是像“如果应用了伤害，则触发此额外操作”这样的操作。
	 *
	 * NewValue是一个可变引用，因此您可以对新应用的值进行截断。
	 *
	 * Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	 * There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 * This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	 *	
	 * NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/** 在对属性进行任何修改之后调用。 */
	/** Called just after any modification happens to an attribute. */
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/**
	 * 当属性聚合器存在时，在对属性的基础值进行任何修改之前调用。
	 * 这个函数应该执行限制操作（假设您希望在PreAttributeChange中将基础值与最终值一起限制）。
	 * 这个函数不应该调用与游戏相关的事件或回调。在PreAttributeChange()中执行这些操作，它将在属性的最终值实际改变之前被调用。
	 *
	 * This is called just before any modification happens to an attribute's base value when an attribute aggregator exists.
	 * This function should enforce clamping (presuming you wish to clamp the base value along with the final value in PreAttributeChange)
	 * This function should NOT invoke gameplay related events or callbacks. Do those in PreAttributeChange() which will be called prior to the
	 * final value of the attribute actually changing.
	 */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/** 在对属性的基础值进行任何修改之后调用，当属性聚合器存在时。 */
	/** Called just after any modification happens to an attribute's base value when an attribute aggregator exists. */
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

#pragma endregion


#pragma region AttributeSet_Aggregator

public:
	/** 当此属性集中的属性创建了FAggregator时的回调函数。允许自定义设置FAggregator::EvaluationMetaData */
	/** Callback for when an FAggregator is created for an attribute in this set. Allows custom setup of FAggregator::EvaluationMetaData */
	virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override { }

	/**
	 * 这里可以添加“On Aggregator Change”类型的事件，并在将活动的GameplayEffect添加到或从属性聚合器时调用。
	 * 但是，在这些情况下很难提供所有的信息 - 属性聚合器可能因多种原因而发生变化：被添加、被移除、被修改、修改器变化、免疫、堆叠规则等等。
	 *
	 * An "On Aggregator Change" type of event could go here, and that could be called when active gameplay effects are added or removed to an attribute aggregator.
	 * It is difficult to give all the information in these cases though - aggregators can change for many reasons: being added, being removed, being modified, having a modifier change, immunity, stacking rules, etc.
	 */

#pragma endregion


#pragma region AttributeSet_DataDriven

public:
	// 从用户自定义的数据表初始化属性数据
	// @param DataTable 用来初始化属性集的数据表
	// @param RowName 在数据表中搜索初始化属性集的数据的行名
	// @param bCompletelyOverride 如果为true，则初始化会覆盖掉所有活跃的效果，直接重置属性的基础值和当前值
	//
	// Initializes attribute data from a user custom DataTable
	// @param DataTable The data table used to initialize the attribute set
	// @param RowName The row name to search for data to initialize the attribute set in the data table
	// @param bCompletelyOverride If true, the initialization will override all active effects, directly resetting the base and current values of the attributes
	virtual void InitFromCustomDataTable(const UDataTable* DataTable, FName RowName, bool bCompletelyOverride = false);

#pragma endregion
};