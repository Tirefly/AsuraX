// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"
#include "TireflyGameplayAbilityParameter.generated.h"


class UFireflyAbilitySystemComponent;


namespace TireflyAbilityParameterHelper
{
	static UFireflyGameplayAbilityAsset* GetAbilityAsset(const UObject* InObject);
}


// GameplayAbility参数的来源类型
UENUM(BlueprintType)
enum class ETireflyAbilityParamSourceType : uint8
{
	Caster = 0		UMETA(ToolTip = "技能的释放者"),
	Target = 1		UMETA(ToolTip = "技能效果的目标")
};


USTRUCT(BlueprintType)
struct FTireflyAbilityParamInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UFireflyAbilitySystemComponent> CasterASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UFireflyAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayAbilitySpecHandle AbilityHandle = FGameplayAbilitySpecHandle();

	UPROPERTY(BlueprintReadWrite)
	int32 Level = 1;

public:
	FTireflyAbilityParamInfo() {}

	FTireflyAbilityParamInfo(UFireflyAbilitySystemComponent* InCasterASC, UFireflyAbilitySystemComponent* InTargetASC
		, const FGameplayAbilitySpecHandle& InAbilityHandle, const int32& InLevel)
		: CasterASC(InCasterASC), TargetASC(InTargetASC), AbilityHandle(InAbilityHandle), Level(InLevel) {}
};


UCLASS(Abstract, CollapseCategories, BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class FIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterAbstract : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FName GetAbilityParameterName() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetAbilityOtherParameters() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetAbilityOtherNumericParameters() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetAbilityOtherGameplayEffectParameters() const;

public:
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintNativeEvent, Meta = (ForceAsFunction, DisplayName = "Post Init Properties"), Category = "TireflyGameplayAbilityParameter")
	void K2_PostInitProperties();
	virtual void K2_PostInitProperties_Implementation() {}
};


UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterBase : public UTireflyGameplayAbilityParameterAbstract
{
	GENERATED_BODY()

public:
	// 用于展示的文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MultiLine = "true", EditCondition = "IsShowcaseTextEditable", EditConditionHides))
	FText ShowcaseText;

public:
	// 获取用于展示的文本
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FText GetShowcaseText() const;
	virtual FText GetShowcaseText_Implementation() const { return ShowcaseText; }

	// 是否允许编辑展示文本
	UFUNCTION(BlueprintNativeEvent)
	bool IsShowcaseTextEditable() const;
	virtual bool IsShowcaseTextEditable_Implementation() const { return true; }
};


// GameplayAbility参数的基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameter : public UTireflyGameplayAbilityParameterBase
{
	GENERATED_BODY()
};


// GameplayAbility参数细节的基础结构
UCLASS(Abstract)
class FIREFLYGAMEPLAYABILITIES_API UTireflyGameplayAbilityParameterDetail : public UTireflyGameplayAbilityParameterAbstract
{
	GENERATED_BODY()
};