// Copyright tzlFirefly. All Rights Reserved.

using UnrealBuildTool;

public class FireflyGameplayAbilities : ModuleRules
{
	public FireflyGameplayAbilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "StructUtils" });
		
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
		PublicDependencyModuleNames.Add("InputCore");
		PublicDependencyModuleNames.Add("Slate");
		PublicDependencyModuleNames.Add("SlateCore");
			
		// Private Dependency Modules
		PrivateDependencyModuleNames.Add("GameplayAbilities");
		PrivateDependencyModuleNames.Add("GameplayTasks");
		PrivateDependencyModuleNames.Add("GameplayTags");
		PrivateDependencyModuleNames.Add("EnhancedInput");
		PrivateDependencyModuleNames.Add("DeveloperSettings");
		PrivateDependencyModuleNames.Add("StructUtils");
		PrivateDependencyModuleNames.Add("NetCore");
		
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
