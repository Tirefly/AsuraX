// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_Description.generated.h"


// 技能资产元素：文本描述
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_DescriptionBase : public UTireflyAbilityAssetElement_DisplayText
{
	GENERATED_BODY()
};


// 技能资产元素：通用的文本描述
UCLASS(DisplayName = "Generic Description")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GenericDescription : public UTireflyAbilityParam_DescriptionBase
{
	GENERATED_BODY()

public:
	virtual FText GetDisplayText_Implementation() const override;
};