// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/FireflyGameplayAbility.h"
//#include "CoreBasics/CoreGameplayTypes.h"
#include "LegendGameplayAbility.generated.h"


class ACombatProjectile;
class ULegendGameplayAbilityAsset;


UENUM(BlueprintType)
enum class ELegendAbilityType : uint8
{
	None,
	CombatUnitAbility
};


UCLASS()
class LEGENDSTD_API ULegendGameplayAbility : public UFireflyGameplayAbility
{
	GENERATED_BODY()

#pragma region GameplayAbility

public:
	ULegendGameplayAbility(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
	ELegendAbilityType AbilityType = ELegendAbilityType::None;

#pragma endregion


#pragma region GameplayAbility_DataAsset

protected:
	void LoadGameplayAbilityAsset();

#pragma endregion


#pragma region GameplayAbility_Granting

protected:
	virtual void OnAbilityGranted_Implementation() override;

#pragma endregion


// #pragma region GameplayAbility_CombatExtra
//
// public:
// 	// 动态构建战斗抛射物的实时结构
// 	UFUNCTION(BlueprintCallable, Category = CombatExtra)
// 	bool ConstructProjectileRuntimeFromConfig(TSubclassOf<ACombatProjectile>& ProjectileClass, FName& ProjectileID
// 		, FCombatExtraSkin& ProjectileSkin, FCombatProjectileEffect& ProjectileEffect, int32 Index);
//
// 	// 动态构建战斗领域的实时结构
// 	UFUNCTION(BlueprintCallable, Category = CombatExtra)
// 	bool ConstructDomainRuntimeFromConfig(TSubclassOf<ACombatDomain>& DomainClass, FName& DomainID
// 		, FCombatExtraSkin& DomainSkin, FCombatDomainEffect& DomainEffect, int32 Index);
//
// #pragma endregion
};