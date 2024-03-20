// Copyright tzlFirefly. All Rights Reserved.

#include "FireflyGameplayAbilitiesModule.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"

#define LOCTEXT_NAMESPACE "FFireflyGameplayAbilitiesModule"

void FFireflyGameplayAbilitiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register post engine delegate to handle various checks related to user settings
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FFireflyGameplayAbilitiesModule::OnPostEngineInitialized);
}

void FFireflyGameplayAbilitiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FFireflyGameplayAbilitiesModule::OnPostEngineInitialized()
{
	/**
	 * 初始化引擎后，检查AbilitySystemGlobals及其TargetDataStructCache是否已初始化。
	 * 如果未初始化，则进行初始化。
	 */
	
    // 获取并检查TargetDataStructCache中ScriptStructs的数量，以判断是否已初始化
	const TArray<UScriptStruct*> StructsCache = UAbilitySystemGlobals::Get().TargetDataStructCache.ScriptStructs;
	const bool bStructsCacheInitialized = StructsCache.Num() > 0;
	
	// 记录TargetDataStructCache的初始化状态及数量日志
	UE_LOG(LogAbilitySystem, Verbose, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals's TargetDataStructCache Initialized: %s (Num: %d)")
		, (bStructsCacheInitialized ? TEXT("true") : TEXT("false"))
		, StructsCache.Num());
	
	// 记录AbilitySystemGlobals的初始化状态日志
	UE_LOG(LogAbilitySystem, Verbose, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals Initialized: %s")
		,(UAbilitySystemGlobals::Get().IsAbilitySystemGlobalsInitialized() ? TEXT("true") : TEXT("false")));

	if (!bStructsCacheInitialized)
	{
		// 如果AbilitySystemGlobals未初始化，则记录日志并进行初始化
		UE_LOG(LogAbilitySystem, Display, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals has not been initialized. Initializing Now."));
		UAbilitySystemGlobals::Get().InitGlobalData();
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFireflyGameplayAbilitiesModule, FireflyGameplayAbilities)