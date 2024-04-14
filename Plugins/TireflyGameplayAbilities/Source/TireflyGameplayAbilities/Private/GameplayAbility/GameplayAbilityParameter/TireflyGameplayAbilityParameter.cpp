// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"

#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


TArray<FName> UTireflyGameplayAbilityParameter::GetAbilityParamOptions() const
{
	TArray<FName> OutOptions;
	if (const auto AbilityAsset = Cast<UTireflyGameplayAbilityAsset>(GetOuter()))
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

TArray<FName> UTireflyGameplayAbilityParameterDetail::GetAbilityParamOptions() const
{
	TArray<FName> OutOptions;
	if (const UTireflyGameplayAbilityParameter* Param = Cast<UTireflyGameplayAbilityParameter>(GetOuter()))
	{
		OutOptions = Param->GetAbilityParamOptions();
	}

	return OutOptions;
}
