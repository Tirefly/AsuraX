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
		if (ProcessDurationSetByCaller())
		{
			MarkPackageDirty();
		}
	}
	else if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UGameplayEffect, Modifiers))
	{
		if (ProcessModiferSetByCaller())
		{
			MarkPackageDirty();
		}
	}
}

bool UTireflyGameplayEffect::ProcessDurationSetByCaller()
{
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	const UGameplayTagsManager* TagManager = &UGameplayTagsManager::Get();
	if (!IsValid(SettingsGAS) || !IsValid(TagManager))
	{
		return false;
	}

	FGameplayTag DurationTag = SettingsGAS->GetGenericDurationSetByCallerTag();
	if (!TagManager->FindTagNode(DurationTag.GetTagName()).IsValid() ||
		DurationPolicy != EGameplayEffectDurationType::HasDuration ||
		DurationMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller)
	{
		return false;
	}

	FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = DurationTag;
    DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

	return true;
}

bool UTireflyGameplayEffect::ProcessModiferSetByCaller()
{
	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	if (!IsValid(SettingsGAS))
	{
		return false;
	}

	bool bModified = false;
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
		bModified = true;
	}

	return bModified;
}
