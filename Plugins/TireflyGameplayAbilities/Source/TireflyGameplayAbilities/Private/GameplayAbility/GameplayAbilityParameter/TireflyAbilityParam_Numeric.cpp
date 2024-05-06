// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"

#include "AbilitySystemLog.h"
#include "TireflyAbilitySystemComponent.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"


float UTireflyAbilityParam_Numeric::GetParamValueCalculated(const float& InParamValue) const
{
	float OutParamValue = InParamValue;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
		OutParamValue *= 0.01f;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Negate)))
		OutParamValue = -OutParamValue;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::PlusOne)))
		OutParamValue += 1.f;
	
	return OutParamValue;
}

void UTireflyAbilityParam_ConstantNumeric::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property
		&& (PropertyChangedEvent.Property->GetName() == "ParamValue"
			|| PropertyChangedEvent.Property->GetName() == "CalcMode"))
	{
		if (ParamValue == 0.f)
		{
			ShowcaseText = FText::FromString("0");
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

FText UTireflyAbilityParam_ConstantNumeric::GetShowcaseText_Implementation() const
{
	FString OutText = FString::Printf(TEXT("%g"), ParamValue);
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
		OutText += TEXT("%");
	return FText::FromString(OutText);
}

float UTireflyAbilityParam_ConstantNumeric::GetParamValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	return GetParamValueCalculated(ParamValue);
}

float UTireflyAbilityParam_AttributeCapture::GetParamValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (IsValid(CasterASC) && CaptureSource == ETireflyAbilityParamSourceType::Caster)
	{
		return CasterASC->GetNumericAttribute(AttributeToCapture);
	}

	if (IsValid(TargetASC) && CaptureSource == ETireflyAbilityParamSourceType::Target)
	{
		return TargetASC->GetNumericAttribute(AttributeToCapture);
	}

	return 0.f;
}

void UTireflyAbilityParam_LevelBasedNumeric_Array::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property
		&& (PropertyChangedEvent.Property->GetName() == "ParamValue"
			|| PropertyChangedEvent.Property->GetName() == "CalcMode"))
	{
		if (ParamValue.IsEmpty())
		{
			ShowcaseText = FText::FromString("");
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

FText UTireflyAbilityParam_LevelBasedNumeric_Array::GetShowcaseText_Implementation() const
{
	FString OutText;
	for (auto& Pair : ParamValue)
	{
		FString Param = FString::Printf(TEXT("%g"), Pair.Value);
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
			Param += TEXT("%");
		OutText += FString::Printf(TEXT("%s/"), *Param);
	}
	OutText.RemoveFromEnd(TEXT("/"));
	return FText::FromString(OutText);
}

float UTireflyAbilityParam_LevelBasedNumeric_Array::GetParmaValueAtLevel(int32 Level) const
{
	return GetParamValueCalculated(ParamValue.FindRef(Level));
}

float UTireflyAbilityParam_LevelBasedNumeric_Array::GetParamValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (!IsValid(CasterASC) || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return GetParmaValueAtLevel(Level);
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		return GetParmaValueAtLevel(CasterASC->GetAbilityCasterLevel());
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = CasterASC->FindAbilitySpecFromHandle(AbilityHandle))
		{
			return GetParmaValueAtLevel(Spec->Level);
		}
	}

	return GetParmaValueAtLevel(Level);
}

void UTireflyAbilityParam_LevelBasedNumeric_Curve::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property
		&& (PropertyChangedEvent.Property->GetName() == "ParamValue"
			|| PropertyChangedEvent.Property->GetName() == "CalcMode"))
	{
		if (ParamValue.GetRichCurveConst()->Keys.IsEmpty())
		{
			ShowcaseText = FText::FromString("");
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

FText UTireflyAbilityParam_LevelBasedNumeric_Curve::GetShowcaseText_Implementation() const
{
	FString OutText;
	if (const FRichCurve* RichCurve = ParamValue.GetRichCurveConst())
	{
		float FirstKey = RichCurve->GetFirstKey().Value;
		float LastKey = RichCurve->GetLastKey().Value;
		OutText = FString::Printf(TEXT("%g"), FirstKey)
		+ FString((CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage))) ? "%" : "")
		+ TEXT("~") + FString::Printf(TEXT("%g"), LastKey)
		+ FString((CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage))) ? "%" : "");
	}
	
	return FText::FromString(OutText);
}

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParmaValueAtLevel(int32 Level) const
{
	float MinVal = 0.f, MaxVal = 0.f;
	ParamValue.GetRichCurveConst()->GetValueRange(MinVal, MaxVal);
	float OutValue = ParamValue.GetRichCurveConst()->Eval(Level);
	
	return GetParamValueCalculated(OutValue);
}

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParamValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (!IsValid(CasterASC) || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return GetParmaValueAtLevel(Level);
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		return GetParmaValueAtLevel(CasterASC->GetAbilityCasterLevel());
	}

	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = CasterASC->FindAbilitySpecFromHandle(AbilityHandle))
		{
			return GetParmaValueAtLevel(Spec->Level);
		}
	}

	return GetParmaValueAtLevel(Level);
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

float UTireflyAbilityParam_MathExpression::GetParamValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	FString ExpressionToCalc = MathExpression;
	const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
	FRegexMatcher NumericMatcher(VariablePattern, ExpressionToCalc);
	while (NumericMatcher.FindNext())
	{
		FString VariableName = NumericMatcher.GetCaptureGroup(0);
		UTireflyAbilityParam_ExpressionVariable* Variable = ExpressionVariables.FindRef(VariableName);
		if (!IsValid(Variable))
		{
			UE_LOG(LogAbilitySystem, Error, TEXT("Abililty Parameter {%s}-{%s} Variable {%s} not found!"),
				*TireflyAbilityParameterHelper::GetAbilityAsset(this)->GetName(),
				*GetAbilityParameterName().ToString(),
				*NumericMatcher.GetCaptureGroup(0))
			return 0.f;
		}

		float VariableValue = Variable->GetParamValue(CasterASC, TargetASC, AbilityHandle, Level);
		ExpressionToCalc = ExpressionToCalc.Replace(*VariableName,	*FString::SanitizeFloat(VariableValue));
	}

	float OutValue = 0.f;
	FMath::Eval(ExpressionToCalc, OutValue);
	return GetParamValueCalculated(OutValue);
}
