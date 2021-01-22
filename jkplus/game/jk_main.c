/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main game module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Cvar table list and variables
=====================================================================
*/

int pauseGameStartTime = 0;

typedef struct { // Cvar table struct

	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;
	qboolean	trackChange;

} cvarTable_t;

vmCvar_t	jkcvar_test1;
vmCvar_t	jkcvar_test2;

vmCvar_t	jkcvar_serverMotd;
vmCvar_t	jkcvar_serverMotdTime;
vmCvar_t	jkcvar_serverIdle;
vmCvar_t	jkcvar_serverIdleFile;
vmCvar_t	jkcvar_serverClosed;
vmCvar_t	jkcvar_serverClosedText;

vmCvar_t	jkcvar_allowBlackNames;
vmCvar_t	jkcvar_allowMultiDuel;
vmCvar_t	jkcvar_allowCustomDuel;
vmCvar_t	jkcvar_allowDuelChat;
vmCvar_t	jkcvar_noDuplicatedNames;
vmCvar_t	jkcvar_chatProtect;
vmCvar_t	jkcvar_chatProtectTime;
vmCvar_t	jkcvar_playerIgnore;
vmCvar_t	jkcvar_teleportChat;
vmCvar_t	jkcvar_teleportFrag;

vmCvar_t	jkcvar_voteCustomMap;
vmCvar_t	jkcvar_voteControl;
vmCvar_t	jkcvar_voteResults;
vmCvar_t	jkcvar_voteWaitTime;
vmCvar_t	jkcvar_voteMinPlayers;
vmCvar_t	jkcvar_itemForcePhysics;
vmCvar_t	jkcvar_duelStartHealth;
vmCvar_t	jkcvar_duelStartArmor;
vmCvar_t	jkcvar_duelEndStats;

vmCvar_t	jkcvar_dropFlag;
vmCvar_t	jkcvar_dropFlagTime;
vmCvar_t	jkcvar_damagePlums;
vmCvar_t	jkcvar_pauseGame;
vmCvar_t	jkcvar_customHats;

vmCvar_t	jkcvar_emotesEnabled;
vmCvar_t	jkcvar_emotesBreak;
vmCvar_t	jkcvar_emotesFreeze;
vmCvar_t	jkcvar_emotesPunchDamage;

vmCvar_t	jkcvar_gamePlay;
vmCvar_t	jkcvar_altDimensions;
vmCvar_t	jkcvar_randomBegin;
vmCvar_t	jkcvar_serverNews;
vmCvar_t	jkcvar_serverNewsTime;

vmCvar_t	jkcvar_forcePlugin;
vmCvar_t	jkcvar_macroScan;
vmCvar_t	jkcvar_antiWarp;
vmCvar_t	jkcvar_antiWarpTime;
vmCvar_t	jkcvar_mapFixes;
vmCvar_t	jkcvar_useAnim;

vmCvar_t	jkcvar_jetPack;
vmCvar_t	jkcvar_jetPackPhysics;

static cvarTable_t	JKModCvarTable[] = 
{
	{ &jkcvar_serverMotd,			"jk_serverMotd",			"0",					CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_serverMotdTime,		"jk_serverMotdTime",		"5",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverIdle,			"jk_serverIdle",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverIdleFile,		"jk_serverIdleFile",		"jkmod_server.cfg",		CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverClosed,			"jk_serverClosed",			"0",					CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_serverClosedText,		"jk_serverClosedText",		"Closed.",				CVAR_ARCHIVE,						0, qfalse },

	{ &jkcvar_allowBlackNames,		"jk_allowBlackNames",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowMultiDuel,		"jk_allowMultiDuel",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowCustomDuel,		"jk_allowCustomDuel",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowDuelChat,		"jk_allowDuelChat",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_noDuplicatedNames,	"jk_noDuplicatedNames",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatProtect,			"jk_chatProtect",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatProtectTime,		"jk_chatProtectTime",		"10",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_playerIgnore,			"jk_playerIgnore",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportChat,			"jk_teleportChat",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportFrag,			"jk_teleportFrag",			"1",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_voteCustomMap,		"jk_voteCustomMap",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteControl,			"jk_voteControl",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteResults,			"jk_voteResults",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteWaitTime,			"jk_voteWaitTime",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteMinPlayers,		"jk_voteMinPlayers",		"2",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_itemForcePhysics,		"jk_itemForcePhysics",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelStartHealth,		"jk_duelStartHealth",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelStartArmor,		"jk_duelStartArmor",		"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelEndStats,			"jk_duelEndStats",			"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_dropFlag,				"jk_dropFlag",				"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_dropFlagTime,			"jk_dropFlagTime",			"15",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_damagePlums,			"jk_damagePlums",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_pauseGame,			"jk_pauseGame",				"0",					CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_customHats,			"jk_customHats",			"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },

	{ &jkcvar_emotesEnabled,		"jk_emotesEnabled",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesBreak,			"jk_emotesBreak",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesFreeze,			"jk_emotesFreeze",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesPunchDamage,	"jk_emotesPunchDamage",		"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_gamePlay,				"jk_gamePlay",				"0",					CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_altDimensions,		"jk_altDimensions",			"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_randomBegin,			"jk_randomBegin",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNews,			"jk_serverNews",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNewsTime,		"jk_serverNewsTime",		"60",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_forcePlugin,			"jk_forcePlugin",			"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_macroScan,			"jk_macroScan",				"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_antiWarp,				"jk_antiWarp",				"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_antiWarpTime,			"jk_antiWarpTime",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_mapFixes,				"jk_mapFixes",				"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_useAnim,				"jk_useAnim",				"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_jetPack,				"jk_jetPack",				"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_jetPackPhysics,		"jk_jetPackPhysics",		"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_test1,				"jk_test1",					"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_test2,				"jk_test2",					"0",					CVAR_ARCHIVE,						0, qtrue },
};

static int JKModCvarTableSize = sizeof(JKModCvarTable) / sizeof(JKModCvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

void JKMod_G_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for(i = 0, cv = JKModCvarTable; i < JKModCvarTableSize; i++, cv++)
	{
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);

		if(cv->vmCvar)
		{
			cv->modificationCount = cv->vmCvar->modificationCount;
		}
	}

	// Launch original register cvars function
	BaseJK2_G_RegisterCvars();
}

void JKMod_G_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for(i = 0, cv = JKModCvarTable; i < JKModCvarTableSize; i++, cv++)
	{
		if(cv->vmCvar)
		{
			trap_Cvar_Update(cv->vmCvar);

			if(cv->modificationCount != cv->vmCvar->modificationCount)
			{
				cv->modificationCount = cv->vmCvar->modificationCount;

				if(cv->trackChange)
				{
					// Server news and random begin warning
					if ((cv->vmCvar == &jkcvar_randomBegin || cv->vmCvar == &jkcvar_serverNews))
					{
						if (!(Q_stricmp(cv->vmCvar->string, "0") == 0 || Q_stricmp(cv->vmCvar->string, "paused") == 0)) {
							G_Printf("%s will load the files upon restarting.\n", cv->cvarName);
						}
					}
					// Normal tracking
					else 
					{
						trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string));
					}
				}

				// Plugin check
				if (cv->vmCvar == &jkcvar_forcePlugin)
				{
					int check = cv->vmCvar->integer ? 1 : 0;
					int num;

					if (check)
					{
						gentity_t *ent;

						for (num = 0, ent = g_entities; num < MAX_CLIENTS; ++num, ++ent)
						{
							if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
							{
								if (!ent->client->pers.jkmodPers.ClientPlugin)
								{
									ClientBegin(ent->s.number, qfalse);
								}
							}
						}
					}
				}

				// Pause game
				if (cv->vmCvar == &jkcvar_pauseGame) 
				{
					int pause = cv->vmCvar->integer ? 1 : 0;
					int num;

					if (pause)
					{
						gentity_t *ent;
						pauseGameStartTime = level.time;

						trap_SendServerCommand(-1, "cp \"Game paused by the server\n\"");
						trap_SendServerCommand(-1, "print \"Game paused by the server\n\"");

						// Save player viewangles
						for (num = 0, ent = g_entities; num < MAX_CLIENTS; ++num, ++ent) 
						{
							if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED) 
							{
								VectorCopy(ent->client->ps.viewangles, ent->client->jkmodClient.PauseSavedView);
							}
						}

						// Stop server news
						if (Q_stricmp(jkcvar_serverNews.string, "1") == 0) trap_Cvar_Set("jk_serverNews", "paused"); // Fix me
					}
					else 
					{
						// Set unpause
						if (pauseGameStartTime > 0 && pauseGameStartTime < level.time) 
						{
							gentity_t *ent;
							const int pausedGameTime = level.time - pauseGameStartTime;

							// Postpone think functions that were blocked during pause
							for (num = MAX_CLIENTS; num < MAX_GENTITIES; ++num)	
							{
								ent = &g_entities[num];

								if (ent && ent->inuse && ent->think && ent->nextthink > 0)
								{
									ent->nextthink += pausedGameTime;
								}
							}

							trap_SendServerCommand(-1, va("cp \"Game unpaused after %s\n\"", JKMod_msToString(pausedGameTime, qtrue)));
							trap_SendServerCommand(-1, va("print \"Game unpaused after %s\n\"", JKMod_msToString(pausedGameTime, qtrue)));

							level.startTime += pausedGameTime;

							// Roll back the time for cg_drawtimer
							trap_SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));

							pauseGameStartTime = 0;

							// Fix so times are correct on scoreboard
							for (num = 0, ent = g_entities; num < MAX_CLIENTS; ++num, ++ent) 
							{
								if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED) 
								{
									ent->client->pers.enterTime += pausedGameTime;

									// If someone joined during pause ensure they don't get negative time
									if (ent->client->pers.enterTime > level.time) ent->client->pers.enterTime = level.time;
									if (ent->client->pers.teamState.flagsince) 
									{
										// If holding a flag update the timer so its not counting pause time
										ent->client->pers.teamState.flagsince += pausedGameTime;
										if (ent->client->pers.teamState.flagsince > level.time) ent->client->pers.teamState.flagsince = level.time;
									}
									if (ent->client->pers.teamState.lastreturnedflag) ent->client->pers.teamState.lastreturnedflag += pausedGameTime;
									if (ent->client->pers.teamState.lastfraggedcarrier) ent->client->pers.teamState.lastfraggedcarrier += pausedGameTime;
									if (ent->client->pers.teamState.lasthurtcarrier) ent->client->pers.teamState.lasthurtcarrier += pausedGameTime;
									// Restore this player viewangles
									if (ent->client->sess.sessionTeam != TEAM_SPECTATOR) SetClientViewAngle(ent, ent->client->jkmodClient.PauseSavedView); 
								}
							}

							// Reload server news
							if (Q_stricmp(jkcvar_serverNews.string, "paused") == 0) trap_SendConsoleCommand(EXEC_APPEND, "jk_serverNews 1\n"); // Fix me
						}
					}
				}

				// Ingame gameplay
				if (cv->vmCvar == &jkcvar_gamePlay)
				{
					char *gameplay = cv->vmCvar->string;

					if (strcmp(gameplay, "0") != 0)
					{
						JKMod_gamePlay(gameplay);
					}
				}
			}
		}
	}

	// Launch original update cvars function
	BaseJK2_G_UpdateCvars();
}

/*
=====================================================================
Random begin initialization
=====================================================================
*/
void JKMod_randomBeginInit(void) 
{
	static char		*linestart;
	static char		*lineend;
	static int		count;

	level.jkmodLevel.RandomBeginCount = 0;
	linestart = JKMod_ReadFile("config/random_begin.cfg");

	if (linestart)
	{
		lineend = strchr(linestart, '\n');
		while (lineend)
		{
			*lineend = 0;
			Q_strncpyz(level.jkmodLevel.RandomBegin[count++], linestart, sizeof(level.jkmodLevel.RandomBegin[0]));
			level.jkmodLevel.RandomBeginCount++;
			linestart = lineend + 1;
			lineend = strchr(linestart, '\n');
			if (count >= MAX_LINES) break;
		}
		if (count < MAX_LINES) {
			Q_strncpyz(level.jkmodLevel.RandomBegin[count++], linestart, sizeof(level.jkmodLevel.RandomBegin[0]));
			level.jkmodLevel.RandomBeginCount++;
		}
		G_Printf("%i random begin messages loaded\n", level.jkmodLevel.RandomBeginCount);
	}
	else
	{
		trap_Cvar_Set("jk_randomBegin", "0");
	}
}

/*
=====================================================================
Server news initialization
=====================================================================
*/
void JKMod_serverNewsInit(void)
{
	static char		*linestart;
	static char		*lineend;
	static int		count;

	level.jkmodLevel.ServerNewsCount = 0;
	linestart = JKMod_ReadFile("config/server_news.cfg");

	if (linestart)
	{
		lineend = strchr(linestart, '\n');
		while (lineend)
		{
			*lineend = 0;
			Q_strncpyz(level.jkmodLevel.ServerNews[count++], linestart, sizeof(level.jkmodLevel.ServerNews[0]));
			level.jkmodLevel.ServerNewsCount++;
			linestart = lineend + 1;
			lineend = strchr(linestart, '\n');
			if (count >= MAX_LINES) break;
		}
		if (count < MAX_LINES) {
			Q_strncpyz(level.jkmodLevel.ServerNews[count++], linestart, sizeof(level.jkmodLevel.ServerNews[0]));
			level.jkmodLevel.ServerNewsCount++;
		}
		G_Printf("%i server news loaded\n", level.jkmodLevel.ServerNewsCount);
	}
	else
	{
		trap_Cvar_Set("jk_serverNews", "0");
	}
}

/*
=====================================================================
Teleport chats initialization
=====================================================================
*/
void JKMod_teleportChatInit(void)
{
	static char		*linestart;

	level.jkmodLevel.TeleportChatsCount = 0;
	linestart = JKMod_ReadFile("config/teleport_chats.cfg");

	if (linestart)
	{
		level.jkmodLevel.TeleportChatsCount += G_ParseInfos(linestart, MAX_TOKEN_CHARS - level.jkmodLevel.TeleportChatsCount, &level.jkmodLevel.TeleportChats[level.jkmodLevel.TeleportChatsCount]);
		G_Printf("%i teleport chats loaded\n", level.jkmodLevel.TeleportChatsCount);
	}
	else
	{
		trap_Cvar_Set("jk_teleportChat", "0");
	}
}

/*
=====================================================================
Server idle check
=====================================================================
*/
void JKMod_serverIdleCheck(void)
{
	if (level.numVotingClients == 0 && jkcvar_serverIdle.integer) 
	{
		if (level.jkmodLevel.ServerIdleTime > 0)
		{
			if (level.jkmodLevel.ServerIdleTime + jkcvar_serverIdle.integer*60000 < level.time + 15000)
			{
				trap_SendServerCommand(-1, "print \"Server idle, changing to defaults in 15 seconds...\n\"");
				level.jkmodLevel.ServerIdleTime =- level.jkmodLevel.ServerIdleTime;
			}
		}
		else 
		{
			if (-level.jkmodLevel.ServerIdleTime + jkcvar_serverIdle.integer*60000 < level.time)
			{
				trap_SendConsoleCommand(EXEC_APPEND, va("exec %s\n", jkcvar_serverIdleFile.string));
				level.jkmodLevel.ServerIdleTime = level.time;
			}
		}
	}
	else 
	{
		if (level.jkmodLevel.ServerIdleTime < 0) trap_SendServerCommand(-1, "print \"Server idle reset aborted!\n\"");
		level.jkmodLevel.ServerIdleTime = level.time;
	}
}

/*
=====================================================================
Main initialization functions
=====================================================================
*/
void JKMod_G_InitGame(int levelTime, int randomSeed, int restart) {
	int					i;

	G_Printf(S_COLOR_CYAN "------ JKMod Initialization ------\n");

	// Launch original init game function
	BaseJK2_G_InitGame(levelTime, randomSeed, restart);

	// Check gameplay
	if (strcmp(jkcvar_gamePlay.string, "0") != 0)
	{
		JKMod_gamePlay(jkcvar_gamePlay.string);
	}

	// Set random begin message
	if (jkcvar_randomBegin.integer && g_gametype.integer != GT_TOURNAMENT)
	{
		JKMod_randomBeginInit();
	}

	// Set server news
	if (jkcvar_serverNews.integer && g_gametype.integer != GT_TOURNAMENT)
	{
		JKMod_serverNewsInit();
	}

	// Set teleport chats
	if (jkcvar_teleportChat.integer)
	{
		JKMod_teleportChatInit();

		// Reset to normal teleport chat
		if (jkcvar_teleportChat.integer == 2) 
		{
			trap_Cvar_Set("jk_teleportChat", "1");
		}
	}

	// Check server idle
	if (jkcvar_serverIdle.integer)
	{
		G_Printf("%i minutes set for server idle\n", jkcvar_serverIdle.integer);
	}
}
