// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbility/FireflyAbilitySystemEngineSubsystem.h"

#include "AbilitySystemGlobals.h"


void UFireflyAbilitySystemEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UAbilitySystemGlobals* GlobalsGAS = &UAbilitySystemGlobals::Get())
	{
		GlobalsGAS->InitGlobalData();
	}
}
