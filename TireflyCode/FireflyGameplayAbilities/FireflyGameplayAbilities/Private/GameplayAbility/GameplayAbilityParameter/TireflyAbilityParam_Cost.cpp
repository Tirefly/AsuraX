// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"

#include "FireflyAbilitySystemLibrary.h"
#include "FireflyAbilitySystemComponent.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"
#include "Kismet/KismetMathLibrary.h"


float UTireflyAbilityParam_GenericCost::GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (CostValue)
	{
		return CostValue->GetParamValue(ParamInfo);
	}
	
	return 0.f;
}

bool UTireflyAbilityParam_GenericCost::CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (!ParamInfo.CasterASC)
	{
		return false;
	}

	float OldCostedValue = GetCostValue(ParamInfo);
	float NewCostedValue = ParamInfo.CasterASC->GetNumericAttribute(GetCostAttribute());

	switch (GetModifierOp())
	{
	case EGameplayModOp::Additive:
		OldCostedValue = OldCostedValue + NewCostedValue;
		break;
	case EGameplayModOp::Multiplicitive:
		OldCostedValue = OldCostedValue * NewCostedValue;
		break;
	case EGameplayModOp::Division:
		OldCostedValue = OldCostedValue / NewCostedValue;
		break;
	case EGameplayModOp::Override:
	default:
		break;
	}

	float RangeMin = 0.f;
	float RangeMax = 0.f;
	if (!UFireflyAbilitySystemLibrary::GetAttributeValueInRange(ParamInfo.CasterASC->GetOwner(),
		GetCostAttribute(), OldCostedValue, NewCostedValue, RangeMin, RangeMax))
	{
		return false;
	}
	
	return UKismetMathLibrary::InRange_FloatFloat(OldCostedValue, RangeMin, RangeMax);
}

FText UTireflyAbilityParam_GenericCost::GetShowcaseText_Implementation() const
{
	if (CostValue)
	{
		return CostValue->GetShowcaseText();
	}
	
	return Super::GetShowcaseText_Implementation();
}
