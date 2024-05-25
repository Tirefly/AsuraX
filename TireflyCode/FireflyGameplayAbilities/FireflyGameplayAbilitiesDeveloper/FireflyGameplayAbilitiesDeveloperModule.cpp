// Copyright tzlFirefly, 2023. All Rights Reserved.

#include "FireflyGameplayAbilitiesDeveloperModule.h"

#include "BlueprintGraph/TireflyAbilityParameterNodePinFactory.h"


#define LOCTEXT_NAMESPACE "FFireflyGameplayAbilitiesDeveloperModule"


void FFireflyGameplayAbilitiesDeveloperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	TSharedPtr<FTireflyAbilityParameterNodePinFactory> TireflyAbilityParameterNodePinFactory = MakeShareable(new FTireflyAbilityParameterNodePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(TireflyAbilityParameterNodePinFactory);
}

void FFireflyGameplayAbilitiesDeveloperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFireflyGameplayAbilitiesDeveloperModule, FireflyGameplayAbilitiesDeveloper)