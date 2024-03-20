// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFireflyGameplayAbilitiesModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	void OnPostEngineInitialized();
};
