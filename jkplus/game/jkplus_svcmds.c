/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main server commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Console command function
=====================================================================
*/

qboolean JKPlus_ConsoleCommand(void)
{
	char	cmd[MAX_TOKEN_CHARS];

	trap_Argv(0, cmd, sizeof(cmd));

	if (!Q_stricmp(cmd, "gameplay"))
	{
		char arg1[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));

		switch (atoi(arg1))
		{
			case VERSION_1_02:
				MV_SetGamePlay(VERSION_1_02);
				trap_SendServerCommand(-1, "print \"Gameplay changed to 1.02\n\"");
				break;
			case VERSION_1_03:
				MV_SetGamePlay(VERSION_1_03);
				trap_SendServerCommand(-1, "print \"Gameplay changed to 1.03\n\"");
				break;
			default:
			case VERSION_1_04:
				MV_SetGamePlay(VERSION_1_04);
				trap_SendServerCommand(-1, "print \"Gameplay changed to 1.04\n\"");
				break;
		}
		return qtrue;
	}

	// Launch original console command function
	return BaseJK2_ConsoleCommand();
}
