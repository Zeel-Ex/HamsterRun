// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class HamsterRunTarget : TargetRules
{
	public HamsterRunTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "HamsterRun" } );
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PreBuildSteps.Add("\"$(ProjectDir)\\Scripts\\update_version.bat\" \"$(ProjectDir)\"");
		}
		else
		{
			PreBuildSteps.Add("bash \"$(ProjectDir)/Scripts/update_version.sh\" \"$(ProjectDir)\"");
		}
	}
}
