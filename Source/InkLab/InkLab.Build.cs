// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InkLab : ModuleRules
{
	public InkLab(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "SlateCore", "Slate", "UMG"});
	}
}
