// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Description.h"

#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "TireflyAbilitySystemLibrary.h"


FText UTireflyAbilityParam_GenericDescription::GetDisplayText_Implementation() const
{
	const UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this);
	if (!AbilityAsset)
	{
		return DisplayText;
	}
	
	FString ShowcaseString = DisplayText.ToString();
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
		
		ShowcaseString = ShowcaseString.Replace(*ParamNameSharp, *Parameter->GetDisplayText().ToString());
	}
	
	return FText::FromString(ShowcaseString);
}
