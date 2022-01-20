/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Client console commands module
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header
#include "../../code/ui/ui_shared.h"	// UI shared header

/*
=====================================================================
Check player emote camera
=====================================================================
*/
void JKMod_CG_EmoteCamera(void)
{
	int cameraAngle = 180;
	int cameraRange = 120;
	int cameraVertOffset = 0;

	if (!(trap_Key_GetCatcher() & KEYCATCH_UI)) return;
	if (trap_Key_GetCatcher() & KEYCATCH_CONSOLE) return;
	if (cg.snap->ps.pm_flags & PMF_FOLLOW) return;
	if (cg.snap->ps.pm_type == PM_SPECTATOR) return;
	
	if (cg_thirdPersonAngle.value != cameraAngle) cg.jkmodCG.prevCameraAngle = cg_thirdPersonAngle.value;
	if (cg_thirdPersonRange.value != cameraRange) cg.jkmodCG.prevCameraRange = cg_thirdPersonRange.value;
	if (cg_thirdPersonVertOffset.value != cameraVertOffset) cg.jkmodCG.prevCameraVertOffset = cg_thirdPersonVertOffset.value;
	
	cg_thirdPersonAngle.value = cg_thirdPersonAngle.value != cameraAngle ? cameraAngle : cg.jkmodCG.prevCameraAngle;
	cg_thirdPersonRange.value = cg_thirdPersonRange.value != cameraRange ? cameraRange : cg.jkmodCG.prevCameraRange;
	cg_thirdPersonVertOffset.value = cg_thirdPersonVertOffset.value != cameraVertOffset ? cameraVertOffset : cg.jkmodCG.prevCameraVertOffset;

	cg.jkmodCG.emoteCamera = !cg.jkmodCG.emoteCamera ? qtrue : qfalse;
}

/*
=====================================================================
Strafe helper toggle
=====================================================================
*/
void JKMod_CG_StrafeHelperToggle(void) 
{
	jkmod_cg_bit_info_t sHelperSettings[] = 
	{
		"Original style",
		"Updated style",
		"Cgaz style",
		"Warsow style",
		"Sound",
		"W",
		"WA",
		"WD",
		"A",
		"D",
		"Rear",
		"Center",
		"Accel bar",
		"Weze style",
		"Line Crosshair"
	};
	const int sHelperSettingsSize = ARRAY_LEN(sHelperSettings);

	if (trap_Argc() == 1) 
	{
		int i = 0;
		for (i = 0; i < sHelperSettingsSize; i++) 
		{
			if ((jkcvar_cg_strafeHelper.integer & (1 << i))) {
				CG_Printf("%2d ^2[X]^7 %s\n", i, sHelperSettings[i].string);
			} else {
				CG_Printf("%2d ^1[ ]^7 %s\n", i, sHelperSettings[i].string);
			}
		}
		CG_Printf("Example: ^3/strafehelper 13^7 (Toggles: ^5%s^7)\n", sHelperSettings[13].string);
		return;
	}
	else 
	{
		char arg[8] = { 0 };
		int index;
		const uint32_t mask = (1 << sHelperSettingsSize) - 1;

		trap_Argv(1, arg, sizeof(arg));
		index = atoi(arg);

		if (index < 0 || index >= sHelperSettingsSize) 
		{
			CG_Printf("^1Invalid option %i. Range is from 0 to %i\n", index, sHelperSettingsSize - 1);
			return;
		}

		// Check speed meter
		if (index == 12 && !(jkcvar_cg_speedMeter.integer & SMETER_ENABLE)) 
		{
			CG_Printf("^3Option %i requires speed meter enabled\n", index);
			return;
		}

		// Radio button these options. Toggle index, and make sure everything else in this group (0,1,2,3,13) is turned off
		if ((index == 0 || index == 1 || index == 2 || index == 3 || index == 13)) 
		{
			int groupMask = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 13);
			int value = jkcvar_cg_strafeHelper.integer;

			groupMask &= ~(1 << index);	// Remove index from groupmask
			value &= ~(groupMask);		// Turn groupmask off
			value ^= (1 << index);		// Toggle index item

			trap_Cvar_Set("jk_cg_strafeHelper", va("%i", value));
		}
		else 
		{
			trap_Cvar_Set("jk_cg_strafeHelper", va("%i", (1 << index) ^ (jkcvar_cg_strafeHelper.integer & mask)));
		}

		trap_Cvar_Update(&jkcvar_cg_strafeHelper);
		CG_Printf("Strafe Helper: %s %s^7\n", sHelperSettings[index].string, ((jkcvar_cg_strafeHelper.integer & (1 << index)) ? "^2Enabled" : "^1Disabled"));
	}
}

/*
=====================================================================
Speed meter toggle
=====================================================================
*/
void JKMod_CG_SpeedMeterToggle(void)
{
	jkmod_cg_bit_info_t sMeterSettings[] = 
	{
		"Enable speed meter",
		"Pre-speed display",
		"Jump height display",
		"Jump distance display",
		"Vertical speed indicator",
		"Yaw speed indicator",
		"Accel meter",
		"Speed graph",
		"Display speed in kilometers instead of units",
		"Display speed in imperial miles instead of units",
	};
	const int sMeterSettingsSize = ARRAY_LEN(sMeterSettings);

	if (trap_Argc() == 1) 
	{
		int i = 0;
		for (i = 0; i < sMeterSettingsSize; i++) 
		{
			if ((jkcvar_cg_speedMeter.integer & (1 << i))) {
				CG_Printf("%2d ^2[X]^7 %s\n", i, sMeterSettings[i].string);
			}
			else {
				CG_Printf("%2d ^1[ ]^7 %s\n", i, sMeterSettings[i].string);
			}
		}
		CG_Printf("Example: ^3/speedmeter 0^7 (Toggles: ^5%s^7)\n", sMeterSettings[0].string);
		return;
	}
	else 
	{
		char arg[8] = { 0 };
		int index;
		const uint32_t mask = (1 << sMeterSettingsSize) - 1;

		trap_Argv(1, arg, sizeof(arg));
		index = atoi(arg);

		if (index < 0 || index >= sMeterSettingsSize) 
		{
			CG_Printf("^1Invalid option %i. Range is from 0 to %i\n", index, sMeterSettingsSize - 1);
			return;
		}

		// WIP
		if (index == 7 || index  == 5) 
		{
			CG_Printf("^3Option %i is not ready yet\n", index);
			return;
		}

		// Radio button these options. Toggle index, and make sure everything else in this group (8,9) is turned off
		if (index == 8 || index == 9) 
		{ 
			int groupMask = (1 << 8) + (1 << 9);
			int value = jkcvar_cg_speedMeter.integer;

			groupMask &= ~(1 << index);	// Remove index from groupmask
			value &= ~(groupMask);		// Turn groupmask off
			value ^= (1 << index);		// Toggle index item

			trap_Cvar_Set("jk_cg_speedMeter", va("%i", value));
		}
		else 
		{
			trap_Cvar_Set("jk_cg_speedMeter", va("%i", (1 << index) ^ (jkcvar_cg_speedMeter.integer & mask)));
		}

		trap_Cvar_Update(&jkcvar_cg_speedMeter);
		CG_Printf("Speed Meter: %s %s^7\n", sMeterSettings[index].string, ((jkcvar_cg_speedMeter.integer & (1 << index)) ? "^2Enabled" : "^1Disabled"));
	}
}
