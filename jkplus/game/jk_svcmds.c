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
		if (jk2gameplay != VERSION_1_02) {
			MV_SetGamePlay(VERSION_1_02);
			trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.02\n\"");
			trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.02\n\"");
		}
		if (jk2startversion == VERSION_1_02) {
			trap_Cvar_Set("jk_gamePlay", "0");
		}
	}
	else if (!Q_stricmp(gameplay, "3") || !Q_stricmp(gameplay, "1.03"))
	{
		if (jk2gameplay != VERSION_1_03) {
			MV_SetGamePlay(VERSION_1_03);
			trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.03\n\"");
			trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.03\n\"");
		}
		if (jk2startversion == VERSION_1_03) {
			trap_Cvar_Set("jk_gamePlay", "0");
		}
	}
	else if (!Q_stricmp(gameplay, "4") || !Q_stricmp(gameplay, "1.04"))
	{
		if (jk2gameplay != VERSION_1_04) {
			MV_SetGamePlay(VERSION_1_04);
			trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.04\n\"");
			trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.04\n\"");
		}
		if (jk2startversion == VERSION_1_04) {
			trap_Cvar_Set("jk_gamePlay", "0");
		}
	}
	else
	{
		trap_Cvar_Set("jk_gamePlay", va("%i", jk2gameplay));
	}
}

/*
=====================================================================
Gameplay command function
=====================================================================
*/
static void JKMod_svCmd_gamePlay(void)
{
	char	arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	trap_Cvar_Set("jk_gamePlay", arg1);
}

/*
=====================================================================
Reload info command function
=====================================================================
*/
static void JKMod_svCmd_reload(void)
{
	char	arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	if (!Q_stricmp(arg1, "randombegin"))
	{
		JKMod_randomBeginInit();
	}
	else if (!Q_stricmp(arg1, "servernews"))
	{
		JKMod_serverNewsInit();
	}
	else if (!Q_stricmp(arg1, "teleportchat"))
	{
		JKMod_teleportChatInit();
	}
}

/*
=====================================================================
Pause game command function
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
Remap shader command function
=====================================================================
*/
static void JKMod_svCmd_remapShader(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];

	float f = level.time * 0.001;

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: remapshader <original> <replacement>\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		Com_sprintf(arg1, sizeof(arg1), "%s", arg1);
		Com_sprintf(arg2, sizeof(arg1), "%s", arg2);

		AddRemap(arg1, arg2, f);

		trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
	}
}

/*
=====================================================================
Change music command function
=====================================================================
*/
static void JKMod_svCmd_changeMusic(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	*music;

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: changemusic <music>\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));
		Com_sprintf(arg1, sizeof(arg1), "%s", arg1);

		G_SpawnString("music", arg1, &music);
		trap_SetConfigstring(CS_MUSIC, music);
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
	if (!Q_stricmp(cmd, "reload"))
	{
		JKMod_svCmd_reload();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "remapshader"))
	{
		JKMod_svCmd_remapShader();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "changemusic"))
	{
		JKMod_svCmd_changeMusic();
		return qtrue;
	}

	// Launch original console command function
	return BaseJK2_ConsoleCommand();
}
