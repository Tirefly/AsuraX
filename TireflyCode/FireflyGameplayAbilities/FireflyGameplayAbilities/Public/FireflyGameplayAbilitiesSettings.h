// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GameplayAttribute/TireflyAttributeRangeConfig.h"
#include "GameplayEffect/FireflyGameplayEffect.h"
#include "FireflyGameplayAbilitiesSettings.generated.h"


struct FGameplayAttribute;


// 游戏性能力系统的开发者设置
// Developer settings of gameplay ability system
UCLASS(Config = FireflyGameplayAbilitiesSettings, DefaultConfig)
class FIREFLYGAMEPLAYABILITIES_API UFireflyGameplayAbilitiesSettings : public UDeveloperSettings
{
	GENERATED_BODY()

#pragma region DeveloperSettings

public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return FName("Project"); }

	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return FName("Project"); }

	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override { return FName("FireflyGameplayAbilities"); }

#if WITH_EDITOR
	/** UObject interface */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	/** Gets the section text, uses the classes DisplayName by default. */
	virtual FText GetSectionText() const override { return FText::FromString("Firefly Gameplay Abilities"); };

	/** Gets the description for the section, uses the classes ToolTip by default. */
	virtual FText GetSectionDescription() const override { return FText::FromString("Developer settings of gameplay ability system"); };
#endif

#pragma endregion


#pragma region GameplayAttribute_RangeSetting

public:
	// 获取指定属性的范围设置
	const FTireflyAttributeRangeConfig* GetAttributeRangeConfig(const FGameplayAttribute& Attribute) const;
	
protected:
	// 属性的范围设置
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Attribute")
	TArray<FTireflyAttributeRangeConfig> AttributeRangeConfigList;

#pragma endregion


#pragma region GameplayEffect_SetByCaller

public:
	FGameplayTag GetGenericDurationSetByCallerTag() const { return GenericDurationSetByCallerTag; }

	const FTireflySetByCallerTagConfig* GetSetByCallerTagConfig(const FGameplayAttribute& Attribute) const;

protected:
	// 通用的GameplayEffect持续时间SetByCaller
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Effect")
	FGameplayTag GenericDurationSetByCallerTag;

	// GameplayEffect中属性对应的SetByCallerTag，帮助开发者更快速的配置GameplayEffect的SetByCaller类属性修改器
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Effect")
	TArray<FTireflySetByCallerTagConfig> GenericSetByCallerTagList;

#pragma endregion


#pragma region GameplayAbilities_Localization

public:
	FName GetAttributeLocalizationTableId() const;

	FName GetGameplayNameLocalizationTableId(const FName& GameplayId) const;

	FName GetGameplayDescriptionLocalizationTableId(const FName& GameplayId) const;

public:
	// 属性的本地化字符表
	UPROPERTY(Config, EditAnywhere, Category = "Localization")
	TSoftObjectPtr<UStringTable> AttributeStringTable;

	// 技能名的本地化字符表
	UPROPERTY(Config, EditAnywhere, Category = "Localization")
	TArray<TSoftObjectPtr<UStringTable>> GameplayNameStringTables;

	// 技能描述的本地化字符表
	UPROPERTY(Config, EditAnywhere, Category = "Localization")
	TArray<TSoftObjectPtr<UStringTable>> GameplayDescriptionStringTables;

#pragma endregion
};
