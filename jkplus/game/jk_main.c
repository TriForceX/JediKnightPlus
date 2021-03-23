/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main game module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern void SpectatorClientEndFrame(gentity_t *ent);

/*
=====================================================================
Cvar table list and variables
=====================================================================
*/

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
vmCvar_t	jkcvar_customHats;

vmCvar_t	jkcvar_emotesEnabled;
vmCvar_t	jkcvar_emotesBreak;
vmCvar_t	jkcvar_emotesFreeze;
vmCvar_t	jkcvar_emotesPunchDamage;

vmCvar_t	jkcvar_gamePlay;
vmCvar_t	jkcvar_altDimension;
vmCvar_t	jkcvar_altDimensionTime;
vmCvar_t	jkcvar_randomBegin;
vmCvar_t	jkcvar_serverNews;
vmCvar_t	jkcvar_serverNewsTime;

vmCvar_t	jkcvar_forcePlugin;
vmCvar_t	jkcvar_macroScan;
vmCvar_t	jkcvar_antiWarp;
vmCvar_t	jkcvar_antiWarpTime;
vmCvar_t	jkcvar_mapFixes;
vmCvar_t	jkcvar_mapDefaultMusic;

vmCvar_t	jkcvar_jetPack;
vmCvar_t	jkcvar_playerMovement;

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
	{ &jkcvar_customHats,			"jk_customHats",			"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },

	{ &jkcvar_emotesEnabled,		"jk_emotesEnabled",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesBreak,			"jk_emotesBreak",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesFreeze,			"jk_emotesFreeze",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesPunchDamage,	"jk_emotesPunchDamage",		"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_gamePlay,				"jk_gamePlay",				"0",					CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_altDimension,			"jk_altDimension",			"0",					CVAR_ARCHIVE | CVAR_LATCH,			0, qfalse },
	{ &jkcvar_altDimensionTime,		"jk_altDimensionTime",		"10",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_randomBegin,			"jk_randomBegin",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNews,			"jk_serverNews",			"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNewsTime,		"jk_serverNewsTime",		"60",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_forcePlugin,			"jk_forcePlugin",			"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_macroScan,			"jk_macroScan",				"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_antiWarp,				"jk_antiWarp",				"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_antiWarpTime,			"jk_antiWarpTime",			"1",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_mapFixes,				"jk_mapFixes",				"0",					CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_mapDefaultMusic,		"jk_mapDefaultMusic",		"0",					CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_jetPack,				"jk_jetPack",				"0",					CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_playerMovement,		"jk_playerMovement",		"0",					CVAR_ARCHIVE,						0, qtrue },

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

				// Ingame gameplay
				if (cv->vmCvar == &jkcvar_gamePlay)
				{
					char *gameplay = cv->vmCvar->string;

					if (strcmp(gameplay, "0") != 0)
					{
						JKMod_gamePlay(gameplay);
					}
				}

				// Player movement
				if (cv->vmCvar == &jkcvar_playerMovement)
				{
					gentity_t *ent;
					int num;

					for (num = 0, ent = g_entities; num < MAX_CLIENTS; ++num, ++ent)
					{
						if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
						{
							JKMod_PlayerMovementCheck(ent);
						}
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
Check rewind time on check paused frame
=====================================================================
*/
void JKMod_PauseTimeRestore(int msec)
{
	int		i, j;

	#define ADJUST(x) if ((x) > 0) (x) += msec

	ADJUST(level.warmupTime);
	ADJUST(level.intermissiontime);
	ADJUST(level.intermissionQueued);
	ADJUST(level.exitTime);
	ADJUST(level.jkmodLevel.idleTime);

	if(level.startTime < level.time) level.startTime += msec;

	trap_SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));
	trap_SetConfigstring(CS_WARMUP, va("%i", level.warmupTime));

	for (i = 0; i < level.maxclients; i++) 
	{
		if (level.clients[i].pers.connected != CON_DISCONNECTED) 
		{
			gentity_t		*ent = &g_entities[i];
			gclient_t		*client = ent->client;
			playerState_t	*ps = &client->ps;

			G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/chars/mothma/misc/40MOM038"));

			ADJUST(ent->s.constantLight);
			ADJUST(ent->s.emplacedOwner);

			ADJUST(ps->weaponChargeTime);
			ADJUST(ps->weaponChargeSubtractTime);
			ADJUST(ps->externalEventTime);
			ADJUST(ps->painTime);
			ADJUST(ps->lastOnGround);
			ADJUST(ps->saberLockTime);
			ADJUST(ps->saberDidThrowTime);
			ADJUST(ps->saberHitWallSoundDebounceTime);
			ADJUST(ps->rocketLastValidTime);
			ADJUST(ps->rocketLockTime);
			ADJUST(ps->rocketTargetTime);
			ADJUST(ps->emplacedTime);
			ADJUST(ps->droneFireTime);
			ADJUST(ps->droneExistTime);
			ADJUST(ps->holocronCantTouchTime);
			ADJUST(ps->electrifyTime);
			ADJUST(ps->saberBlockTime);
			ADJUST(ps->otherKillerTime);
			ADJUST(ps->otherKillerDebounceTime);
			ADJUST(ps->forceHandExtendTime);
			ADJUST(ps->forceRageDrainTime);
			ADJUST(ps->forceGripMoveInterval);
			ADJUST(ps->footstepTime);
			ADJUST(ps->otherSoundTime);
			ADJUST(ps->duelTime);
			ADJUST(ps->holdMoveTime);
			ADJUST(ps->forceAllowDeactivateTime);
			ADJUST(ps->zoomTime);
			ADJUST(ps->zoomLockTime);
			ADJUST(ps->useDelay);
			ADJUST(ps->fallingToDeath);
			ADJUST(ps->saberIdleWound);
			ADJUST(ps->saberAttackWound);
			ADJUST(ps->saberThrowDelay);

			for (j = 0; j < MAX_POWERUPS; j++) {
				if (ps->powerups[j] < INT_MAX) {
					ADJUST(ps->powerups[j]);
				}
			}

			for (j = 0; j < NUM_FORCE_POWERS; j++) {
				ADJUST(ps->holocronsCarried[j]);
			}

			for (j = 0; j < NUM_FORCE_POWERS; j++) {
				ADJUST(ps->fd.forcePowerDebounce[j]);
			}

			for (j = 0; j < NUM_FORCE_POWERS; j++) {
				ADJUST(ps->fd.forcePowerDuration[j]);
			}

			ADJUST(ps->fd.forcePowerRegenDebounceTime);
			ADJUST(ps->fd.forceJumpAddTime);
			ADJUST(ps->fd.forceGripDamageDebounceTime);
			ADJUST(ps->fd.forceGripStarted);
			ADJUST(ps->fd.forceGripBeingGripped);
			ADJUST(ps->fd.forceGripUseTime);
			ADJUST(ps->fd.forceGripSoundTime);
			ADJUST(ps->fd.forceHealTime);
			ADJUST(ps->fd.forceRageRecoveryTime);
			ADJUST(ps->fd.forceDrainTime);

			ADJUST(client->invulnerableTimer);
			ADJUST(client->respawnTime);
			ADJUST(client->inactivityTime);
			ADJUST(client->rewardTime);
			ADJUST(client->airOutTime);
			ADJUST(client->lastKillTime);
			ADJUST(client->lastSaberStorageTime);
			ADJUST(client->dangerTime);
			ADJUST(client->forcePowerSoundDebounce);

			ADJUST(client->pers.enterTime);
			ADJUST(client->pers.teamState.lastfraggedcarrier);
			ADJUST(client->pers.teamState.lasthurtcarrier);
			ADJUST(client->pers.teamState.lastreturnedflag);
			ADJUST(client->pers.teamState.flagsince);
		}
	}

	for (i = 0; i < level.num_entities; i++) 
	{
		if (g_entities[i].inuse) {
			gentity_t	*ent = &g_entities[i];

			if (ent->s.pos.trType != TR_STATIONARY) {
				ent->s.pos.trTime += msec;
			}
			if (ent->s.apos.trType != TR_STATIONARY) {
				ent->s.apos.trTime += msec;
			}

			ADJUST(ent->nextthink);
			ADJUST(ent->aimDebounceTime);
			ADJUST(ent->painDebounceTime);
			ADJUST(ent->attackDebounceTime);
			ADJUST(ent->freetime);
			ADJUST(ent->eventTime);
			ADJUST(ent->timestamp);
			ADJUST(ent->setTime);
			ADJUST(ent->pain_debounce_time);
			ADJUST(ent->fly_sound_debounce_time);
			ADJUST(ent->last_move_time);
			ADJUST(ent->jkmodEnt.time1);
			ADJUST(ent->jkmodEnt.time2);
		}
	}
}

/*
=====================================================================
Check paused frame on main run frame
=====================================================================
*/
qboolean JKMod_PauseFrameCheck(int levelTime) 
{
	static int	pauseLast = 0;
	static int	pauseTime = 0;

	if (level.jkmodLevel.pauseTime > levelTime)
	{
		if (!pauseLast)
		{
			pauseLast = levelTime;
			level.jkmodLevel.idleTime = levelTime;
			
			if (level.jkmodLevel.pauseTimeCustom) {
				trap_SendServerCommand(-1, va("print \"Game paused for %i seconds\n\"", level.jkmodLevel.pauseTimeCustom));
				trap_SendServerCommand(-1, va("cp \"Pause for %i seconds\n\"", level.jkmodLevel.pauseTimeCustom));
			}
			else {
				trap_SendServerCommand(-1, "print \"Game paused by the server\n\"");
				trap_SendServerCommand(-1, "cp \"PAUSE\n\"");
			}
		}

		if (pauseTime != level.jkmodLevel.pauseTime)
		{
			pauseTime = level.jkmodLevel.pauseTime;
			trap_SetConfigstring(CS_PAUSE, va("%d", pauseTime));
		}

		return qtrue;
	}
	else if (pauseLast)
	{
		JKMod_PauseTimeRestore(levelTime - pauseLast);
		trap_SendServerCommand(-1, va("print \"Game unpaused after %s\n\"", JKMod_msToString((levelTime - pauseLast + 1000), qfalse)));
		trap_SendServerCommand(-1, va("cp \"%s\n\"", G_GetStripEdString("SVINGAME", "BEGIN_DUEL")));
		
		pauseLast = 0;
		pauseTime = 0;
		
		trap_SetConfigstring(CS_PAUSE, "");
	}

	return qfalse;
}

/*
=====================================================================
Run paused frame on main run frame
=====================================================================
*/
void JKMod_PauseFrameRun(void) 
{
	int	i;

	for (i = 0; i < level.maxclients; i++) 
	{
		gclient_t	*client = &level.clients[i];

		if (client->pers.connected == CON_CONNECTED && client->sess.spectatorState != SPECTATOR_NOT)
		{
			SpectatorClientEndFrame(&g_entities[i]);
		}
	}

	// Get any cvar changes
	G_UpdateCvars();

	// Cancel vote if timed out
	CheckVote();

	// Check team votes
	CheckTeamVote(TEAM_RED);
	CheckTeamVote(TEAM_BLUE);

	// Tracking changes
	CheckCvars();
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
		if (level.jkmodLevel.idleTime > 0)
		{
			if (level.jkmodLevel.idleTime + jkcvar_serverIdle.integer*60000 < level.time + 15000)
			{
				trap_SendServerCommand(-1, "print \"Server idle, changing to defaults in 15 seconds...\n\"");
				level.jkmodLevel.idleTime =- level.jkmodLevel.idleTime;
			}
		}
		else 
		{
			if (-level.jkmodLevel.idleTime + jkcvar_serverIdle.integer*60000 < level.time)
			{
				trap_SendConsoleCommand(EXEC_APPEND, va("exec %s\n", jkcvar_serverIdleFile.string));
				level.jkmodLevel.idleTime = level.time;
			}
		}
	}
	else 
	{
		if (level.jkmodLevel.idleTime < 0) trap_SendServerCommand(-1, "print \"Server idle reset aborted!\n\"");
		level.jkmodLevel.idleTime = level.time;
	}
}

/*
=====================================================================
Main initialization functions
=====================================================================
*/
void JKMod_G_InitGame(int levelTime, int randomSeed, int restart) 
{
	G_Printf(S_COLOR_CYAN "------ JKPlus Initialization ------\n");

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
		level.jkmodLevel.idleTime = levelTime;
		G_Printf("%i minutes set for server idle\n", jkcvar_serverIdle.integer);
	}
}
