// Copyright Tirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AsuraAttributeSet.h"
#include "AsuraBossAttributeSet.generated.h"


// 所有首领角色共用的基础属性集
UCLASS()
class ASURAX_API UAsuraBossAttributeSet : public UAsuraAttributeSet
{
	GENERATED_BODY()

#pragma region AttributeSet

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion


#pragma region BossAttribute

public:
	// 首领属性：最大架势值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPosture, Category = "Boss Attribute")
	FGameplayAttributeData MaxPosture;

	// 首领属性：架势值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Posture, Category = "Boss Attribute")
	FGameplayAttributeData Posture;

protected:
	UFUNCTION()
	virtual void OnRep_MaxPosture(const FGameplayAttributeData& OldMaxPosture)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraBossAttributeSet, MaxPosture, OldMaxPosture); }
	
	UFUNCTION()
	virtual void OnRep_Posture(const FGameplayAttributeData& OldPosture)
	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UAsuraBossAttributeSet, Posture, OldPosture); }

#pragma endregion
};
