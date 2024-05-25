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
		ProcessDurationSetByCaller();
	}
	else if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UGameplayEffect, Modifiers))
	{
		ProcessModiferSetByCaller();
	}
}

void UFireflyGameplayEffect::ProcessDurationSetByCaller()
{
	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
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

void UFireflyGameplayEffect::ProcessModiferSetByCaller()
{
	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
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
