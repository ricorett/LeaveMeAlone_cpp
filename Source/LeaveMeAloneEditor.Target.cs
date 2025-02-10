// LeaveMeAlone Game by Netologiya. All rights are reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class LeaveMeAloneEditorTarget : TargetRules
{
	public LeaveMeAloneEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "LeaveMeAlone" } );
	}
}
