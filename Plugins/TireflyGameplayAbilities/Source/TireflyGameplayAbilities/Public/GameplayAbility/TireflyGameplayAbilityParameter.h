// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "TireflyGameplayAbilityParameter.generated.h"


class UGameplayEffect;


// 能力参数取值模式
UENUM(BlueprintType)
enum class ETireflyAbilityParamValueMode : uint8
{
	Constant		UMETA(ToolTip = "不考虑其他因素直接取值"),
	AbilityLevel	UMETA(ToolTip = "基于技能等级对参数进行取值"),
	CasterLevel		UMETA(ToolTip = "基于技能释放者等级对参数进行取值"),
};


// GameplayAbility数值类参数计算模式
UENUM(BlueprintType, Meta = (Bitflags))
enum class ETireflyAbilityNumericParamCalcMode : uint8
{
	None = 0			UMETA(ToolTip = "参数值不进行任何处理", Hidden),
	Percentage = 1		UMETA(ToolTip = "参数值参与计算时乘以0.01"),
	Negate = 2			UMETA(ToolTip = "参数值参与计算时取相反数"),
	PlusOne = 3			UMETA(ToolTip = "参数值参与计算时加1"),
};


// GameplayAbility参数的基础结构
USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	FTireflyGameplayAbilityParameter() {}
};


// GameplayAbility的数值型参数
USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyAbilityParam_Numeric
	: public FTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 参数取值模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETireflyAbilityParamValueMode ValueMode = ETireflyAbilityParamValueMode::Constant;

	// 参数值，仅当ValueMode为Constant时生效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "ValueMode == ETireflyAbilityParamValueMode::Constant", EditConditionHides))
	float ParamValue = 0.f;

	// 参数值数组，仅当ValueMode不为Constant时生效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "ValueMode != ETireflyAbilityParamValueMode::Constant", EditConditionHides))
	TArray<float> ParamValues;

	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	FTireflyAbilityParam_Numeric() {}
};


// GameplayAbility的数学表达式参数
USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyAbilityParam_MathExpression
	: public FTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 作为参数的数学表达式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MathExpression;

	// 作为参数的数学表达式变量数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (BaseStruct = "/Script/TireflyGameplayAbilities.TireflyGameplayAbilityParameter", ExcludeBaseStruct))
	TArray<FInstancedStruct> ExpressionVariables;
};


// GameplayAbility的范围数值参数
USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyAbilityParam_RangedMagnitude
	: public FTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 数值参数的最小值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (BaseStruct = "/Script/TireflyGameplayAbilities.TireflyGameplayAbilityParameter", ExcludeBaseStruct))
	FInstancedStruct MinValue;

	// 数值参数的最大值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (BaseStruct = "/Script/TireflyGameplayAbilities.TireflyGameplayAbilityParameter", ExcludeBaseStruct))
	FInstancedStruct MaxValue;
};


USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyGameplayEffectSpecParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "ParamName == NAME_None", EditConditionHides))
	float ParamValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (GetOptions = "GetAbilityParamOptions"))
	FName ParamName;

public:
	void InitFromDataAsset(const UTireflyGameplayAbilityAsset* DataAsset) {}
	
	void InitFromDataTable(const UDataTable* DataTable, const FName& RowName) {}
	
	TArray<FName> GetAbilityParamOptions() const { return TArray<FName>{}; }
};


USTRUCT(BlueprintType)
struct TIREFLYGAMEPLAYABILITIES_API FTireflyAbilityParam_GameplayEffect
	: public FTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
};
