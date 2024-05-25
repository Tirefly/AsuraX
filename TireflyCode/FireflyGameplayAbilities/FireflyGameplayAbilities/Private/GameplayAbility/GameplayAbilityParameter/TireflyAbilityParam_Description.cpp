// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Description.h"

#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "Internationalization/Regex.h"


FText UTireflyAbilityParam_CommonDescription::GetShowcaseText_Implementation() const
{
	const UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this);
	if (!AbilityAsset)
	{
		return ShowcaseText;
	}
	
	FString ShowcaseString = ShowcaseText.ToString();
	const FRegexPattern ParamPattern(TEXT("#([^#]*)#"));
	FRegexMatcher ParamMatcher(ParamPattern, ShowcaseString);
		
	while (ParamMatcher.FindNext())
	{
		FString ParamNameSharp = ParamMatcher.GetCaptureGroup(0);
		FString ParamName = ParamMatcher.GetCaptureGroup(1);
		const UTireflyGameplayAbilityParameter* Parameter = AbilityAsset->AbilityParameters.FindRef(FName(ParamName));
		if (!Parameter)
		{
			continue;
		}
		
		ShowcaseString = ShowcaseString.Replace(*ParamNameSharp, *Parameter->GetShowcaseText().ToString());
	}
	
	return FText::FromString(ShowcaseString);
}
