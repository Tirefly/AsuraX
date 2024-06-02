// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"

#include "TireflyAbilitySystemLibrary.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


FName UTireflyAbilityAssetElement::GetParameterName() const
{
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this)
			{
				return Param.Key;
			}
		}
	}

	return NAME_None;
}

TArray<FName> UTireflyAbilityAssetElement::GetOtherParameters() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this)
			{
				continue;
			}

			OutOptions.Add(Param.Key);
		}
	}
	
	return OutOptions;
}

TArray<FName> UTireflyAbilityAssetElement::GetOtherNumericParameters() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this || Param.Value == nullptr)
			{
				continue;
			}

			if (Param.Value->IsA(UTireflyAbilityParam_Numeric::StaticClass()))
			{
				OutOptions.Add(Param.Key);
			}
		}
	}
	
	return OutOptions;
}

TArray<FName> UTireflyAbilityAssetElement::GetOtherGameplayEffectParameters() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this || Param.Value == nullptr)
			{
				continue;
			}

			if (Param.Value->IsA(UTireflyAbilityParam_GameplayEffectBase::StaticClass()))
			{
				OutOptions.Add(Param.Key);
			}
		}
	}
	
	return OutOptions;
}

void UTireflyAbilityAssetElement::PostInitProperties()
{
	UObject::PostInitProperties();

	K2_PostInitProperties();
}