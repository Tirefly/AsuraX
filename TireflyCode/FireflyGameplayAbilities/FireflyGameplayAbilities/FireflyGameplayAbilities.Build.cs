// Copyright tzlFirefly, 2023. All Rights Reserved.

using UnrealBuildTool;

public class FireflyGameplayAbilities : ModuleRules
{
	public FireflyGameplayAbilities(ReadOnlyTargetRules Target) : base(Target)
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
        PublicDependencyModuleNames.Add("StructUtils");
		PublicDependencyModuleNames.Add("AssetRegistry");


        // Private Dependency Modules
        PrivateDependencyModuleNames.Add("GameplayAbilities");
		PrivateDependencyModuleNames.Add("GameplayTasks");
        PrivateDependencyModuleNames.Add("GameplayTags");
        PrivateDependencyModuleNames.Add("EnhancedInput");
        PrivateDependencyModuleNames.Add("DeveloperSettings");


        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
			PrivateDependencyModuleNames.Add("KismetCompiler");
        }


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
