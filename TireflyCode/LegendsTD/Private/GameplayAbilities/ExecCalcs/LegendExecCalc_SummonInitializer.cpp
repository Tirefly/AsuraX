// Copyright LazyGameStudio, Inc. All Rights Reserved.


#include "GameplayAbilities/ExecCalcs/LegendExecCalc_SummonInitializer.h"

#include "AbilitySystemComponent.h"
#include "CoreBasics/CoreGameplayTag.h"
#include "GameplayAbilities/LegendAttributeSet.h"


void ULegendExecCalc_SummonInitializer::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const float Coefficient = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(GAS_SETBYCALLER_ATTRIBUTEINITIALIZER);
	if (Coefficient <= 0.f)
	{
		return;
	}

	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (!IsValid(SourceASC))
	{
		return;
	}

	auto InitLambda = [SourceASC](FGameplayEffectCustomExecutionOutput& InExecutionOutput
		, const FGameplayAttribute& Attribute, float InCoefficient, bool bOverride)
	{
		InExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attribute,
			bOverride ? EGameplayModOp::Override : EGameplayModOp::Additive,
			SourceASC->GetNumericAttribute(Attribute) * InCoefficient));
	};

	// 体魄
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetPhysiqueAttribute(), Coefficient, true);
	// 最大生命值
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetMaxHealthAttribute(), Coefficient * 0.5f, true);
	// 生命值恢复速度
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetHealthRecoveryRateAttribute(), Coefficient * 0.5f, true);
	// 身法
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetAgilityAttribute(), Coefficient, true);
	// 防御力
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetDefensiveForceAttribute(), Coefficient * 0.5f, true);
	// 攻击速度
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetAttackSpeedAttribute(), Coefficient * 0.5f, true);
	// 灵智
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetWisdomAttribute(), Coefficient, true);
	// 攻击恢复法力量
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetManaRecoveryOfAttackAttribute(), Coefficient * 0.5f, true);
	// 技能强度
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetSkillIntensityAttribute(), Coefficient * 0.5f, true);
	// 攻击力
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetAttackForceAttribute(), Coefficient * 0.5f, true);
	// 生命偷取
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetHealthStealingAttribute(), Coefficient, true);
	// 法力值恢复速度
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetManaRecoveryRateAttribute(), Coefficient, true);
	// 暴击率
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetCriticalRateAttribute(), Coefficient, true);
	// 暴击伤害加成
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetCriticalEnhanceRateAttribute(), Coefficient, true);
	// 伤害加成
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetDamageBonusAttribute(), Coefficient, true);
	// 伤害减免
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetDamageReductionAttribute(), Coefficient, true);
	// 最大法力值
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetMaxManaAttribute(), Coefficient * 0.5f, true);
	// 攻击目标数量
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetAttackTargetSumAttribute(), 1.f, true);
	// 攻击范围
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetAttackRangeAttribute(), 1.f, true);
	// 移动速度
	InitLambda(OutExecutionOutput, ULegendAttributeSet::GetMovementSpeedAttribute(), 1.f, true);
}
