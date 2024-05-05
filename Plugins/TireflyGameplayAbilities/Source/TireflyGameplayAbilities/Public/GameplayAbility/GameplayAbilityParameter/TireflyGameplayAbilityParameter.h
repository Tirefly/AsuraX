// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TireflyGameplayAbilityParameter.generated.h"


class UTireflyGameplayAbilityAsset;


namespace TireflyAbilityParameterHelper
{
	static UTireflyGameplayAbilityAsset* GetAbilityAsset(const UObject* InObject);
}


// GameplayAbility参数的来源类型
UENUM(BlueprintType)
enum class ETireflyAbilityParamSourceType : uint8
{
	Caster = 0		UMETA(ToolTip = "技能的释放者"),
	Target = 1		UMETA(ToolTip = "技能效果的目标")
};


UCLASS(Abstract, CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTireflyGameplayAbilityParameterBase : public UObject
{
	GENERATED_BODY()

public:
	// 用于展示的文本
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (MultiLine = "true"))
	FText ShowcaseText;

public:
	// 获取用于展示的文本
	UFUNCTION(BlueprintPure, Category = "Ability")
	virtual FText GetShowcaseText() const { return ShowcaseText; }

	UFUNCTION()
	virtual TArray<FName> GetAbilityParamOptions() const;

	FName GetAbilityParameterName() const;
};


// GameplayAbility参数的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameter : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()
};


// GameplayAbility参数细节的基础结构
UCLASS(Abstract, CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterDetail : public UObject
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual TArray<FName> GetAbilityParamOptions() const;
};