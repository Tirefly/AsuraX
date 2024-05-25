// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_Description.generated.h"


// GameplayAbility的描述基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_DescriptionBase : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()
};


// GameplayAbility的通用描述
UCLASS(DisplayName = "Common Description")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_CommonDescription : public UTireflyAbilityParam_DescriptionBase
{
	GENERATED_BODY()

public:
	virtual FText GetShowcaseText_Implementation() const override;
};