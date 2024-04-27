// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpecHandle.h"
#include "TireflyAbilityParam_Numeric.generated.h"


class UTireflyAbilitySystemComponent;
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
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_Numeric
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));
};


// GameplayAbility的数学表达式变量基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ExpressionVariable
	: public UTireflyAbilityParam_Numeric
{
	GENERATED_BODY()
};


// GameplayAbility的常数型参数
UCLASS(DisplayName = "Constant Numeric")
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ParamValue = 0.f;

public:
	virtual FString GetShowcaseText() const override;

	// 获取参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue() const;
};


// GameplayAbility的属性抓取参数
UCLASS(DisplayName = "Attribute Capture")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AttributeCapture
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 要抓取的属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeToCapture;

	// 属性抓取来源
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETireflyAbilityParamSourceType CaptureSource = ETireflyAbilityParamSourceType::Caster;

	// 是否抓取快照
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSnapshot = false;

public:
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue(const UTireflyAbilitySystemComponent* CasterASC) const;

	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValueWithTarget(const UTireflyAbilitySystemComponent* CasterASC
		, const UTireflyAbilitySystemComponent* TargetASC) const;
};


// GameplayAbility的基于等级的参数基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBased
	: public UTireflyAbilityParam_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETireflyAbilityNumericParamLevelBasedMode LevelBasedMode;
};


// GameplayAbility的基于等级的数组型参数
UCLASS(DisplayName = "Level Base Array Numeric")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBasedNumeric_Array
	: public UTireflyAbilityParam_LevelBased
{
	GENERATED_BODY()

public:
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

public:
	// 基于等级的参数值曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, float> ParamValue;

public:
	virtual FString GetShowcaseText() const override;
	
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParmaValueAtLevel(int32 Level) const { return ParamValue.FindRef(Level); }

	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue(const UTireflyAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& Handle, int32 Level) const;
};


// GameplayAbility的基于等级的曲线型参数
UCLASS(DisplayName = "Level Base Curve Numeric")
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRuntimeFloatCurve ParamValue;

public:
	virtual FString GetShowcaseText() const override;
	
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParmaValueAtLevel(int32 Level) const;

	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue(const UTireflyAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& Handle, int32 Level) const;
};


// GameplayAbility的数学表达式参数
UCLASS(DisplayName = "Math Expression")
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MathExpression;

	// 作为参数的数学表达式变量数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, UTireflyAbilityParam_ExpressionVariable*> ExpressionVariables;

public:
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue(const UTireflyAbilitySystemComponent* CasterASC, const FGameplayAbilitySpecHandle& Handle, int32 Level) const;

	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValueWithTarget(const UTireflyAbilitySystemComponent* CasterASC, const UTireflyAbilitySystemComponent* TargetASC
		, const FGameplayAbilitySpecHandle& Handle, int32 Level) const;
};