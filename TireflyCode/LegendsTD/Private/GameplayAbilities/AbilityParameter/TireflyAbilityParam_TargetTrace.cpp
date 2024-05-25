// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/AbilityParameter/TireflyAbilityParam_TargetTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Libraries/LegendTraceLibrary.h"


bool UTireflyAbilityParam_TargetTrace_Sphere::ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance,
	FGameplayAbilityTargetDataHandle& OutTargetData, bool bDrawDebug, float DrawTime)
{
	TArray<AActor*> OutTargets;
	bool bResult = ULegendTraceLibrary::SphereTraceTargetInGrid(Actor, Actor, Alliance, Radius,
		OutTargets, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
	OutTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(OutTargets, bOneTargetPerHandle);
	return bResult;
}

bool UTireflyAbilityParam_TargetTrace_Sector::ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance,
	FGameplayAbilityTargetDataHandle& OutTargetData, bool bDrawDebug, float DrawTime)
{
	TArray<AActor*> OutTargets;
	bool bResult = ULegendTraceLibrary::SectorTraceTargetInGrid(Actor, Actor, Alliance, Radius, Angle,
		OutTargets, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
	OutTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(OutTargets, bOneTargetPerHandle);
	return bResult;
}

bool UTireflyAbilityParam_TargetTrace_Box::ExecuteTargetTrace_Implementation(AActor* Actor, AController* Alliance,
	FGameplayAbilityTargetDataHandle& OutTargetData, bool bDrawDebug, float DrawTime)
{
	TArray<AActor*> OutTargets;
	bool bResult = ULegendTraceLibrary::BoxTraceTargetInGrid(Actor, Actor, Alliance, BoxExtent,
		OutTargets, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
	OutTargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(OutTargets, bOneTargetPerHandle);
	return bResult;
}