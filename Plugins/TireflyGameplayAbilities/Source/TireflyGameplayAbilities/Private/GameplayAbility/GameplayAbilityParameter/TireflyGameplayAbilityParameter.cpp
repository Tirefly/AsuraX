// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"

#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


UTireflyGameplayAbilityAsset* TireflyAbilityParameterHelper::GetAbilityAsset(const UObject* InObject)
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

TArray<FName> UTireflyGameplayAbilityParameterBase::GetAbilityParamOptions_Implementation() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
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

FName UTireflyGameplayAbilityParameterBase::GetAbilityParameterName() const
{
	if (const UTireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
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

TArray<FName> UTireflyGameplayAbilityParameterDetail::GetAbilityParamOptions_Implementation() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityParameter* Param = Cast<UTireflyGameplayAbilityParameter>(GetOuter()))
	{
		OutOptions = Param->GetAbilityParamOptions();
	}

	return OutOptions;
}
