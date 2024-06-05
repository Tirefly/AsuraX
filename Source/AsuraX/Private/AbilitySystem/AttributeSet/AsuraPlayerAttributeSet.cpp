// Copyright Tirefly. All Rights Reserved.


#include "AbilitySystem/AttributeSet/AsuraPlayerAttributeSet.h"

#include "Net/UnrealNetwork.h"


void UAsuraPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraPlayerAttributeSet, MaxSprite, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraPlayerAttributeSet, Sprite, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraPlayerAttributeSet, SpriteGain, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraPlayerAttributeSet, SpriteLoss, COND_OwnerOnly, REPNOTIFY_Always);
}
