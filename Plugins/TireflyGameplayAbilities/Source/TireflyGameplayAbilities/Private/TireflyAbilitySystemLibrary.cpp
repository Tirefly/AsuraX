// Copyright Tirefly. All Rights Reserved.


#include "TireflyAbilitySystemLibrary.h"

#include "AbilitySystemGlobals.h"
#include "TireflyAbilitySystemComponent.h"
#include "TireflyAbilitySystemSettings.h"


class UTireflyGameplayAbilitiesSettings;

UTireflyAbilitySystemComponent* UTireflyAbilitySystemLibrary::GetTireflyAbilitySystemComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	return Cast<UTireflyAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor));
}

bool UTireflyAbilitySystemLibrary::NotEqual_AttributeAttribute(FGameplayAttribute A, FString B)
{
	return A.GetName() != B;                                                           
}

bool UTireflyAbilitySystemLibrary::GetAttributeValueInRange(const UAbilitySystemComponent* ASC,
	const FGameplayAttribute& Attribute, float ValueToClamp, float& NewValue, float& MinValue, float& MaxValue)
{
	// 尝试获取TireflyAbilitySystem的默认设置
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	if (!IsValid(SettingsGAS))
	{
		return false;
	}

	// 查找指定属性的范围设置
	const FTireflyAttributeRangeConfig* AttributeRangeConfig = SettingsGAS->AttributeRangeConfigList.Find(Attribute);
	if (!AttributeRangeConfig)
	{
		return false;
	}

	// 检查最小值和最大值的类型是否被设置
	if (AttributeRangeConfig->MinValueType == ETireflyAttributeRangeType::None
		&& AttributeRangeConfig->MaxValueType == ETireflyAttributeRangeType::None)
	{
		return false;
	}

	// 计算最小值和最大值
	MinValue = AttributeRangeConfig->MinValueType == ETireflyAttributeRangeType::Attribute ? ASC->GetNumericAttribute(AttributeRangeConfig->MinAttribute)
		: (AttributeRangeConfig->MinValueType == ETireflyAttributeRangeType::Numeric ? AttributeRangeConfig->MinValue : -3.4e+38);
	MaxValue = AttributeRangeConfig->MaxValueType == ETireflyAttributeRangeType::Attribute ? ASC->GetNumericAttribute(AttributeRangeConfig->MaxAttribute)
		: (AttributeRangeConfig->MaxValueType == ETireflyAttributeRangeType::Numeric ? AttributeRangeConfig->MaxValue : 3.4e+38);
	
	// 将输入值限制在最小值和最大值之间，并将结果赋给NewValue
	NewValue = FMath::Clamp<float>(ValueToClamp, MinValue, MaxValue);

	return false;
}
