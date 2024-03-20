// Copyright tzlFirefly. All Rights Reserved.


#include "K2Node_GetParamSpecByAsset.h"

#include "GameplayAbility/FireflyGameplayAbility.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "K2Node_GetParamSpecByAsset"

namespace GetParamSpecByAssetHelper
{
	const FName ParamNamePinName = "ParamName";
	const FName OutParamSpecPinName = "OutParamSpec";
	const FName ResultPinName = "Result";
}

bool UK2Node_GetParamSpecByAsset::IsCompatibleWithGraph(UEdGraph const* TargetGraph) const
{
	UBlueprint* MyBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	if (MyBlueprint && MyBlueprint->GeneratedClass)
	{
		if (MyBlueprint->GeneratedClass->IsChildOf(UFireflyGameplayAbility::StaticClass()))
		{
			return true;
		}
	}
		
	return false;
}

void UK2Node_GetParamSpecByAsset::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
}

FText UK2Node_GetParamSpecByAsset::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return Super::GetNodeTitle(TitleType);
}

FText UK2Node_GetParamSpecByAsset::GetTooltipText() const
{
	return Super::GetTooltipText();
}

void UK2Node_GetParamSpecByAsset::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_GetParamSpecByAsset::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
}

FText UK2Node_GetParamSpecByAsset::GetMenuCategory() const
{
	return Super::GetMenuCategory();
}
