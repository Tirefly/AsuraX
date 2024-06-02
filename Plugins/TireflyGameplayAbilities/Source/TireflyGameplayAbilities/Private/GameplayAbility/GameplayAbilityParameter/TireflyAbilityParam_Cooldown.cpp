// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cooldown.h"

#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


void UTireflyAbilityParam_GenericCooldown::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

float UTireflyAbilityParam_GenericCooldown::GetCooldownDuration_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (CooldownTime)
	{
		return CooldownTime->GetParamValue(ParamInfo);
	}
	
	return 0.f;
}

FText UTireflyAbilityParam_GenericCooldown::GetDisplayText_Implementation() const
{
	if (CooldownTime)
	{
		return CooldownTime->GetDisplayText();
	}
	
	return Super::GetDisplayText_Implementation();
}
