// Copyright Tirefly. All Rights Reserved.


#include "AbilitySystem/AttributeSet/AsuraAttributeSet.h"

#include "Net/UnrealNetwork.h"


void UAsuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 基础属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Strength, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Physique, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Agility, COND_OwnerOnly, REPNOTIFY_Always);

	// 派生属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Attack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Defense, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Health, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, AttackSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, MoveSpeed, COND_OwnerOnly, REPNOTIFY_Always);

	// 战斗属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, HitRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, DodgeRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalResistance, COND_OwnerOnly, REPNOTIFY_Always);
}
