// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FireflyAbilitySystemLibrary.generated.h"

class UFireflyGameplayAbilityAsset;
struct FInstancedStruct;

/**
 * 
 */
UCLASS()
class FIREFLYGAMEPLAYABILITIES_API UFireflyAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region GameplayAbility_DataAsset
	
public:
	// 获取能力的数据资产中，指定名称的参数细则
	UFUNCTION(BlueprintCallable, Category = Ability)
	static bool GetAbilityParamSpecOfAsset(const UFireflyGameplayAbilityAsset* AbilityAsset, FName ParamName
		, FInstancedStruct& OutParamSpec, int32 AbilityLevel = 1, int32 CasterLevel = 1);

	// 获取指定ID的能力数据资产中，指定名称的参数细则
	UFUNCTION(BlueprintCallable, Category = Ability)
	static bool GetAbilityParamSpecOfAssetID(FName AbilityID, FName ParamName, FInstancedStruct& OutParamSpec
		, int32 AbilityLevel = 1, int32 CasterLevel = 1);
	
#pragma endregion
};
