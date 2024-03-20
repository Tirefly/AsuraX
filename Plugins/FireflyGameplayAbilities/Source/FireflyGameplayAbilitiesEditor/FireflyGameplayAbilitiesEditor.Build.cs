// Copyright tzlFirefly. All Rights Reserved.

using UnrealBuildTool;

public class FireflyGameplayAbilitiesEditor : ModuleRules
{
    public FireflyGameplayAbilitiesEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        OptimizeCode = CodeOptimization.InShippingBuildsOnly;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
            );

        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
            }
            );

        // Public Dependency Modules
        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("Slate");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("NetCore");

        // Private Dependency Modules
        PrivateDependencyModuleNames.Add("GameplayAbilities");
        PrivateDependencyModuleNames.Add("GameplayTasks");
        PrivateDependencyModuleNames.Add("GameplayTags");
        PrivateDependencyModuleNames.Add("FireflyGameplayAbilities");
        PrivateDependencyModuleNames.Add("KismetCompiler");
        PrivateDependencyModuleNames.Add("BlueprintGraph");
        PrivateDependencyModuleNames.Add("GraphEditor");
        PrivateDependencyModuleNames.Add("UnrealEd");

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
            );
    }
}