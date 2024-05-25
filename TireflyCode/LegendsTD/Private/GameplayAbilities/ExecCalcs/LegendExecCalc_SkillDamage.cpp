// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/ExecCalcs/LegendExecCalc_SkillDamage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Arena/CombatUnits/CombatUnit_Summon.h"
#include "GameplayAbilities/LegendAttributeSet.h"

#include "CoreBasics/CoreGameplayTag.h"
#include "Kismet/KismetMathLibrary.h"


void ULegendExecCalc_SkillDamage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const float InDamage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(GAS_SETBYCALLER_DAMAGE);
	if (InDamage <= 0.f)
	{
		return;
	}

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!IsValid(SourceASC) || !IsValid(TargetASC))
	{
		return;
	}

	// 若伤害来源是一个召唤物，则将发起者设置为召唤物的所有者
	AActor* Instigator = SourceASC->GetAvatarActor();
	if (const ACombatUnit_Summon* SummonSource = Cast<ACombatUnit_Summon>(Instigator))
	{
		if (IsValid(SummonSource->SummonOwnerASC))
		{
			Instigator = SummonSource->SummonOwnerASC->GetAvatarActor();
		}
	}

	// 伤害来源的能力
	const UGameplayAbility* Ability = ExecutionParams.GetOwningSpec().GetEffectContext().GetAbilityInstance_NotReplicated();

	float OutDamage = InDamage - TargetASC->GetNumericAttribute(ULegendAttributeSet::GetDefensiveForceAttribute());
	// 伤害减去防御力
	if (OutDamage <= 0.f)
	{
		return;
	}

	// 计算Source伤害加成、目标伤害减免
	const float SourceDamageBonus = SourceASC->GetNumericAttribute(ULegendAttributeSet::GetDamageBonusAttribute());
	const float TargetDamageReduction = TargetASC->GetNumericAttribute(ULegendAttributeSet::GetDamageReductionAttribute());
	OutDamage *= (1.f + SourceDamageBonus - TargetDamageReduction);
	if (OutDamage <= 0.f)
	{
		return;
	}

	// 计算技能强度
	OutDamage = OutDamage * (1 + SourceASC->GetNumericAttribute(ULegendAttributeSet::GetSkillIntensityAttribute()) * 0.01);

	// 计算暴击
	float SourceCriticalRate = 0.f;
	if (ExecutionParams.GetOwningSpec().SetByCallerTagMagnitudes.Find(GAS_SETBYCALLER_SKILLCANCRITICAL))
	{
		SourceCriticalRate = SourceASC->GetNumericAttribute(ULegendAttributeSet::GetCriticalRateAttribute());
	}
	else if (ExecutionParams.GetOwningSpec().SetByCallerTagMagnitudes.Find(GAS_SETBYCALLER_SKILLCRITICALRATE))
	{
		SourceCriticalRate = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(GAS_SETBYCALLER_SKILLCRITICALRATE);
	}
	const float SourceCriticalEnhanceRate = SourceASC->GetNumericAttribute(ULegendAttributeSet::GetCriticalEnhanceRateAttribute());
	bool bCritical = false;
	if (UKismetMathLibrary::RandomBoolWithWeight(SourceCriticalRate))
	{
		OutDamage *= SourceCriticalEnhanceRate;
		bCritical = true;

		// 暴击成功事件
		{
			FGameplayEventData CriticalEvent;
			CriticalEvent.EventTag = GAS_GAMEPLAYEVENT_CRITICAL;
			CriticalEvent.OptionalObject = Ability;
			CriticalEvent.Instigator = Instigator;
			CriticalEvent.Target = TargetASC->GetAvatarActor();
			CriticalEvent.EventMagnitude = OutDamage;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceASC->GetAvatarActor()
				, CriticalEvent.EventTag, CriticalEvent);
		}

		// 被暴击事件
		{
			FGameplayEventData GetCriticalEvent;
			GetCriticalEvent.EventTag = GAS_GAMEPLAYEVENT_GETCRITICAL;
			GetCriticalEvent.OptionalObject = Ability;
			GetCriticalEvent.Instigator = Instigator;
			GetCriticalEvent.Target = TargetASC->GetAvatarActor();
			GetCriticalEvent.EventMagnitude = OutDamage;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor()
				, GetCriticalEvent.EventTag, GetCriticalEvent);
		}
	}

	if (bCritical)
	{
		// TODO: 暴击伤害数字
	}
	else
	{
		// TODO: 伤害数字
	}

	// 攻击命中目标事件
	{
		FGameplayEventData HitTargetEvent;
		HitTargetEvent.EventTag = GAS_GAMEPLAYEVENT_HITTARGET;
		HitTargetEvent.OptionalObject = Ability;
		HitTargetEvent.Instigator = Instigator;
		HitTargetEvent.Target = TargetASC->GetAvatarActor();
		HitTargetEvent.EventMagnitude = OutDamage;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceASC->GetAvatarActor(), HitTargetEvent.EventTag, HitTargetEvent);
	}

	// 被攻击命中事件
	{
		FGameplayEventData GetHitEvent;
		GetHitEvent.EventTag = GAS_GAMEPLAYEVENT_GETHIT;
		GetHitEvent.OptionalObject = Ability;
		GetHitEvent.Instigator = Instigator;
		GetHitEvent.Target = TargetASC->GetAvatarActor();
		GetHitEvent.EventMagnitude = OutDamage;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(), GetHitEvent.EventTag, GetHitEvent);
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULegendAttributeSet::GetDamageSufferedAttribute()
		, EGameplayModOp::Type::Override, OutDamage));
}
