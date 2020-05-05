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
Gameplay changer
=====================================================================
*/
void JKMod_gamePlay(char *gameplay)
{
	if (!Q_stricmp(gameplay, "2") || !Q_stricmp(gameplay, "1.02"))
	{
		MV_SetGamePlay(VERSION_1_02);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.02\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.02\n\"");
	}
	else if (!Q_stricmp(gameplay, "3") || !Q_stricmp(gameplay, "1.03"))
	{
		MV_SetGamePlay(VERSION_1_03);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.03\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.03\n\"");
	}
	else if (!Q_stricmp(gameplay, "4") || !Q_stricmp(gameplay, "1.04"))
	{
		MV_SetGamePlay(VERSION_1_04);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.04\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.04\n\"");
	}
	else
	{
		trap_Cvar_Set("jk_gamePlay", va("%i", jk2gameplay));
	}
}

static void JKMod_svCmd_gamePlay(void)
{
	char	arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	trap_Cvar_Set("jk_gamePlay", arg1);
}

/*
=====================================================================
Pause game command functions
=====================================================================
*/
static void JKMod_svCmd_pauseGame(void)
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
qboolean JKMod_ConsoleCommand(void)
{
	char	cmd[MAX_TOKEN_CHARS];

	trap_Argv(0, cmd, sizeof(cmd));

	if (!Q_stricmp(cmd, "gameplay"))
	{
		JKMod_svCmd_gamePlay();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "pausegame"))
	{
		JKMod_svCmd_pauseGame();
		return qtrue;
	}

	// Launch original console command function
	return BaseJK2_ConsoleCommand();
}
