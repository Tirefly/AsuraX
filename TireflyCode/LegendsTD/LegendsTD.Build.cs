// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LegendsTD : ModuleRules
{
	public LegendsTD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        OptimizeCode = CodeOptimization.Never;


        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("Slate");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("EnhancedInput");
        PublicDependencyModuleNames.Add("GameplayAbilities");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("GameplayTags");
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("DataRegistry");
        PublicDependencyModuleNames.Add("Niagara");
        PublicDependencyModuleNames.Add("Json");
        PublicDependencyModuleNames.Add("JsonUtilities");
        PublicDependencyModuleNames.Add("StructUtils");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");


        PrivateDependencyModuleNames.Add("FireflyGameplayAbilities");
        PrivateDependencyModuleNames.Add("FireflyObjectPool");
        PrivateDependencyModuleNames.Add("FireflyFunctionLibraries");
        PrivateDependencyModuleNames.Add("FireflyGridMap");
        PrivateDependencyModuleNames.Add("LLFuntionSet");
        PrivateDependencyModuleNames.Add("UIFrame");


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
