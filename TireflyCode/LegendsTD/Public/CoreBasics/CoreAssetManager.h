// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CoreAssetManager.generated.h"


class ULegendBuilder_CombatUnit;
class ULegendConfig_EnemyFormat;
class ULegendConfig_CombatUnit;


/** 项目使用的默认资产管理器 */
UCLASS(Config = Game)
class LEGENDSTD_API UCoreAssetManager : public UAssetManager
{
	GENERATED_BODY()

#pragma region AssetManager

public:
	UCoreAssetManager() {}

	virtual void StartInitialLoading() override;

	virtual void FinishInitialLoading() override;

	// Returns the AssetManager singleton object.
	static UCoreAssetManager& Get();

	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

protected:
	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	static bool ShouldLogAssetLoads();

private:	
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<const UObject*> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;

#pragma endregion


#pragma region DataAsset_CombatUnitConfig

public:
	const ULegendConfig_CombatUnit* GetCombatUnitConfig() const;

protected:
	UPROPERTY(Config)
	TSoftObjectPtr<ULegendConfig_CombatUnit> CombatUnitConfig = nullptr;

#pragma endregion


#pragma region DataAsset_EnemyFormatConfig

public:
	const ULegendConfig_EnemyFormat* GetEnemyFormatConfig() const;

protected:
	UPROPERTY(Config)
	TSoftObjectPtr<ULegendConfig_EnemyFormat> EnemyFormatConfig = nullptr;

#pragma endregion
};


#pragma region DataAsset_Config

template <typename AssetType>
AssetType* UCoreAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UCoreAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}

#pragma endregion