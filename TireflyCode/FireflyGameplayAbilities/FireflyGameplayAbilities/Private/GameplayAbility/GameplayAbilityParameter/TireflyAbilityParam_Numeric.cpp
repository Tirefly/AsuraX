// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemLog.h"
#include "FireflyAbilitySystemComponent.h"
#include "FireflyGameplayAbilitiesSettings.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "Internationalization/Regex.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"


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

float UTireflyAbilityParam_ConstantNumeric::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	return GetParamValueCalculated(ParamValue);
}

void UTireflyAbilityParam_AttributeCapture::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && (PropertyChangedEvent.MemberProperty->GetName() ==
		GET_MEMBER_NAME_STRING_CHECKED(UTireflyAbilityParam_AttributeCapture, AttributeToCapture)))
	{
		const UFireflyGameplayAbilitiesSettings* Settings = GetDefault<UFireflyGameplayAbilitiesSettings>();
		if (!Settings)
		{
			return;
		}

		ShowcaseText.FromStringTable(Settings->GetAttributeLocalizationTableId(), AttributeToCapture.AttributeName);
	}
}

float UTireflyAbilityParam_AttributeCapture::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (IsValid(ParamInfo.CasterASC) && CaptureSource == ETireflyAbilityParamSourceType::Caster)
	{
		return ParamInfo.CasterASC->GetNumericAttribute(AttributeToCapture);
	}

	if (IsValid(ParamInfo.TargetASC) && CaptureSource == ETireflyAbilityParamSourceType::Target)
	{
		return ParamInfo.TargetASC->GetNumericAttribute(AttributeToCapture);
	}

	return 0.f;
}

FText UTireflyAbilityParam_AttributeCapture::GetShowcaseText_Implementation() const
{
	const UFireflyGameplayAbilitiesSettings* Settings = GetDefault<UFireflyGameplayAbilitiesSettings>();
	if (!Settings)
	{
		return Super::GetShowcaseText_Implementation();
	}

	UStringTable* StringTableObj = Settings->AttributeStringTable.Get();
	if (!StringTableObj)
	{
		return Super::GetShowcaseText_Implementation();
	}

	FStringTableConstPtr StringTableRef = StringTableObj->GetStringTable();
	if (!StringTableRef.IsValid())
	{
		return Super::GetShowcaseText_Implementation();
	}

	const FStringTable* StringTable = StringTableRef.Get();

	FString ShowcaseString;
	StringTable->GetSourceString(FTextKey(AttributeToCapture.AttributeName), ShowcaseString);
	return FText::FromString(ShowcaseString);
}

float UTireflyAbilityParam_OtherNumericParam::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
	{
		if (UTireflyGameplayAbilityParameter* Param = AbilityAsset->AbilityParameters.FindRef(ParameterName))
		{
			if (UTireflyAbilityParam_Numeric* NumericParam = Cast<UTireflyAbilityParam_Numeric>(Param))
			{
				return NumericParam->GetParamValue(ParamInfo);
			}
		}
	}
	
	return Super::GetParamValue_Implementation(ParamInfo);
}

FText UTireflyAbilityParam_OtherNumericParam::GetShowcaseText_Implementation() const
{
	if (UFireflyGameplayAbilityAsset* AbilityAsset = TireflyAbilityParameterHelper::GetAbilityAsset(this))
	{
		if (UTireflyGameplayAbilityParameter* Param = AbilityAsset->AbilityParameters.FindRef(ParameterName))
		{
			if (UTireflyAbilityParam_Numeric* NumericParam = Cast<UTireflyAbilityParam_Numeric>(Param))
			{
				return NumericParam->GetShowcaseText();
			}
		}
	}
	
	return FText::FromString("");
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

float UTireflyAbilityParam_LevelBasedNumeric_Array::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (!IsValid(ParamInfo.CasterASC) || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return GetParmaValueAtLevel(ParamInfo.Level);
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		return GetParmaValueAtLevel(ParamInfo.CasterASC->GetAbilityCasterLevel());
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = ParamInfo.CasterASC->FindAbilitySpecFromHandle(ParamInfo.AbilityHandle))
		{
			return GetParmaValueAtLevel(Spec->Level);
		}
	}

	return GetParmaValueAtLevel(ParamInfo.Level);
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

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (!IsValid(ParamInfo.CasterASC) || LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::None)
	{
		return GetParmaValueAtLevel(ParamInfo.Level);
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster)
	{
		return GetParmaValueAtLevel(ParamInfo.CasterASC->GetAbilityCasterLevel());
	}

	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability)
	{
		if (const FGameplayAbilitySpec* Spec = ParamInfo.CasterASC->FindAbilitySpecFromHandle(ParamInfo.AbilityHandle))
		{
			return GetParmaValueAtLevel(Spec->Level);
		}
	}

	return GetParmaValueAtLevel(ParamInfo.Level);
}

void UTireflyAbilityParam_MathExpression::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == "MathExpression")
	{
		ExpressionVariables.Empty();
		ShowcaseText = FText::FromString("");
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
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == "ExpressionVariables")
	{
		ShowcaseText = FText::FromString("");
		if (ExpressionVariables.Num() == 0 || MathExpression.IsEmpty())
		{
			return;
		}
		
		const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
		FRegexMatcher NumericMatcher(VariablePattern, MathExpression);

		FString Expression = MathExpression;
		while (NumericMatcher.FindNext())
		{
			FString VarName = NumericMatcher.GetCaptureGroup(0);
			UTireflyAbilityParam_Numeric* Variable = ExpressionVariables.FindRef(VarName);
			if (!Variable)
			{
				return;
			}
			
			Expression = Expression.Replace(*VarName, *Variable->GetShowcaseText().ToString());
		}
		ShowcaseText = FText::FromString(Expression);
	}
}

float UTireflyAbilityParam_MathExpression::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
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

		float VariableValue = Variable->GetParamValue(ParamInfo);
		ExpressionToCalc = ExpressionToCalc.Replace(*VariableName,	*FString::SanitizeFloat(VariableValue));
	}

	float OutValue = 0.f;
	FMath::Eval(ExpressionToCalc, OutValue);
	return GetParamValueCalculated(OutValue);
}

FText UTireflyAbilityParam_MathExpression::GetShowcaseText_Implementation() const
{
	if (ExpressionVariables.Num() == 0 || MathExpression.IsEmpty())
	{
		return Super::GetShowcaseText_Implementation();
	}
		
	const FRegexPattern VariablePattern(TEXT("<([^>]*)>"));
	FRegexMatcher NumericMatcher(VariablePattern, MathExpression);

	FString Expression = MathExpression;
	while (NumericMatcher.FindNext())
	{
		FString VarName = NumericMatcher.GetCaptureGroup(0);
		UTireflyAbilityParam_Numeric* Variable = ExpressionVariables.FindRef(VarName);
		if (!Variable)
		{
			return Super::GetShowcaseText_Implementation();
		}
			
		Expression = Expression.Replace(*VarName, *Variable->GetShowcaseText().ToString());
	}
	return FText::FromString(Expression);
}
