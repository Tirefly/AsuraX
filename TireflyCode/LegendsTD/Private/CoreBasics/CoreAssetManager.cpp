// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreBasics/CoreAssetManager.h"

#include "CoreBasics/CoreLogChannels.h"


void UCoreAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

void UCoreAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

UCoreAssetManager& UCoreAssetManager::Get()
{
	check(GEngine);

	if (UCoreAssetManager* Singleton = Cast<UCoreAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLegend, Fatal, TEXT("UCoreAssetManager::Get() Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to CoreAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UCoreAssetManager>();
}

void UCoreAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

UObject* UCoreAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UCoreAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

const ULegendConfig_CombatUnit* UCoreAssetManager::GetCombatUnitConfig() const
{
	return GetAsset(CombatUnitConfig);
}

const ULegendConfig_EnemyFormat* UCoreAssetManager::GetEnemyFormatConfig() const
{
	return GetAsset(EnemyFormatConfig);
}
