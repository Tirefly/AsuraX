// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/LegendAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FireflyAbilitySystemLibrary.h"
#include "FireflyGameplayAbilitiesSettings.h"
#include "GameplayEffectExtension.h"
#include "Arena/CombatUnits/CombatUnit_Summon.h"
#include "CoreBasics/CoreGameplayTag.h"
#include "DataAsset/LegendConfig_CombatUnit.h"
#include "Libraries/LegendSystemLibrary.h"


ULegendAttributeSet::ULegendAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULegendAttributeSet::PostDamageExecute(const FGameplayEffectModCallbackData& Data)
{
	const UAbilitySystemComponent* SourceASC = Data.EffectSpec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = &Data.Target;
	const FGameplayModifierEvaluatedData& EvaluatedData = Data.EvaluatedData;
	const UGameplayAbility* Ability = Data.EffectSpec.GetEffectContext().GetAbilityInstance_NotReplicated();

	if (!IsValid(SourceASC) || !IsValid(TargetASC) || !IsValid(Ability) || EvaluatedData.Magnitude <= 0.f)
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

	const float InDamage = EvaluatedData.Magnitude;
	const float Shield = TargetASC->GetNumericAttribute(GetShieldAttribute());
	const float CurrentHealth = TargetASC->GetNumericAttribute(GetCurrentHealthAttribute());
	if (CurrentHealth <= 0.f)
	{
		return;
	}

	float OutDamage = InDamage;
	if (Shield > 0.f)
	{
		TargetASC->ApplyModToAttribute(GetShieldAttribute(), EGameplayModOp::Additive, -InDamage);
		if (InDamage - Shield <= 0.f)
		{
			return;
		}

		OutDamage = InDamage - Shield;
	}

	TargetASC->ApplyModToAttribute(GetCurrentHealthAttribute(), EGameplayModOp::Additive, -OutDamage);
	if (CurrentHealth - OutDamage > 0.f)
	{
		return;
	}

	// 打败目标事件
	{
		FGameplayEventData DefeatTargetEvent;
		DefeatTargetEvent.EventTag = GAS_GAMEPLAYEVENT_DEFEATTARGET;
		DefeatTargetEvent.OptionalObject = Ability;
		DefeatTargetEvent.Instigator = Instigator;
		DefeatTargetEvent.Target = TargetASC->GetAvatarActor();
		DefeatTargetEvent.EventMagnitude = InDamage;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceASC->GetAvatarActor()
			, DefeatTargetEvent.EventTag, DefeatTargetEvent);
	}

	// 被打败事件
	{
		FGameplayEventData DeathEvent;
		DeathEvent.EventTag = GAS_GAMEPLAYEVENT_DEATH;
		DeathEvent.OptionalObject = Ability;
		DeathEvent.Instigator = Instigator;
		DeathEvent.Target = TargetASC->GetAvatarActor();
		DeathEvent.EventMagnitude = InDamage;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor()
			, DeathEvent.EventTag, DeathEvent);
	}
}

void ULegendAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageSufferedAttribute())
	{
		PostDamageExecute(Data);
	}
}

void ULegendAttributeSet::ClampAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const ULegendConfig_CombatUnit* ConfigCU = ULegendSystemLibrary::GetCombatUnitConfig();
	if (!ConfigCU)
	{
		return;
	}

	float RangeMin = 0.f;
	float RangeMax = 0.f;
	ConfigCU->GetAttributeRange(Attribute, ASC, RangeMin, RangeMax);

	NewValue = FMath::Clamp<double>(NewValue, RangeMin, RangeMax);
}

void ULegendAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttributeValue(Attribute, NewValue);
}

void ULegendAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttributeValue(Attribute, NewValue);
}

void ULegendAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttribute& CurrentAttribute,
	const FGameplayAttribute& MaxAttribute, float OldValue, float NewValue) const
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const float CurrentValue = ASC->GetNumericAttribute(CurrentAttribute);

	float NewCurrentValue;
	if (NewValue > OldValue)
	{
		NewCurrentValue = CurrentValue + NewValue - OldValue;
	}
	else if (NewValue <= OldValue)
	{
		NewCurrentValue = FMath::Min<float>(CurrentValue, NewValue);
	}
	else
	{
		NewCurrentValue = NewValue;
	}

	ASC->ApplyModToAttribute(CurrentAttribute, EGameplayModOp::Override, NewCurrentValue);
}

void ULegendAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// 最大生命值
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHealthAttribute(), GetMaxHealthAttribute(), OldValue, NewValue);
	}
}
