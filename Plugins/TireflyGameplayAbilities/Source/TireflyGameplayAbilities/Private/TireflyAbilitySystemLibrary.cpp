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
	
	UTireflyGameplayAbilityParameter* OutParameter =  AbilityClass->GetDefaultObject<UTireflyGameplayAbility>()->GetAbilityAssetParameter(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityRefPure(
	UTireflyGameplayAbility* AbilityRef, FName Parameter, bool& Success)
{
	return GetAbilityParamOfAbilityRef(AbilityRef, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UTireflyAbilitySystemLibrary::GetAbilityParamOfAbilityRef(
	UTireflyGameplayAbility* AbilityRef, FName Parameter, bool& Success)
{
	if (!AbilityRef)
	{
		Success = false;
		return nullptr;
	}
	
	UTireflyGameplayAbilityParameter* OutParameter =  AbilityRef->GetAbilityAssetParameter(Parameter);
	Success = OutParameter != nullptr ? true : false;
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
