// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArenaAllianceCharacter.generated.h"


class UFireflyAbilitySystemComponent;


UCLASS()
class LEGENDSTD_API AArenaAllianceCharacter : public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region Character

public:
	// Sets default values for this character's properties
	AArenaAllianceCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;

#pragma endregion


#pragma region GameplayAbilities

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// 技能系统组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFireflyAbilitySystemComponent> AbilitySystem;

#pragma endregion
};
