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
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterBase : public UObject
{
	GENERATED_BODY()

public:
	// 用于展示的文本
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (MultiLine = "true", EditCondition = "IsShowcaseTextEditable", EditConditionHides))
	FText ShowcaseText;

public:
	// 获取用于展示的文本
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability")
	FText GetShowcaseText() const;
	virtual FText GetShowcaseText_Implementation() const { return ShowcaseText; }

	// 是否允许编辑展示文本
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool IsShowcaseTextEditable() const;
	virtual bool IsShowcaseTextEditable_Implementation() const { return true; }

	// 获取参数的可选选项
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	TArray<FName> GetAbilityParamOptions() const;
	virtual TArray<FName> GetAbilityParamOptions_Implementation() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
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
	// 获取参数的可选选项
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	TArray<FName> GetAbilityParamOptions() const;
	virtual TArray<FName> GetAbilityParamOptions_Implementation() const;
};