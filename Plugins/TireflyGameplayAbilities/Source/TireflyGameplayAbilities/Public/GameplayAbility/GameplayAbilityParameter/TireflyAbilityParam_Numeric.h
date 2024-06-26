// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "AttributeSet.h"
#include "TireflyAbilityParam_Numeric.generated.h"


class UTireflyAbilitySystemComponent;
class UTireflyGameplayAbilityAsset;


// GameplayAbility数值类参数等级计算模式
UENUM(BlueprintType)
enum class ETireflyAbilityNumericParamLevelBasedMode : uint8
{
	None = 0			UMETA(ToolTip = "参数值不考虑任何等级"),
	Caster = 1			UMETA(ToolTip = "参数值考虑技能的施法者等级"),
	Target = 2			UMETA(ToolTip = "参数值考虑技能的目标等级"),
	Ability = 3			UMETA(ToolTip = "参数值考虑技能等级")
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


// 技能参数：数值类型
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_Numeric
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Bitmask,
		BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	// 获取参数值，返回值会经过“CalcMode”处理
	UFUNCTION(BlueprintPure, Category = "Ability|Parameter")
	float GetParamValueCalculated(const float& InParamValue) const;
	
	// 获取参数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	float GetParamValue(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const;
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const
	{
		return 0.f;
	}
};


// 能作为数学表达式变量的数值类技能参数
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ExpressionVariable
	: public UTireflyAbilityParam_Numeric
{
	GENERATED_BODY()
};


// 数值类技能参数：常量
UCLASS(DisplayName = "Numeric: Constant")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ConstantNumeric
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
	virtual FText GetDisplayText_Implementation() const override;

	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// 数值类技能参数：属性值
UCLASS(DisplayName = "Numeric: Attribute Value")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AttributeCapture
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 属性类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute AttributeType;

	// 属性来源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityParamSourceType AttributeSource = ETireflyAbilityParamSourceType::Caster;

	// 是否抓取快照
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSnapshot = false;

public:
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// 数值类技能参数：另一个数值参数
UCLASS(DisplayName = "Numeric: Another Param")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AnotherNumericParam
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (GetOptions = "GetOtherNumericParameters"))
	FName ParameterName = NAME_None;

public:
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;

	virtual bool IsDisplayTextEditable_Implementation() const override { return false; }
};


// 数值类技能参数：基于等级的
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBased
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireflyAbilityNumericParamLevelBasedMode LevelBasedMode;

public:
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	float GetParmaValueAtLevel(int32 Level = 1) const;
	virtual float GetParmaValueAtLevel_Implementation(int32 Level = 1) const { return 0; }

	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	FText GetDisplayTextAtLevel(int32 Level = 1) const;
	virtual FText GetDisplayTextAtLevel_Implementation(int32 Level = 1) const { return FText(); }

	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};


// 数值类技能参数：基于等级的，用数组表示
UCLASS(DisplayName = "Numeric: Level Base Array")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBasedNumeric_Array
	: public UTireflyAbilityParam_LevelBased
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// 基于等级的参数值容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, float> ParamValue;

public:
	virtual FText GetDisplayText_Implementation() const override;
	
	virtual float GetParmaValueAtLevel_Implementation(int32 Level = 1) const override;
};


// 数值类技能参数：基于等级的，用曲线表示
UCLASS(DisplayName = "Numeric: Level Base Curve")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBasedNumeric_Curve
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
	virtual FText GetDisplayText_Implementation() const override;
	
	virtual float GetParmaValueAtLevel_Implementation(int32 Level = 1) const override;
};


// 数值类技能参数：数学表达式
UCLASS(DisplayName = "Numeric: Math Expression")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_MathExpression
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TMap<FString, TObjectPtr<UTireflyAbilityParam_ExpressionVariable>> ExpressionVariables;

public:
	virtual float GetParamValue_Implementation(FTireflyAbilityParamInfo ParamInfo = FTireflyAbilityParamInfo()) const override;
};