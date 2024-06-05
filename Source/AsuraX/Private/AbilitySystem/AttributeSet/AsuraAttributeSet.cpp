// Copyright Tirefly. All Rights Reserved.


#include "AbilitySystem/AttributeSet/AsuraAttributeSet.h"

#include "Net/UnrealNetwork.h"


void UAsuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 属性派生：力量
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Strength, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Attack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalMultiplier, COND_OwnerOnly, REPNOTIFY_Always);

	// 属性派生：体魄
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Physique, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Defense, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Health, COND_OwnerOnly, REPNOTIFY_Always);

	// 属性派生：身法
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Agility, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, AttackSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, MoveSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	
	// 战斗属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, HitRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, DodgeRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, CriticalResistance, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, DamageBonus, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, DamageReduce, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Damage, COND_OwnerOnly, REPNOTIFY_Always);

	// 五行属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Metal, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Wood, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Water, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Fire, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsuraAttributeSet, Earth, COND_OwnerOnly, REPNOTIFY_Always);
}
