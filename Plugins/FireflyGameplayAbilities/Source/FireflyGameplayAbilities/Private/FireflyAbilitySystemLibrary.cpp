// Copyright tzlFirefly. All Rights Reserved.


#include "FireflyAbilitySystemLibrary.h"

#include "Engine/AssetManager.h"
#include "GameplayAbility/FireflyAbilityParameter.h"
#include "GameplayAbility/FireflyGameplayAbilityAsset.h"

bool UFireflyAbilitySystemLibrary::GetAbilityParamSpecOfAsset(const UFireflyGameplayAbilityAsset* AbilityAsset,
	FName ParamName, FInstancedStruct& OutParamSpec, int32 AbilityLevel, int32 CasterLevel)
{
	// 安全判定
	if (!IsValid(AbilityAsset) || ParamName == NAME_None)
	{
		return false;
	}

	// 尝试获取能力资产中指定名称的参数
	const FFireflyAbilityParameter* AbilityParameter = AbilityAsset->AbilityParameters.Find(ParamName);
	if (!AbilityParameter)
	{
		return false;
	}

	// 若能力参数的取值模式是“固定常数”，返回设定好的参数细则
	if (AbilityParameter->ParameterUsingMode == EFireflyAbilityParamMode::Constant)
	{
		OutParamSpec = AbilityParameter->ParameterSpec;
		return true;
	}

	// 若能力参数的取值模式不是“固定常数”，返回指定等级的参数细则
	if (const FInstancedStruct* ParamSpec = AbilityParameter->ParameterSpecs.Find(
		AbilityParameter->ParameterUsingMode == EFireflyAbilityParamMode::AbilityLevelBased
			? AbilityLevel : CasterLevel))
	{
		OutParamSpec = *ParamSpec;
		return true;
	}
	
	return false;
}

bool UFireflyAbilitySystemLibrary::GetAbilityParamSpecOfAssetID(FName AbilityID, FName ParamName,
	FInstancedStruct& OutParamSpec, int32 AbilityLevel, int32 CasterLevel)
{
	if (!UAssetManager::Get().IsInitialized() || AbilityID == NAME_None || ParamName == NAME_None)
	{
		return false;
	}

	const UFireflyGameplayAbilityAsset* AbilityAsset = UAssetManager::Get().GetPrimaryAssetObject<UFireflyGameplayAbilityAsset>(
		FPrimaryAssetId(UFireflyGameplayAbilityAsset::GameplayAbilityAsset, AbilityID));
	if (!IsValid(AbilityAsset))
	{
		return false;
	}

	return GetAbilityParamSpecOfAsset(AbilityAsset, ParamName, OutParamSpec, AbilityLevel, CasterLevel);
}
