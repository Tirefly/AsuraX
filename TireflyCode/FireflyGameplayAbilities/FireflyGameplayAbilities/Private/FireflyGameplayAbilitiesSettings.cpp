// Copyright Tirefly. All Rights Reserved.


#include "FireflyGameplayAbilitiesSettings.h"

#include "Internationalization/StringTable.h"
#include "Kismet/KismetStringTableLibrary.h"

void UFireflyGameplayAbilitiesSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

const FTireflyAttributeRangeConfig* UFireflyGameplayAbilitiesSettings::GetAttributeRangeConfig(
	const FGameplayAttribute& Attribute) const
{
	FTireflyAttributeRangeConfig* OutConfig = nullptr;
	for (auto Config : AttributeRangeConfigList)
	{
		if (Config.AttributeType == Attribute)
		{
			OutConfig = &Config;
			break;
		}
	}

	return OutConfig;
}

const FTireflySetByCallerTagConfig* UFireflyGameplayAbilitiesSettings::GetSetByCallerTagConfig(
	const FGameplayAttribute& Attribute) const
{
	for (const FTireflySetByCallerTagConfig& Config : GenericSetByCallerTagList)
	{
		if (Config.AttributeType == Attribute)
		{
			return &Config;
		}
	}

	return nullptr;
}

FName UFireflyGameplayAbilitiesSettings::GetAttributeLocalizationTableId() const
{
	UStringTable* StringTableObj = AttributeStringTable.Get();
	if (!StringTableObj)
	{
		return NAME_None;
	}
	
	return StringTableObj->GetStringTableId();
}

FName UFireflyGameplayAbilitiesSettings::GetGameplayNameLocalizationTableId(const FName& GameplayId) const
{
	for (const auto& StringTablePath : GameplayNameStringTables)
	{
		UStringTable* StringTableObj = StringTablePath.Get();
		if (!StringTableObj)
		{
			continue;
		}

		TArray<FString> Keys = UKismetStringTableLibrary::GetKeysFromStringTable(StringTableObj->GetStringTableId());
		if (Keys.Contains(GameplayId.ToString()))
		{
			return StringTableObj->GetStringTableId();
		}
	}

	return NAME_None;
}

FName UFireflyGameplayAbilitiesSettings::GetGameplayDescriptionLocalizationTableId(const FName& GameplayId) const
{
	for (const auto& StringTablePath : GameplayDescriptionStringTables)
	{
		UStringTable* StringTableObj = StringTablePath.Get();
		if (!StringTableObj)
		{
			continue;
		}

		TArray<FString> Keys = UKismetStringTableLibrary::GetKeysFromStringTable(StringTableObj->GetStringTableId());
		if (Keys.Contains(GameplayId.ToString()))
		{
			return StringTableObj->GetStringTableId();
		}
	}

	return NAME_None;
}
