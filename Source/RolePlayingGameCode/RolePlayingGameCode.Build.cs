// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RolePlayingGameCode : ModuleRules
{
	public RolePlayingGameCode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Niagara", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
