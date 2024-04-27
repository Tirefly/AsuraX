// Copyright Tirefly. All Rights Reserved.

#include "TireflyGameplayAbilitiesDeveloperModule.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "BlueprintGraph/TireflyAbilityParameterNodePinFactory.h"


#define LOCTEXT_NAMESPACE "FTireflyGameplayAbilitiesDeveloperModule"


void FTireflyGameplayAbilitiesDeveloperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	TSharedPtr<FTireflyAbilityParameterNodePinFactory> TireflyAbilityParameterNodePinFactory = MakeShareable(new FTireflyAbilityParameterNodePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(TireflyAbilityParameterNodePinFactory);
}

void FTireflyGameplayAbilitiesDeveloperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTireflyGameplayAbilitiesDeveloperModule, TireflyGameplayAbilitiesDeveloper)