// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Libraries/LegendTraceLibrary.h"

#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Arena/CombatUnits/CombatUnitBase.h"
#include "GameplayAbilities/LegendAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"


bool ULegendTraceLibrary::BoxTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, FVector BoxSize,
	TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter, bool bDrawDebug, float DrawTime)
{
	if (!IsValid(Actor) || !IsValid(Alliance))
	{
		return false;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), false);
	Params.bReturnPhysicalMaterial = false;

	const FQuat Rotation = Actor->GetActorForwardVector().ToOrientationQuat();
	const FVector Center = bTraceCenter ? Actor->GetActorLocation()
		: Actor->GetActorLocation() + Actor->GetActorForwardVector() * (BoxSize.X * 0.5f + GetCombatUnitRadius(Actor));

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(Overlaps, Center, Rotation
		, FCollisionObjectQueryParams(ECC_Pawn), FCollisionShape::MakeBox(BoxSize * 0.5f), Params);

	TArray<AActor*>	HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		const AActor* HitActor = Overlaps[i].OverlapObjectHandle.GetManagingActor();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			const ACombatUnitBase* HitCombatUnit = Cast<ACombatUnitBase>(HitActor);
			if (!IsValid(HitCombatUnit))
			{
				continue;
			}

			if (bSearchAlly)
			{
				if (HitCombatUnit->GetCurrentAlliance() != Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
			else
			{
				if (HitCombatUnit->GetCurrentAlliance() == Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
		}
	}

	if (TargetSum > 0 && TargetSum < HitActors.Num())
	{
		for (int32 i = HitActors.Num() - 1; i >= TargetSum; --i)
		{
			HitActors.RemoveAtSwap(i);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugBox(World, Center, BoxSize * 0.5, Rotation, FColor::Red, false, DrawTime);
		for (const AActor* HitActor : HitActors)
		{
			FVector HitPos = HitActor->GetActorLocation();
			DrawDebugDirectionalArrow(World, HitPos, HitPos + FVector::ZAxisVector * 150.f
				, 1.f, FColor::Green, false, DrawTime);
		}
	}
#endif

	if (HitActors.IsEmpty())
	{
		return false;
	}

	OutTargets = HitActors;
	return true;
}

bool ULegendTraceLibrary::SphereTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, float Radius,
	TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter, bool bDrawDebug, float DrawTime)
{
	if (!IsValid(Actor) || !IsValid(Alliance))
	{
		return false;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), false);
	Params.bReturnPhysicalMaterial = false;

	const FVector Center = bTraceCenter ? Actor->GetActorLocation()
		: Actor->GetActorLocation() + Actor->GetActorForwardVector() * (Radius + GetCombatUnitRadius(Actor));

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity
		, FCollisionObjectQueryParams(ECC_Pawn), FCollisionShape::MakeSphere(Radius), Params);

	TArray<AActor*>	HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		const AActor* HitActor = Overlaps[i].OverlapObjectHandle.GetManagingActor();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			const ACombatUnitBase* HitCombatUnit = Cast<ACombatUnitBase>(HitActor);
			if (!IsValid(HitCombatUnit))
			{
				continue;
			}

			if (bSearchAlly)
			{
				if (HitCombatUnit->GetCurrentAlliance() != Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
			else
			{
				if (HitCombatUnit->GetCurrentAlliance() == Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
		}
	}

	if (TargetSum > 0 && TargetSum < HitActors.Num())
	{
		for (int32 i = HitActors.Num() - 1; i >= TargetSum; --i)
		{
			HitActors.RemoveAtSwap(i);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugSphere(World, Center, Radius, 8, FColor::Red, false, DrawTime);
		for (const AActor* HitActor : HitActors)
		{
			FVector HitPos = HitActor->GetActorLocation();
			DrawDebugDirectionalArrow(World, HitPos, HitPos + FVector::ZAxisVector * 150.f
				, 1.f, FColor::Green, false, DrawTime);
		}
	}
#endif

	if (HitActors.IsEmpty())
	{
		return false;
	}

	OutTargets = HitActors;
	return true;
}

bool ULegendTraceLibrary::SectorTraceTarget(const UObject* WorldContext, AActor* Actor, AController* Alliance, float Radius, float Angle,
	TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter, bool bDrawDebug, float DrawTime)
{
	if (!IsValid(Actor) || !IsValid(Alliance))
	{
		return false;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), false);
	Params.bReturnPhysicalMaterial = false;

	const FVector Center = bTraceCenter ? Actor->GetActorLocation()
		: Actor->GetActorLocation() + Actor->GetActorForwardVector() * (Radius + GetCombatUnitRadius(Actor));

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn), FCollisionShape::MakeSphere(Radius), Params);

	TArray<AActor*>	HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		const AActor* HitActor = Overlaps[i].OverlapObjectHandle.GetManagingActor();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			if (UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(
				Actor->GetActorLocation(), HitActor->GetActorLocation())) > Angle)
			{
				continue;
			}

			const ACombatUnitBase* HitCombatUnit = Cast<ACombatUnitBase>(HitActor);
			if (!IsValid(HitCombatUnit))
			{
				continue;
			}

			if (bSearchAlly)
			{
				if (HitCombatUnit->GetCurrentAlliance() != Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
			else
			{
				if (HitCombatUnit->GetCurrentAlliance() == Alliance)
				{
					continue;
				}
				HitActors.Add(Overlaps[i].OverlapObjectHandle.GetManagingActor());
			}
		}
	}

	if (TargetSum > 0 && TargetSum < HitActors.Num())
	{
		for (int32 i = HitActors.Num() - 1; i >= TargetSum; --i)
		{
			HitActors.RemoveAtSwap(i);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		DrawDebugSphere(World, Center, Radius, 8, FColor::Red, false, DrawTime);
		for (const AActor* HitActor : HitActors)
		{
			FVector HitPos = HitActor->GetActorLocation();
			DrawDebugDirectionalArrow(World, HitPos, HitPos + FVector::ZAxisVector * 150.f
				, 1.f, FColor::Green, false, DrawTime);
		}
	}
#endif

	if (HitActors.IsEmpty())
	{
		return false;
	}

	OutTargets = HitActors;
	return true;
}

float ULegendTraceLibrary::GetCombatUnitRadius(AActor* Actor)
{
	const ACombatUnitBase* CombatUnit = Cast<ACombatUnitBase>(Actor);
	if (!IsValid(CombatUnit))
	{
		return 128.f;
	}

	return CombatUnit->GetAbilitySystemComponent()->GetNumericAttribute(ULegendAttributeSet::GetSizeAttribute()) * 64.f;
}

float ULegendTraceLibrary::GetGlobalGridRadius()
{
	float GlobalGridRadius = 0.f;
	if (GConfig->GetFloat(TEXT("/Script/LegendsTD.ArenaGridMapBase"),
		TEXT("GlobalGridRadius"), GlobalGridRadius, GGameIni))
	{
		return GlobalGridRadius;
	}
	return 120.f;
}

bool ULegendTraceLibrary::BoxTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance,
	FVector BoxSize, TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter, bool bDrawDebug,
	float DrawTime)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}
	
	return BoxTraceTarget(WorldContext, Actor, Alliance, BoxSize * GetGlobalGridRadius(), OutTargets
		, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
}

bool ULegendTraceLibrary::SphereTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance,
	float Radius, TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter, bool bDrawDebug,
	float DrawTime)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}
	
	return SphereTraceTarget(WorldContext, Actor, Alliance, Radius * GetGlobalGridRadius() * 2.f, OutTargets
		, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
}

bool ULegendTraceLibrary::SectorTraceTargetInGrid(const UObject* WorldContext, AActor* Actor, AController* Alliance,
	float Radius, float Angle, TArray<AActor*>& OutTargets, bool bSearchAlly, int32 TargetSum, bool bTraceCenter,
	bool bDrawDebug, float DrawTime)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}
	
	return SectorTraceTarget(WorldContext, Actor, Alliance, Radius * GetGlobalGridRadius() * 2.f, Angle, OutTargets
		, bSearchAlly, TargetSum, bTraceCenter, bDrawDebug, DrawTime);
}
