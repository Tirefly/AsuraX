// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_AbilityDescription.generated.h"


// GameplayAbility的描述基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_DescriptionBase : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()
};


// GameplayAbility的通用描述
UCLASS(DisplayName = "Common Description")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CommonDescription : public UTireflyAbilityParam_DescriptionBase
{
	GENERATED_BODY()

public:
	virtual FText GetShowcaseText() const override;
};