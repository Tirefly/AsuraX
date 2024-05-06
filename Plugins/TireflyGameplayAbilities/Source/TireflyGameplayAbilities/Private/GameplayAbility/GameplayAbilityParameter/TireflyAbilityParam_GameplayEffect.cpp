// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_GameplayEffect.h"

#include "AbilitySystemGlobals.h"
#include "GameplayTagsManager.h"
#include "TireflyAbilitySystemComponent.h"
#include "TireflyAbilitySystemSettings.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"


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
	UTireflyAbilitySystemComponent* CasterASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level)
{
	if (!GameplayEffect || !CasterASC)
	{
		return FGameplayEffectSpecHandle();
	}

	const UTireflyAbilitySystemSettings* SettingsGAS = GetDefault<UTireflyAbilitySystemSettings>();
	const UGameplayTagsManager* TagManager = &UGameplayTagsManager::Get();
	if (!IsValid(SettingsGAS) || !IsValid(TagManager))
	{
		return FGameplayEffectSpecHandle();
	}
	
	FGameplayAbilitySpec* AbilitySpec =CasterASC->FindAbilitySpecFromHandle(AbilityHandle);
	FGameplayAbilityActorInfo* ActorInfo = CasterASC->AbilityActorInfo.Get();
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
		ContextSetting->ModifyGameplayEffectContext(Context, CasterASC, AbilityHandle, Level);
	}

	// Make the spec of GameplayEffect
	FGameplayEffectSpecHandle EffectSpecHandle = CasterASC->MakeOutgoingSpec(GameplayEffect, Level, Context);
	if (!EffectSpecHandle.IsValid())
	{
		return FGameplayEffectSpecHandle();
	}

	// Copy over set by caller magnitudes
	EffectSpecHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
	// Set the duration
	{
		const float Duration = DurationTime ? DurationTime->GetParamValue(CasterASC, nullptr, AbilityHandle, Level) : 1.f;
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
	EffectSpecHandle.Data->Period = PeriodTime ? PeriodTime->GetParamValue(CasterASC, nullptr, AbilityHandle, Level) : 0.f;
	// Set the stack count
	EffectSpecHandle.Data->SetStackCount(StackToApply ? StackToApply->GetParamValue(CasterASC, nullptr, AbilityHandle, Level) : 1.f);
	// Set the set by caller magnitudes
	for (UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier* SetByCaller : SetByCallerModifiers)
	{
		SetByCaller->AssignSetByCallerModifier(EffectSpecHandle, CasterASC, AbilityHandle, Level);
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

void UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier::PostEditChangeProperty(
	FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "Magnitude")
	{
		if (Magnitude)
		{
			MagnitudeName = NAME_None;
		}
	}
	if (PropertyChangedEvent.Property->GetName() == "MagnitudeName")
	{
		if (MagnitudeName != NAME_None)
		{
			Magnitude = nullptr;
		}
	}
}

TArray<FName> UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier::GetSetByCallerModifierParamNames() const
{
	TArray<FName> ParamNames;
	if (UTireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
	{
		for (const TPair<FName, UTireflyGameplayAbilityParameter*>& ParamPair : AbilityAsset->AbilityParameters)
		{
			if (UTireflyAbilityParam_Numeric* NumericParam = Cast<UTireflyAbilityParam_Numeric>(ParamPair.Value))
			{
				ParamNames.Add(ParamPair.Key);
			}
		}
	}

	return ParamNames;
}

UTireflyAbilityParam_Numeric* UTireflyAbilityParamDetail_GameplayEffect_SetByCallerModifier::GetMagnitude() const
{
	if (Magnitude)
	{
		return Magnitude;
	}

	if (MagnitudeName != NAME_None)
	{
		if (UTireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
		{
			if (UTireflyAbilityParam_Numeric* NumericParam = Cast<UTireflyAbilityParam_Numeric>(
				AbilityAsset->AbilityParameters.FindRef(MagnitudeName)))
			{
				return NumericParam;
			}
		}
	}

	return nullptr;
}

void UTireflyAbilityParamDetail_GameplayEffect_TagSetByCaller::AssignSetByCallerModifier_Implementation(
	FGameplayEffectSpecHandle& EffectSpecHandle, UTireflyAbilitySystemComponent* CasterASC,
	const FGameplayAbilitySpecHandle AbilityHandle, int32 Level)
{
	const UTireflyAbilityParam_Numeric* NumericMagnitude = GetMagnitude();
	if (!NumericMagnitude)
	{
		return;
	}
	
	float& SetByCallerMagnitude = EffectSpecHandle.Data->SetByCallerTagMagnitudes.FindOrAdd(SetByCallerTag);
	SetByCallerMagnitude = NumericMagnitude->GetParamValue(CasterASC, nullptr, AbilityHandle, Level);
}

void UTireflyAbilityParamDetail_GameplayEffect_NameSetByCaller::AssignSetByCallerModifier_Implementation(
	FGameplayEffectSpecHandle& EffectSpecHandle, UTireflyAbilitySystemComponent* CasterASC,
	const FGameplayAbilitySpecHandle AbilityHandle, int32 Level)
{
	const UTireflyAbilityParam_Numeric* NumericMagnitude = GetMagnitude();
	if (!NumericMagnitude)
	{
		return;
	}
	
	float& SetByCallerMagnitude = EffectSpecHandle.Data->SetByCallerNameMagnitudes.FindOrAdd(SetByCallerName);
	SetByCallerMagnitude = NumericMagnitude->GetParamValue(CasterASC, nullptr, AbilityHandle, Level);
}
