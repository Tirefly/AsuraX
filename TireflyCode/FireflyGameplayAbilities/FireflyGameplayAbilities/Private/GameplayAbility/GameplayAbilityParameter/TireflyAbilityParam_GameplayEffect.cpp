// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"

#include "AbilitySystemGlobals.h"
#include "GameplayTagsManager.h"
#include "FireflyAbilitySystemComponent.h"
#include "FireflyGameplayAbilitiesSettings.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


FGameplayEffectSpecHandle UTireflyAbilityParam_OtherGameplayEffectParam::MakeOutgoingGameplayEffectSpec_Implementation(
	FTireflyAbilityParamInfo ParamInfo)
{
	if (UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
	{
		if (UTireflyGameplayAbilityParameter* Param = AbilityAsset->AbilityParameters.FindRef(ParameterName))
		{
			if (UTireflyAbilityParam_GameplayEffectBase* ParamGE = Cast<UTireflyAbilityParam_GameplayEffectBase>(Param))
			{
				return ParamGE->MakeOutgoingGameplayEffectSpec(ParamInfo);
			}
		}
	}
	
	return Super::MakeOutgoingGameplayEffectSpec_Implementation(ParamInfo);
}

void UTireflyAbilityParam_GameplayEffect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UTireflyAbilityParam_GameplayEffect, GameplayEffect))
	{
		if (!GameplayEffect)
		{
			return;
		}
		UGameplayEffect* EffectCDO = GameplayEffect->GetDefaultObject<UGameplayEffect>();

		SetByCallerModifiers.Empty();
		for (auto& ModifierInfo : EffectCDO->Modifiers)
		{
			if (ModifierInfo.ModifierMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller)
			{
				continue;
			}

			if (ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataName != NAME_None)
			{
				auto* NameSetByCaller = NewObject<UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller>(this);
				NameSetByCaller->SetByCallerName = ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataName;
				SetByCallerModifiers.Add(NameSetByCaller);
			}
			else if (ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataTag.IsValid())
			{
				auto* TagSetByCaller = NewObject<UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller>(this);
				TagSetByCaller->SetByCallerTag = ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataTag;
				SetByCallerModifiers.Add(TagSetByCaller);
			}
		}
	}
}

FGameplayEffectSpecHandle UTireflyAbilityParam_GameplayEffect::MakeOutgoingGameplayEffectSpec_Implementation(
	FTireflyAbilityParamInfo ParamInfo)
{
	if (!GameplayEffect || !ParamInfo.CasterASC)
	{
		return FGameplayEffectSpecHandle();
	}

	const UFireflyGameplayAbilitiesSettings* SettingsGAS = GetDefault<UFireflyGameplayAbilitiesSettings>();
	const UGameplayTagsManager* TagManager = &UGameplayTagsManager::Get();
	if (!IsValid(SettingsGAS) || !IsValid(TagManager))
	{
		return FGameplayEffectSpecHandle();
	}
	
	FGameplayAbilitySpec* AbilitySpec = ParamInfo.CasterASC->FindAbilitySpecFromHandle(ParamInfo.AbilityHandle);
	FGameplayAbilityActorInfo* ActorInfo = ParamInfo.CasterASC->AbilityActorInfo.Get();
	if (!AbilitySpec || !ActorInfo)
	{
		return FGameplayEffectSpecHandle();
	}

	// Make the context of GameplayEffect
	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
	// By default, use the owner and avatar as the instigator and causer
	Context.AddInstigator(ActorInfo->OwnerActor.Get(), ActorInfo->AvatarActor.Get());
	// add in the ability tracking here.
	Context.SetAbility(AbilitySpec->Ability);
	// Pass along the source object to the effect
	Context.AddSourceObject(AbilitySpec->SourceObject.Get());
	for (UTireflyAbilityParamDetail_GameplayEffect_ContextSetting* ContextSetting : ContextSettings)
	{
		ContextSetting->ModifyGameplayEffectContext(Context, ParamInfo);
	}

	// Make the spec of GameplayEffect
	FGameplayEffectSpecHandle EffectSpecHandle = ParamInfo.CasterASC->MakeOutgoingSpec(GameplayEffect, ParamInfo.Level, Context);
	if (!EffectSpecHandle.IsValid())
	{
		return FGameplayEffectSpecHandle();
	}

	// Copy over set by caller magnitudes
	EffectSpecHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
	// Set the duration
	{
		const float Duration = DurationTime ? DurationTime->GetParamValue(ParamInfo) : 1.f;
		FGameplayTag DurationTag = SettingsGAS->GetGenericDurationSetByCallerTag();
		if (TagManager->FindTagNode(DurationTag.GetTagName()).IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(DurationTag, Duration);
		}
		else
		{
			EffectSpecHandle.Data->Duration = Duration;
		}
	}
	// Set the period
	EffectSpecHandle.Data->Period = PeriodTime ? PeriodTime->GetParamValue(ParamInfo) : 0.f;
	// Set the stack count
	EffectSpecHandle.Data->StackCount = StackToApply ? StackToApply->GetParamValue(ParamInfo) : 1.f;
	// Set the set by caller magnitudes
	for (UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier* SetByCaller : SetByCallerModifiers)
	{
		SetByCaller->AssignSetByCallerModifier(EffectSpecHandle, ParamInfo);
	}

	return EffectSpecHandle;
}

bool UTireflyAbilityParam_GameplayEffect::IsEffectHasDuration() const
{
	if (GameplayEffect)
	{
		if (UGameplayEffect* EffectCDO = GameplayEffect->GetDefaultObject<UGameplayEffect>())
		{
			return EffectCDO->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		}
	}

	return false;
}

bool UTireflyAbilityParam_GameplayEffect::IsEffectNotInstant() const
{
	if (GameplayEffect)
	{
		if (UGameplayEffect* EffectCDO = GameplayEffect->GetDefaultObject<UGameplayEffect>())
		{
			return EffectCDO->DurationPolicy != EGameplayEffectDurationType::Instant;
		}
	}

	return false;
}

void UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller::AssignSetByCallerModifier_Implementation(
	FGameplayEffectSpecHandle& EffectSpecHandle, FTireflyAbilityParamInfo ParamInfo)
{
	if (!Magnitude)
	{
		return;
	}
	
	float& SetByCallerMagnitude = EffectSpecHandle.Data->SetByCallerTagMagnitudes.FindOrAdd(SetByCallerTag);
	SetByCallerMagnitude = Magnitude->GetParamValue(ParamInfo);
}

void UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller::AssignSetByCallerModifier_Implementation(
	FGameplayEffectSpecHandle& EffectSpecHandle, FTireflyAbilityParamInfo ParamInfo)
{
	if (!Magnitude)
	{
		return;
	}
	
	float& SetByCallerMagnitude = EffectSpecHandle.Data->SetByCallerNameMagnitudes.FindOrAdd(SetByCallerName);
	SetByCallerMagnitude = Magnitude->GetParamValue(ParamInfo);
}
