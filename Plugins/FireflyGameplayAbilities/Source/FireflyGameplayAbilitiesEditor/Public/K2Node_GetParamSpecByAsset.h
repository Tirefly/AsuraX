// Copyright tzlFirefly. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_GetParamSpecByAsset.generated.h"

/**
 * 
 */
UCLASS()
class FIREFLYGAMEPLAYABILITIESEDITOR_API UK2Node_GetParamSpecByAsset : public UK2Node
{
	GENERATED_BODY()

#pragma region EdGraphNode

public:
	// 确定是否可以为指定的图形创建此类型的节点
	virtual bool IsCompatibleWithGraph(UEdGraph const* TargetGraph) const override;

	// 分配引脚，在内部通过CreatePin()创建节点的引脚
	virtual void AllocateDefaultPins() override;

	// 获取节点标题
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	// 获取节点的工具描述，即注释
	virtual FText GetTooltipText() const override;

	// 展开节点，节点的实际运行逻辑在此处实现
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

#pragma endregion


#pragma region K2Node

public:
	// 将节点注册到蓝图图表上下文
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	// 节点是否是纯节点
	virtual bool IsNodePure() const override { return true; }

	// 覆盖以提供要在下列出的特定节点类型的默认类别
	virtual FText GetMenuCategory() const override;

#pragma endregion
};
