// Copyright Tirefly. All Rights Reserved.


#include "TireflyAbilitySystemLibrary.h"

#include "AbilitySystemGlobals.h"
#include "TireflyAbilitySystemComponent.h"
#include "TireflyAbilitySystemSettings.h"
#include "Engine/AssetManager.h"
#include "GameplayAbility/TireflyGameplayAbility.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


class UTireflyGameplayAbilitiesSettings;


UTireflyAbilitySystemComponent* UTireflyAbilitySystemLibrary::GetTireflyAbilitySystemComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	return Cast<UTireflyAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor));
}

UTireflyGameplayAbilityAsset* UTireflyAbilitySystemLibrary::GetAbilityAsset(const UObject* InObject)
{
	UTireflyGameplayAbilityAsset* OuterAsset = nullptr;
	UObject* Outer = InObject->GetOuter();

	while (Outer)
	{
		if (Outer->IsA<UTireflyGameplayAbilityAsset>())
		{
			OuterAsset = Cast<UTireflyGameplayAbilityAsset>(Outer);
			break;
		}
		Outer = Outer->GetOuter();
	}
	
	return OuterAsset;
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityClassPure(
	TSubclassOf<UTireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success)
{
	return GetAbilityParamOfAbilityClass(AbilityClass, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityClass(
	TSubclassOf<UTireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success)
{
	if (!AbilityClass)
	{
		Success = false;
		return nullptr;
	}
	
	auto OutParameter =  AbilityClass->GetDefaultObject<UTireflyGameplayAbility>()->GetAbilityParameter<UTireflyGameplayAbilityParameter>(Parameter);
	Success = (OutParameter != nullptr) ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityIdPure(FName AbilityId,
	FName Parameter, bool& Success)
{
	return GetAbilityParamOfAbilityId(AbilityId, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityId(FName AbilityId,
	FName Parameter, bool& Success)
{
	UAssetManager* AS = &UAssetManager::Get();
	if (!AS)
	{
		Success = false;
		return nullptr;
	}

	UTireflyGameplayAbilityAsset* AbilityAsset = AS->GetPrimaryAssetObject<UTireflyGameplayAbilityAsset>(FPrimaryAssetId(
		UTireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityId));
	if (!AbilityAsset)
	{
		Success = false;
		return nullptr;
	}

	UTireflyGameplayAbilityParameter* OutParameter = AbilityAsset->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAssetRefPure(
	UTireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success)
{
	return GetAbilityParamOfAssetRef(AssetRef, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAssetRef(
	UTireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success)
{
	UTireflyGameplayAbilityParameter* OutParameter = AssetRef->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAssetPakPure(FString AssetPak,
	FName Parameter, bool& Success)
{
	return GetAbilityParamOfAssetPak(AssetPak, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAssetPak(FString AssetPak,
	FName Parameter, bool& Success)
{
	UTireflyGameplayAbilityAsset* AbilityAsset = Cast<UTireflyGameplayAbilityAsset>(FSoftObjectPath(AssetPak).TryLoad());
	if (!AbilityAsset)
	{
		Success = false;
		return nullptr;
	}
	
	UTireflyGameplayAbilityParameter* OutParameter = AbilityAsset->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

bool UTireflyAbilitySystemLibrary::GetAttributeValueInRange(const AActor* Actor,
	FGameplayAttribute Attribute, float ValueToClamp, float& NewValue, FVector2D& OutRange)
{
	if (!Actor)
	{
		NewValue = ValueToClamp;
		return false;
	}

	const UAbilitySystemComponent* const ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!ASC)
	{
		NewValue = ValueToClamp;
		return false;
	}
	
	// 尝试获取TireflyAbilitySystem的默认设置
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	if (!IsValid(SettingsGAS))
	{
		NewValue = ValueToClamp;
		return false;
	}

	// 查找指定属性的范围设置
	const FTireflyAttributeRangeConfig* AttributeRangeConfig = SettingsGAS->GetAttributeRangeConfig(Attribute);
	if (!AttributeRangeConfig)
	{
		NewValue = ValueToClamp;
		return false;
	}

	// 检查最小值和最大值的类型是否被设置
	if (AttributeRangeConfig->MinValueType == ETireflyAttributeRangeType::None
		&& AttributeRangeConfig->MaxValueType == ETireflyAttributeRangeType::None)
	{
		NewValue = ValueToClamp;
		return false;
	}

	// 计算最小值
	switch (AttributeRangeConfig->MinValueType)
	{
	case ETireflyAttributeRangeType::Attribute:
		{
			OutRange.X = ASC->GetNumericAttribute(AttributeRangeConfig->MinAttribute);
			break;
		}
	case ETireflyAttributeRangeType::Numeric:
		{
			OutRange.X = AttributeRangeConfig->MinValue;
			break;
		}
	case ETireflyAttributeRangeType::None:
		{
			OutRange.X = (double)INT64_MIN;
			break;
		}
	}

	// 计算最大值
	switch (AttributeRangeConfig->MaxValueType)
	{
	case ETireflyAttributeRangeType::Attribute:
		{
			OutRange.Y = ASC->GetNumericAttribute(AttributeRangeConfig->MaxAttribute);
			break;
		}
	case ETireflyAttributeRangeType::Numeric:
		{
			OutRange.Y = AttributeRangeConfig->MaxValue;
			break;
		}
	case ETireflyAttributeRangeType::None:
		{
			OutRange.Y = (double)INT64_MAX;
			break;
		}
	}
	
	// 将输入值限制在最小值和最大值之间，并将结果赋给NewValue
	NewValue = FMath::Clamp<float>(ValueToClamp, OutRange.X, OutRange.Y);
	return true;
}