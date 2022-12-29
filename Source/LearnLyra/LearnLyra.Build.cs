// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LearnLyra : ModuleRules
{
	public LearnLyra(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.AddRange(
			new string[] {
				"LearnLyra"
			}
		);


		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"GameplayTags",
			"ModularGameplay",
			"GameFeatures",
			"AIModule",
			"GameplayTasks",
			"GameplayAbilities",
			});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"CommonUI",
			"CommonInput",
			"InputCore",
			"NetCore",
			"EnhancedInput",
			"GameplayMessageRuntime",
			});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
