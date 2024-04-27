// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"

#include "TireflyAbilitySystemComponent.h"


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

FString UTireflyAbilityParam_ConstantNumeric::GetShowcaseText() const
{
	FString OutText = FString::Printf(TEXT("%g"), ParamValue);
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
		OutText += TEXT("%");
	return OutText;
}

float UTireflyAbilityParam_ConstantNumeric::GetParamValue() const
{
	float OutValue = ParamValue;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
		OutValue *= 0.01f;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Negate)))
		OutValue = -OutValue;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::PlusOne)))
		OutValue += 1.f;
	return OutValue;
}

float UTireflyAbilityParam_AttributeCapture::GetParamValue(const UTireflyAbilitySystemComponent* CasterASC) const
{
	if (IsValid(CasterASC))
	{
		return CasterASC->GetNumericAttribute(AttributeToCapture);
	}

	return 0.f;
}

float UTireflyAbilityParam_AttributeCapture::GetParamValueWithTarget(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC) const
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
			ShowcaseText.Empty();
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

FString UTireflyAbilityParam_LevelBasedNumeric_Array::GetShowcaseText() const
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
	return OutText;
}

float UTireflyAbilityParam_LevelBasedNumeric_Array::GetParamValue(const UTireflyAbilitySystemComponent* ASC,
	const FGameplayAbilitySpecHandle& Handle, int32 Level) const
{
	if (!IsValid(ASC) || !Handle.IsValid() || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return  GetParmaValueAtLevel(Level);
	}
	
	float OutValue = 0.f;
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		OutValue = ParamValue.FindRef(ASC->GetAbilityCasterLevel());
	}
	else if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle))
		{
			OutValue = ParamValue.FindRef(Spec->Level);
		}
	}

	return OutValue;
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
			ShowcaseText.Empty();
			return;
		}
		ShowcaseText = GetShowcaseText();
	}
}

FString UTireflyAbilityParam_LevelBasedNumeric_Curve::GetShowcaseText() const
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
		
	return OutText;
}

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParmaValueAtLevel(int32 Level) const
{
	float MinVal = 0.f, MaxVal = 0.f;
	ParamValue.GetRichCurveConst()->GetValueRange(MinVal, MaxVal);
	float OutValue = ParamValue.GetRichCurveConst()->Eval(Level);
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
		OutValue *= 0.01f;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Negate)))
		OutValue = -OutValue;
	if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::PlusOne)))
		OutValue += 1.f;
	return OutValue;
}

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParamValue(const UTireflyAbilitySystemComponent* ASC,
	const FGameplayAbilitySpecHandle& Handle, int32 Level) const
{
	if (!IsValid(ASC) || !Handle.IsValid() || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return  GetParmaValueAtLevel(Level);
	}
	
	float OutValue = 0.f;
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		OutValue = GetParmaValueAtLevel(ASC->GetAbilityCasterLevel());
	}
	else if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle))
		{
			OutValue = GetParmaValueAtLevel(Spec->Level);
		}
	}

	return OutValue;
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

float UTireflyAbilityParam_MathExpression::GetParamValue(const UTireflyAbilitySystemComponent* CasterASC,
	const FGameplayAbilitySpecHandle& Handle, int32 Level) const
{
	if (!IsValid(CasterASC) || !Handle.IsValid())
	{
		return 0.f;
	}

	float OutValue = 0.f;

	FString ExpressionToCalc = MathExpression;
	const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
	FRegexMatcher NumericMatcher(VariablePattern, ExpressionToCalc);
	while (NumericMatcher.FindNext())
	{
		UTireflyAbilityParam_Numeric* Variable = ExpressionVariables.FindRef(NumericMatcher.GetCaptureGroup(0));
		if (!IsValid(Variable))
		{
			// TODO: Log error
			break;
		}

		if (UTireflyAbilityParam_ConstantNumeric* ConstantNumeric = Cast<UTireflyAbilityParam_ConstantNumeric>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(ConstantNumeric->GetParamValue()));
		}
		else if (UTireflyAbilityParam_AttributeCapture* AttributeCapture = Cast<UTireflyAbilityParam_AttributeCapture>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(AttributeCapture->GetParamValue(CasterASC)));
		}
		else if (UTireflyAbilityParam_LevelBasedNumeric_Array* LevelBasedNumeric_Array = Cast<UTireflyAbilityParam_LevelBasedNumeric_Array>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(LevelBasedNumeric_Array->GetParamValue(CasterASC, Handle, Level)));
		}
		else if (UTireflyAbilityParam_LevelBasedNumeric_Curve* LevelBasedNumeric_Curve = Cast<UTireflyAbilityParam_LevelBasedNumeric_Curve>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(LevelBasedNumeric_Curve->GetParamValue(CasterASC, Handle, Level)));
		}
	}

	return OutValue;
}

float UTireflyAbilityParam_MathExpression::GetParamValueWithTarget(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle& Handle, int32 Level) const
{
	if (!IsValid(CasterASC) || !IsValid(TargetASC) || !Handle.IsValid())
	{
		return 0.f;
	}

	float OutValue = 0.f;

	FString ExpressionToCalc = MathExpression;
	const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
	FRegexMatcher NumericMatcher(VariablePattern, ExpressionToCalc);
	while (NumericMatcher.FindNext())
	{
		UTireflyAbilityParam_Numeric* Variable = ExpressionVariables.FindRef(NumericMatcher.GetCaptureGroup(0));
		if (!IsValid(Variable))
		{
			// TODO: Log error
			break;
		}

		if (UTireflyAbilityParam_ConstantNumeric* ConstantNumeric = Cast<UTireflyAbilityParam_ConstantNumeric>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(ConstantNumeric->GetParamValue()));
		}
		else if (UTireflyAbilityParam_AttributeCapture* AttributeCapture = Cast<UTireflyAbilityParam_AttributeCapture>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(AttributeCapture->GetParamValueWithTarget(CasterASC, TargetASC)));
		}
		else if (UTireflyAbilityParam_LevelBasedNumeric_Array* LevelBasedNumeric_Array = Cast<UTireflyAbilityParam_LevelBasedNumeric_Array>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(LevelBasedNumeric_Array->GetParamValue(CasterASC, Handle, Level)));
		}
		else if (UTireflyAbilityParam_LevelBasedNumeric_Curve* LevelBasedNumeric_Curve = Cast<UTireflyAbilityParam_LevelBasedNumeric_Curve>(Variable))
		{
			ExpressionToCalc = ExpressionToCalc.Replace(*NumericMatcher.GetCaptureGroup(0)
				, *FString::SanitizeFloat(LevelBasedNumeric_Curve->GetParamValue(CasterASC, Handle, Level)));
		}
	}

	return OutValue;
}
