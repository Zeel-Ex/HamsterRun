// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class HamsterRun : ModuleRules
{
	public HamsterRun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"ApplicationCore", 
			"EngineSettings", 
			"AdvancedSessions", 
			"OnlineSubsystemSteam", 
			"Steamworks",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
