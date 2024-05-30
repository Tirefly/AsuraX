// Copyright Tirefly. All Rights Reserved.


#include "GameplayEffect/FireflyGameplayEffect.h"

#include "GameplayTagsManager.h"
#include "FireflyGameplayAbilitiesSettings.h"


UFireflyGameplayEffect::UFireflyGameplayEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFireflyGameplayEffect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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

bool UFireflyGameplayEffect::ProcessDurationSetByCaller()
{
	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
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

bool UFireflyGameplayEffect::ProcessModiferSetByCaller()
{
	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
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
		break;
	}

	return bModified;
}
