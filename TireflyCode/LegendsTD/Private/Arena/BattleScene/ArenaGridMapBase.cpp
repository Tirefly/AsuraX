// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "Arena/BattleScene/ArenaGridMapBase.h"

#include "ProceduralMeshComponent.h"
#include "Libraries/LegendTraceLibrary.h"


AArenaGridMapBase::AArenaGridMapBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GridRadius = ULegendTraceLibrary::GetGlobalGridRadius();

	if (MeshOfMap)
	{
		MeshOfMap->SetCollisionProfileName("SceneGrid");
	}
}
