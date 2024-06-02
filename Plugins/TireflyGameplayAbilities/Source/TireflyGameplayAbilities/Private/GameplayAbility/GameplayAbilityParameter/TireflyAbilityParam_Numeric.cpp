// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"

#include "AbilitySystemLog.h"
#include "TireflyAbilitySystemComponent.h"
#include "TireflyAbilitySystemLibrary.h"
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

	if (PropertyChangedEvent.MemberProperty
		&& (PropertyChangedEvent.MemberProperty->GetName() == "ParamValue"
			|| PropertyChangedEvent.MemberProperty->GetName() == "CalcMode"))
	{
		if (ParamValue == 0.f)
		{
			DisplayText = FText::FromString("0");
			return;
		}
		DisplayText = GetDisplayText();
	}
}

FText UTireflyAbilityParam_ConstantNumeric::GetDisplayText_Implementation() const
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

float UTireflyAbilityParam_AttributeCapture::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (IsValid(ParamInfo.CasterASC) && AttributeSource == ETireflyAbilityParamSourceType::Caster)
	{
		return ParamInfo.CasterASC->GetNumericAttribute(AttributeType);
	}

	if (IsValid(ParamInfo.TargetASC) && AttributeSource == ETireflyAbilityParamSourceType::Target)
	{
		return ParamInfo.TargetASC->GetNumericAttribute(AttributeType);
	}

	return 0.f;
}

float UTireflyAbilityParam_AnotherNumericParam::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (UTireflyGameplayAbilityAsset* AbilityAsset = UTireflyAbilitySystemLibrary::GetAbilityAsset(this))
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

float UTireflyAbilityParam_LevelBased::GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Caster && IsValid(ParamInfo.CasterASC))
	{
		return GetParmaValueAtLevel(ParamInfo.CasterASC->GetAbilityCasterLevel());
	}

	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Target && IsValid(ParamInfo.TargetASC))
	{
		return GetParmaValueAtLevel(ParamInfo.TargetASC->GetAbilityCasterLevel());
	}
	
	if (LevelBasedMode == ETireflyAbilityNumericParamLevelBasedMode::Ability && ParamInfo.AbilityHandle.IsValid())
	{
		if (const FGameplayAbilitySpec* Spec = ParamInfo.CasterASC->FindAbilitySpecFromHandle(ParamInfo.AbilityHandle))
		{
			return GetParmaValueAtLevel(Spec->Level);
		}
	}

	return GetParmaValueAtLevel(1.f);
}

void UTireflyAbilityParam_LevelBasedNumeric_Array::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty
		&& (PropertyChangedEvent.MemberProperty->GetName() == "ParamValue"
			|| PropertyChangedEvent.MemberProperty->GetName() == "CalcMode"))
	{
		if (ParamValue.IsEmpty())
		{
			DisplayText = FText::FromString("");
			return;
		}
		DisplayText = GetDisplayText();
	}
}

FText UTireflyAbilityParam_LevelBasedNumeric_Array::GetDisplayText_Implementation() const
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

float UTireflyAbilityParam_LevelBasedNumeric_Array::GetParmaValueAtLevel_Implementation(int32 Level) const
{
	return GetParamValueCalculated(ParamValue.FindRef(Level));
}

void UTireflyAbilityParam_LevelBasedNumeric_Curve::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty
		&& (PropertyChangedEvent.MemberProperty->GetName() == "ParamValue"
			|| PropertyChangedEvent.MemberProperty->GetName() == "CalcMode"))
	{
		if (ParamValue.GetRichCurveConst()->Keys.IsEmpty())
		{
			DisplayText = FText::FromString("");
			return;
		}
		DisplayText = GetDisplayText();
	}
}

FText UTireflyAbilityParam_LevelBasedNumeric_Curve::GetDisplayText_Implementation() const
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

float UTireflyAbilityParam_LevelBasedNumeric_Curve::GetParmaValueAtLevel_Implementation(int32 Level) const
{
	float MinVal = 0.f, MaxVal = 0.f;
	ParamValue.GetRichCurveConst()->GetValueRange(MinVal, MaxVal);
	float OutValue = ParamValue.GetRichCurveConst()->Eval(Level);
	
	return GetParamValueCalculated(OutValue);
}

void UTireflyAbilityParam_MathExpression::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetName() == "MathExpression")
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
				*UTireflyAbilitySystemLibrary::GetAbilityAsset(this)->GetName(),
				*GetParameterName().ToString(),
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
