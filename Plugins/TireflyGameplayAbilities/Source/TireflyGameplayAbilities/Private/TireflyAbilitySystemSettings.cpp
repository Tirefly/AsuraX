// Copyright Tirefly. All Rights Reserved.


#include "TireflyAbilitySystemSettings.h"

void UTireflyAbilitySystemSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

const FTireflyAttributeRangeConfig* UTireflyAbilitySystemSettings::GetAttributeRangeConfig(
	const FGameplayAttribute& Attribute) const
{
	FTireflyAttributeRangeConfig* OutConfig = nullptr;
	for (auto Config : AttributeRangeConfigList)
	{
		if (Config.AttributeType == Attribute)
		{
			OutConfig = &Config;
		}
	}

	return OutConfig;
}

const FTireflySetByCallerTagConfig* UTireflyAbilitySystemSettings::GetSetByCallerTagConfig(
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
