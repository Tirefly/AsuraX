// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "TireflyGameplayAbilityParameter.generated.h"


class UTireflyGameplayAbilityAsset;
class UTireflyAbilitySystemComponent;


// GameplayAbility参数的来源类型
UENUM(BlueprintType)
enum class ETireflyAbilityParamSourceType : uint8
{
	Caster = 0		UMETA(ToolTip = "技能的释放者"),
	Target = 1		UMETA(ToolTip = "技能效果的目标")
};


// 技能参数信息
USTRUCT(BlueprintType)
struct FTireflyAbilityParamInfo
{
	GENERATED_BODY()

public:
	// 技能释放者的技能系统组件
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTireflyAbilitySystemComponent> CasterASC = nullptr;

	// 技能目标的技能系统组件
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTireflyAbilitySystemComponent> TargetASC = nullptr;

	// 技能的句柄
	UPROPERTY(BlueprintReadWrite)
	FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle();

public:
	FTireflyAbilityParamInfo() {}

	FTireflyAbilityParamInfo(UTireflyAbilitySystemComponent* InCasterASC, UTireflyAbilitySystemComponent* InTargetASC
		, const FGameplayAbilitySpecHandle& InAbilityHandle)
		: CasterASC(InCasterASC), TargetASC(InTargetASC), AbilityHandle(InAbilityHandle) {}
};


// 技能资产的基础元素
UCLASS(Abstract, CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class UTireflyAbilityAssetElement : public UObject
{
	GENERATED_BODY()

public:
	// 获取该元素所在的技能参数的名字
	UFUNCTION(BlueprintPure, Category = "Ability|Parameter")
	FName GetParameterName() const;

	// 获取不包含该元素所在的技能参数以外的所有其他技能参数名字
	UFUNCTION(BlueprintPure, Category = "Ability|Parameter")
	TArray<FName> GetOtherParameters() const;

	// 获取不包含该元素所在的技能参数以外的所有其他数值类技能参数名字
	UFUNCTION(BlueprintPure, Category = "Ability|Parameter")
	TArray<FName> GetOtherNumericParameters() const;

	// 获取不包含该元素所在的技能参数以外的所有其他游戏效果类技能参数名字
	UFUNCTION(BlueprintPure, Category = "Ability|Parameter")
	TArray<FName> GetOtherGameplayEffectParameters() const;

public:
	virtual void PostInitProperties() override;

	// 初始化技能资产的基础元素
	UFUNCTION(BlueprintNativeEvent, Meta = (ForceAsFunction, DisplayName = "Initialize"), Category = "Ability|Parameter")
	void K2_PostInitProperties();
	virtual void K2_PostInitProperties_Implementation() {}
	
};


// 需要显示文本的技能资产元素
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilityAssetElement_DisplayText : public UTireflyAbilityAssetElement
{
	GENERATED_BODY()

public:
	// 用于展示的文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MultiLine = "true", EditCondition = "IsDisplayTextEditable", EditConditionHides))
	FText DisplayText;

public:
	// 获取用于展示的文本
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Ability|Parameter")
	FText GetDisplayText() const;
	virtual FText GetDisplayText_Implementation() const { return DisplayText; }

	// 是否允许编辑展示文本
	UFUNCTION(BlueprintNativeEvent, Category = "Ability|Parameter")
	bool IsDisplayTextEditable() const;
	virtual bool IsDisplayTextEditable_Implementation() const { return true; }
};


// 技能参数的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameter : public UTireflyAbilityAssetElement_DisplayText
{
	GENERATED_BODY()
};


// GameplayAbility参数细节的基础结构
UCLASS(Abstract)
class TIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterDetail : public UTireflyAbilityAssetElement
{
	GENERATED_BODY()
};