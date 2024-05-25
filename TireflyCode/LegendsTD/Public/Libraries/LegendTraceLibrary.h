// Copyright LazyGameStudio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LegendTraceLibrary.generated.h"


UCLASS()
class LEGENDSTD_API ULegendTraceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region TraceTarget_Basic
	
public:
	// 在Actor的周围或面前生成一个盒体检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	// UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool BoxTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, FVector BoxSize, TArray<AActor*>& OutTargets
		, bool bSearchAlly = false, int32 TargetSum = 0, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

	// 在Actor的周围或面前生成一个球体检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	// UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool SphereTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, float Radius, TArray<AActor*>& OutTargets
		, bool bSearchAlly = false, int32 TargetSum = 0, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

	// 在Actor的周围或面前生成一个扇形检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	// UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool SectorTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, float Radius, float Angle, TArray<AActor*>& OutTargets
		, bool bSearchAlly = false, int32 TargetSum = 0, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

	// 获取战斗单位的半径
	UFUNCTION(BlueprintPure, Category = TraceTarget)
	static float GetCombatUnitRadius(AActor* Actor);

#pragma endregion


#pragma region TraceTarget_GridMap

public:
	// 获取全局的地图棋格半径
	UFUNCTION(BlueprintPure, Category = TraceTarget)
	static float GetGlobalGridRadius();

	// （棋格地图专用）在Actor的周围或面前生成一个盒体检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool BoxTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance
		, FVector BoxSize, TArray<AActor*>& OutTargets, bool bSearchAlly = false, int32 TargetSum = 0
		, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

	// （棋格地图专用）在Actor的周围或面前生成一个球体检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool SphereTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance
		, float Radius, TArray<AActor*>& OutTargets, bool bSearchAlly = false, int32 TargetSum = 0
		, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

	// （棋格地图专用）在Actor的周围或面前生成一个扇形检测多个目标
	// bTraceCenter为true，则检测Actor周围的Target；为false，则检测Actor正前方范围内的Actor
	UFUNCTION(BlueprintCallable, Category = TraceTarget, Meta = (WorldContext = "WorldContext", AdvancedDisplay = "bDrawDebug, DrawTime"))
	static bool SectorTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance
		, float Radius, float Angle, TArray<AActor*>& OutTargets, bool bSearchAlly = false, int32 TargetSum = 0
		, bool bTraceCenter = true, bool bDrawDebug = true, float DrawTime = 0.5f);

#pragma endregion
};
