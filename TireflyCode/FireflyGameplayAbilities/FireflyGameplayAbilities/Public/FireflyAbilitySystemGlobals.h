// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FireflyAbilitySystemGlobals.generated.h"


// 技能系统全局配置
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/** Should be called once as part of project setup to load global data tables and tags */
	virtual void InitGlobalData() override;

	virtual void LoadGlobalLocalizationStringTables();

public:
	UPROPERTY()
	TMap<FName, FName> GameplayNameLocalizations;

	UPROPERTY()
	TMap<FName, FName> GameplayDescriptionLocalizations;
};
