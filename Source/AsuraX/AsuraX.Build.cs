// Copyright Tirefly. All Rights Reserved.

using UnrealBuildTool;

public class AsuraX : ModuleRules
{
	public AsuraX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		OptimizeCode = CodeOptimization.InShippingBuildsOnly;
	
		// Public Dependency Modules
		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		PublicDependencyModuleNames.Add("InputCore");
		
		// Private Dependency Modules
		PrivateDependencyModuleNames.Add("GameplayAbilities");
		PrivateDependencyModuleNames.Add("GameplayTasks");
		PrivateDependencyModuleNames.Add("GameplayTags");
		PrivateDependencyModuleNames.Add("AIModule");
		PrivateDependencyModuleNames.Add("EnhancedInput");
		PrivateDependencyModuleNames.Add("StructUtils");
		PrivateDependencyModuleNames.Add("Niagara");
		PrivateDependencyModuleNames.Add("TireflyGameplayAbilities");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
