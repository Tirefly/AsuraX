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

	ProcessModiferSetByCaller();
}

void UTireflyGameplayEffect::ProcessDurationSetByCaller()
{
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	const UGameplayTagsManager* TagManager = &UGameplayTagsManager::Get();
	if (!IsValid(SettingsGAS) || !IsValid(TagManager))
	{
		return;
	}

	if (!TagManager->FindTagNode(SettingsGAS->GenericDurationSetByCallerTag.GetTagName()).IsValid()
		|| DurationPolicy != EGameplayEffectDurationType::HasDuration
		|| DurationMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller)
	{
		return;
	}

	FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = SettingsGAS->GenericDurationSetByCallerTag;
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
		const FGameplayTag* SetByCallerTag = SettingsGAS->GenericSetByCallerTagList.Find(Modifier.Attribute);
		if (Modifier.ModifierMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller
			|| !SetByCallerTag)
		{
			continue;
		}

		FSetByCallerFloat SetByCaller;
		SetByCaller.DataTag = *SetByCallerTag;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);
	}
}
