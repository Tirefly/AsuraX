// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/TireflyGameplayAbility.h"

#include "Engine/AssetManager.h"
#include "TireflyAbilitySystemComponent.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cooldown.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"
#include "GameplayEffect/TireflyGameplayEffect_GenericCooldown.h"
#include "GameplayEffect/TireflyGameplayEffect_GenericCost.h"


UTireflyGameplayAbility::UTireflyGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CooldownGameplayEffectClass = UTireflyGameplayEffect_GenericCooldown::StaticClass();
	CostGameplayEffectClass = UTireflyGameplayEffect_GenericCost::StaticClass();
}

void UTireflyGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(UTireflyGameplayAbility, AbilityId))
	{
		if (AbilityId == NAME_None)
		{
			AbilityAsset = nullptr;
			AbilityAssetPath.Empty();
		}
		else
		{
			const UAssetManager& AssetManager = UAssetManager::Get();
			AbilityAsset = AssetManager.GetPrimaryAssetObject<UTireflyGameplayAbilityAsset>(
				FPrimaryAssetId(UTireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityId));
			if (AbilityAsset)
			{
				AbilityAssetPath = AbilityAsset.GetPathName();
			}
		}
	}
}

TArray<FName> UTireflyGameplayAbility::GetAbilityParamOptions() const
{
	TArray<FName> OutOptions;
	if (AbilityAsset)
	{
		AbilityAsset->AbilityParameters.GenerateKeyArray(OutOptions);
	}

	return OutOptions;
}

const FGameplayTagContainer* UTireflyGameplayAbility::GetCooldownTags() const
{
	if (AbilityAsset)
	{
		if (AbilityAsset->CooldownTime)
		{
			return &AbilityAsset->CooldownTime->GetCooldownTags();
		}
	}

	return Super::GetCooldownTags();
}

bool UTireflyGameplayAbility::CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown, OptionalRelevantTags))
	{
		return false;
	}

	auto TireflyASC = Cast<UTireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return false;
	}

	FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(*GetCooldownTags());
	auto CooldownDuration = TireflyASC->GetActiveEffectsTimeRemainingAndDuration(EffectQuery);
	if (CooldownDuration.IsEmpty())
	{
		return false;
	}
			
	TireflyASC->OnAbilityCooldownCommitted.Broadcast(Handle, CooldownDuration[0].Key, CooldownDuration[0].Value);
	return true;
}

void UTireflyGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CooldownGameplayEffectClass != UTireflyGameplayEffect_GenericCooldown::StaticClass())
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo));
	if (!SpecHandle.IsValid())
	{
		return;
	}

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	if (!Spec)
	{
		return;
	}

	Spec->SetDuration(GetCooldownDuration(), true);
	Spec->AppendDynamicAssetTags(*GetCooldownTags());
	Spec->DynamicGrantedTags.AppendTags(*GetCooldownTags());
	SpecHandle.Data->SetStackCount(1);

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

float UTireflyGameplayAbility::GetCooldownDuration_Implementation() const
{
	auto TireflyASC = Cast<UTireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return 0.f;
	}

	if (AbilityAsset)
	{
		if (AbilityAsset->CooldownTime)
		{
			return AbilityAsset->CooldownTime->GetCooldownDuration(TireflyASC,
				nullptr, GetCurrentAbilitySpecHandle(), GetAbilityLevel());
		}
	}

	return 0.f;
}

UGameplayEffect* UTireflyGameplayAbility::GetCostGameplayEffect() const
{
	if (CostGameplayEffectClass != UTireflyGameplayEffect_GenericCost::StaticClass())
	{
		return Super::GetCostGameplayEffect();
	}

	if (IsValid(AbilityAsset))
	{
		if (!AbilityAsset->CostSettings.IsEmpty())
		{
			return UTireflyGameplayEffect_GenericCost::StaticClass()->GetDefaultObject<UGameplayEffect>();
		}
	}

	return nullptr;
}

bool UTireflyGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (CostGameplayEffectClass != UTireflyGameplayEffect_GenericCost::StaticClass())
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	TArray<UTireflyAbilityParam_CostBase*> CostSettings = GetCostSettings();
	if (CostSettings.IsEmpty())
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	auto TireflyASC = Cast<UTireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	for (auto CostSetting : CostSettings)
	{
		if (!CostSetting->CheckCost(TireflyASC, nullptr, Handle, GetAbilityLevel()))
		{
			return false;
		}
	}

	return true;
}

bool UTireflyGameplayAbility::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	auto TireflyASC = Cast<UTireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return false;
	}

	TireflyASC->OnAbilityCostCommitted.Broadcast(Handle);
	return true;
}

void UTireflyGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

TArray<UTireflyAbilityParam_CostBase*> UTireflyGameplayAbility::GetCostSettings_Implementation() const
{
	if (IsValid(AbilityAsset))
	{
		if (!AbilityAsset->CostSettings.IsEmpty())
		{
			return AbilityAsset->CostSettings;
		}
	}

	return TArray<UTireflyAbilityParam_CostBase*>{};
}
