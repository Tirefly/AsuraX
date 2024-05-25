// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayEffectContainer/FireflyTargetType.h"


void UFireflyTargetType_UseOwner::GetTargets_Implementation(AActor* Target, FGameplayEventData EventData,
	TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(Target);
}

void UFireflyTargetType_UseEventData::GetTargets_Implementation(AActor* Target, FGameplayEventData EventData,
	TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	if (const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult())
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target.Get()));
	}
}
