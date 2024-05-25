// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cooldown.h"

#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


float UTireflyAbilityParam_GenericCooldown::GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (CooldownTime)
	{
		return CooldownTime->GetParamValue(ParamInfo);
	}
	
	return 0.f;
}

FText UTireflyAbilityParam_GenericCooldown::GetShowcaseText_Implementation() const
{
	if (CooldownTime)
	{
		return CooldownTime->GetShowcaseText();
	}
	
	return Super::GetShowcaseText_Implementation();
}
