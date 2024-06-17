// Copyright Epic Games, Inc. All Rights Reserved.

#include "TireflyAnimNotifiesModule.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FTireflyAnimNotifiesModule"

void FTireflyAnimNotifiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("TireflyAnimNotifies/Config"));
}

void FTireflyAnimNotifiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTireflyAnimNotifiesModule, TireflyAnimNotifies)