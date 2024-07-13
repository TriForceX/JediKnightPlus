/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
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
		G_Printf("Options:\n"
			"^3random_begin\n"
			"^3server_news\n"
			"^3teleport_chats\n"
			"^3dimension_data\n"
			"^3shader_remaps\n"
			);
	}
	else
	{
		if (!Q_stricmp(arg1, "random_begin")) 
			JKMod_RandomBeginLoad();
		else if (!Q_stricmp(arg1, "server_news")) 
			JKMod_ServerNewsLoad();
		else if (!Q_stricmp(arg1, "teleport_chats")) 
			JKMod_TeleportChatLoad();
		else if (!Q_stricmp(arg1, "dimension_data")) 
			JKMod_DimensionLoad();
		else if (!Q_stricmp(arg1, "shader_remaps")) 
			JKMod_RemapShaders();
		else 
			G_Printf("Option %s not allowed\n", arg1);
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

// Options for jk_playerTweaks cvar
static jkmod_bit_info_t togglePlayerTweaks[] = 
{
	"JKA JETPACK STYLE",
	"WEAPON STAND FIX",
	"DISRUPTOR ZOOM WALK",
	"BUTTON USE STAND",
	"SPECTATOR NO-CLIP",
	"RESIST PUSH ANIM",
	"DUAL SABER MOVES",
	"GHOST SABERS",
	"CHAIR FORCE USABLE",
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
	"SCORE RESTART",
	"PAUSE GAME",
	"MAP NUMBER",
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
	"FULL FORCE",
	"PRIVATE ROOM",
};

// Options for jk_spawnActivateSaber cvar
static jkmod_bit_info_t toggleSpawnActivateSaber[] = 
{
	"FFA",
	"HOLOCRON",
	"JEDI MASTER",
	"DUEL TOURNAMENT",
	"SINGLE PLAYER",
	"TEAM FFA",
	"SAGA",
	"CTF",
	"CTY",
	"PRIVATE DUEL",
};

// Options for jk_mapFixes cvar
static jkmod_bit_info_t toggleMapFixes[] =
{
	"SP MAPS VOTING",
	"FX RUNNER DAMAGE",
	"FX RUNNER ANGLES",
	"TURRET CHAIR MODEL",
	"TARG. SPEAKER SOUND",
	"TARG. SPEAKER FLAGS",
	"SP DEFAULT MUSIC",
	"NO LOOPING SOUND",
	"FUNC STATIC SPAWN",
	"NO BAD BRUSH MODEL",
	"NO FUNC USABLE REMOVE",
	"NO PRINT BAD SPAWN",
	"SP ELEVATOR RETURN",
	"SP DOOR USE BUTTON",
	"SP DOOR TRIGGER SPAWN",
	"SPECTATOR DOOR STUCK",
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
			"^3jk_playerTweaks\n"
			"^3jk_voteControl\n"
			"^3jk_altDimension\n"
			"^3jk_altDimensionBase\n"
			"^3jk_altDimensionSpawn\n"
			"^3jk_altDimensionNoScore\n"
			"^3jk_spawnActivateSaber\n"
			"^3jk_mapFixes\n"
			"^3bot_forGimmick\n"
			);
	}
	else
	{
		int bits;
		int i, val, exclude[MAX_ITEMS] = { 0 };
		int toggleModSize = 0;
		char *toggleModNote = { 0 };
		qboolean toggleModRadio = qfalse;
		jkmod_bit_info_t *toggleModOptions;

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		if (!Q_stricmp(arg1, "jk_playerTweaks")) 
		{
			toggleModOptions = togglePlayerTweaks;
			toggleModSize = ARRAY_LEN(togglePlayerTweaks);
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
			exclude[0] = qtrue;
			exclude[1] = qtrue;
			exclude[8] = qtrue;
		}
		else if (!Q_stricmp(arg1, "jk_altDimensionSpawn")) 
		{
			toggleModOptions = toggleAltDimension;
			toggleModSize = ARRAY_LEN(toggleAltDimension);
		}
		else if (!Q_stricmp(arg1, "jk_altDimensionNoScore")) 
		{
			toggleModOptions = toggleAltDimension;
			toggleModSize = ARRAY_LEN(toggleAltDimension);
		}
		else if (!Q_stricmp(arg1, "jk_spawnActivateSaber")) 
		{
			toggleModOptions = toggleSpawnActivateSaber;
			toggleModSize = ARRAY_LEN(toggleSpawnActivateSaber);
		}
		else if (!Q_stricmp(arg1, "jk_mapFixes"))
		{
			toggleModOptions = toggleMapFixes;
			toggleModSize = ARRAY_LEN(toggleMapFixes);
			toggleModNote = "Map restart is required to apply changes";
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
			int first = -1;
			for (i = 0; i < toggleModSize; i++) 
			{
				if (exclude[i]) continue;
				if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
				else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
				if (first == -1) first = i;
			}
			G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, 0, toggleModOptions[first].string);
			if (toggleModNote) G_Printf("Note: %s\n", toggleModNote);
			G_Printf("Cvar value: %s %i\n", arg1, bits);
			return;
		}

		if (JKMod_ValidNumber(arg2)) 
		{
			val = atoi(arg2);

			if (val >= 0 && val < toggleModSize && !exclude[val]) 
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
				int first = -1;
				G_Printf("^1Specified an option that does not exist\n");
				for (i = 0; i < toggleModSize; i++) 
				{
					if (exclude[i]) continue;
					if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
					else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
					if (first == -1) first = i;
				}
				G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, 0, toggleModOptions[first].string);
				if (toggleModNote) G_Printf("Note: %s\n", toggleModNote);
			}
		}
		else 
		{
			int first = -1;
			G_Printf("^1Specified an option is not valid\n");
			for (i = 0; i < toggleModSize; i++ ) 
			{
				if (exclude[i]) continue;
				if (bits & (1 << i))	G_Printf("%2d ^2[X]^7 %s\n", i, toggleModOptions[i].string);
				else					G_Printf("%2d ^1[ ]^7 %s\n", i, toggleModOptions[i].string);
				if (first == -1) first = i;
			}
			G_Printf("Example: ^3/togglemod %s %i^7 (Toggles: ^5%s^7)\n", arg1, 0, toggleModOptions[first].string);
			if (toggleModNote) G_Printf("Note: %s\n", toggleModNote);
		}
		G_Printf("Cvar value: %s %i\n", arg1, bits);
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
		int			i;
		unsigned	dimension;
		char*		dimensionName;
		qboolean	found = qfalse;

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
				dimensionName = JKModDimensionData[i].name;
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

					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\"", dimensionName));
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
				
				trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\"", dimensionName));
				JKMod_DimensionSet(ent, dimension);
			}
		}
	}
}

/*
=====================================================================
Force dimension function
=====================================================================
*/
static void JKMod_svCmd_teleportPlayer(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];
	int		target, destiny;

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: teleport <targetplayer> <destinyplayer|coordinates>\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		target = JKMod_CheckValidClient(NULL, arg1);

		if (target != -1)
		{
			vec3_t temporigin, tempangles, tempfwd;
			gentity_t *targetPlayer = &g_entities[target];
			destiny = JKMod_CheckValidClient(NULL, arg2);

			// To player
			if (destiny != -1)
			{
				gentity_t *destinyPlayer = &g_entities[destiny];
				vec3_t mins = { -15, -15, DEFAULT_MINS_2 }, maxs = { 15, 15, DEFAULT_MAXS_2 };

				if (target == destiny)
				{
					G_Printf("You can't teleport him to himself\n");
					return;
				}

				if (!JKMod_CheckSolid(destinyPlayer, 50, mins, maxs, qfalse))
				{
					G_Printf("You can't teleport in this place\n");
					return;
				}

				tempangles[ROLL] = 0;
				tempangles[PITCH] = 0;
				tempangles[YAW] = AngleNormalize360(destinyPlayer->client->ps.viewangles[YAW] + 180);
			
				VectorCopy(destinyPlayer->client->ps.origin, temporigin);
				AngleVectors(destinyPlayer->client->ps.viewangles, tempfwd, NULL, NULL);
				tempfwd[2] = 0;
				VectorNormalize(tempfwd);
				VectorMA(destinyPlayer->client->ps.origin, 50, tempfwd, temporigin);
				temporigin[2] = destinyPlayer->client->ps.origin[2] + 5;

				destinyPlayer->client->ps.forceHandExtend = HANDEXTEND_FORCEPULL;
				destinyPlayer->client->ps.forceHandExtendTime = level.time + 300;

				if (targetPlayer->jkmodEnt.dimensionNumber != destinyPlayer->jkmodEnt.dimensionNumber)
				{
					int destinyDimension = destinyPlayer->client->ps.stats[JK_DIMENSION] == DIMENSION_DUEL ? DIMENSION_FREE : destinyPlayer->client->ps.stats[JK_DIMENSION];
					
					if (targetPlayer->client->ps.duelInProgress)
					{
						gentity_t *duelAgainst = &g_entities[targetPlayer->client->ps.duelIndex];

						JKMod_DuelRemove(targetPlayer);
						JKMod_DuelRemove(duelAgainst);

						if (duelAgainst->client->ps.stats[JK_DIMENSION] == DIMENSION_FREE && jkcvar_duelPassThrough.integer && JKMod_OthersInBox(duelAgainst)) JKMod_AntiStuckBox(duelAgainst);
						if (duelAgainst->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE) JKMod_DimensionSet(duelAgainst, DIMENSION_FREE);
					}

					if (destinyPlayer->client->ps.stats[JK_DIMENSION] == DIMENSION_PRIVATE) {
						JKMod_JoinPrivate(targetPlayer, destinyPlayer->client->pers.jkmodPers.privateRoom[PRIVATE_NUM], destinyPlayer->jkmodEnt.dimensionNumber);
					} else {
						JKMod_DimensionSet(targetPlayer, destinyDimension);
					}
				}
			}
			// To coords
			else
			{
				char	arg3[MAX_STRING_CHARS];
				char	arg4[MAX_STRING_CHARS];
				char	arg5[MAX_STRING_CHARS];

				trap_Argv(3, arg3, sizeof(arg3));
				trap_Argv(4, arg4, sizeof(arg4));
				trap_Argv(5, arg5, sizeof(arg5));

				G_Printf("Checking coordinates...\n");

				temporigin[0] = atoi(arg2);
				temporigin[1] = atoi(arg3);
				temporigin[2] = atoi(arg4);
				tempangles[YAW] = atoi(arg5);

				if (temporigin[0] && temporigin[1] && temporigin[2]) 
				{
					tempangles[ROLL] = 0;
					tempangles[PITCH] = 0;
				}
				else
				{
					G_Printf("Invalid coordinates!\n");
					return;
				}
			}
			
			JKMod_TeleportPlayer(targetPlayer, temporigin, tempangles, qfalse, 0, "default", "default");
			G_Printf("Player teleported!\n");
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
Lock team function
=====================================================================
*/
static void JKMod_svCmd_lockTeam(void)
{
	team_t	team = TEAM_NUM_TEAMS;
	char	arg1[MAX_STRING_CHARS];

	if (trap_Argc() < 2)
	{
		G_Printf("Usage: lockteam <team>\nNotes: Use the command again to unlock\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));

		if (!Q_stricmp(arg1, "spectator") || !Q_stricmp(arg1, "s")) {
			team = TEAM_SPECTATOR;
		}
		else if (!Q_stricmp(arg1, "free") || !Q_stricmp(arg1, "f")) {
			team = TEAM_FREE;
		}
		else if (!Q_stricmp(arg1, "red") || !Q_stricmp(arg1, "r")) {
			team = TEAM_RED;
		}
		else if (!Q_stricmp(arg1, "blue") || !Q_stricmp(arg1, "b")) {
			team = TEAM_BLUE;
		}
		
		if (team != TEAM_NUM_TEAMS) {
			int i;
			gentity_t *ent;
			qboolean check = level.jkmodLocals.lockedTeam[team];

			level.jkmodLocals.lockedTeam[team] = check ? qfalse : qtrue;
			trap_SendServerCommand(-1, va("print \"Server: %s team %s\n\"", (check ? "Unlocked" : "Locked"), JKMod_TeamName(team, CASE_UPPER)));

			for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
			{
				if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
				{
					trap_SendServerCommand(-1, va("cp \"Team %s%s%s has been %s\"", TeamColorString(team), JKMod_TeamName(team, CASE_NORMAL), S_COLOR_WHITE, (check ? "unlocked" : "locked")));
				}
			}
		} else {
			G_Printf("Invalid team!\n");
		}
	}
}

/*
=====================================================================
Force team function
=====================================================================
*/
void JKMod_svCmd_forceTeam(void)
{
	int		i;
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: forceteam <player|number|all> <team>\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		if (!Q_stricmp(arg1, "all"))
		{
			gentity_t* ent;

			for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
			{
				if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
				{
					SetTeam(ent, arg2);
				}
			}
		}
		else
		{
			int target = JKMod_CheckValidClient(NULL, arg1);

			if (target != -1)
			{
				gentity_t* ent = &g_entities[target];

				SetTeam(ent, arg2);
			}
		}
	}
}

/*
=====================================================================
Control bots function
=====================================================================
*/
static void JKMod_svCmd_controlBots(void)
{
	char	arg1[MAX_STRING_CHARS];
	char	arg2[MAX_STRING_CHARS];
	int		bot, owner;

	if (trap_Argc() < 3)
	{
		G_Printf("Usage: control <bot> <owner>\n");
	}
	else
	{
		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		bot = JKMod_CheckValidClient(NULL, arg1);

		// Check bot
		if (bot != -1)
		{
			gentity_t *botPlayer = &g_entities[bot];
			owner = JKMod_CheckValidClient(NULL, arg2);

			// Check owner
			if (owner != -1)
			{
				gentity_t *ownerPlayer = &g_entities[owner];

				if (bot == owner)
				{
					G_Printf("You can't let control him to himself\n");
					return;
				}

				if (!(botPlayer->r.svFlags & SVF_BOT)) {
					G_Printf("This player is not a bot\n");
					return;
				}

				if ((ownerPlayer->r.svFlags & SVF_BOT)) {
					G_Printf("This owner is not a player\n");
					return;
				}

				if (!(ownerPlayer->client->sess.spectatorState == SPECTATOR_FOLLOW && ownerPlayer->client->sess.spectatorClient == botPlayer->s.number)) {
					trap_SendServerCommand(ownerPlayer - g_entities, va("print \"Please follow bot %i (%s) on spectator to take control of it\n\"", botPlayer->s.number, botPlayer->client->pers.netname));
					return;
				}
				
				JKMod_botControl(botPlayer->s.number, ownerPlayer->s.number, "toggle");
			}
		}
	}
}

/*
=====================================================================
Reset players scores function
=====================================================================
*/
void JKMod_svCmd_resetScores_f(void)
{
	int i;
	gentity_t *ent;
	
	for (i=0 ; i < level.numConnectedClients ; i++) 
	{
		ent = &g_entities[level.sortedClients[i]];

		if (ent->inuse && ent->client) 
		{
			ent->client->ps.persistant[PERS_SCORE] = 0;
			ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT] = 0;
			ent->client->ps.persistant[PERS_EXCELLENT_COUNT] = 0;
			ent->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT] = 0;
			ent->client->ps.persistant[PERS_DEFEND_COUNT] = 0;
			ent->client->ps.persistant[PERS_ASSIST_COUNT] = 0;
			ent->client->ps.persistant[PERS_CAPTURES] = 0;
			ent->client->ps.persistant[PERS_KILLED] = 0;
			ent->client->ps.persistant[PERS_HITS] = 0;
		}
	}

	level.teamScores[TEAM_RED] = 0;
	level.teamScores[TEAM_BLUE] = 0;
	CalculateRanks();
	trap_SendServerCommand(-1, "print \"Scores have been reset\n\"");
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
	if (!Q_stricmp(cmd, "teleport"))
	{
		JKMod_svCmd_teleportPlayer();
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
	if (!Q_stricmp(cmd, "lockteam"))
	{
		JKMod_svCmd_lockTeam();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "control"))
	{
		JKMod_svCmd_controlBots();
		return qtrue;
	}
	if (!Q_stricmp(cmd, "resetscores"))
	{
		JKMod_svCmd_resetScores_f();
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
