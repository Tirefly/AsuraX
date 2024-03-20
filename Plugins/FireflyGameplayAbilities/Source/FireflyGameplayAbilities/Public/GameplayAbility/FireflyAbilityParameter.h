// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "FireflyAbilityParameter.generated.h"

// 能力参数的取值模式
UENUM(BlueprintType)
enum class EFireflyAbilityParamMode : uint8
{
	Constant			UMETA(ToolTip="直接作为固定常量使用此参数\nUse this parameter directly as a fixed constant"),
	AbilityLevelBased	UMETA(ToolTip="根据能力等级使用此参数\nUse this parameter based on the ability level"),
	CasterLevelBased	UMETA(ToolTip="根据使用者等级使用此参数\nUse this parameter based on the caster level"),
};

// 能力参数细则的基础结构
USTRUCT(BlueprintType)
struct FIREFLYGAMEPLAYABILITIES_API FFireflyAbilityParamSpec
{
	GENERATED_USTRUCT_BODY()
};

// 能力参数细则：一个简单的数值
USTRUCT(BlueprintType)
struct FIREFLYGAMEPLAYABILITIES_API FFireflyAbilityParamSpec_SingleNumeric
	: public FFireflyAbilityParamSpec
{
	GENERATED_USTRUCT_BODY()

	float GetParamValue() const { return NumericParam; }

	FString ParamToString() const { return FString::SanitizeFloat(NumericParam); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NumericParam = 0.f;
};

// 能力参数细则：一个被转化成百分数的数值
USTRUCT(BlueprintType)
struct FIREFLYGAMEPLAYABILITIES_API FFireflyAbilityParamSpec_PercentNumeric
	: public FFireflyAbilityParamSpec
{
	GENERATED_USTRUCT_BODY()

	float GetParamValue() const { return PercentParam; }

	FString ParamToString() const { return FString::SanitizeFloat(PercentParam) + FString("%"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ForceUnits="%"))
	float PercentParam = 0.f;
};

USTRUCT(BlueprintType)
struct FIREFLYGAMEPLAYABILITIES_API FFireflyAbilityParamSpec_String
	: public FFireflyAbilityParamSpec
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StringParam;
};

// 能力参数数据
USTRUCT(BlueprintType)
struct FIREFLYGAMEPLAYABILITIES_API FFireflyAbilityParameter
{
	GENERATED_USTRUCT_BODY()

	// 能力参数的取值模式
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFireflyAbilityParamMode ParameterUsingMode;

	// 能力参数细则，仅当能力参数取值模式为“固定常数”时可以被使用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (BaseStruct = "/Script/FireflyGameplayAbilities.FireflyAbilityParamSpec", ExcludeBaseStruct
		, EditCondition = "ParameterUsingMode == EFireflyAbilityParamMode::Constant", EditConditionHides))
	FInstancedStruct ParameterSpec;

	// 能力参数细则，仅当能力参数取值模式为“基于等级”时可以被使用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (BaseStruct = "/Script/FireflyGameplayAbilities.FireflyAbilityParamSpec", ExcludeBaseStruct
		, EditCondition = "ParameterUsingMode == EFireflyAbilityParamMode::LevelBased", EditConditionHides))
	TMap<int32, FInstancedStruct> ParameterSpecs;
};