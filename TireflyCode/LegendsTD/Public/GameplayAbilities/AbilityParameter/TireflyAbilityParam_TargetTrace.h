// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"
#include "TireflyAbilityParam_TargetTrace.generated.h"


// GameplayAbility的目标检测轨迹参数基础结构
UCLASS(Abstract)
class LEGENDSTD_API UTireflyAbilityParam_TargetTrace
	: public UTireflyGameplayAbilityParameter
{
	GENERATED_BODY()

public:
	// True则检测友方单位，False则检测敌方单位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSearchAlly = false;

	// 目标检测数量，为0则检测所有目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetSum = 0;

	// True则以技能释放者为中心进行检测，False则仅检测技能释放者的正前方
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceCenter = true;

	// True则每个目标对应一个TargetDataHandle，False则所有目标共用一个TargetDataHandle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOneTargetPerHandle = false;

	// 执行目标检测
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Ability, Meta = (AdvancedDisplay = "bDrawDebug, DrawTime"))
	bool ExecuteTargetTrace(AActor* Actor, AController* Alliance, FGameplayAbilityTargetDataHandle& OutTargetData
		, bool bDrawDebug = true, float DrawTime = 5.0f);
	virtual bool ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance, FGameplayAbilityTargetDataHandle& OutTargetData
		, bool bDrawDebug = true, float DrawTime = 5.0f)
	{
		return false;
	}
};


// GameplayAbility的圆形目标检测轨迹参数
UCLASS(DisplayName = "Target Trace 球形")
class LEGENDSTD_API UTireflyAbilityParam_TargetTrace_Sphere
	: public UTireflyAbilityParam_TargetTrace
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 0.f;

	virtual bool ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance, FGameplayAbilityTargetDataHandle& OutTargetData
		, bool bDrawDebug = true, float DrawTime = 5.0f) override;
};


// GameplayAbility的扇形目标检测轨迹参数
UCLASS(DisplayName = "Target Trace 锥形")
class LEGENDSTD_API UTireflyAbilityParam_TargetTrace_Sector
	: public UTireflyAbilityParam_TargetTrace
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = 0.f;

	virtual bool ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance, FGameplayAbilityTargetDataHandle& OutTargetData
		, bool bDrawDebug = true, float DrawTime = 5.0f) override;
};


// GameplayAbility的盒体目标检测轨迹参数
UCLASS(DisplayName = "Target Trace 盒形")
class LEGENDSTD_API UTireflyAbilityParam_TargetTrace_Box
	: public UTireflyAbilityParam_TargetTrace
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxExtent = FVector::ZeroVector;

	virtual bool ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance, FGameplayAbilityTargetDataHandle& OutTargetData
		, bool bDrawDebug = true, float DrawTime = 5.0f) override;
};