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
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	if (!IsValid(SettingsGAS))
	{
		return false;
	}

	auto FindAttributeRangeSetting = [SettingsGAS, Attribute]()
	{
		FTireflyAttributeRangeSetting* OutSetting = nullptr;
		for (const FTireflyAttributeRangeSetting& Setting : SettingsGAS->AttributeRangeSettingList)
		{
			if (Setting.AttributeType == Attribute)
			{
				*OutSetting = Setting;
			}
		}

		return OutSetting;
	};

	const FTireflyAttributeRangeSetting* AttributeRangeSetting = FindAttributeRangeSetting();
	if (!AttributeRangeSetting)
	{
		return false;
	}

	if (AttributeRangeSetting->MinValueType == ETireflyAttributeRangeType::None
		&& AttributeRangeSetting->MaxValueType == ETireflyAttributeRangeType::None)
	{
		return false;
	}

	MinValue = AttributeRangeSetting->MinValueType == ETireflyAttributeRangeType::Attribute ? ASC->GetNumericAttribute(AttributeRangeSetting->MinAttribute)
		: (AttributeRangeSetting->MinValueType == ETireflyAttributeRangeType::Numeric ? AttributeRangeSetting->MinValue : -3.4e+38);
	MaxValue = AttributeRangeSetting->MaxValueType == ETireflyAttributeRangeType::Attribute ? ASC->GetNumericAttribute(AttributeRangeSetting->MaxAttribute)
		: (AttributeRangeSetting->MaxValueType == ETireflyAttributeRangeType::Numeric ? AttributeRangeSetting->MaxValue : 3.4e+38);
	NewValue = FMath::Clamp<float>(ValueToClamp, MinValue, MaxValue);

	return false;
}
