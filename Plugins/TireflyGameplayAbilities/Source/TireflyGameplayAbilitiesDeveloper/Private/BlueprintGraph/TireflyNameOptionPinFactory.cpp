// Copyright Tirefly. All Rights Reserved.
#include "BlueprintGraph/TireflyNameOptionPinFactory.h"

#include "K2Node_CallFunction.h"
#include "SGraphPinNameList.h"


TSharedPtr<SGraphPin> FTireflyNameOptionPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Name)
	{
		return nullptr;
	}

	UK2Node_CallFunction* CallFunctionNode = Cast<UK2Node_CallFunction>(InPin->GetOuter());
	if (!CallFunctionNode)
	{
		return nullptr;
	}

	const FString PinOptionFunctionName = CallFunctionNode->GetPinMetaData(InPin->PinName, FName("GetParamOptions"));
	if (PinOptionFunctionName.IsEmpty())
	{
		return nullptr;
	}

	UObject* OwningObject = nullptr;
	if (const UBlueprint* OwningBlueprint = CallFunctionNode->GetBlueprint())
	{
		if (OwningBlueprint->GeneratedClass)
		{
			OwningObject = OwningBlueprint->GeneratedClass->GetDefaultObject();
		}
	}
	if (!OwningObject)
	{
		return nullptr;
	}

	UFunction* FuncToExec = OwningObject->FindFunction(FName(*PinOptionFunctionName));
	if (!FuncToExec)
	{
		return nullptr;
	}

	TArray<FName> OptionNames;
	OwningObject->ProcessEvent(FuncToExec, &OptionNames);
	TArray<TSharedPtr<FName>> OutOptions;
	for (const FName& OptionName : OptionNames)
	{
		OutOptions.Add(MakeShareable(new FName(OptionName)));
	}
	
	return SNew(SGraphPinNameList, InPin, OutOptions);
}

// FString FunctionString = "ActionGame.ActionSystemLibrary.ProcessHandler";
// FSoftObjectPtr FunctionPtr;
// FConstructorHelpers::FObjectFinder<UFunction> FunctionFinder(*FunctionString);
// if (FunctionFinder.Succeeded())
// {
// 	FunctionPtr = FunctionFinder.Object;
// }
//
// if (FunctionPtr.IsValid())
// {
// 	// 创建函数调用
// 	FFunctionParams Params;
// 	FFrame Stack(nullptr, FunctionPtr.Get(), Params);
//
// 	// 调用函数
// 	FunctionPtr.Get()->Invoke(Stack, nullptr);
//
// 	// 获取返回值
// 	TArray<FName> ResultArray;
// 	Stack.Step(Stack.Object, &ResultArray);
//
// 	// 现在你可以使用ResultArray来处理函数的返回值
// }