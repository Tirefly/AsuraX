// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "FireflyAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AttributeSet.h"

#include "FireflyAbilitySystemComponent.h"
#include "FireflyGameplayAbilitiesSettings.h"
#include "Engine/AssetManager.h"
#include "GameplayAbility/FireflyGameplayAbility.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "GameplayEffect/FireflyGameplayEffect.h"
#include "GameplayEffect/FireflyGameplayEffectUIData.h"


UFireflyAbilitySystemComponent* UFireflyAbilitySystemLibrary::GetFireflyAbilitySystemComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UFireflyAbilitySystemComponent>();
}

bool UFireflyAbilitySystemLibrary::NotEqual_AttributeAttribute(FGameplayAttribute A, FString B)
{
	return A.GetName() != B;
}

bool UFireflyAbilitySystemLibrary::GetAttributeValueInRange(const AActor* Actor, FGameplayAttribute Attribute,
	float ValueToClamp, float& NewValue, float& RangeMin, float& RangeMax)
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
	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
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
			RangeMin = ASC->GetNumericAttribute(AttributeRangeConfig->MinAttribute);
			break;
		}
	case ETireflyAttributeRangeType::Numeric:
		{
			RangeMin = AttributeRangeConfig->MinValue;
			break;
		}
	case ETireflyAttributeRangeType::None:
		{
			RangeMin = (double)INT64_MIN;
			break;
		}
	}

	// 计算最大值
	switch (AttributeRangeConfig->MaxValueType)
	{
	case ETireflyAttributeRangeType::Attribute:
		{
			RangeMax = ASC->GetNumericAttribute(AttributeRangeConfig->MaxAttribute);
			break;
		}
	case ETireflyAttributeRangeType::Numeric:
		{
			RangeMax = AttributeRangeConfig->MaxValue;
			break;
		}
	case ETireflyAttributeRangeType::None:
		{
			RangeMax = (double)INT64_MAX;
			break;
		}
	}
	
	// 将输入值限制在最小值和最大值之间，并将结果赋给NewValue
	NewValue = FMath::Clamp<float>(ValueToClamp, RangeMin, RangeMax);
	return true;
}

float UFireflyAbilitySystemLibrary::CalculateMathExpression(const FString& Expression)
{
	float OutResult;
	FMath::Eval(Expression, OutResult);

	return OutResult;
}

bool UFireflyAbilitySystemLibrary::IsActiveGameplayEffectHandleValid(const FActiveGameplayEffectHandle& Handle)
{
	return Handle.IsValid();
}

bool UFireflyAbilitySystemLibrary::ActiveGameplayEffectHandleHasAssetTag(const FActiveGameplayEffectHandle& Handle,
	FGameplayTag Tag)
{
	if (const UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent())
	{
		if (const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle))
		{
			if (const UGameplayEffect* Def = Effect->Spec.Def)
			{
				return Def->InheritableGameplayEffectTags.CombinedTags.HasTagExact(Tag)
					|| Effect->Spec.GetDynamicAssetTags().HasTagExact(Tag);
			}
		}
	}

	return false;
}

bool UFireflyAbilitySystemLibrary::ActiveGameplayEffectHandleHasGrantedTag(const FActiveGameplayEffectHandle& Handle,
	FGameplayTag Tag)
{
	if (const UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent())
	{
		if (const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle))
		{
			if (const UGameplayEffect* Def = Effect->Spec.Def)
			{
				return Def->InheritableOwnedTagsContainer.CombinedTags.HasTagExact(Tag)
					|| Effect->Spec.DynamicGrantedTags.HasTagExact(Tag);
			}
		}
	}

	return false;
}

const UGameplayEffectUIData* UFireflyAbilitySystemLibrary::GetActiveGameplayEffectUIData(
	const FActiveGameplayEffectHandle& Handle, TSubclassOf<UGameplayEffectUIData> DataType)
{
	if (const UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent())
	{
		if (const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle))
		{
			if (const UGameplayEffect* Def = Effect->Spec.Def)
			{
				return UAbilitySystemBlueprintLibrary::GetGameplayEffectUIData(Def->GetClass(), DataType);
			}
		}
	}

	return nullptr;
}

bool UFireflyAbilitySystemLibrary::IsGameplayAbilityHandleValid(const FGameplayAbilitySpecHandle& Handle)
{
	return Handle.IsValid();
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAbilityClassPure(
	TSubclassOf<UFireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success)
{
	return GetAbilityParamOfAbilityClass(AbilityClass, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAbilityClass(
	TSubclassOf<UFireflyGameplayAbility> AbilityClass, FName Parameter, bool& Success)
{
	if (!AbilityClass)
	{
		Success = false;
		return nullptr;
	}
	
	auto OutParameter =  AbilityClass->GetDefaultObject<UFireflyGameplayAbility>()->GetAbilityParameter<UTireflyGameplayAbilityParameter>(Parameter);
	Success = (OutParameter != nullptr) ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAbilityIdPure(FName AbilityId,
	FName Parameter, bool& Success)
{
	return GetAbilityParamOfAbilityId(AbilityId, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAbilityId(FName AbilityId,
	FName Parameter, bool& Success)
{
	UAssetManager* AS = &UAssetManager::Get();
	if (!AS)
	{
		Success = false;
		return nullptr;
	}

	UFireflyGameplayAbilityAsset* AbilityAsset = AS->GetPrimaryAssetObject<UFireflyGameplayAbilityAsset>(FPrimaryAssetId(
		UFireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityId));
	if (!AbilityAsset)
	{
		Success = false;
		return nullptr;
	}

	UTireflyGameplayAbilityParameter* OutParameter = AbilityAsset->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAssetRefPure(
	UFireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success)
{
	return GetAbilityParamOfAssetRef(AssetRef, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAssetRef(
	UFireflyGameplayAbilityAsset* AssetRef, FName Parameter, bool& Success)
{
	UTireflyGameplayAbilityParameter* OutParameter = AssetRef->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAssetPakPure(FString AssetPak,
	FName Parameter, bool& Success)
{
	return GetAbilityParamOfAssetPak(AssetPak, Parameter, Success);
}

UTireflyGameplayAbilityParameter* UFireflyAbilitySystemLibrary::GetAbilityParamOfAssetPak(FString AssetPak,
	FName Parameter, bool& Success)
{
	UFireflyGameplayAbilityAsset* AbilityAsset = Cast<UFireflyGameplayAbilityAsset>(FSoftObjectPath(AssetPak).TryLoad());
	if (!AbilityAsset)
	{
		Success = false;
		return nullptr;
	}
	
	UTireflyGameplayAbilityParameter* OutParameter = AbilityAsset->AbilityParameters.FindRef(Parameter);
	Success = OutParameter != nullptr ? true : false;
	return OutParameter;
}

bool UFireflyAbilitySystemLibrary::GetModifierEvaluatedData(const FGameplayModifierEvaluatedData& InEvaluatedData,
                                                            FGameplayAttribute& OutAttribute, TEnumAsByte<EGameplayModOp::Type>& OutModifierOp, float& OutMagnitude,
                                                            FActiveGameplayEffectHandle& OutHandle)
{
	if (!InEvaluatedData.IsValid)
	{
		return false;
	}

	OutAttribute = InEvaluatedData.Attribute;
	OutModifierOp = InEvaluatedData.ModifierOp;
	OutMagnitude = InEvaluatedData.Magnitude;
	OutHandle = InEvaluatedData.Handle;

	return true;
}

FGameplayModifierEvaluatedData UFireflyAbilitySystemLibrary::MakeModifierEvaluatedData(
	const FGameplayAttribute& Attribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float Magnitude,
	const FActiveGameplayEffectHandle& Handle)
{
	return FGameplayModifierEvaluatedData(Attribute, ModifierOp, Magnitude, Handle);
}

bool UFireflyAbilitySystemLibrary::ActorHasMatchingGameplayTag(const AActor* InActor, FGameplayTag InTag)
{
	if (!IsValid(InActor))
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor);
	if (!IsValid(ASC))
	{
		return false;
	}

	return ASC->HasMatchingGameplayTag(InTag);
}
