// Copyright tzlFirefly. All Rights Reserved.

#include "FireflyGameplayAbilitiesEditorModule.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"


#define LOCTEXT_NAMESPACE "FFireflyGameplayAbilitiesEditorModule"


void FFireflyGameplayAbilitiesEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFireflyGameplayAbilitiesEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FFireflyGameplayAbilitiesEditorModule::OnPostEngineInitialized()
{
	/**
	 * UAbilitySystemGlobals' IsAbilitySystemGlobalsInitialized doesn't really work as it's checking for GlobalAttributeSetInitter and
     * GlobalAttributeSetDefaultsTableNames needs to be configured for AllocAttributeSetInitter to be called (at least for 4.26)
     */
    
    const TArray<UScriptStruct*> StructsCache = UAbilitySystemGlobals::Get().TargetDataStructCache.ScriptStructs;
    const bool bStructsCacheInitialized = StructsCache.Num() > 0;
    UE_LOG(LogAbilitySystem, Verbose, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals's TargetDataStructCache Initialized: %s (Num: %d)")
    	, (bStructsCacheInitialized ? TEXT("true") : TEXT("false"))
    	, StructsCache.Num());
    UE_LOG(LogAbilitySystem, Verbose, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals Initialized: %s")
    	,(UAbilitySystemGlobals::Get().IsAbilitySystemGlobalsInitialized() ? TEXT("true") : TEXT("false")));
    
    if (!bStructsCacheInitialized)
    {
    	UE_LOG(LogAbilitySystem, Display, TEXT("FireflyGameplayAbilitiesModule::OnPostEngineInitialized() AbilitySystemGlobals has not been initialized. Initializing Now."));
    	UAbilitySystemGlobals::Get().InitGlobalData();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFireflyGameplayAbilitiesEditorModule, FireflyGameplayAbilitiesEditor)