// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class coursework302 : ModuleRules
{
	public coursework302(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
