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

public:
	virtual void GetAbilityParamInfo(FTireflyAbilityParamInfo& ParamInfo) override;

protected:
	void LoadGameplayAbilityAsset();

#pragma endregion


#pragma region GameplayAbility_Granting

protected:
	virtual void OnAbilityGranted_Implementation() override;

#pragma endregion
};