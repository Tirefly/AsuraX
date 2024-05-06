// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cooldown.h"

#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


float UTireflyAbilityParam_CommonCooldown::GetCooldownDuration_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (CooldownTime)
	{
		return CooldownTime->GetParamValue(CasterASC, TargetASC, AbilityHandle, Level);
	}
	
	return 0.f;
}

FText UTireflyAbilityParam_CommonCooldown::GetShowcaseText_Implementation() const
{
	if (CooldownTime)
	{
		return CooldownTime->GetShowcaseText();
	}
	
	return Super::GetShowcaseText_Implementation();
}
