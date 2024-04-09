// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/TireflyGameplayAbilityParameter.h"

#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


void UTireflyAbilityParam_ConstantNumeric::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property
		&& (PropertyChangedEvent.Property->GetName() == "ParamValue"
			|| PropertyChangedEvent.Property->GetName() == "CalcMode"))
	{
		if (ParamValue == 0.f)
		{
			ShowcaseText = FString("0");
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

void UTireflyAbilityParam_LevelBaseNumeric_Array::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property
		&& (PropertyChangedEvent.Property->GetName() == "ParamValue"
			|| PropertyChangedEvent.Property->GetName() == "CalcMode"))
	{
		if (ParamValue.IsEmpty())
		{
			ShowcaseText.Empty();
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

void UTireflyAbilityParam_MathExpression::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == "MathExpression")
	{
		ExpressionVariables.Empty();
		if (MathExpression.IsEmpty())
		{
			return;
		}
		
		const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
		FRegexMatcher NumericMatcher(VariablePattern, MathExpression);

		while (NumericMatcher.FindNext())
		{
			ExpressionVariables.Add(NumericMatcher.GetCaptureGroup(0));
		}
	}
}

TArray<FName> UTireflyAbilityParam_GameplayEffect::GetAbilityParamOptions() const
{
	TArray<FName> OutOptions;
	if (const auto AbilityAsset = Cast<UTireflyGameplayAbilityAsset>(GetOuter()))
	{
		for (const auto& Param : AbilityAsset->AbilityParameters)
		{
			if (Param.Value == this)
			{
				continue;
			}

			OutOptions.Add(Param.Key);
		}
	}
	
	return OutOptions;
}
