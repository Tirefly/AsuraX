// Copyright tzlFirefly, 2023. All Rights Reserved.


#include "GameplayAbility/FireflyGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "FireflyAbilitySystemComponent.h"
#include "GameplayEffectContainer/FireflyTargetType.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "GameplayEffect/FireflyGameplayEffect_GenericCost.h"

#include "EnhancedInputComponent.h"
#include "Engine/AssetManager.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cooldown.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"
#include "GameplayEffect/FireflyGameplayEffect_GenericCooldown.h"


UFireflyGameplayAbility::UFireflyGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bActivateAbilityOnGranted = false;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	InputActivationPolicy = FIREFLY_TO_FLAG(EFireflyAbilityInputActivationPolicy::None);

	CooldownGameplayEffectClass = UFireflyGameplayEffect_GenericCooldown::StaticClass();
	CostGameplayEffectClass = UFireflyGameplayEffect_GenericCost::StaticClass();
}

void UFireflyGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	OnAbilityGranted();

	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UFireflyGameplayAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(UFireflyGameplayAbility, AbilityId).ToString())
	{
		if (AbilityId == NAME_None)
		{
			AbilityAsset = nullptr;
			AbilityAssetPath.Empty();
		}
		else
		{
			const UAssetManager& AssetManager = UAssetManager::Get();
			AbilityAsset = AssetManager.GetPrimaryAssetObject<UFireflyGameplayAbilityAsset>(
				FPrimaryAssetId(UFireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityId));
			if (AbilityAsset)
			{
				AbilityAssetPath = AbilityAsset.GetPathName();
			}
		}
	}
}

void UFireflyGameplayAbility::GetAbilityParamInfo(FTireflyAbilityParamInfo& ParamInfo)
{
	ParamInfo.CasterASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	ParamInfo.AbilityHandle = GetCurrentAbilitySpecHandle();
}

int32 UFireflyGameplayAbility::GetCasterLevel() const
{
	if (auto ASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		return ASC->GetAbilityCasterLevel();
	}

	return 1;
}

void UFireflyGameplayAbility::OnAbilityGranted_Implementation()
{
	if (!IsInstantiated() || !IsValid(GetCurrentSourceObject()))
	{
		return;
	}
}

void UFireflyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bCommitAbilityOnActivated)
	{
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
}

const FGameplayTagContainer* UFireflyGameplayAbility::GetCooldownTags() const
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

bool UFireflyGameplayAbility::CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown, OptionalRelevantTags))
	{
		return false;
	}

	auto TireflyASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
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

void UFireflyGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CooldownGameplayEffectClass != UFireflyGameplayEffect_GenericCooldown::StaticClass())
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
	SpecHandle.Data->StackCount = 1;

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

float UFireflyGameplayAbility::GetCooldownDuration_Implementation() const
{
	auto TireflyASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return 0.f;
	}

	if (AbilityAsset)
	{
		if (AbilityAsset->CooldownTime)
		{
			return AbilityAsset->CooldownTime->GetCooldownDuration(FTireflyAbilityParamInfo(TireflyASC,
				nullptr, GetCurrentAbilitySpecHandle(), GetAbilityLevel()));
		}
	}

	return 0.f;
}

UGameplayEffect* UFireflyGameplayAbility::GetCostGameplayEffect() const
{
	if (CostGameplayEffectClass != UFireflyGameplayEffect_GenericCost::StaticClass())
	{
		return Super::GetCostGameplayEffect();
	}

	if (IsValid(AbilityAsset))
	{
		if (!AbilityAsset->CostSettings.IsEmpty())
		{
			return UFireflyGameplayEffect_GenericCost::StaticClass()->GetDefaultObject<UGameplayEffect>();
		}
	}

	return nullptr;
}

bool UFireflyGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (CostGameplayEffectClass != UFireflyGameplayEffect_GenericCost::StaticClass())
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	TArray<UTireflyAbilityParam_CostBase*> CostSettings = GetCostSettings();
	if (CostSettings.IsEmpty())
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	auto TireflyASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo(TireflyASC, nullptr, Handle, GetAbilityLevel());
	for (auto CostSetting : CostSettings)
	{
		if (!CostSetting->CheckCost(ParamInfo))
		{
			return false;
		}
	}

	return true;
}

bool UFireflyGameplayAbility::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	auto TireflyASC = Cast<UFireflyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!TireflyASC)
	{
		return false;
	}

	TireflyASC->OnAbilityCostCommitted.Broadcast(Handle);
	return true;
}

void UFireflyGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

TArray<UTireflyAbilityParam_CostBase*> UFireflyGameplayAbility::GetCostSettings_Implementation() const
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


FFireflyGameplayEffectContainerSpec UFireflyGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FFireflyGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FFireflyGameplayEffectContainerSpec ReturnSpec;
	
	if (IsValid(GetAbilitySystemComponentFromActorInfo()))
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UFireflyTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}

	return ReturnSpec;
}

FFireflyGameplayEffectContainerSpec UFireflyGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	if (const FFireflyGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag))
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}

	return FFireflyGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UFireflyGameplayAbility::ApplyEffectContainerSpec(
	const FFireflyGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UFireflyGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	const FFireflyGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}