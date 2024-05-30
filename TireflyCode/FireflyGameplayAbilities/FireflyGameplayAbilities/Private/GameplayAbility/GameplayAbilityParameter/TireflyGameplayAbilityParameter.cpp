// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"

#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


UFireflyGameplayAbilityAsset* TireflyAbilityParameterHelper::GetAbilityAsset(const UObject* InObject)
{
	UFireflyGameplayAbilityAsset* OuterAsset = nullptr;
	UObject* Outer = InObject->GetOuter();

	while (Outer)
	{
		if (Outer->IsA<UFireflyGameplayAbilityAsset>())
		{
			OuterAsset = Cast<UFireflyGameplayAbilityAsset>(Outer);
			break;
		}
		Outer = Outer->GetOuter();
	}
	
	return OuterAsset;
}

FName UTireflyGameplayAbilityParameterAbstract::GetAbilityParameterName() const
{
	if (const UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
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

TArray<FName> UTireflyGameplayAbilityParameterAbstract::GetAbilityOtherParameters() const
{
	TArray<FName> OutOptions;
	if (const UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
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

TArray<FName> UTireflyGameplayAbilityParameterAbstract::GetAbilityOtherNumericParameters() const
{
	TArray<FName> OutOptions;
	if (const UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
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

TArray<FName> UTireflyGameplayAbilityParameterAbstract::GetAbilityOtherGameplayEffectParameters() const
{
	TArray<FName> OutOptions;
	if (const UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this || Param.Value == nullptr)
			{
				continue;
			}

			if (Param.Value->IsA(UTireflyAbilityParam_GameplayEffect::StaticClass()))
			{
				OutOptions.Add(Param.Key);
			}
		}
	}
	
	return OutOptions;
}

void UTireflyGameplayAbilityParameterAbstract::PostInitProperties()
{
	UObject::PostInitProperties();

	K2_PostInitProperties();
}