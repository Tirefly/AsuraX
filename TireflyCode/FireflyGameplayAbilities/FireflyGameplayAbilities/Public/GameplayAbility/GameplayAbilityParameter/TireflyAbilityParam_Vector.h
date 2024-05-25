// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_Vector.generated.h"


// 技能参数：向量
UCLASS(DisplayName = "Vector Parameter")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_Vector : public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 向量技能参数值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FVector ParamValue;

public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	FVector GetParamValue() const { return ParamValue; }
};
