// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TireflyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 扩展了一些功能支持的AttributeSet，扩展的功能如下：
 */
UCLASS()
class TIREFLYGAMEPLAYABILITIES_API UTireflyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	UTireflyAttributeSet(const FObjectInitializer& ObjectInitializer);

#pragma endregion


#pragma region AttributeSet_GameplayEffectExecute

public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

#pragma endregion


#pragma region AttributeSet_AttributeChange

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

#pragma endregion


#pragma region AttributeSet_DataDriven

public:
	/**
	 * 利用数据表的指定行初始化属性数据，数据表行结构可以在蓝图创建
	 * ，数据表行结构中的变量名必须与属性集中要被初始化的属性名一致
	 * @param DataTable 要用来初始化属性集的数据表引用
	 * @param RowName 数据表中用来初始化属性集的行名
	 * @param bIgnoreGameplayEffect 如果bIgnoreGameplayEffect为true，则完全无视GameplayEffect，覆盖属性的BaseValue和CurrentValue；
	 * 若为false，则仅覆盖BaseValue且不影响GameplayEffect对属性的修改
	 */
	virtual void InitFromDataTableRow(const UDataTable* DataTable, FName RowName, bool bIgnoreGameplayEffect = false);
	
#pragma endregion
};
