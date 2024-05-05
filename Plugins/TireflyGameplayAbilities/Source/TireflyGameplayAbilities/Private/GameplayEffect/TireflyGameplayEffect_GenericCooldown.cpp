// Copyright Tirefly. All Rights Reserved.


#include "GameplayEffect/TireflyGameplayEffect_GenericCooldown.h"


UTireflyGameplayEffect_GenericCooldown::UTireflyGameplayEffect_GenericCooldown(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}
