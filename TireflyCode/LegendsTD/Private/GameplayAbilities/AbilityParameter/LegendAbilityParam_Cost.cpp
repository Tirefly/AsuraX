// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/AbilityParameter/LegendAbilityParam_Cost.h"

#include "FireflyAbilitySystemComponent.h"
#include "GameplayAbilities/LegendAttributeSet.h"

FGameplayAttribute ULegendAbilityParam_Cost::GetCostAttribute() const
{
	return ULegendAttributeSet::GetCurrentManaAttribute();
}

TEnumAsByte<EGameplayModOp::Type> ULegendAbilityParam_Cost::GetModifierOp() const
{
	return EGameplayModOp::Additive;
}

float ULegendAbilityParam_Cost::GetCostValue_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (!ParamInfo.CasterASC)
	{
		return 0.f;
	}

	float MaxMana = ParamInfo.CasterASC->GetNumericAttribute(ULegendAttributeSet::GetMaxManaAttribute());
	return -MaxMana;
}

bool ULegendAbilityParam_Cost::CheckCost_Implementation(FTireflyAbilityParamInfo ParamInfo) const
{
	if (!ParamInfo.CasterASC)
	{
		return false;
	}

	float CurrentMana = ParamInfo.CasterASC->GetNumericAttribute(ULegendAttributeSet::GetCurrentManaAttribute());
	float MaxMana = ParamInfo.CasterASC->GetNumericAttribute(ULegendAttributeSet::GetMaxManaAttribute());
	
	return CurrentMana == MaxMana;
}
