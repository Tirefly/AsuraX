// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "GameplayAttribute/FireflyAttributeSet.h"

#include "AbilitySystemLog.h"
#include "FireflyAbilitySystemComponent.h"
#include "CustomExecution/FireflyAbilitySystemCustomExecution.h"
#include "Kismet/KismetSystemLibrary.h"


UFireflyAttributeSet::UFireflyAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UFireflyAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (const UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPreGameplayEffectExecute(Data);
	}

	return Super::PreGameplayEffectExecute(Data);
}

void UFireflyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (const UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPostGameplayEffectExecute(Data);
	}
}

void UFireflyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPreAttributeChange(FireflyASC, Attribute, NewValue);
	}

	Super::PreAttributeChange(Attribute, NewValue);
}

void UFireflyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPostAttributeChange(FireflyASC, Attribute, OldValue, NewValue);
	}
}

void UFireflyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPreAttributeBaseChange(FireflyASC, Attribute, NewValue);
	}

	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UFireflyAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);

	if (UFireflyAbilitySystemComponent* FireflyASC = Cast<UFireflyAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		FireflyASC->GetCustomExecutionContainer()->HandleAttributeSetPostAttributeBaseChange(FireflyASC, Attribute, OldValue, NewValue);
	}
}

void UFireflyAttributeSet::InitFromCustomDataTable(const UDataTable* DataTable, FName RowName, bool bCompletelyOverride)
{
	if (!IsValid(DataTable))
	{
		ABILITY_LOG(Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable data table to initialize AttributeSet %s of %s is invalid.")
			, *GetClass()->GetName()
			, *(GetOwningActor() ? GetOwningActor()->GetName() : TEXT("NONE")));
		return;
	}

	const UScriptStruct* StructType = DataTable->GetRowStruct();
	if (!StructType)
	{
		ABILITY_LOG(Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable DataTable %s has an invalid row struct."), *DataTable->GetPathName());
		return;
	}

	const uint8* RowData = DataTable->FindRowUnchecked(RowName);
	if (!RowData)
	{
		ABILITY_LOG(Warning, TEXT("UFireflyAttributeSet::InitFromCustomDataTable DataTable %s has no row named as %s.")
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
		if (bCompletelyOverride)
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
