/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main server commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Gameplay command functions
=====================================================================
*/

static void JKPlus_svCmd_gamePlay(void) 
{
	char arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	if (!Q_stricmp(arg1, "2") || !Q_stricmp(arg1, "1.02"))
	{
		MV_SetGamePlay(VERSION_1_02);
		trap_SendServerCommand(-1, "print \"Server gameplay was changed to 1.02\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.02\n\"");
	}
	else if (!Q_stricmp(arg1, "3") || !Q_stricmp(arg1, "1.03"))
	{
		MV_SetGamePlay(VERSION_1_03);
		trap_SendServerCommand(-1, "print \"Server gameplay was changed to 1.03\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.03\n\"");
	}
	else 
	{
		MV_SetGamePlay(VERSION_1_04);
		trap_SendServerCommand(-1, "print \"Server gameplay was changed to 1.04\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.04\n\"");
	}
}

/*
=====================================================================
Pause game command functions
=====================================================================
*/

static void JKPlus_svCmd_pauseGame(void)
{
	if (jkcvar_pauseGame.integer == 0)
	{
		trap_Cvar_Set("jk_pauseGame", "1");
	}
	else
	{
		trap_Cvar_Set("jk_pauseGame", "0");
	}
}

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
		JKPlus_svCmd_gamePlay();
		return qtrue;
	}

	if (!Q_stricmp(cmd, "pausegame"))
	{
		JKPlus_svCmd_pauseGame();
		return qtrue;
	}

	// Launch original console command function
	return BaseJK2_ConsoleCommand();
}
