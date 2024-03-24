// Copyright Tirefly. All Rights Reserved.


#include "GameplayAttribute/TireflyAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "UObject/PropertyAccessUtil.h"


UTireflyAttributeSet::UTireflyAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UTireflyAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UTireflyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UTireflyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UTireflyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UTireflyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UTireflyAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UTireflyAttributeSet::InitFromDataTableRow(const UDataTable* DataTable, FName RowName, bool bIgnoreGameplayEffect)
{
	if (!IsValid(DataTable))
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable data table to initialize AttributeSet %s of %s is invalid.")
			, *GetClass()->GetName()
			, *(GetOwningActor() ? GetOwningActor()->GetName() : TEXT("NONE")));
		return;
	}

	const UScriptStruct* StructType = DataTable->GetRowStruct();
	if (!StructType)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable DataTable %s has an invalid row struct."), *DataTable->GetPathName());
		return;
	}

	const uint8* RowData = DataTable->FindRowUnchecked(RowName);
	if (!RowData)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable DataTable %s has no row named as %s.")
			, *DataTable->GetPathName()
			, *RowName.ToString());
		return;
	}

	// 迭代属性集中的所有数据
	for (TFieldIterator<FProperty> It(GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;
		if (!FGameplayAttribute::IsGameplayAttributeDataProperty(Property))
		{
			continue;
		}

		// 根据属性名在数据表行中查找对应的初始化数据
		const FProperty* ColumnProperty = PropertyAccessUtil::FindPropertyByName(FName(Property->GetName()), StructType);
		if (!ColumnProperty)
		{
			continue;
		}

		// 确定用来初始化属性的数据值
		double AttributeValue;
		if (ColumnProperty->IsA(FFloatProperty::StaticClass()))
		{
			AttributeValue = *ColumnProperty->ContainerPtrToValuePtr<float>(RowData);
		}
		else if (ColumnProperty->IsA(FDoubleProperty::StaticClass()))
		{
			AttributeValue = *ColumnProperty->ContainerPtrToValuePtr<double>(RowData);
		}
		else
		{
			continue;
		}

		// 覆盖掉所有激活的效果，直接重置属性的基础值和当前值
		if (bIgnoreGameplayEffect)
		{
			FGameplayAttributeData* AttributePtr = Property->ContainerPtrToValuePtr<FGameplayAttributeData>(this);
			AttributePtr->SetBaseValue(AttributeValue);
			AttributePtr->SetCurrentValue(AttributeValue);
		}
		// 考虑活跃的游戏性效果，只初始化属性的基础值
		else
		{
			if (UAbilitySystemComponent* OwningASC = GetOwningAbilitySystemComponent())
			{
				OwningASC->SetNumericAttributeBase(FGameplayAttribute(Property), AttributeValue);
			}
		}
	}

	PrintDebug();
}
