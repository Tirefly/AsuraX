// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TireflyAbilityParam_Numeric.generated.h"


class UFireflyAbilitySystemComponent;
class UTireflyGameplayAbilityAsset;


// GameplayAbility数值类参数等级计算模式
UENUM(BlueprintType)
enum class ETireflyAbilityNumericParamLevelBasedMode : uint8
{
	None = 0			UMETA(ToolTip = "参数值不考虑任何等级"),
	Caster = 1			UMETA(ToolTip = "参数值考虑施法者等级"),
	Ability = 2			UMETA(ToolTip = "参数值考虑技能等级")
};


// GameplayAbility数值类参数计算模式
UENUM(BlueprintType, Meta = (Bitflags))
enum class ETireflyAbilityNumericParamCalcMode : uint8
{
	None = 0			UMETA(ToolTip = "参数值不进行任何处理", Hidden),
	Percentage = 1		UMETA(ToolTip = "参数值参与计算时乘以0.01，适用于描述中带有“15%概率”、“造成35%攻击力的伤害”的技能"),
	Negate = 2			UMETA(ToolTip = "参数值参与计算时取相反数，适用于描述中带有“降低50点护甲”的技能"),
	PlusOne = 3			UMETA(ToolTip = "参数值参与计算时加1，结合Percentage，适用于描述中带有“增加15%攻击力”的技能"),
};


// 能作为数学表达式变量的GameplayAbility数值型参数基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_Numeric
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Bitmask,
		BitmaskEnum = "/Script/FireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	uint8 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	// 获取参数值，返回值会经过“CalcMode”处理
	UFUNCTION(BlueprintPure)
	float GetParamValueCalculated(const float& InParamValue) const;
	
	// 获取参数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetParamValue(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}
};


// GameplayAbility的数学表达式变量基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ExpressionVariable
	: public UTireflyAbilityParam_Numeric
{
	GENERATED_BODY()
};


// GameplayAbility的常数型参数
UCLASS(DisplayName = "Constant Numeric")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ConstantNumeric
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 参数值，仅当ValueMode为Constant时生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ParamValue = 0.f;

public:
	virtual FText GetShowcaseText_Implementation() const override;

	// 获取参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// GameplayAbility的属性抓取参数
UCLASS(DisplayName = "Attribute Capture")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AttributeCapture
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 要抓取的属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute AttributeToCapture;

	// 属性抓取来源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityParamSourceType CaptureSource = ETireflyAbilityParamSourceType::Caster;

	// 是否抓取快照
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSnapshot = false;

public:
	// 获取参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};


// GameplayAbility的OtherNumericParam
UCLASS(DisplayName = "Other Numeric Param")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_OtherNumericParam
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (GetOptions = "GetAbilityOtherNumericParameters"))
	FName ParameterName = NAME_None;

public:
	// 获取参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};


// GameplayAbility的基于等级的参数基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBased
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityNumericParamLevelBasedMode LevelBasedMode;

public:
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure)
	virtual float GetParmaValueAtLevel(int32 Level = 1) const { return 0; }
};


// GameplayAbility的基于等级的数组型参数
UCLASS(DisplayName = "Level Base Array Numeric")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBasedNumeric_Array
	: public UTireflyAbilityParam_LevelBased
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 基于等级的参数值曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, float> ParamValue;

public:
	virtual FText GetShowcaseText_Implementation() const override;
	
	// 获取特定等级的参数值
	virtual float GetParmaValueAtLevel(int32 Level = 1) const override;

	// 获取参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// GameplayAbility的基于等级的曲线型参数
UCLASS(DisplayName = "Level Base Curve Numeric")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBasedNumeric_Curve
	: public UTireflyAbilityParam_LevelBased
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 基于等级的参数值曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRuntimeFloatCurve ParamValue;

public:
	virtual FText GetShowcaseText_Implementation() const override;
	
	// 获取特定等级的参数值
	virtual float GetParmaValueAtLevel(int32 Level) const override;

	// 获取参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// GameplayAbility的数学表达式参数
UCLASS(DisplayName = "Math Expression")
class FIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_MathExpression
	: public UTireflyAbilityParam_Numeric
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 作为参数的数学表达式，表达式中的变量参数需要包含在<>内，如“<x>+15*<y>+<w>*<z>”
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MathExpression;

	// 作为参数的数学表达式变量数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UTireflyAbilityParam_ExpressionVariable*> ExpressionVariables;

public:
	// 获取数学表达式的参数值
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual FText GetShowcaseText_Implementation() const override;
};