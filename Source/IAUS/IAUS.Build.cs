// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class IAUS : ModuleRules
{
	public IAUS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrecompileForTargets = PrecompileTargetsType.Any;

		PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Public/Components"),
			Path.Combine(ModuleDirectory, "Public/Composites"),
			Path.Combine(ModuleDirectory, "Public/Considerations"),
			Path.Combine(ModuleDirectory, "Public/Decorators"),
			Path.Combine(ModuleDirectory, "Public")
		});

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"AIModule",
				"GameplayTasks",
			}
		);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(
				new string[] {
					"Slate",
					"SlateCore",
					"EditorStyle",
				}
			);
		}
	}
}
