// Copyright Tirefly. All Rights Reserved.


#include "AbilitySystem/AttributeSet/AsuraBossAttributeSet.h"

#include "Net/UnrealNetwork.h"


void UAsuraBossAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraBossAttributeSet, MaxPosture, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraBossAttributeSet, Posture, COND_OwnerOnly, REPNOTIFY_Always);
}
