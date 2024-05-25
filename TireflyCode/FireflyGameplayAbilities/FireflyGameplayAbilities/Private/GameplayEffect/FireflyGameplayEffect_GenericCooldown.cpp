// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "GameplayEffect/FireflyGameplayEffect_GenericCooldown.h"


UFireflyGameplayEffect_GenericCooldown::UFireflyGameplayEffect_GenericCooldown(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}
