// Copyright Tirefly. All Rights Reserved.


#include "GameplayEffect/TireflyGameplayEffect.h"

#include "GameplayTagsManager.h"
#include "TireflyAbilitySystemSettings.h"


UTireflyGameplayEffect::UTireflyGameplayEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTireflyGameplayEffect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UGameplayEffect, DurationMagnitude))
	{
		ProcessDurationSetByCaller();
	}
	else if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UGameplayEffect, Modifiers))
	{
		ProcessModiferSetByCaller();
	}
}

void UTireflyGameplayEffect::ProcessDurationSetByCaller()
{
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	const UGameplayTagsManager* TagManager = &UGameplayTagsManager::Get();
	if (!IsValid(SettingsGAS) || !IsValid(TagManager))
	{
		return;
	}

	FGameplayTag DurationTag = SettingsGAS->GetGenericDurationSetByCallerTag();
	if (!TagManager->FindTagNode(DurationTag.GetTagName()).IsValid() ||
		DurationPolicy != EGameplayEffectDurationType::HasDuration ||
		DurationMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller)
	{
		return;
	}

	FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = DurationTag;
    DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
}

void UTireflyGameplayEffect::ProcessModiferSetByCaller()
{
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	if (!IsValid(SettingsGAS))
	{
		return;
	}
	
	for (auto& Modifier : Modifiers)
	{
		const FTireflySetByCallerTagConfig* Config = SettingsGAS->GetSetByCallerTagConfig(Modifier.Attribute);
		if (Modifier.ModifierMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller || !Config)
		{
			continue;
		}

		FSetByCallerFloat SetByCaller;
		SetByCaller.DataTag = Config->SetByCallerTag;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
	}
}
