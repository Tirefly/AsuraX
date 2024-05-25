// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "FireflyAbilitySystemGlobals.h"

#include "FireflyGameplayAbilitiesSettings.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Kismet/KismetStringTableLibrary.h"


void UFireflyAbilitySystemGlobals::InitGlobalData()
{
	Super::InitGlobalData();

	LoadGlobalLocalizationStringTables();
}

void UFireflyAbilitySystemGlobals::LoadGlobalLocalizationStringTables()
{
	const UFireflyGameplayAbilitiesSettings* Settings = GetDefault<UFireflyGameplayAbilitiesSettings>();
	if (!Settings)
	{
		return;
	}

	Settings->AttributeStringTable.LoadSynchronous();
	
	for (const auto& StringTablePath : Settings->GameplayNameStringTables)
	{
		StringTablePath.LoadSynchronous();
	}
	
	for (const auto& StringTablePath : Settings->GameplayDescriptionStringTables)
	{
		StringTablePath.LoadSynchronous();
	}
}
