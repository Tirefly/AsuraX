// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LegendConfig_CombatUnit.h"

#include "AbilitySystemComponent.h"


ULegendConfig_CombatUnit::ULegendConfig_CombatUnit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ULegendConfig_CombatUnit::GetAttributeRange(const FGameplayAttribute& Attribute,
	UAbilitySystemComponent* ASC, float& RangeMin, float& RangeMax) const
{
	const FFireflyAttributeRange* Range = AttributeRangeSetting.Find(Attribute);
	if (!Range)
	{
		return false;
	}

	if (!ASC)
	{
		return false;
	}

	switch (Range->MaxValueType)
	{
	case EFireflyAttributeRangeType::None:
		RangeMax = (double)INT64_MAX;
		break;
	case EFireflyAttributeRangeType::Numeric:
		RangeMax = Range->MaxValue;
		break;
	case EFireflyAttributeRangeType::Attribute:
		RangeMax = ASC->GetNumericAttribute(Range->MaxAttribute);
		break;
	}

	switch (Range->MinValueType)
	{
	case EFireflyAttributeRangeType::None:
		RangeMin = (double)INT64_MIN;
		break;
	case EFireflyAttributeRangeType::Numeric:
		RangeMin = Range->MinValue;
		break;
	case EFireflyAttributeRangeType::Attribute:
		RangeMin = ASC->GetNumericAttribute(Range->MinAttribute);
		break;
	}

	return true;
}
