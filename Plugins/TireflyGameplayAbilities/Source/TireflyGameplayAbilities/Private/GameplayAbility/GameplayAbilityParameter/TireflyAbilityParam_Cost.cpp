// Copyright Tirefly. All Rights Reserved.


#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Cost.h"

#include "TireflyAbilitySystemLibrary.h"
#include "TireflyAbilitySystemComponent.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyAbilityParam_Numeric.h"
#include "Kismet/KismetMathLibrary.h"


float UTireflyAbilityParam_CommonCost::GetCostValue_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (CostValue)
	{
		return CostValue->GetParamValue(CasterASC, TargetASC, AbilityHandle, Level);
	}
	
	return 0.f;
}

bool UTireflyAbilityParam_CommonCost::CheckCost_Implementation(const UTireflyAbilitySystemComponent* CasterASC,
	const UTireflyAbilitySystemComponent* TargetASC, const FGameplayAbilitySpecHandle AbilityHandle, int32 Level) const
{
	if (!CasterASC)
	{
		return false;
	}

	float OldCostedValue = GetCostValue(CasterASC, TargetASC, AbilityHandle, Level);
	float NewCostedValue = CasterASC->GetNumericAttribute(GetCostAttribute());
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
	
	if (!UTireflyAbilitySystemLibrary::GetAttributeValueInRange(CasterASC->GetOwner(),
		GetCostAttribute(), OldCostedValue, NewCostedValue, CostRange))
	{
		return false;
	}
	
	return UKismetMathLibrary::InRange_FloatFloat(OldCostedValue, CostRange.X, CostRange.Y);
}

FText UTireflyAbilityParam_CommonCost::GetShowcaseText_Implementation() const
{
	if (CostValue)
	{
		return CostValue->GetShowcaseText();
	}
	
	return Super::GetShowcaseText_Implementation();
}
