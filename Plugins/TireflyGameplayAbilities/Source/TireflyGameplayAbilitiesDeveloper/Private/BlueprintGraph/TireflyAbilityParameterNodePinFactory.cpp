// Copyright Tirefly. All Rights Reserved.


#include "BlueprintGraph/TireflyAbilityParameterNodePinFactory.h"

#include "SGraphPinNameList.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "K2Node/K2Node_GetAbilityParameter.h"


TSharedPtr<SGraphPin> FTireflyAbilityParameterNodePinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Name)
	{
		return nullptr;
	}

	UK2Node_GetAbilityParameter* GetAbilityParameterNode = Cast<UK2Node_GetAbilityParameter>(InPin->GetOuter());
	if (!GetAbilityParameterNode)
	{
		return nullptr;
	}

	if (InPin == GetAbilityParameterNode->GetAbilityAssetSourcePin())
	{
		return nullptr;
	}

	UTireflyGameplayAbilityAsset* AbilityAsset = GetAbilityParameterNode->GetAbilityAsset();
	if (!AbilityAsset)
	{
		return nullptr;
	}

	TArray<TSharedPtr<FName>> ParameterNames;
	for (const TPair<FName, UTireflyGameplayAbilityParameter*>& ParameterPair : AbilityAsset->AbilityParameters)
	{
		ParameterNames.Add(MakeShareable(new FName(ParameterPair.Key)));
	}

	return SNew(SGraphPinNameList, InPin, ParameterNames);
}
