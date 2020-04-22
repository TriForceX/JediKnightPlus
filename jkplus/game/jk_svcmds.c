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
	char	arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	trap_Cvar_Set("jk_gamePlay", arg1);
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
