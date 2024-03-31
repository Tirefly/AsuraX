// Copyright Tirefly. All Rights Reserved.

using UnrealBuildTool;

public class TireflyGameplayAbilitiesDeveloper : ModuleRules
{
    public TireflyGameplayAbilitiesDeveloper(ReadOnlyTargetRules Target) : base(Target)
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
        

        // Private Dependency Modules
        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");
        PrivateDependencyModuleNames.Add("NetCore");
        PrivateDependencyModuleNames.Add("GameplayAbilities");
        PrivateDependencyModuleNames.Add("TireflyGameplayAbilities");
        PrivateDependencyModuleNames.Add("BlueprintGraph");
        PrivateDependencyModuleNames.Add("UnrealEd");


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
            );
    }
}