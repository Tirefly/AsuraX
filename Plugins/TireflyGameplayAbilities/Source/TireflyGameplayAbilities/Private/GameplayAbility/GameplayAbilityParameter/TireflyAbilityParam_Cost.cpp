// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"

#include "TireflyAbilitySystemLibrary.h"
#include "TireflyAbilitySystemComponent.h"
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
	FVector2D CostRange = FVector2D::ZeroVector;

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
	
	if (!UTireflyAbilitySystemLibrary::GetAttributeValueInRange(ParamInfo.CasterASC->GetOwner(),
		GetCostAttribute(), OldCostedValue, NewCostedValue, CostRange))
	{
		return false;
	}
	
	return UKismetMathLibrary::InRange_FloatFloat(OldCostedValue, CostRange.X, CostRange.Y);
}

FText UTireflyAbilityParam_GenericCost::GetDisplayText_Implementation() const
{
	if (CostValue)
	{
		return CostValue->GetDisplayText();
	}
	
	return Super::GetDisplayText_Implementation();
}
