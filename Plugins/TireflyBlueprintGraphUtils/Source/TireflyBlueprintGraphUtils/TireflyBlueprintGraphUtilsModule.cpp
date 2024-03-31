// Copyright Epic Games, Inc. All Rights Reserved.

#include "TireflyBlueprintGraphUtilsModule.h"

#include "TireflyNameOptionPinFactory.h"


#define LOCTEXT_NAMESPACE "FTireflyBlueprintGraphUtilsModule"

void FTireflyBlueprintGraphUtilsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	TSharedPtr<FTireflyNameOptionPinFactory> TireflyNameOptionPinFactory = MakeShareable(new FTireflyNameOptionPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(TireflyNameOptionPinFactory);
}

void FTireflyBlueprintGraphUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTireflyBlueprintGraphUtilsModule, TireflyBlueprintGraphUtils)