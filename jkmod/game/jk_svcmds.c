/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Main server commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern jkmod_dimension_data_t JKModDimensionData[];
extern int JKModDimensionDataSize;

/*
=====================================================================
Gameplay command function
=====================================================================
*/
static void JKMod_svCmd_gamePlay(void)
{
	char	arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	if (trap_Argc() < 2) {
		G_Printf("Usage: gameplay <version>\n");
	} else {
		JKMod_SetGamePlay(arg1);
	}
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

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: reload <option>\n");
	}
	else
	{
		if (!Q_stricmp(arg1, "random_begin")) JKMod_CVU_randomBegin();
		else if (!Q_stricmp(arg1, "server_news")) JKMod_CVU_serverNews();
		else if (!Q_stricmp(arg1, "teleport_chats")) JKMod_CVU_teleportChat();
		else G_Printf("Option %s not allowed\n", arg1);
	}
}

/*
=====================================================================
Pause game command function
=====================================================================
*/
static void JKMod_svCmd_pauseGame(void)
{
	char	arg1[MAX_STRING_CHARS];

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: pause <on|seconds|off>\n");
	}
	else 
	{
		trap_Argv(1, arg1, sizeof(arg1));

		if (!Q_stricmp(arg1, "off")) 
		{
			if (level.jkmodLocals.pauseTime > level.time) level.jkmodLocals.pauseTime = level.time;
		}
		else 
		{
			if (level.intermissionQueued || level.intermissiontime) return;

			if (!Q_stricmp(arg1, "on")) 
			{
				level.jkmodLocals.pauseTime = INT_MAX;
				level.jkmodLocals.pauseTimeCustom = 0;
			}
			else // Seconds
			{
				level.jkmodLocals.pauseTime = level.time + 1000 * atoi(arg1);
				level.jkmodLocals.pauseTimeCustom = atoi(arg1);
			}
		}
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

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: remapshader <original> <replacement>\n");
	}
	else
	{
		float f = level.time * 0.001;

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

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

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: changemusic <music>\n");
	}
	else
	{
		fileHandle_t	f;
		char			*music;

		trap_Argv(1, arg1, sizeof(arg1));

		if (trap_FS_FOpenFile(va("music/%s", arg1), &f, FS_READ) >= 0)
		{
			trap_FS_FCloseFile(f);
			G_SpawnString("music", va("music/%s", arg1), &music);
			trap_SetConfigstring(CS_MUSIC, music);
		}
		else
		{
			G_Printf("Music %s doesn't exist\n", arg1);
		}
	}
}

/*
=====================================================================
Toggle mod cvar options
=====================================================================
*/

// Options for jk_playerMovement cvar
static jkmod_bit_info_t togglePlayerMovement[] = 
{
	"JKA JETPACK STYLE",
	"WEAPON STAND FIX",
	"DISRUPTOR ZOOM WALK",
	"BUTTON USE STAND",
	"SPECTATOR NO-CLIP",
	"RESIST PUSH ANIM",
	"DUAL SABER MOVES",
};

// Options for jk_voteControl cvar
static jkmod_bit_info_t toggleVoteControl[] = 
{
	"MAP RESTART",
	"NEXT MAP",
	"MAP",
	"GAME TYPE",
	"KICK",
	"CLIENT KICK",
	"DO WARM UP",
	"TIME LIMIT",
	"FRAG LIMIT",
	"POLL",
	"ITEM PHYSICS",
	"PAUSE GAME",
};

// Options for jk_altDimension cvar
static jkmod_bit_info_t toggleAltDimension[] = 
{
	"NORMAL",
	"PRIVATE DUEL",
	"GUNS ARENA",
	"RACE DEFRAG",
	"SABER ONLY",
	"INSTA KILL",
	"CHEATS MODE",
};

// Options for bot_forGimmick cvar
static jkmod_bit_info_t toggleBotForGimmick[] = 
{
	"STATIC",
	"ATTACK",
	"ALT ATTACK",
	"CROUCH",
	"JUMP",
	"TAUNT",
	"TALK",
	"GOD",
};

// Toggle mod command
static void JKMod_svCmd_toggleMod(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: togglemod <cvar> <option>\n");
		G_Printf("Cvars:\n"
			"^3jk_playerMovement\n"
			"^3jk_voteControl\n"
			"^3jk_altDimension\n"
			"^3jk_altDimensionBase\n"
			"^3bot_forGimmick\n"
			);
	}
	else
	{
		int bits;
		int i, val, start = 0;
		int toggleModSize;
		char *toggleModNote;
		qboolean toggleModRadio = qfalse;
		jkmod_bit_info_t *toggleModOptions;

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		if (!Q_stricmp(arg1, "jk_playerMovement")) 
		{
			toggleModOptions = togglePlayerMovement;
			toggleModSize = ARRAY_LEN(togglePlayerMovement);
		}
		else if (!Q_stricmp(arg1, "jk_voteControl")) 
		{
			toggleModOptions = toggleVoteControl;
			toggleModSize = ARRAY_LEN(toggleVoteControl);
		}
		else if (!Q_stricmp(arg1, "jk_altDimension")) 
		{
			toggleModOptions = toggleAltDimension;
			toggleModSize = ARRAY_LEN(toggleAltDimension);
			toggleModNote = "Dimensions defined as base should be changed first before disable";
		}
		else if (!Q_stricmp(arg1, "jk_altDimensionBase")) 
		{
			toggleModOptions = toggleAltDimension;
			toggleModSize = ARRAY_LEN(toggleAltDimension);
			toggleModNote = "For normal dimension change this cvar manually to 0";
			toggleModRadio = qtrue;
			start = 2;
		}
		else if (!Q_stricmp(arg1, "bot_forGimmick")) 
		{
			toggleModOptions = toggleBotForGimmick;
			toggleModSize = ARRAY_LEN(toggleBotForGimmick);
			toggleModNote = "Any of these actions will make the bots static";
		}
		else
		{
			G_Printf("Cvar %s not allowed\n", arg1);
			return;
		}

		bits = trap_Cvar_VariableIntegerValue(arg1);

		if (trap_Argc() != 3) 
		{
			for (i = start; i < toggleModSize; i++) 
			{
				if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
				else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
			}
			G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, start, toggleModOptions[start].string);
			if (toggleModNote[0] != '\0') G_Printf("Note: %s\n", toggleModNote);
			return;
		}

		if (JKMod_ValidNumber(arg2)) 
		{
			val = atoi(arg2);

			if (val >= start && val < toggleModSize) 
			{
				level.jkmodLocals.cvarToggleMod = qtrue;
				if (toggleModRadio) {
					bits = (1 << val);
					trap_Cvar_Set(arg1, va("%i",bits));
					trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", arg1, toggleModOptions[val].string));
				} else {
					bits ^= (1 << val);
					trap_Cvar_Set(arg1, va("%i",bits));
					trap_SendServerCommand(-1, va("print \"Server: %s %s %s\n\"", arg1, ((bits & (1 << val)) ? "enabled" : "disabled"), toggleModOptions[val].string));
				}
			}
			else 
			{
				G_Printf("^1Specified an option that does not exist\n");
				for (i = start; i < toggleModSize; i++) 
				{
					if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
					else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
				}
				G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, start, toggleModOptions[start].string);
				if (toggleModNote[0] != '\0') G_Printf("Note: %s\n", toggleModNote);
			}
		}
		else 
		{
			G_Printf("^1Specified an option is not valid\n");
			for(i = start; i < toggleModSize; i++ ) 
			{
				if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
				else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
			}
			G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, start, toggleModOptions[start].string);
			if (toggleModNote[0] != '\0') G_Printf("Note: %s\n", toggleModNote);
		}
	}
}

/*
=====================================================================
Force dimension function
=====================================================================
*/
static void JKMod_svCmd_forceDimension(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: forcedimension <player|number|all> <dimension>\n");
	}
	else
	{
		int i;
		unsigned dimension;
		qboolean found = qfalse;

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		if (g_gametype.integer != GT_FFA) {
			G_Printf("You can't change dimension in this game type\n");
			return;
		}

		for (i = 0; i < JKModDimensionDataSize; i++)
		{
			if (!Q_stricmp(arg2, JKModDimensionData[i].command)) {
				dimension = JKModDimensionData[i].dimension;
				found = qtrue;
				break;
			} 
		}

		if (!found) {
			G_Printf("Dimension %s doesn't exist\n", arg2);
			return;
		}

		if (!(jkcvar_altDimension.integer & dimension)) {
			G_Printf("Dimension %s is not available\n", arg2);
			return;
		}

		if (!Q_stricmp(arg1, "all"))
		{
			gentity_t *ent;

			for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
			{
				if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
				{
					if (ent->client->ps.duelInProgress)
					{
						JKMod_DuelRemove(ent);
					}

					JKMod_DimensionSet(ent, dimension);
				}
			}
		}
		else
		{
			int target = JKMod_CheckValidClient(NULL, arg1);

			if (target != -1)
			{
				gentity_t *ent = &g_entities[target];

				if (ent->client->ps.duelInProgress)
				{
					gentity_t *duelAgainst = &g_entities[ent->client->ps.duelIndex];

					JKMod_DuelRemove(ent);
					JKMod_DuelRemove(duelAgainst);

					if (duelAgainst->client->ps.stats[JK_DIMENSION] == DIMENSION_FREE && jkcvar_duelPassThrough.integer && JKMod_OthersInBox(duelAgainst)) JKMod_AntiStuckBox(duelAgainst);
					if (duelAgainst->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE) JKMod_DimensionSet(duelAgainst, DIMENSION_FREE);
				}
				
				JKMod_DimensionSet(ent, dimension);
			}
		}
	}
}

/*
=====================================================================
List directory function
=====================================================================
*/
static void JKMod_svCmd_listDir(void)
{
	if (trap_Argc() < 2)
	{
		G_Printf("Usage: listdir <folder> [extension] [filter]\n");
	}
	else
	{
		char		arg1[MAX_STRING_CHARS];
		char		arg2[MAX_STRING_CHARS];
		char		arg3[MAX_STRING_CHARS];

		int			numFiles;
		int			filelen;
		char		filelist[4096];
		static char	filename[MAX_QPATH];
		char		*fileptr;
		int			i;

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));
		trap_Argv(3, arg3, sizeof(arg3));

		numFiles = trap_FS_GetFileList(arg1, VALIDSTRING(arg2) ? arg2 : "", filelist, sizeof(filelist));
		fileptr = filelist;

		G_Printf( "Directory of %s (ext: %s filter: %s)\n", arg1, arg2, arg3 );
		G_Printf( "---------------\n" );

		for (i = 0; i < numFiles; i++, fileptr += filelen+1)
		{
			filelen = strlen(fileptr);
			strcpy(filename, va("%s/", arg1));
			strcat(filename, fileptr);

			if (trap_Argc() > 3) {
				if (strstr(filename, arg3)) G_Printf("%s\n", filename);
			} else {
				G_Printf("%s\n", filename);
			}
		}
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
	if (!Q_stricmp(cmd, "pause"))
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
	if (!Q_stricmp(cmd, "togglemod"))
	{
		JKMod_svCmd_toggleMod();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "forcedimension"))
	{
		JKMod_svCmd_forceDimension();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "whois"))
	{
		JKMod_Cmd_WhoIs(NULL);
		return qtrue;
	}
	if (!Q_stricmp(cmd, "listdir"))
	{
		JKMod_svCmd_listDir();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "checkcvars") && jkcvar_gameTypeConfig.integer && !level.jkmodLocals.cvarTempUnlock)
	{
		level.jkmodLocals.cvarTempUnlock = 1;
		G_RegisterCvars();
		return qtrue;
	}

	// Launch original console command function
	return BaseJK2_ConsoleCommand();
}
