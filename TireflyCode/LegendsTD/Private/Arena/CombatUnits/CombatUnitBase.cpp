// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CombatUnits/CombatUnitBase.h"

#include "CoreBasics/CoreAssetManager.h"
#include "CoreBasics/CoreGameplayTag.h"
#include "CoreBasics/CoreLogChannels.h"
#include "DataAsset/LegendGameplayAbilityAsset.h"
#include "DataAsset/LegendBuilder_CombatUnit.h"
#include "DataAsset/LegendConfig_CombatUnit.h"
#include "Libraries/LegendSystemLibrary.h"

#include "GameplayAbilities/LegendAbilitySystemComponent.h"
#include "GameplayAbilities/LegendAttributeSet.h"
#include "GameplayAbility/FireflyGameplayAbility.h"
#include "GameplayEffect/FireflyGameplayEffect.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "FireflyGridMapBase.h"
#include "FireflyGridMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "FireflyObjectPoolWorldSubsystem.h"

#include "Arena/GameFramework/ArenaGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Synergy/SynergyControllerInterface.h"


// Sets default values
ACombatUnitBase::ACombatUnitBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 初始化战斗单位属性
	CombatUnitID = NAME_None;
	CombatUnitType = ELegendCombatUnitType::None;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    if (IsValid(RootScene))
    {
	    SetRootComponent(RootScene);
    }

	// 骨架组件
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    if (IsValid(SkeletalMesh))
    {
    	SkeletalMesh->SetupAttachment(GetRootComponent());
    	SkeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		SkeletalMesh->SetGenerateOverlapEvents(false);
		SkeletalMesh->SetCollisionProfileName(TEXT("Pawn"));
		SkeletalMesh->SetCanEverAffectNavigation(false);
		SkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		SkeletalMesh->bComponentUseFixedSkelBounds = true;
		SkeletalMesh->bEnableUpdateRateOptimizations = true;
		SkeletalMesh->KinematicBonesUpdateType = EKinematicBonesUpdateToPhysics::SkipSimulatingBones;
    }

	// 角色移动组件
	GridMovement = CreateDefaultSubobject<UFireflyGridMovementComponent>(TEXT("GridMovement"));
    if (IsValid(GridMovement))
    {
		// 使战斗单位平滑旋转
		bUseControllerRotationYaw = false;
    }

	// 技能系统组件
	AbilitySystem = CreateDefaultSubobject<ULegendAbilitySystemComponent>(TEXT("AbilitySystem"));
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->PrimaryComponentTick.bCanEverTick = false;
		AbilitySystem->PrimaryComponentTick.bStartWithTickEnabled = false;
	}

	// 右手武器组件
	WeaponRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRight"));
	if (IsValid(WeaponRight))
	{
		WeaponRight->PrimaryComponentTick.bCanEverTick = false;
		WeaponRight->PrimaryComponentTick.bStartWithTickEnabled = false;

		WeaponRight->SetupAttachment(SkeletalMesh, FName("RigHub-R-WeaponSocket"));
		WeaponRight->SetGenerateOverlapEvents(false);
		WeaponRight->SetCollisionProfileName(TEXT("NoCollision"));
		WeaponRight->SetCanEverAffectNavigation(false);
		WeaponRight->bUseAttachParentBound = true;
	}

	// 左手武器组件
	WeaponLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponLeft"));
	if (IsValid(WeaponLeft))
	{
		WeaponLeft->PrimaryComponentTick.bCanEverTick = false;
		WeaponLeft->PrimaryComponentTick.bStartWithTickEnabled = false;

		WeaponLeft->SetupAttachment(SkeletalMesh, FName("RigHub-L-WeaponSocket"));
		WeaponLeft->SetGenerateOverlapEvents(false);
		WeaponLeft->SetCollisionProfileName(TEXT("NoCollision"));
		WeaponLeft->SetCanEverAffectNavigation(false);
		WeaponLeft->bUseAttachParentBound = true;
	}

	// 头部冠饰组件
	Headdress = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Headdress"));
	if (IsValid(Headdress))
	{
		Headdress->PrimaryComponentTick.bCanEverTick = false;
		Headdress->PrimaryComponentTick.bStartWithTickEnabled = false;

		Headdress->SetupAttachment(SkeletalMesh, FName("RigHub-Head"));
		Headdress->SetGenerateOverlapEvents(false);
		Headdress->SetCollisionProfileName(TEXT("NoCollision"));
		Headdress->SetCanEverAffectNavigation(false);
		Headdress->bUseAttachParentBound = true;
	}
}

void ACombatUnitBase::BeginPlay()
{
	InitializeAbilitySystem();

	Super::BeginPlay();

	if (AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this))
	{
		if (ArenaGM->GetCurrentGameStage() == ELegendGameStage::InBattle)
		{
			ToggleBrainLogic(true);
		}
		else
		{
			ToggleBrainLogic(false);
		}

		ArenaGM->OnGameStageUpdated.AddDynamic(this, &ACombatUnitBase::HandleGameStageUpdated);
	}
}

void ACombatUnitBase::InitializeCombatUnit(FName InCombatUnitID)
{
	const UCoreAssetManager& CoreAM = UCoreAssetManager::Get();
	if (!CoreAM.IsValid())
	{
		return;
	}

	CombatUnitBuilder = ULegendSystemLibrary::GetCombatUnitBuilder(InCombatUnitID);
	if (!IsValid(CombatUnitBuilder))
	{
		return;
	}

	const FCombatUnitBuilderData* BuilderData = CombatUnitBuilder->CoreBuilderData.GetPtr<FCombatUnitBuilderData>();
	if (!BuilderData)
	{
		return;
	}

	CombatUnitID = InCombatUnitID;
	CombatUnitType = BuilderData->CombatUnitType;
}

ULegendBuilder_CombatUnit* ACombatUnitBase::GetCombatUnitBuilder() const
{
	return CombatUnitBuilder;
}

/**
 * 判断当前单位是否可以被指定对手为目标。
 * 
 * @param Opponent 指定的对手战斗单位。
 * @return 返回一个布尔值，如果当前单位可以被指定对手为目标，则为true；否则为false。
 */
bool ACombatUnitBase::CanBeTargeted(ACombatUnitBase* Opponent) const
{
    // 验证对手是否为有效对象
	if (!IsValid(Opponent))
	{
		return false;
	}

    // 验证当前单位和对手的联盟状态是否有效
	if (!IsValid(GetCurrentAlliance()) || !IsValid(Opponent->GetCurrentAlliance()))
	{
		return false;
	}

    // 检查当前单位或对手是否处于死亡状态
	const bool bIsDead = AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_DEATH);
	const bool bOpponentDead = Opponent->AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_DEATH);
	if (bIsDead || bOpponentDead)
	{
		return false;
	}

    // 验证对手的移动组件是否有效
	const UFireflyGridMovementComponent* OpponentMovement = Opponent->GetGridMovement();
	if (!IsValid(OpponentMovement))
	{
		return false;
	}
	
    // 验证当前单位和对手所在的网格是否有效
	UFireflyGridBase* CurrentGrid = GetCurrentMovementGrid();
	UFireflyGridBase* OpponentGrid = Opponent->GetCurrentMovementGrid();
	if (!IsValid(CurrentGrid) || !IsValid(OpponentGrid))
	{
		return false;
	}
	
    // 验证当前单位和对手所在的网格地图是否有效
	const AFireflyGridMapBase* CurrentMap = CurrentGrid->GridMap;
	const AFireflyGridMapBase* OpponentMap = OpponentGrid->GridMap;
	if (!IsValid(CurrentMap) || !IsValid(OpponentMap))
	{
		return false;
	}

    // 检查从当前单位到对手是否存在路径，并且路径长度是否在攻击范围内
	return CurrentMap->IsPathExist(Opponent, OpponentGrid, CurrentGrid
		, AbilitySystem->GetNumericAttribute(ULegendAttributeSet::GetAttackRangeAttribute()));
}


void ACombatUnitBase::InitOnRespawned(APawn* InInstigator, UFireflyGridBase* Grid)
{
	SetInstigator(InInstigator);
	BackToOriginAlliance();

	SetCurrentMovementGrid(Grid);
}

UFireflyGridBase* ACombatUnitBase::GetCurrentMovementGrid() const
{
	if (IsValid(GridMovement))
	{
		return GridMovement->CurrentGrid;
	}

	return nullptr;
}

void ACombatUnitBase::SetCurrentMovementGrid(UFireflyGridBase* InGrid)
{
	if (!IsValid(GridMovement))
	{
		return;
	}

	if (!IsValid(InGrid))
	{
		GridMovement->StopMovementImmediately();
	}
	
	GridMovement->SetCurrentGrid(InGrid);
	SetTransformToCurrentGrid();
}

void ACombatUnitBase::SetTransformToCurrentGrid()
{
	const UFireflyGridBase* Grid = GetCurrentMovementGrid();
	if (!IsValid(Grid))
	{
		return;
	}

	SetActorTransform(Grid->WorldTransform , false, nullptr, ETeleportType::ResetPhysics);
}

bool ACombatUnitBase::CanPerformUpgrade() const
{
	return CurrentGrade < MaxGrade
		|| AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_INFINITEGRADE);
}

void ACombatUnitBase::HandleCurrentGradeChanged(int32 NewGrade, int32 OldGrade)
{
	HandleCurrentGradeChanged_GameplayAttribute(NewGrade, OldGrade);
	HandleCurrentGradeChanged_GameplayAbility(NewGrade, OldGrade);
}

void ACombatUnitBase::PerformUpgradeOperation()
{
	++CurrentGrade;
	HandleCurrentGradeChanged(CurrentGrade, CurrentGrade - 1);
	OnCurrentGradeChanged.Broadcast(CurrentGrade);
}

void ACombatUnitBase::SetCurrentGrade(int32 NewGrade)
{
	// 如果新等级和当前等级相同，或者新等级小于或等于0，就直接返回，不进行后续操作
	if (NewGrade == CurrentGrade || NewGrade < 0)
	{
		return;
	}

	// 保存修改前的等级
	const int32 OldGrade = CurrentGrade;

	// 如果新等级大于当前等级，并且新等级大于最大等级，且单位状态没有标记为无限等级
	if (NewGrade > CurrentGrade
		&& NewGrade > MaxGrade
		&& !AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_INFINITEGRADE))
	{
		// 则将当前等级设为最大等级
		CurrentGrade = MaxGrade;
	}
	else
	{
		// 否则，将当前等级设为新的等级
		CurrentGrade = NewGrade;
	}

	// 处理等级改变，参数为当前等级和修改前的等级
	HandleCurrentGradeChanged(CurrentGrade, OldGrade);
	// 广播战斗单位当前等级改变事件
	OnCurrentGradeChanged.Broadcast(CurrentGrade);
}

void ACombatUnitBase::InitializeCombatUnitSkin() const
{
	if (!IsValid(CombatUnitBuilder.Get()))
	{
		return;
	}

	SkeletalMesh->SetMaterial(0, CombatUnitBuilder->DefaultSkinData.MainBodySkin);
	Headdress->SetStaticMesh(CombatUnitBuilder->DefaultSkinData.HeaddressSkin);
	Headdress->SetMaterial(0, CombatUnitBuilder->DefaultSkinData.MainBodySkin);
	WeaponRight->SetStaticMesh(CombatUnitBuilder->DefaultSkinData.WeaponRightSkin);
	WeaponLeft->SetStaticMesh(CombatUnitBuilder->DefaultSkinData.WeaponLeftSkin);
}

void ACombatUnitBase::SetCurrentAlliance(AController* NewAlliance)
{
	AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this);
	if (!IsValid(ArenaGM))
	{
		UE_LOG(LogLegend, Warning, TEXT("ACombatUnitBase::SetCurrentAlliance() ArenaGameMode is invalid!"));
		return;
	}

	if (!IsValid(NewAlliance))
	{
		if (ArenaGM->IsCombatUnitInAlliance(this, CurrentAlliance))
		{
			ArenaGM->HandleCombatUnitQuitAlliance(this, CurrentAlliance);
		}

		return;
	}

	if (CurrentAlliance == NewAlliance)
	{
		if (ArenaGM->IsCombatUnitInAlliance(this, NewAlliance))
		{
			return;
		}

		ArenaGM->HandleCombatUnitJoinAlliance(this, NewAlliance, nullptr);
	}
	else
	{
		ArenaGM->HandleCombatUnitJoinAlliance(this, NewAlliance, CurrentAlliance);
		CurrentAlliance = NewAlliance;
	}
}

AController* ACombatUnitBase::GetOriginAlliance() const
{
	if (!IsValid(GetInstigator()))
	{
		return nullptr;
	}

	return GetInstigatorController();
}

void ACombatUnitBase::BackToOriginAlliance()
{
	if (!IsValid(GetInstigator()))
	{
		return;
	}

	SetCurrentAlliance(GetInstigatorController());
}

AActor* ACombatUnitBase::GetCurrentTarget() const
{
	const AAIController* AIC = GetController<AAIController>();
	if (!IsValid(AIC))
	{
		return nullptr;
	}

	const UBlackboardComponent* Blackboard = AIC->GetBlackboardComponent();
	if (!IsValid(Blackboard))
	{
		return nullptr;
	}

	return Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));
}

void ACombatUnitBase::InitializeAbilitySystem()
{
	AbilitySystem->InitAbilityActorInfo(this, this);
	InitGAS_CustomExecution();
	InitGAS_GameplayAttribute();
	InitGAS_GameplayAbility();
	InitGAS_GameplayEffect();
}

void ACombatUnitBase::InitGAS_CustomExecution()
{
	const ULegendConfig_CombatUnit* CombatUnitConfig = ULegendSystemLibrary::GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return;
	}

	for (const TSubclassOf<UFireflyAbilitySystemCustomExecution> ExecutionClass : CombatUnitConfig->CombatUnitInitialCustomExecutions)
	{
		AbilitySystem->AddCustomExecution(ExecutionClass);
	}
}

void ACombatUnitBase::InitGAS_GameplayAttribute()
{
	AbilitySystem->AddAttributeSet(ULegendAttributeSet::StaticClass());
	ResetGameplayAttributeValues();
}

void ACombatUnitBase::InitGAS_GameplayAbility()
{
	const ULegendConfig_CombatUnit* ConfigCU = ULegendSystemLibrary::GetCombatUnitConfig();
	if (!IsValid(ConfigCU))
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility> AbilityClass : ConfigCU->CombatUnitInitialAbilities)
	{
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityClass));
	}
}

void ACombatUnitBase::InitGAS_GameplayEffect()
{
	const ULegendConfig_CombatUnit* CombatUnitConfig = ULegendSystemLibrary::GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return;
	}

	const FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
	
	for (const TSubclassOf<UGameplayEffect> EffectClass : CombatUnitConfig->CombatUnitInitialEffects)
	{
		AbilitySystem->BP_ApplyGameplayEffectToSelf(EffectClass,1.f, EffectContext);
	}
}

void ACombatUnitBase::HandleCurrentGradeChanged_GameplayAttribute(int32 AdaptedNewGrade, int32 OldGrade)
{
	if (AdaptedNewGrade == 0)
	{
		ResetGameplayAttributeValues();
		return;
	}

	const ULegendConfig_CombatUnit* CombatUnitConfig = ULegendSystemLibrary::GetCombatUnitConfig();
	if (!IsValid(CombatUnitConfig))
	{
		return;
	}

	const UDataTable* DataTable = CombatUnitConfig->CombatUnitAttributeTable.FindRef(AdaptedNewGrade);
	if (!IsValid(DataTable))
	{
		return;
	}

	InitCombatUnitAttributes(DataTable, (CombatUnitType == ELegendCombatUnitType::Boss
		? FName(CombatUnitID.ToString() + FString("_Boss")) : CombatUnitID));
}

void ACombatUnitBase::HandleCurrentGradeChanged_GameplayAbility(int32 AdaptedNewGrade, int32 OldGrade)
{
	if (!IsValid(CombatUnitBuilder))
	{
		return;
	}

	if (AdaptedNewGrade > OldGrade)
	{
		/** 让英雄获取对应等级的技能 */
		for (int32 GradeIndex = OldGrade + 1; GradeIndex <= AdaptedNewGrade; ++GradeIndex)
		{
			const ULegendGameplayAbilityAsset* CurrentSkillData = CombatUnitBuilder->GetSkillConfigAtGrade(GradeIndex);
			if (!CurrentSkillData)
			{
				continue;
			}

			for (auto& SkillState : SkillsLearningState)
			{
				if (SkillState.Key == CurrentSkillData)
				{
					SkillState.Value = true;
					OnSkillLearningStateChanged.Broadcast(SkillState.Key, SkillState.Value);
					break;
				}
			}

			if (!IsValid(CurrentSkillData->AbilityClass))
			{
				continue;
			}

			SkillAbilities.Emplace(AbilitySystem->GiveAbility(FGameplayAbilitySpec(CurrentSkillData->AbilityClass
				, 1, INDEX_NONE, CombatUnitBuilder)));
		}
	}
	else
	{
		/** 让英雄移除对应等级的技能 */
		for (int32 GradeIndex = OldGrade; GradeIndex > AdaptedNewGrade; --GradeIndex)
		{
			const ULegendGameplayAbilityAsset* CurrentSkillData = CombatUnitBuilder->GetSkillConfigAtGrade(GradeIndex);
			if (!CurrentSkillData)
			{
				continue;
			}

			for (auto& SkillState : SkillsLearningState)
			{
				if (SkillState.Key == CurrentSkillData)
				{
					SkillState.Value = false;
					OnSkillLearningStateChanged.Broadcast(SkillState.Key, SkillState.Value);
					break;
				}
			}

			const FGameplayAbilitySpec* Spec = AbilitySystem->FindAbilitySpecFromClass(CurrentSkillData->AbilityClass);
			if (!Spec)
			{
				continue;
			}

			SkillAbilities.RemoveSingleSwap(Spec->Handle);
			AbilitySystem->ClearAbility(Spec->Handle);
		}
	}
}

void ACombatUnitBase::InitCombatUnitAttributes(const UDataTable* DataTable, FName RowName) const
{
	if (!IsValid(DataTable) || RowName == NAME_None || CombatUnitType == ELegendCombatUnitType::Summon)
	{
		return;
	}

	const FLegendCombatUnitAttributeTableRow* AttributeTableRow = DataTable->FindRow<FLegendCombatUnitAttributeTableRow>(RowName, TEXT(""));
	if (!AttributeTableRow)
	{
		return;
	}

	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetPhysiqueAttribute(), EGameplayModOp::Override, AttributeTableRow->Physique);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetMaxHealthAttribute(), EGameplayModOp::Override, AttributeTableRow->MaxHealth);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetHealthRecoveryRateAttribute(), EGameplayModOp::Override, AttributeTableRow->HealthRecoveryRate);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetAgilityAttribute(), EGameplayModOp::Override, AttributeTableRow->Agility);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetDefensiveForceAttribute(), EGameplayModOp::Override, AttributeTableRow->DefensiveForce);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetAttackSpeedAttribute(), EGameplayModOp::Override, AttributeTableRow->AttackSpeed);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetWisdomAttribute(), EGameplayModOp::Override, AttributeTableRow->Wisdom);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetManaRecoveryOfAttackAttribute(), EGameplayModOp::Override, AttributeTableRow->ManaRecoveryOfAttack);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetSkillIntensityAttribute(), EGameplayModOp::Override, AttributeTableRow->SkillIntensity);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetAttackForceAttribute(), EGameplayModOp::Override, AttributeTableRow->AttackForce);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetMaxManaAttribute(), EGameplayModOp::Override, AttributeTableRow->MaxMana);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetAttackRangeAttribute(), EGameplayModOp::Override, AttributeTableRow->AttackRange);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetAttackTargetSumAttribute(), EGameplayModOp::Override, AttributeTableRow->AttackTargetSum);
	AbilitySystem->ApplyModToAttribute(ULegendAttributeSet::GetMovementSpeedAttribute(), EGameplayModOp::Override, AttributeTableRow->MovementSpeed);
}

UAbilitySystemComponent* ACombatUnitBase::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}

EMainAttributeType ACombatUnitBase::GetMainAttributeType() const
{
	if (!IsValid(CombatUnitBuilder))
	{
		return EMainAttributeType::Physique;
	}

	return CombatUnitBuilder->CoreBuilderData.Get<FCombatUnitBuilderData>().MainAttributeType;
}

void ACombatUnitBase::ToggleBrainLogic(bool bOpen)
{
	if (const AAIController* BrainController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = BrainController->GetBrainComponent())
		{
			if (bOpen)
			{
				Brain->StartLogic();
				if (!AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_BATTLING))
				{
					AbilitySystem->AddLooseGameplayTag(GAS_COMBATUNITSTATE_BATTLING);
				}
			}
			else
			{
				Brain->Cleanup();
				if (AbilitySystem->HasMatchingGameplayTag(GAS_COMBATUNITSTATE_BATTLING))
				{
					AbilitySystem->RemoveLooseGameplayTag(GAS_COMBATUNITSTATE_BATTLING);
				}
			}
		}
	}
}

void ACombatUnitBase::ToggleCollision(bool bEnable)
{
	SetActorEnableCollision(bEnable);
}

void ACombatUnitBase::ToggleVisibility(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
}

void ACombatUnitBase::ToggleAliveState(bool bAlive)
{
	/** 通知GameMode把战斗单位从阵营的Alive容器转移到Dead容器 */
	AArenaGameMode* GM = ULegendSystemLibrary::GetArenaGameMode(this);
	if (!IsValid(GM))
	{
		UE_LOG(LogLegend, Warning, TEXT("ACombatUnitBase::ToggleAliveState() ArenaGameMode is invalid!"));
		return;
	}

	if (bAlive)
	{
		AbilitySystem->RemoveLooseGameplayTag(GAS_COMBATUNITSTATE_DEATH);
		GM->HandleCombatUnitReborn(CurrentAlliance, this);
	}
	else
	{
		AbilitySystem->AddLooseGameplayTag(GAS_COMBATUNITSTATE_DEATH);
		GM->HandleCombatUnitDead(CurrentAlliance, this);
	}
}

void ACombatUnitBase::ToggleAliveLocation(bool bAlive)
{
	if (bAlive)
	{
		SetCurrentMovementGrid(DeathGrid);
		return;
	}

	if (const AArenaGameMode* ArenaGM = ULegendSystemLibrary::GetArenaGameMode(this))
	{
		if (ArenaGM->GetCurrentGameStage() == ELegendGameStage::InBattle)
		{
			DeathGrid = GetCurrentMovementGrid();
		}
	}
	
	SetCurrentMovementGrid(nullptr);
	AddActorWorldOffset(FVector(0.f, 0.f, -1024.f)
			, true, nullptr, ETeleportType::ResetPhysics);
}

void ACombatUnitBase::ToggleSynergy(bool bJoinSynergy)
{
	ISynergyControllerInterface* RC = Cast<ISynergyControllerInterface>(GetOriginAlliance());
	if (!RC)
	{
		return;
	}

	if (bJoinSynergy)
	{
		RC->HandleCombatUnitJoined(this);
	}
	else
	{
		RC->HandleCombatUnitQuit(this);
	}
}

void ACombatUnitBase::ToggleNormalAttackAbility(bool bLearn)
{
	if (!IsValid(CombatUnitBuilder))
	{
		return;
	}

	if (bLearn)
	{
		const ULegendGameplayAbilityAsset* NormalAttackConfig = CombatUnitBuilder->NormalAttackConfig;
		if (!IsValid(NormalAttackConfig))
		{
			return;
		}

		NormalAttackAbility = AbilitySystem->GiveAbility(FGameplayAbilitySpec(
			NormalAttackConfig->AbilityClass, 1, INDEX_NONE, CombatUnitBuilder));
	}
	else
	{
		AbilitySystem->ClearAbility(NormalAttackAbility);
	}
}

void ACombatUnitBase::ToggleSkillAbilitiesLearningState(bool bInit)
{
	if (!IsValid(CombatUnitBuilder))
	{
		return;
	}

	if (bInit)
	{
		for (ULegendGameplayAbilityAsset* Skill : CombatUnitBuilder->SkillConfigs)
		{
			if (!IsValid(Skill))
			{
				continue;
			}

			SkillsLearningState.Emplace(Skill, false);
		}
	}
	else
	{
		SkillsLearningState.Empty();
	}
}

void ACombatUnitBase::ToggleCollideWithPawn(ECollisionResponse NewResponse) const
{
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
}

void ACombatUnitBase::ResetGameplayAttributeValues() const
{
	if (const ULegendConfig_CombatUnit* ConfigCU = ULegendSystemLibrary::GetCombatUnitConfig())
	{
		for (auto AttributeInit : ConfigCU->AttributeInitializer)
		{
			AbilitySystem->SetNumericAttributeBase(AttributeInit.Key, AttributeInit.Value);
		}
	}
}

void ACombatUnitBase::RemoveSkillAbilities()
{
	for (FGameplayAbilitySpecHandle SpecHandle : SkillAbilities)
	{
		AbilitySystem->ClearAbility(SpecHandle);
	}

	SkillAbilities.Empty();
}

void ACombatUnitBase::RemoveActiveCombatEffects() const
{
	AbilitySystem->RemoveActiveEffectsWithTags(FGameplayTagContainer(GAS_ASSET_EFFECT_ONLYCOMBAT));
}

void ACombatUnitBase::HandleRecycled()
{
	if (const UFireflyObjectPoolWorldSubsystem* SubsystemOP = GetWorld()->GetSubsystem<UFireflyObjectPoolWorldSubsystem>())
	{
		SubsystemOP->ActorPool_ReleaseActor(this);
	}
}

void ACombatUnitBase::HandleGameStageUpdated(ELegendGameStage NewGameStage, ELegendGameStage OldGameStage)
{
	if (NewGameStage == ELegendGameStage::InBattle)
	{
		HandleStartedBattle();
	}
	else
	{
		HandleFinishedBattle();
	}
}

void ACombatUnitBase::StartDead()
{
	ToggleAliveState(false);
	ToggleBrainLogic(false);
	RemoveActiveCombatEffects();
}

void ACombatUnitBase::FinishDead()
{
	ToggleCollision(false);
	ToggleVisibility(false);
	ToggleAliveLocation(false);
}

void ACombatUnitBase::StartRevive()
{
	ToggleVisibility(true);
	ToggleAliveLocation(true);
	ToggleCollision(true);
}

void ACombatUnitBase::FinishRevive()
{
	ToggleAliveState(true);
	ToggleBrainLogic(true);
}

void ACombatUnitBase::RecoverHealthByPercent(float Percent)
{
	AbilitySystem->SetNumericAttributeBase(ULegendAttributeSet::GetCurrentHealthAttribute(),
		AbilitySystem->GetNumericAttribute(ULegendAttributeSet::GetMaxHealthAttribute()) * Percent);
}

void ACombatUnitBase::RecoverManaByPercent(float Percent)
{
	AbilitySystem->SetNumericAttributeBase(ULegendAttributeSet::GetCurrentManaAttribute(),
		AbilitySystem->GetNumericAttribute(ULegendAttributeSet::GetMaxManaAttribute()) * Percent);
}

void ACombatUnitBase::PoolingBeginPlay_Implementation()
{
	BackToOriginAlliance();
	
	InitializeCombatUnit(CombatUnitID);
	InitializeCombatUnitSkin();
	ToggleNormalAttackAbility(true);
	ToggleSkillAbilitiesLearningState(true);
	PerformUpgradeOperation();

	ToggleAliveState(true);
	ToggleCollision(true);
	ToggleVisibility(true);
	DeathGrid = nullptr;
}

void ACombatUnitBase::PoolingEndPlay_Implementation()
{
	SetCurrentGrade(0);
	SetCurrentAlliance(nullptr);
	SetCurrentMovementGrid(nullptr);
	ToggleAliveLocation(false);
	ToggleSynergy(false);
	ToggleBrainLogic(false);
	ToggleCollision(false);
	ToggleVisibility(false);

	ResetGameplayAttributeValues();
	ToggleNormalAttackAbility(false);
	ToggleSkillAbilitiesLearningState(false);
	RemoveSkillAbilities();
	RemoveActiveCombatEffects();

	CombatUnitBuilder = nullptr;
	DeathGrid = nullptr;
	CombatUnitID = NAME_None;
}

TArray<FName> ACombatUnitBase::GetCombatUnitSynergyIDs() const
{
	if (!IsValid(CombatUnitBuilder))
	{
		return TArray<FName>{};
	}

	const FCombatUnitBuilderData* BuilderDataCU = CombatUnitBuilder->CoreBuilderData.GetPtr<FCombatUnitBuilderData>();
	if (!BuilderDataCU)
	{
		return TArray<FName>{};
	}

	return BuilderDataCU->SynergyIDs;
}
