// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GameplayAttribute/TireflyAttributeRangeConfig.h"
#include "TireflyAbilitySystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config = TireflyAbilitySystemSettings, DefaultConfig)
class TIREFLYGAMEPLAYABILITIES_API UTireflyAbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

#pragma region DeveloperSettings

public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return FName("Project"); }

	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return FName("Game"); }

	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override { return FName("TireflyAbilitySystem"); }

#if WITH_EDITOR
	/** UObject interface */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	/** Gets the section text, uses the classes DisplayName by default. */
	virtual FText GetSectionText() const override { return FText::FromString("Tirefly Ability System"); };

	/** Gets the description for the section, uses the classes ToolTip by default. */
	virtual FText GetSectionDescription() const override { return FText::FromString("Developer settings of gameplay ability system"); };
#endif

#pragma endregion

	
#pragma region GameplayAttribute_RangeSetting

public:
	// 属性的范围设置
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Attribute")
	TMap<FGameplayAttribute, FTireflyAttributeRangeConfig> AttributeRangeConfigList;

#pragma endregion


#pragma region GameplayEffect_SetByCaller

public:
	// 通用的GameplayEffect持续时间SetByCaller
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Effect")
	FGameplayTag GenericDurationSetByCallerTag;

	// GameplayEffect中属性对应的SetByCallerTag，帮助开发者更快速的配置GameplayEffect的SetByCaller类属性修改器
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Effect")
	TMap<FGameplayAttribute, FGameplayTag> GenericSetByCallerTagList;

#pragma endregion
};
