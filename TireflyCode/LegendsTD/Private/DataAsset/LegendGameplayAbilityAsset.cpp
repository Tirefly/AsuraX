// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "DataAsset/LegendGameplayAbilityAsset.h"


ULegendGameplayAbilityAsset::ULegendGameplayAbilityAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void ULegendGameplayAbilityAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif