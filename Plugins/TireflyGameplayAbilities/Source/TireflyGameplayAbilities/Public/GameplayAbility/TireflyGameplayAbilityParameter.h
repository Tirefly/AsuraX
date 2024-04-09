// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TireflyGameplayAbilityParameter.generated.h"


class UGameplayEffect;
class UTireflyGameplayAbilityAsset;


// GameplayAbility数值类参数计算模式
UENUM(BlueprintType, Meta = (Bitflags))
enum class ETireflyAbilityNumericParamCalcMode : uint8
{
	None = 0			UMETA(ToolTip = "参数值不进行任何处理", Hidden),
	Percentage = 1		UMETA(ToolTip = "参数值参与计算时乘以0.01，适用于描述中带有“15%概率”、“造成35%攻击力的伤害”的技能"),
	Negate = 2			UMETA(ToolTip = "参数值参与计算时取相反数，适用于描述中带有“降低50点护甲”的技能"),
	PlusOne = 3			UMETA(ToolTip = "参数值参与计算时加1，结合Percentage，适用于描述中带有“增加15%攻击力”的技能"),
};


// GameplayAbility参数的基础结构
UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameter : public UObject
{
	GENERATED_BODY()

public:
	// 用于展示的文本
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ShowcaseText;

public:
	// 获取用于展示的文本
	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual FString GetShowcaseText() const { return ShowcaseText; }
};


// 能作为数学表达式变量的GameplayAbility数值型参数基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameter_ExpressionVariable
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()
};


// GameplayAbility的常数型参数
UCLASS(CollapseCategories, DisplayName = "Constant Numeric")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_ConstantNumeric
	: public UTireflyGameplayAbilityParameter_ExpressionVariable
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

	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	virtual FString GetShowcaseText() const override
	{
		FString OutText = FString::Printf(TEXT("%g"), ParamValue);
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
			OutText += TEXT("%");
		return OutText;
	}

	// 获取参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParamValue() const
	{
		float OutValue = ParamValue;
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
			OutValue *= 0.01f;
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Negate)))
			OutValue = -OutValue;
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::PlusOne)))
			OutValue += 1.f;
		return OutValue;
	}
};


// GameplayAbility的属性抓取参数
UCLASS(CollapseCategories, DisplayName = "Attribute Capture")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_AttributeCapture
	: public UTireflyGameplayAbilityParameter_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 要抓取的属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeToCapture;

	// 属性抓取来源
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (GetOptions = "GetAttributeCaptureSourceOptions"))
	FName CaptureSource = FName("Caster");

	// 是否抓取快照
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSnapshot = false;

public:
	UFUNCTION(CallInEditor)
	static TArray<FName> GetAttributeCaptureSourceOptions() { return TArray<FName>{"Caster", "Target" }; }
};


// GameplayAbility的基于等级的数组型参数
UCLASS(CollapseCategories, DisplayName = "Level Base Array Numeric")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBaseNumeric_Array
	: public UTireflyGameplayAbilityParameter_ExpressionVariable
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

	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	virtual FString GetShowcaseText() const override
	{
		FString OutText;
		for (auto& Pair : ParamValue)
		{
			FString Param = FString::Printf(TEXT("%g"), Pair.Value);
			if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
				Param += TEXT("%");
			OutText += FString::Printf(TEXT("%s/"), *Param);
		}
		OutText.RemoveFromEnd(TEXT("/"));
		return OutText;
	}
	
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParmaValueAtLevel(int32 Level) const
	{
		return ParamValue.FindRef(Level);
	}
};


// GameplayAbility的基于等级的曲线型参数
UCLASS(CollapseCategories, DisplayName = "Level Base Curve Numeric")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_LevelBaseNumeric_Curve
	: public UTireflyGameplayAbilityParameter_ExpressionVariable
{
	GENERATED_BODY()

public:
	// 基于等级的参数值曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRuntimeFloatCurve ParamValue;

	// 参数值计算模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = "/Script/TireflyGameplayAbilities.ETireflyAbilityNumericParamCalcMode"))
	int32 CalcMode = (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::None));

public:
	// 获取特定等级的参数值
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetParmaValueAtLevel(int32 Level) const
	{
		float MinVal = 0.f, MaxVal = 0.f;
		ParamValue.GetRichCurveConst()->GetValueRange(MinVal, MaxVal);
		float OutValue = ParamValue.GetRichCurveConst()->Eval(Level);
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Percentage)))
			OutValue *= 0.01f;
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::Negate)))
			OutValue = -OutValue;
		if (CalcMode & (1 << static_cast<uint8>(ETireflyAbilityNumericParamCalcMode::PlusOne)))
			OutValue += 1.f;
		return OutValue;
	}
};


// GameplayAbility的数学表达式参数
UCLASS(CollapseCategories, DisplayName = "Math Expression")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_MathExpression
	: public UTireflyGameplayAbilityParameter
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
	TMap<FString, UTireflyGameplayAbilityParameter_ExpressionVariable*> ExpressionVariables;
};


// GameplayAbility的GameplayEffect运行时参数
UCLASS(CollapseCategories, DisplayName = "Gameplay Effect")
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityParam_GameplayEffect
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// GameplayEffect类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;

	// GameplayEffect的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsDurationParamNameNone()", EditConditionHides))
	float DurationTime = 0.f;

	// GameplayEffect的持续时间参数名称，用来指定Ability的其他参数作为该效果运行时的持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "DurationTime == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName DurationParamName = NAME_None;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsPeriodParamNameNone()", EditConditionHides))
	float PeriodTime = 0.f;

	// GameplayEffect的周期间隔参数名称，用来指定Ability的其他参数作为该效果运行时的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "PeriodTime == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName PeriodParamName = NAME_None;

	// GameplayEffect的周期间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "IsStackToApplyParamNameNone()", EditConditionHides))
	int32 StackToApply = 1;

	// GameplayEffect的层数应用参数名称，用来指定Ability的其他参数作为该效果运行时的层数应用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "StackToApply == 0", EditConditionHides, GetOptions = "GetAbilityParamOptions"))
	FName StackToApplyParamName = NAME_None;

	// TODO：Modifiers

	// TODO：GameplayEffectContext

public:
	UFUNCTION()
	TArray<FName> GetAbilityParamOptions() const;

	UFUNCTION()
	bool IsDurationParamNameNone() const { return DurationParamName.IsNone(); }

	UFUNCTION()
	bool IsPeriodParamNameNone() const { return PeriodParamName.IsNone(); }

	UFUNCTION()
	bool IsStackToApplyParamNameNone() const { return StackToApplyParamName.IsNone(); }
};
