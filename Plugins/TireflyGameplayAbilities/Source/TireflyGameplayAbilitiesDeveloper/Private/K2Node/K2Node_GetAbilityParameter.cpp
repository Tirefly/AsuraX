// Copyright Tirefly. All Rights Reserved.


#include "K2Node/K2Node_GetAbilityParameter.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "TireflyAbilitySystemLibrary.h"
#include "Engine/AssetManager.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "GameplayAbility/TireflyGameplayAbility.h"
#include "GameplayAbility/TireflyGameplayAbilityAsset.h"
#include "GameplayAbility/GameplayAbilityParameter/TireflyGameplayAbilityParameter.h"


#define LOCTEXT_NAMESPACE "TireflyGameplayAbilities"


namespace GetAbilityParameter_Helper
{
	const FName ExecOutputPinName = "ExecThen";
	const FName ParameterPinName = "Parameter";
	const FName AbilityClassPinName = "AbilityClass";
	const FName AbilityRefPinName = "AbilityRef";
	const FName AbilityIdPinName = "AbilityId";
	const FName AssetRefPinName = "AssetRef";
	const FName AssetPakPinName = "AssetPak";	
	const FName SuccessPinName = "Success";
	const FName ResultPinName = "Result";

	const FString ExecOutputPinFriendlyName(TEXT(" "));
	const FString ParameterPinFriendlyName(TEXT("Parameter"));
	const FString AbilityClassPinFriendlyName(TEXT("Ability Class"));
	const FString AbilityRefPinFriendlyName(TEXT("Ability Ref"));
	const FString AbilityIdPinFriendlyName(TEXT("Ability Id"));
	const FString AssetRefPinFriendlyName(TEXT("Asset Ref"));
	const FString AssetPakPinFriendlyName(TEXT("Asset Pak"));	
	const FString SuccessPinFriendlyName(TEXT("Success"));
}


FText UK2Node_GetAbilityParameter::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("TireflyGameplayAbilities_Title", "Get Gameplay Ability Parameter");
}

FText UK2Node_GetAbilityParameter::GetTooltipText() const
{
	return LOCTEXT("TireflyGameplayAbilities_Tooltip", "Get Gameplay Ability Asset Parameter By Name");
}

FText UK2Node_GetAbilityParameter::GetMenuCategory() const
{
	return LOCTEXT("TireflyGameplayAbilities_Category", "Tirefly Gameplay Abilities");
}

void UK2Node_GetAbilityParameter::GetMenuActions(
	FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FSlateIcon UK2Node_GetAbilityParameter::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Function_16x");

	return Icon;
}

void UK2Node_GetAbilityParameter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Unspecified)
	{
		return;
	}

	TArray<UEdGraphPin*> OldPins = MoveTemp(Pins);
	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(UK2Node_GetAbilityParameter, AbilityAssetSource))
	{
		BreakAllNodeLinks();
		for (UEdGraphPin* Pin : OldPins)
		{
			RemovePin(Pin);
		}

		AllocateDefaultPins();

		UEdGraph* Graph = GetGraph();
		Graph->NotifyGraphChanged();

		return;
	}
	
	UEdGraphPin* OldAbilityAssetSourcePin = nullptr;
	if (PropertyChangedEvent.Property->GetName() != GET_MEMBER_NAME_CHECKED(UK2Node_GetAbilityParameter, AbilityAssetSource))
	{
		BreakAllNodeLinks();
		for (auto& Pin : OldPins)
		{
			if (Pin->PinName == GetAbilityParameter_Helper::AbilityRefPinName || Pin->PinName == UEdGraphSchema_K2::PN_Self)
			{
				OldAbilityAssetSourcePin = Pin;
				break;
			}
			else if (Pin->PinName == GetAbilityParameter_Helper::AbilityIdPinName)
			{
				OldAbilityAssetSourcePin = Pin;
				break;
			}
			else if (Pin->PinName == GetAbilityParameter_Helper::AssetRefPinName)
			{
				OldAbilityAssetSourcePin = Pin;
				break;
			}
			else if (Pin->PinName == GetAbilityParameter_Helper::AssetPakPinName)
			{
				OldAbilityAssetSourcePin = Pin;
				break;
			}
		}
	}
	
	UEdGraphPin* OldParameterPin = nullptr;
	for (auto& Pin : OldPins)
	{
		if (Pin->PinName == GetAbilityParameter_Helper::ParameterPinName)
		{
			OldParameterPin = Pin;
			break;
		}
	}

	AllocateDefaultPins();

	UTireflyGameplayAbilityAsset* AbilityAsset = GetAbilityAsset(OldAbilityAssetSourcePin);
	FName Parameter = FName(OldParameterPin->DefaultValue);
	RecreateVariantPinsInternal(AbilityAsset, Parameter);

	RestoreSplitPins(OldPins);
	RewireOldPinsToNewPins(OldPins, Pins, nullptr);

	for (auto& Pin : OldPins)
	{
		RemovePin(Pin);
	}

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

void UK2Node_GetAbilityParameter::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	// 创建中介节点：确认获取能力参数的函数名
	FName CallFunctionName = NAME_None;
	switch (AbilityAssetSource)
	{
	case ETireflyAbilityAssetSource::AbilityClass:
		{
			CallFunctionName = bPureNode ? GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityClassPure)
				: GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityClass);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityRef:
		{
			CallFunctionName = bPureNode ? GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityRefPure)
				: GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityRef);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityId:
		{
			CallFunctionName = bPureNode ? GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityIdPure)
				: GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAbilityId);
			break;
		}
	case ETireflyAbilityAssetSource::AssetRef:
		{
			CallFunctionName = bPureNode ? GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAssetRefPure)
				: GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAssetRef);
			break;
		}
	case ETireflyAbilityAssetSource::AssetPak:
		{
			CallFunctionName = bPureNode ? GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAssetPakPure)
				: GET_FUNCTION_NAME_CHECKED(UTireflyAbilitySystemLibrary, GetAbilityParamOfAssetPak);
			break;
		}
	}
	if (CallFunctionName == NAME_None)
	{
		return;
	}
	// 创建中介节点：创建调用函数"获取能力参数"节点
	UK2Node_CallFunction* CallFunctionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFunctionNode->FunctionReference.SetExternalMember(CallFunctionName, UTireflyAbilitySystemLibrary::StaticClass());
	CallFunctionNode->AllocateDefaultPins();

	// 链接中介节点：链接执行引脚
	if (!bPureNode)
	{
		UEdGraphPin* CallFunctionExecInputPin = GetExecInputPin();
		UEdGraphPin* CallFunctionExecOutputPin = GetExecInputPin();
		UEdGraphPin* ExecInputPin = GetExecInputPin();
		UEdGraphPin* ExecOutputPin = GetExecInputPin();
		
		CompilerContext.MovePinLinksToIntermediate(*ExecInputPin, *CallFunctionExecInputPin);
		CompilerContext.MovePinLinksToIntermediate(*ExecOutputPin, *CallFunctionExecOutputPin);
	}
	// 链接中介节点：链接能力资产来源引脚
	UEdGraphPin* CallFunctionAbilityAssetSourcePin = nullptr;
	switch (AbilityAssetSource)
	{
	case ETireflyAbilityAssetSource::AbilityClass:
		{
			CallFunctionAbilityAssetSourcePin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::AbilityClassPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityRef:
		{
			CallFunctionAbilityAssetSourcePin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::AbilityRefPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityId:
		{
			CallFunctionAbilityAssetSourcePin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::AbilityIdPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetRef:
		{
			CallFunctionAbilityAssetSourcePin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::AssetRefPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetPak:
		{
			CallFunctionAbilityAssetSourcePin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::AssetPakPinName);
			break;
		}
	}
	UEdGraphPin* AbilityAssetSourcePin = GetAbilityAssetSourcePin();
	if (AbilityAssetSourcePin->LinkedTo.Num() > 0)
	{
		// 如果能力资产来源引脚有链接，则直接将CallFunctionNode的能力资产来源节点与之链接
		CompilerContext.MovePinLinksToIntermediate(*AbilityAssetSourcePin, *CallFunctionAbilityAssetSourcePin);
	}
	else
	{
		// 如果能力资产来源引脚无链接，则将能力资产引脚的DefaultObject赋给CallFunctionNode的能力资产来源节点
		switch (AbilityAssetSource)
		{
		case ETireflyAbilityAssetSource::AbilityClass:
			{
				if (AbilityAssetSourcePin->PinName == UEdGraphSchema_K2::PN_Self)
				{
					if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
					{
						CallFunctionAbilityAssetSourcePin->DefaultObject = Blueprint->GeneratedClass;
						break;
					}
				}
				CallFunctionAbilityAssetSourcePin->DefaultObject = AbilityAssetSourcePin->DefaultObject;
				break;
			}
		case ETireflyAbilityAssetSource::AbilityRef:
			{
				if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
				{
					if (UClass* BlueprintClass = Blueprint->GeneratedClass)
					{
						CallFunctionAbilityAssetSourcePin->DefaultObject = Blueprint->GeneratedClass->GetDefaultObject();
						break;
					}
				}
				CallFunctionAbilityAssetSourcePin->DefaultObject = AbilityAssetSourcePin->DefaultObject;
				break;
			}
		case ETireflyAbilityAssetSource::AssetRef:
			{
				CallFunctionAbilityAssetSourcePin->DefaultObject = AbilityAssetSourcePin->DefaultObject;
				break;
			}
		case ETireflyAbilityAssetSource::AbilityId:
		case ETireflyAbilityAssetSource::AssetPak:
			{
				CallFunctionAbilityAssetSourcePin->DefaultValue = AbilityAssetSourcePin->DefaultValue;
				break;
			}
		}
	}
	// 链接中介节点：链接能力参数名引脚
	UEdGraphPin* CallFunctionParameterPin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::ParameterPinName);
	UEdGraphPin* ParameterPin = GetParameterPin();
	CompilerContext.MovePinLinksToIntermediate(*ParameterPin, *CallFunctionParameterPin);
	// 链接中介节点：链接是否成功引脚
	UEdGraphPin* CallFunctionSuccessPin = CallFunctionNode->FindPinChecked(GetAbilityParameter_Helper::SuccessPinName);
	UEdGraphPin* SuccessPin = GetSuccessPin();
	CompilerContext.MovePinLinksToIntermediate(*SuccessPin, *CallFunctionSuccessPin);
	// 链接中介节点：链接结果引脚
	UEdGraphPin* CallFunctionResultPin = CallFunctionNode->GetReturnValuePin();
	UEdGraphPin* ResultPin = GetResultPin();
	CallFunctionResultPin->PinType = ResultPin->PinType;
	CallFunctionResultPin->PinType.PinSubCategory = ResultPin->PinType.PinSubCategory;
	CallFunctionResultPin->PinType.PinSubCategoryObject = ResultPin->PinType.PinSubCategoryObject;
	CallFunctionResultPin->PinType.PinValueType = ResultPin->PinType.PinValueType;
	CompilerContext.MovePinLinksToIntermediate(*ResultPin, *CallFunctionResultPin);

	BreakAllNodeLinks();
}

void UK2Node_GetAbilityParameter::AllocateDefaultPins()
{
	CreateExecInputPin();
	CreateExecOutputPin();
	CreateAbilityAssetSourcePin();
	CreateParameterPin();
	CreateSuccessPin();
	CreateResultPin();
	
	Super::AllocateDefaultPins();
}

void UK2Node_GetAbilityParameter::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	UEdGraphPin* OldAssetSourcePin = nullptr;
	for (auto& Pin : OldPins)
	{
		if (Pin->PinName == GetAbilityParameter_Helper::AbilityClassPinName
			|| Pin->PinName == GetAbilityParameter_Helper::AbilityRefPinName
			|| Pin->PinName == UEdGraphSchema_K2::PN_Self)
		{
			OldAssetSourcePin = Pin;
			break;
		}
		else if (Pin->PinName == GetAbilityParameter_Helper::AbilityIdPinName)
		{
			OldAssetSourcePin = Pin;
			break;
		}
		else if (Pin->PinName == GetAbilityParameter_Helper::AssetRefPinName)
		{
			OldAssetSourcePin = Pin;
			break;
		}
		else if (Pin->PinName == GetAbilityParameter_Helper::AssetPakPinName)
		{
			OldAssetSourcePin = Pin;
			break;
		}
	}
	UEdGraphPin* OldParameterPin = nullptr;
	for (auto& Pin : OldPins)
	{
		if (Pin->PinName == GetAbilityParameter_Helper::ParameterPinName)
		{
			OldParameterPin = Pin;
			break;
		}
	}

	AllocateDefaultPins();

	if (OldAssetSourcePin == nullptr || OldParameterPin == nullptr)
	{
		RestoreSplitPins(OldPins);
		return;
	}

	RecreateVariantPinsInternal(GetAbilityAsset(OldAssetSourcePin), FName(OldParameterPin->DefaultValue));

	RestoreSplitPins(OldPins);

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

void UK2Node_GetAbilityParameter::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	if (Pin == nullptr)
	{
		return;
	}

	if (Pin->PinName == GetAbilityParameter_Helper::AbilityClassPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AbilityRefPinName ||
		Pin->PinName == UEdGraphSchema_K2::PN_Self ||
		Pin->PinName == GetAbilityParameter_Helper::AbilityIdPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AssetRefPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AssetPakPinName)
	{
		RecreateVariantPins();
	}
	else if (Pin->PinName == GetAbilityParameter_Helper::ParameterPinName)
	{
		RecreateVariantPins();
	}
}

void UK2Node_GetAbilityParameter::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin == nullptr)
	{
		return;
	}

	if (Pin->PinName == GetAbilityParameter_Helper::AbilityClassPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AbilityRefPinName ||
		Pin->PinName == UEdGraphSchema_K2::PN_Self ||
		Pin->PinName == GetAbilityParameter_Helper::AbilityIdPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AssetRefPinName ||
		Pin->PinName == GetAbilityParameter_Helper::AssetPakPinName)
	{
		RecreateVariantPins();
	}
}

void UK2Node_GetAbilityParameter::CreateExecInputPin()
{
	if (bPureNode)
	{
		return;
	}
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
}

void UK2Node_GetAbilityParameter::CreateExecOutputPin()
{
	if (bPureNode)
	{
		return;
	}
	UEdGraphPin* ExecOutputPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetAbilityParameter_Helper::ExecOutputPinName);
	ExecOutputPin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::ExecOutputPinFriendlyName);
}

void UK2Node_GetAbilityParameter::CreateAbilityAssetSourcePin()
{
	switch (AbilityAssetSource)
	{
	case ETireflyAbilityAssetSource::AbilityClass:
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class,
				UTireflyGameplayAbility::StaticClass(), GetAbilityParameter_Helper::AbilityClassPinName);
			if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
			{
				if (Blueprint->GeneratedClass.GetDefaultObject()->IsA(UTireflyGameplayAbility::StaticClass()))
				{
					Pin->PinName = UEdGraphSchema_K2::PN_Self;
				}
			}
			Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::AbilityClassPinFriendlyName);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityRef:
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object,
				UTireflyGameplayAbility::StaticClass(), GetAbilityParameter_Helper::AbilityRefPinName);
			if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
			{
				if (Blueprint->GeneratedClass.GetDefaultObject()->IsA(UTireflyGameplayAbility::StaticClass()))
				{
					Pin->PinName = UEdGraphSchema_K2::PN_Self;
				}
			}
			Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::AbilityRefPinFriendlyName);
			break;
		}
	case ETireflyAbilityAssetSource::AbilityId:
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetAbilityParameter_Helper::AbilityIdPinName);
			Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::AbilityIdPinFriendlyName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetRef:
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object,
				UTireflyGameplayAbilityAsset::StaticClass(), GetAbilityParameter_Helper::AssetRefPinName);
			Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::AssetRefPinFriendlyName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetPak:
		{
			UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetAbilityParameter_Helper::AssetPakPinName);
			Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::AssetPakPinFriendlyName);
			break;
		}
	}
}

void UK2Node_GetAbilityParameter::CreateParameterPin()
{
	UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetAbilityParameter_Helper::ParameterPinName);
	Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::ParameterPinFriendlyName);
}

void UK2Node_GetAbilityParameter::CreateSuccessPin()
{
	UEdGraphPin* Pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, GetAbilityParameter_Helper::SuccessPinName);
	Pin->PinFriendlyName = FText::AsCultureInvariant(GetAbilityParameter_Helper::SuccessPinFriendlyName);
}

void UK2Node_GetAbilityParameter::CreateResultPin(UClass* ResultType, const FString& PinFriendlyName)
{
	UEdGraphPin* Pin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, GetAbilityParameter_Helper::ResultPinName);
	Pin->PinType.PinSubCategoryObject = (ResultType == nullptr ? UTireflyGameplayAbilityParameter::StaticClass() : ResultType);
	Pin->PinFriendlyName = FText::AsCultureInvariant(PinFriendlyName.IsEmpty() ? FString("Result") : PinFriendlyName);
}

void UK2Node_GetAbilityParameter::RecreateVariantPins()
{
	Modify();

	TArray<UEdGraphPin*> UnusedPins = MoveTemp(Pins);
	for (int32 Index = 0; Index < UnusedPins.Num(); ++Index)
	{
		UEdGraphPin* OldPin = UnusedPins[Index];
		if (OldPin->PinName != GetAbilityParameter_Helper::ResultPinName)
		{
			UnusedPins.RemoveAt(Index--, 1, false);
			Pins.Add(OldPin);
		}
	}

	UTireflyGameplayAbilityAsset* AbilityAsset = GetAbilityAsset();
	FName Parameter = FName(GetParameterPin()->DefaultValue);
	RecreateVariantPinsInternal(AbilityAsset, Parameter);

	RestoreSplitPins(UnusedPins);
	RewireOldPinsToNewPins(UnusedPins, Pins, nullptr);

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

void UK2Node_GetAbilityParameter::RecreateVariantPinsInternal(const UTireflyGameplayAbilityAsset* Asset,
	const FName& Parameter)
{
	if (UEdGraphPin* Pin = GetResultPin())
	{
		RemovePin(Pin);
	}
	
	if (Asset == nullptr || Parameter == NAME_None)
	{
		CreateResultPin();
		return;
	}

	UTireflyGameplayAbilityParameter* ParameterRef = Asset->AbilityParameters.FindRef(Parameter);
	if (ParameterRef == nullptr)
	{
		CreateResultPin();
		return;
	}
	
	CreateResultPin(ParameterRef->GetClass(), Parameter.ToString());
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetExecInputPin() const
{
	if (!bPureNode)
	{
		return GetExecPin();
	}
	
	return nullptr;
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetExecOutputPin() const
{
	if (!bPureNode)
	{
		return FindPin(GetAbilityParameter_Helper::ExecOutputPinName);
	}
	
	return nullptr;
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetAbilityAssetSourcePin() const
{
	UEdGraphPin* OutPin = nullptr;
	switch (AbilityAssetSource)
	{
	case ETireflyAbilityAssetSource::AbilityClass:
		{
			OutPin = FindPin(GetAbilityParameter_Helper::AbilityClassPinName);
			if (!OutPin)
			{
				OutPin = FindPin(UEdGraphSchema_K2::PN_Self);
			}
			break;
		}
	case ETireflyAbilityAssetSource::AbilityRef:
		{
			OutPin = FindPin(GetAbilityParameter_Helper::AbilityRefPinName);
			if (!OutPin)
			{
				OutPin = FindPin(UEdGraphSchema_K2::PN_Self);
			}
			break;
		}
	case ETireflyAbilityAssetSource::AbilityId:
		{
			OutPin = FindPin(GetAbilityParameter_Helper::AbilityIdPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetRef:
		{
			OutPin = FindPin(GetAbilityParameter_Helper::AssetRefPinName);
			break;
		}
	case ETireflyAbilityAssetSource::AssetPak:
		{
			OutPin = FindPin(GetAbilityParameter_Helper::AssetPakPinName);
			break;
		}
	}

	return OutPin;
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetParameterPin() const
{
	return FindPin(GetAbilityParameter_Helper::ParameterPinName);
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetSuccessPin() const
{
	return FindPin(GetAbilityParameter_Helper::SuccessPinName);
}

UEdGraphPin* UK2Node_GetAbilityParameter::GetResultPin() const
{
	return FindPin(GetAbilityParameter_Helper::ResultPinName);
}

UTireflyGameplayAbilityAsset* UK2Node_GetAbilityParameter::GetAbilityAsset(UEdGraphPin* Pin) const
{
	UEdGraphPin* AssetSourcePin = Pin;
	if (AssetSourcePin == nullptr)
	{
		AssetSourcePin = GetAbilityAssetSourcePin();
	}
	if (AssetSourcePin == nullptr)
	{
		return nullptr;
	}

	if (AssetSourcePin->LinkedTo.Num() > 0)
	{
		return nullptr;
	}

	UTireflyGameplayAbilityAsset* OutAsset = nullptr;
	switch (AbilityAssetSource)
	{
	case ETireflyAbilityAssetSource::AbilityClass:
		{
			if (AssetSourcePin->PinName == UEdGraphSchema_K2::PN_Self)
			{
				if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
				{
					if (UClass* BlueprintClass = Blueprint->GeneratedClass)
					{
						if (UTireflyGameplayAbility* Ability = BlueprintClass->GetDefaultObject<UTireflyGameplayAbility>())
						{
							OutAsset = Ability->GetAbilityAsset();
						}
					}
				}
			}
			else
			{
				if (UClass* AbilityClass = Cast<UClass>(AssetSourcePin->DefaultObject))
				{
					if (UTireflyGameplayAbility* Ability = AbilityClass->GetDefaultObject<UTireflyGameplayAbility>())
					{
						OutAsset = Ability->GetAbilityAsset();
					}
				}
			}
			break;
		}
	case ETireflyAbilityAssetSource::AbilityRef:
		{
			if (AssetSourcePin->PinName == UEdGraphSchema_K2::PN_Self)
			{
				if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
				{
					if (UClass* BlueprintClass = Blueprint->GeneratedClass)
					{
						if (UTireflyGameplayAbility* Ability = BlueprintClass->GetDefaultObject<UTireflyGameplayAbility>())
						{
							OutAsset = Ability->GetAbilityAsset();
						}
					}
				}
			}
			break;
		}
	case ETireflyAbilityAssetSource::AbilityId:
		{
			if (UAssetManager* AS = &UAssetManager::Get())
			{
				OutAsset = AS->GetPrimaryAssetObject<UTireflyGameplayAbilityAsset>(FPrimaryAssetId(
					UTireflyGameplayAbilityAsset::GameplayAbilityAsset, FName(AssetSourcePin->DefaultValue)));
			}
			break;
		}
	case ETireflyAbilityAssetSource::AssetRef:
		{
			OutAsset = Cast<UTireflyGameplayAbilityAsset>(AssetSourcePin->DefaultObject);
			break;
		}
	case ETireflyAbilityAssetSource::AssetPak:
		{
			FSoftObjectPath AssetPath(AssetSourcePin->DefaultValue);
			OutAsset = Cast<UTireflyGameplayAbilityAsset>(AssetPath.TryLoad()); 
			break;
		}
	}

	return OutAsset;
}

#undef LOCTEXT_NAMESPACE
