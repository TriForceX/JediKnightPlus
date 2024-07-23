/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Main game module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern void SpectatorClientEndFrame(gentity_t *ent);
extern void CheckVote(void);
extern void CheckTeamVote(team_t team);
extern void CheckCvars(void);
extern int G_ParseInfos(char *buf, int max, char *infos[]);
extern int trap_RealTime(qtime_t *qtime);

/*
=====================================================================
Cvar table list and variables
=====================================================================
*/

vmCvar_t	jkcvar_serverMotd;
vmCvar_t	jkcvar_serverMotdTime;
vmCvar_t	jkcvar_serverMotdTimePrint;
vmCvar_t	jkcvar_serverMotdTele;
vmCvar_t	jkcvar_serverMotdOnce;
vmCvar_t	jkcvar_serverIdle;
vmCvar_t	jkcvar_serverIdleFile;
vmCvar_t	jkcvar_serverClosed;
vmCvar_t	jkcvar_serverClosedText;
vmCvar_t	jkcvar_serverConnectMsg;
vmCvar_t	jkcvar_serverJoinSound;

vmCvar_t	jkcvar_allowBlackNames;
vmCvar_t	jkcvar_allowSaberHolocrons;
vmCvar_t	jkcvar_allowTaunt2;
vmCvar_t	jkcvar_allowMultiDuel;
vmCvar_t	jkcvar_allowCustomDuel;
vmCvar_t	jkcvar_allowDuelChat;
vmCvar_t	jkcvar_allowDuelChat;

vmCvar_t	jkcvar_noDuplicatedNames;
vmCvar_t	jkcvar_chatProtect;
vmCvar_t	jkcvar_chatProtectTime;
vmCvar_t	jkcvar_chatAutoStatus;
vmCvar_t	jkcvar_chatAutoStatusTime;
vmCvar_t	jkcvar_chatColors;
vmCvar_t	jkcvar_playerIgnore;
vmCvar_t	jkcvar_spawnActivateSaber;
vmCvar_t	jkcvar_saberIdleDamage;
vmCvar_t	jkcvar_teleportChat;
vmCvar_t	jkcvar_teleportChatTime;
vmCvar_t	jkcvar_teleportChatFx;
vmCvar_t	jkcvar_teleportChatSound;
vmCvar_t	jkcvar_teleportFrag;

vmCvar_t	jkcvar_voteAnyMap;
vmCvar_t	jkcvar_voteControl;
vmCvar_t	jkcvar_voteResults;
vmCvar_t	jkcvar_voteWaitTime;
vmCvar_t	jkcvar_voteMinPlayers;
vmCvar_t	jkcvar_voteMaxPass;
vmCvar_t	jkcvar_itemForcePhysics;
vmCvar_t	jkcvar_duelStartHealth;
vmCvar_t	jkcvar_duelStartArmor;
vmCvar_t	jkcvar_duelStartEmote;
vmCvar_t	jkcvar_duelEndStats;
vmCvar_t	jkcvar_duelEndSlow;
vmCvar_t	jkcvar_duelEndSlowScale;
vmCvar_t	jkcvar_duelDistance;
vmCvar_t	jkcvar_duelPassThrough;
vmCvar_t	jkcvar_duelAutoAccept;

vmCvar_t	jkcvar_dropFlag;
vmCvar_t	jkcvar_dropFlagTime;
vmCvar_t	jkcvar_damagePlums;
vmCvar_t	jkcvar_customHats;

vmCvar_t	jkcvar_emotesEnabled;
vmCvar_t	jkcvar_emotesBreak;
vmCvar_t	jkcvar_emotesFreeze;
vmCvar_t	jkcvar_emotesPunchDamage;

vmCvar_t	jkcvar_gamePlay;
vmCvar_t	jkcvar_gameTypeConfig;
vmCvar_t	jkcvar_altDimension;
vmCvar_t	jkcvar_altDimensionBase;
vmCvar_t	jkcvar_altDimensionTime;
vmCvar_t	jkcvar_altDimensionSpawn;
vmCvar_t	jkcvar_altDimensionNoScore;
vmCvar_t	jkcvar_privateInviteTime;
vmCvar_t	jkcvar_privateInviteBots;
vmCvar_t	jkcvar_privateInactiveBots;
vmCvar_t	jkcvar_forceChangeInstant;
vmCvar_t	jkcvar_forceChangeTime;
vmCvar_t	jkcvar_randomBegin;
vmCvar_t	jkcvar_serverNews;
vmCvar_t	jkcvar_serverNewsTime;
vmCvar_t	jkcvar_serverNewsExtras;

vmCvar_t	jkcvar_pluginRequired;
vmCvar_t	jkcvar_macroScan;
vmCvar_t	jkcvar_macroScanAlert;
vmCvar_t	jkcvar_antiWarp;
vmCvar_t	jkcvar_antiWarpTime;
vmCvar_t	jkcvar_mapFixes;
vmCvar_t	jkcvar_mapDefaultMusic;
vmCvar_t	jkcvar_mapCycleFromFile;
vmCvar_t	jkcvar_mapName;

vmCvar_t	jkcvar_jetPack;
vmCvar_t	jkcvar_playerTweaks;
vmCvar_t	jkcvar_botsAI;
vmCvar_t	jkcvar_dualSaber;

vmCvar_t	jkcvar_test1;
vmCvar_t	jkcvar_test2;
vmCvar_t	developer;

static jkmod_cvar_table_t JKModCvarTable[] = 
{
	{ &jkcvar_serverMotd,			"jk_serverMotd",			"0",					NULL,						CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_serverMotdTime,		"jk_serverMotdTime",		"5",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverMotdTimePrint,	"jk_serverMotdTimePrint",	"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverMotdTele,		"jk_serverMotdTele",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverMotdOnce,		"jk_serverMotdOnce",		"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverIdle,			"jk_serverIdle",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverIdleFile,		"jk_serverIdleFile",		"jkmod_server.cfg",		NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverClosed,			"jk_serverClosed",			"0",					JKMod_CVU_serverClosed,		CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_serverClosedText,		"jk_serverClosedText",		"Closed.",				NULL,						CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_serverConnectMsg,		"jk_serverConnectMsg",		"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverJoinSound,		"jk_serverJoinSound",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_allowBlackNames,		"jk_allowBlackNames",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowSaberHolocrons,	"jk_allowSaberHolocrons",	"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowTaunt2,			"jk_allowTaunt2",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowMultiDuel,		"jk_allowMultiDuel",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowCustomDuel,		"jk_allowCustomDuel",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_allowDuelChat,		"jk_allowDuelChat",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_noDuplicatedNames,	"jk_noDuplicatedNames",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatProtect,			"jk_chatProtect",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatProtectTime,		"jk_chatProtectTime",		"10",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatAutoStatus,		"jk_chatAutoStatus",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatAutoStatusTime,	"jk_chatAutoStatusTime",	"3",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_chatColors,			"jk_chatColors",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_playerIgnore,			"jk_playerIgnore",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_spawnActivateSaber,	"jk_spawnActivateSaber",	"-1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_saberIdleDamage,		"jk_spawnActivateSaber",	"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportChat,			"jk_teleportChat",			"0",					JKMod_CVU_teleportChat,		CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportChatTime,		"jk_teleportChatTime",		"10",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportChatFx,		"jk_teleportChatFx",		"default",				NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportChatSound,	"jk_teleportChatSound",		"default",				NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_teleportFrag,			"jk_teleportFrag",			"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_voteAnyMap,			"jk_voteAnyMap",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteControl,			"jk_voteControl",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteResults,			"jk_voteResults",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteWaitTime,			"jk_voteWaitTime",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteMinPlayers,		"jk_voteMinPlayers",		"2",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_voteMaxPass,			"jk_voteMaxPass",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_itemForcePhysics,		"jk_itemForcePhysics",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelStartHealth,		"jk_duelStartHealth",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelStartArmor,		"jk_duelStartArmor",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelStartEmote,		"jk_duelStartEmote",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelEndStats,			"jk_duelEndStats",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelEndSlow,			"jk_duelEndSlow",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelEndSlowScale,		"jk_duelEndTimeScale",		"0.5",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_duelDistance,			"jk_duelDistance",			"1024",					NULL,						CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_duelPassThrough,		"jk_duelPassThrough",		"0",					NULL,						CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_duelAutoAccept,		"jk_duelAutoAccept",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_dropFlag,				"jk_dropFlag",				"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_dropFlagTime,			"jk_dropFlagTime",			"15",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_damagePlums,			"jk_damagePlums",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_customHats,			"jk_customHats",			"0",					NULL,						CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },

	{ &jkcvar_emotesEnabled,		"jk_emotesEnabled",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesBreak,			"jk_emotesBreak",			"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesFreeze,			"jk_emotesFreeze",			"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_emotesPunchDamage,	"jk_emotesPunchDamage",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_gamePlay,				"jk_gamePlay",				"0",					JKMod_CVU_gamePlay,			CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_gameTypeConfig,		"jk_gameTypeConfig",		"0",					NULL,						CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_altDimension,			"jk_altDimension",			"0",					JKMod_CVU_altDimension,		CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_altDimensionBase,		"jk_altDimensionBase",		"0",					JKMod_CVU_altDimension,		CVAR_ARCHIVE,						0, qfalse },
	{ &jkcvar_altDimensionTime,		"jk_altDimensionTime",		"10",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_altDimensionSpawn,	"jk_altDimensionSpawn",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_altDimensionNoScore,	"jk_altDimensionNoScore",	"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_privateInviteTime,	"jk_privateInviteTime",		"10",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_privateInviteBots,	"jk_privateInviteBots",		"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_privateInactiveBots,	"jk_privateInactiveBots",	"30",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_forceChangeInstant,	"jk_forceChangeInstant",	"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_forceChangeTime,		"jk_forceChangeTime",		"10",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_randomBegin,			"jk_randomBegin",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNews,			"jk_serverNews",			"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNewsTime,		"jk_serverNewsTime",		"60",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_serverNewsExtras,		"jk_serverNewsExtras",		"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_pluginRequired,		"jk_pluginRequired",		"0",					JKMod_CVU_pluginRequired,	CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_macroScan,			"jk_macroScan",				"0",					NULL,						CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_macroScanAlert,		"jk_macroScanAlert",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_antiWarp,				"jk_antiWarp",				"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_antiWarpTime,			"jk_antiWarpTime",			"1",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_mapFixes,				"jk_mapFixes",				"0",					NULL,						CVAR_ARCHIVE | CVAR_LATCH,			0, qtrue },
	{ &jkcvar_mapDefaultMusic,		"jk_mapDefaultMusic",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_mapCycleFromFile,		"jk_mapCycleFromFile",		"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },

	{ &jkcvar_jetPack,				"jk_jetPack",				"0",					JKMod_CVU_jetPack,			CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },
	{ &jkcvar_playerTweaks,			"jk_playerTweaks",			"0",					JKMod_CVU_playerTweaks,		CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_botsAI,				"jk_botsAI",				"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_dualSaber,			"jk_dualSaber",				"0",					JKMod_CVU_dualSaber,		CVAR_ARCHIVE | CVAR_SERVERINFO,		0, qtrue },

	{ &jkcvar_test1,				"jk_test1",					"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &jkcvar_test2,				"jk_test2",					"0",					NULL,						CVAR_ARCHIVE,						0, qtrue },
	{ &developer,					"developer",				"0",					NULL,						CVAR_NONE,							0, qfalse },
};

static int JKModCvarTableSize = ARRAY_LEN(JKModCvarTable);
static int JKModCvarAltDimension = 0;
static int JKModPauseDelay = 0;
static int JKModRemapShadersFileCount;
static char *JKModRemapShadersFile[MAX_TOKEN_CHARS];

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

// Register cvars
void JKMod_G_RegisterCvars(void)
{
	int					i;
	jkmod_cvar_table_t	*cv;
	qboolean			GTconfigLoaded = jkcvar_gameTypeConfig.integer ? level.jkmodLocals.cvarTempUnlock == 2 : qtrue;

	// Register current map name
	trap_Cvar_Register(&jkcvar_mapName, "mapname", "", CVAR_NONE);

	// Register all the cvars
	for (i = 0, cv = JKModCvarTable; i < JKModCvarTableSize; i++, cv++)
	{
		// Cvar latch temp unlock toggle
		if (cv->cvarFlags & (CVAR_LATCH | CVAR_TEMP) && jkcvar_gameTypeConfig.integer) 
		{
			cv->cvarFlags ^= CVAR_LATCH;
			cv->cvarFlags ^= CVAR_TEMP;
			JKMod_Printf("%s latched from jkmod %s\n", cv->cvarName, (cv->cvarFlags & CVAR_LATCH ? "^5locked" : "^3unlocked"));
		}

		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);

		if (cv->update && GTconfigLoaded) cv->update();

		if (cv->vmCvar)
		{
			cv->modificationCount = cv->vmCvar->modificationCount;
		}
	}

	// Disable dimensions if mvapi not available
	if (!mvapi && jkcvar_altDimension.integer != 0) 
	{
		trap_Cvar_Set("jk_altDimension", "0");
		trap_Cvar_Set("jk_altDimensionBase", "0");
		G_Printf(S_COLOR_YELLOW "WARNING: Dimensions has been disabled (MVAPI not available).\n");
	}

	// Launch original register cvars function
	BaseJK2_G_RegisterCvars();
}

// Update cvars
void JKMod_G_UpdateCvars(void)
{
	int					i;
	jkmod_cvar_table_t	*cv;
	qboolean			GTconfigLoaded = jkcvar_gameTypeConfig.integer ? level.jkmodLocals.cvarTempUnlock == 2 : qtrue;

	// Update all the cvars
	for (i = 0, cv = JKModCvarTable; i < JKModCvarTableSize; i++, cv++)
	{
		if (cv->vmCvar)
		{
			trap_Cvar_Update(cv->vmCvar);

			if(cv->modificationCount != cv->vmCvar->modificationCount)
			{
				cv->modificationCount = cv->vmCvar->modificationCount;

				if (cv->update && GTconfigLoaded) cv->update();

				if (cv->trackChange && !level.jkmodLocals.cvarToggleMod)
				{
					trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string));
				}
			}
		}
	}

	// Launch original update cvars function
	BaseJK2_G_UpdateCvars();

	// Reset toggle mod check
	if (level.jkmodLocals.cvarToggleMod) level.jkmodLocals.cvarToggleMod = qfalse;
}


// Update gameplay cvar
void JKMod_CVU_gamePlay(void)
{
	JKMod_SetGamePlay(jkcvar_gamePlay.string);
}

// Update teleport chat
void JKMod_CVU_teleportChat(void)
{
	// Save/load pos check
	if (jkcvar_teleportChat.integer != 2)
	{
		gentity_t *ent;
		int i;

		for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
		{
			if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED && ent->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE)
			{
				// Clear saved teleport
				ARRAY_CLEAR(ent->client->pers.jkmodPers.teleportChat);
				ent->client->pers.jkmodPers.teleportChatCheck = 0;
	
				// Clear custom spawn
				ARRAY_CLEAR(ent->client->pers.jkmodPers.customSpawn);
				ent->client->pers.jkmodPers.customSpawnCheck = 0;
			}
		}
	}
}

// Update alt dimension cvar
void JKMod_CVU_altDimension(void)
{
	JKModCvarAltDimension = !JKModCvarAltDimension ? 1 : 2;
	
	if (JKModCvarAltDimension > 1 && g_gametype.integer == GT_FFA) 
	{
		gentity_t *ent;
		int i;
		int dimension = jkcvar_altDimension.integer;
		int dimensionBase = jkcvar_altDimensionBase.integer;
		int dimensionWarning = jkcvar_altDimension.integer || !jkcvar_altDimension.integer && jkcvar_altDimensionBase.integer;

		JKModCvarAltDimension = 3;

		// Check dimensions
		if (dimensionBase == DIMENSION_DUEL || dimensionBase == DIMENSION_PRIVATE)
		{
			dimensionBase = DIMENSION_FREE;
			if (dimensionWarning) G_Printf(S_COLOR_YELLOW "WARNING: Generic dimensions are not meant to be used as base!\n");
		}
		if (dimension != -1 && !dimensionBase && !(dimension & DIMENSION_FREE)) 
		{
			for (i = 1; i < (1 << DIMENSION_MAX); i <<= 1)
			{
				if (i & DIMENSION_FREE) continue;
				if (i & DIMENSION_DUEL) continue;
				if (i & DIMENSION_PRIVATE) continue;
				if (dimension & i) { dimensionBase = i; break; }
			}
			if (dimension & (DIMENSION_DUEL | DIMENSION_PRIVATE) && !(dimension & DIMENSION_FREE)) dimension |= DIMENSION_FREE;
			if (dimensionWarning) G_Printf(S_COLOR_YELLOW "WARNING: Normal dimension not present! Adding a new one...\n", dimension, dimensionBase);
		}
		if (dimension != -1 && dimensionBase && !(dimension & dimensionBase))
		{
			dimension |= dimensionBase;
			if (dimensionWarning) G_Printf(S_COLOR_YELLOW "WARNING: Base dimension is not present! Adding it back...\n");
		}

		// Update dimensions
		if (jkcvar_altDimension.integer != dimension) trap_Cvar_Set("jk_altDimension", va("%i", dimension));
		if (jkcvar_altDimensionBase.integer != dimensionBase) trap_Cvar_Set("jk_altDimensionBase", va("%i", dimensionBase));

		level.jkmodLocals.dimensionBase = dimensionBase ? dimensionBase : DIMENSION_FREE;

		// Check clients
		for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
		{
			if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
			{
				if (!(dimension & DIMENSION_PRIVATE)) 
				{
					ARRAY_CLEAR(ent->client->pers.jkmodPers.privateRoom);
				}
				if (dimension && ent->client->ps.duelInProgress && !(dimension & DIMENSION_FREE)) 
				{
					JKMod_DuelRemove(ent);
				}
				if (dimension && !(ent->client->ps.stats[JK_DIMENSION] & dimension) || (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_DUEL && !(dimension & DIMENSION_FREE))) 
				{
					trap_SendServerCommand(ent - g_entities, "cp \"Server changed dimensions!\nYou are back to the default one\"");

					if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_DUEL && (dimension & DIMENSION_FREE)) {
						// Back to normal
						JKMod_DimensionSet(ent, DIMENSION_FREE);
					} else {
						// Back to base
						JKMod_DimensionSet(ent, level.jkmodLocals.dimensionBase);
					}
				}
			}
		}
	}
}

// Update jetpack cvar
void JKMod_CVU_jetPack(void)
{
	if (jkcvar_jetPack.integer == 0)
	{
		gentity_t *ent;
		int i;

		for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
		{
			if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
			{
				if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) 
				{
					ent->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
					ent->client->ps.stats[JK_FUEL] = 0;
					ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
				}
			}
		}
	}
}

// Update player tweaks cvar
void JKMod_CVU_playerTweaks(void)
{
	gentity_t *ent;
	int i;

	for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
	{
		if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
		{
			JKMod_PlayerTweaksCheck(ent);
		}
	}
}

// Update plugin required cvar
void JKMod_CVU_pluginRequired(void)
{
	if (jkcvar_pluginRequired.integer == 2)
	{
		gentity_t *ent;
		int i;

		for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
		{
			if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
			{
				if (!ent->client->pers.jkmodPers.clientPlugin) ClientBegin(ent->s.number, qfalse);
			}
		}
	}
}

// Update server closed cvar
void JKMod_CVU_serverClosed(void)
{
	if (VALIDCVAR(jkcvar_serverClosed.string)) {
		trap_Cvar_Set( "g_needpass", "1" );
	} else {
		trap_Cvar_Set( "g_needpass", "0" );
	}
}

// Update dual saber cvar
void JKMod_CVU_dualSaber(void)
{
	gentity_t *ent;
	int i;

	for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
	{
		if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED)
		{
			if (!jkcvar_dualSaber.integer && ent->client->pers.jkmodPers.dualSaber) 
			{
				ent->client->ps.dualBlade = qfalse;
				ent->client->pers.jkmodPers.dualSaber = qfalse;
			}
		}
	}
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
	ADJUST(level.voteTime);
	ADJUST(level.jkmodLocals.idleTime);
	ADJUST(level.jkmodLocals.serverNewsTime);

	level.startTime += msec;

	trap_SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));
	trap_SetConfigstring(CS_WARMUP, va("%i", level.warmupTime));
	trap_SetConfigstring(CS_VOTE_TIME, va("%i", level.voteTime));

	for (i = 0; i < level.maxclients; i++) 
	{
		if (level.clients[i].pers.connected != CON_DISCONNECTED) 
		{
			gentity_t		*ent = &g_entities[i];
			gclient_t		*client = ent->client;
			playerState_t	*ps = &client->ps;

			G_Sound(ent, CHAN_AUTO, G_SoundIndex("sound/chars/mothma/misc/40MOM038"));

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
			ADJUST(client->pers.jkmodPers.raceStartTime);
			ADJUST(client->pers.jkmodPers.emoteLastTime);
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
	static int	pauseSecs = 0;
	static int	i = 0;

	if (level.jkmodLocals.pauseTime > levelTime)
	{
		if (!pauseLast)
		{
			pauseLast = levelTime;
			level.jkmodLocals.idleTime = levelTime;

			for (i = 0; i < level.maxclients; i++) 
			{
				if (level.clients[i].pers.connected != CON_DISCONNECTED) 
				{
					gentity_t *ent = &g_entities[i];

					if (level.jkmodLocals.pauseTimeCustom) 
					{
						const char *timeCustom = JKMod_MsToWord((level.jkmodLocals.pauseTimeCustom * 1000), qfalse);
						trap_SendServerCommand(i, va("print \"Game paused for %s\n\"", timeCustom));
						if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(i, va("cp \"Pause for %s\"", timeCustom));
					}
					else 
					{
						trap_SendServerCommand(i, "print \"Game paused by the server\n\"");
						if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(i, "cp \"PAUSE\"");
					}
				}
			}
		}

		if (pauseTime != level.jkmodLocals.pauseTime)
		{
			pauseTime = level.jkmodLocals.pauseTime;
			trap_SetConfigstring(JK_CS_PAUSE, va("%d", pauseTime));
		}

		if (JKModPauseDelay <= levelTime)
		{
			pauseSecs = (level.jkmodLocals.pauseTime - levelTime) / 1000 + 1;

			if (pauseSecs <= 3) 
			{
				for (i = 0; i < level.maxclients; i++) 
				{
					if (level.clients[i].pers.connected != CON_DISCONNECTED) 
					{
						gentity_t *ent = &g_entities[i];

						switch (pauseSecs)
						{
							case 3: 
								G_Sound(ent, CHAN_AUTO, G_SoundIndex("sound/chars/mothma/misc/40MOM035"));
								if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(i, va("print \"Resume in %i seconds\n\"", pauseSecs));
								break;
							case 2: 
								G_Sound(ent, CHAN_AUTO, G_SoundIndex("sound/chars/mothma/misc/40MOM036"));
								if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(i, va("print \"Resume in %i seconds\n\"", pauseSecs));
								break;
							case 1: 
								G_Sound(ent, CHAN_AUTO, G_SoundIndex("sound/chars/mothma/misc/40MOM037")); 
								if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(i, va("print \"Resume in %i seconds\n\"", pauseSecs));
								break;
						}
					}
				}
			}

			JKModPauseDelay = levelTime + 1000;
		}

		return qtrue;
	}
	else if (pauseLast)
	{
		JKMod_PauseTimeRestore(levelTime - pauseLast);
		trap_SendServerCommand(-1, va("print \"Game unpaused after %s\n\"", JKMod_MsToWord((levelTime - pauseLast + 1000), qfalse)));
		trap_SendServerCommand(-1, va("cp \"%s\"", G_GetStripEdString("SVINGAME", "BEGIN_DUEL")));
		
		pauseLast = 0;
		pauseTime = 0;
		pauseSecs = 0;
		JKModPauseDelay = 0;
		
		trap_SetConfigstring(JK_CS_PAUSE, "");
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
Teleport chats load
=====================================================================
*/
void JKMod_TeleportChatLoad(void)
{
	if (g_gametype.integer != GT_TOURNAMENT)
	{
		char	*lineStart;
		int		found = 0;

		level.jkmodLocals.teleportChatsCount = 0;
		lineStart = JKMod_ReadFile("configs/teleport_chats.cfg");

		if (VALIDSTRING(lineStart)) level.jkmodLocals.teleportChatsCount += G_ParseInfos(lineStart, MAX_TOKEN_CHARS - level.jkmodLocals.teleportChatsCount, &level.jkmodLocals.teleportChats[level.jkmodLocals.teleportChatsCount]);

		if (level.jkmodLocals.teleportChats[0]) 
		{
			int			i = 0;
			char		command[MAX_TOKEN_CHARS];
			char		map[MAX_TOKEN_CHARS];
			char		message[MAX_STRING_CHARS] = { 0 };

			for (i = 0; i < level.jkmodLocals.teleportChatsCount; i++)
			{
				strcpy(command, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "command"));
				strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));

				if (Q_stricmp(map, jkcvar_mapName.string) == 0)
				{
					Q_strcat(message, sizeof(message), va("%s,", command));
					found++;
				}
			}

			if (found) 
			{
				message[strlen(message) - 1] = '\0';
				trap_SetConfigstring(JK_CS_TELEPORTS, va("%s", message));
			}
		}

		if (level.jkmodLocals.teleportChatsCount) {
			G_Printf("%i teleport chats loaded%s\n", level.jkmodLocals.teleportChatsCount, (found ? va(" (%i for this map)", found) : ""));
		}
	}
}

/*
=====================================================================
Random begin messages load
=====================================================================
*/
void JKMod_RandomBeginLoad(void) 
{
	if (g_gametype.integer != GT_TOURNAMENT)
	{
		char	*lineStart = NULL;
		char	*lineEnd = NULL;
		int		lineNum = 0;
		int		itemCount = 0;

		level.jkmodLocals.randomBeginCount = 0;
		lineStart = JKMod_ReadFile("configs/random_begin.cfg");

		if (VALIDSTRING(lineStart))
		{
			lineEnd = strchr(lineStart, '\n');
			while (VALIDSTRING(lineEnd))
			{
				*lineEnd = '\0';
				if (VALIDTEXT(lineStart))
				{
					Q_strncpyz(level.jkmodLocals.randomBegin[itemCount], lineStart, sizeof(level.jkmodLocals.randomBegin[0]));
					level.jkmodLocals.randomBeginCount++;
					itemCount++;
				}
				lineStart = lineEnd + 1;
				lineEnd = strchr(lineStart, '\n');
				lineNum++;
				if (lineNum >= MAX_FILE_VARS) break;
			}
        
			if (VALIDTEXT(lineStart))
			{
				Q_strncpyz(level.jkmodLocals.randomBegin[itemCount], lineStart, sizeof(level.jkmodLocals.randomBegin[0]));
				level.jkmodLocals.randomBeginCount++;
				itemCount++;
			}
		}
		
		if (itemCount) {
			G_Printf("%i random begin messages loaded\n", level.jkmodLocals.randomBeginCount);
		}
	}
}

/*
=====================================================================
Server news load
=====================================================================
*/
void JKMod_ServerNewsLoad(void)
{
	if (g_gametype.integer != GT_TOURNAMENT)
	{
		char	*lineStart = NULL;
		char	*lineEnd = NULL;
		int		lineNum = 0;
		int		itemCount = 0;

		level.jkmodLocals.serverNewsCount = 0;
		lineStart = JKMod_ReadFile("configs/server_news.cfg");

		if (VALIDSTRING(lineStart))
		{
			lineEnd = strchr(lineStart, '\n');
			while (VALIDSTRING(lineEnd))
			{
				*lineEnd = '\0';
				if (VALIDTEXT(lineStart))
				{
					Q_strncpyz(level.jkmodLocals.serverNews[itemCount], lineStart, sizeof(level.jkmodLocals.serverNews[0]));
					level.jkmodLocals.serverNewsCount++;
					itemCount++;
				}
				lineStart = lineEnd + 1;
				lineEnd = strchr(lineStart, '\n');
				lineNum++;
				if (lineNum >= MAX_FILE_VARS) break;
			}
        
			if (VALIDTEXT(lineStart))
			{
				Q_strncpyz(level.jkmodLocals.serverNews[itemCount], lineStart, sizeof(level.jkmodLocals.serverNews[0]));
				level.jkmodLocals.serverNewsCount++;
				itemCount++;
			}
		}
		
		if (itemCount) {
			G_Printf("%i server news loaded\n", level.jkmodLocals.serverNewsCount);
		}
	}
}

/*
=====================================================================
Server news check
=====================================================================
*/
void JKMod_ServerNewsCheck(void)
{
	if (jkcvar_serverNews.integer && level.jkmodLocals.serverNewsCount && level.numVotingClients > 0 && g_gametype.integer != GT_TOURNAMENT)
	{
		if (level.jkmodLocals.serverNewsTime < level.time)
		{
			qtime_t		systemTime;
			char		*systemTimeType;
			gentity_t	*ent;
			int			i;

			level.jkmodLocals.serverNewsTime = level.time + (jkcvar_serverNewsTime.integer * 1000);

			trap_RealTime(&systemTime);
			systemTimeType = (systemTime.tm_hour > 11 && systemTime.tm_hour < 24) ? "pm" : "am";

			for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
			{
				if (ent && ent->client && ent->client->pers.connected == CON_CONNECTED && ent->client->sess.sessionTeam != TEAM_SPECTATOR && ent->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE)
				{
					if (jkcvar_serverNewsExtras.integer == 1)
						trap_SendServerCommand(ent - g_entities, va("print \"Server News ^5[^7%02i^5:^7%02i%s^5]^7: %s\n\"", systemTime.tm_hour, systemTime.tm_min, systemTimeType, level.jkmodLocals.serverNews[level.jkmodLocals.serverNewsNum]));
					else if (jkcvar_serverNewsExtras.integer == 2)
						trap_SendServerCommand(ent - g_entities, va("print \"Server News ^5[^7%04d^5-^7%02d^5-^7%02d^5]^7: %s\n\"", systemTime.tm_year+1900, systemTime.tm_mon+1, systemTime.tm_mday, level.jkmodLocals.serverNews[level.jkmodLocals.serverNewsNum]));
					else
						trap_SendServerCommand(ent - g_entities, va("print \"Server News: %s\n\"", level.jkmodLocals.serverNews[level.jkmodLocals.serverNewsNum]));
				}
			}

			if (level.jkmodLocals.serverNewsNum < (level.jkmodLocals.serverNewsCount - 1)) {
				level.jkmodLocals.serverNewsNum++;
			} else {
				level.jkmodLocals.serverNewsNum = 0;
			}
		}
	}
}

/*
=====================================================================
Server idle check
=====================================================================
*/
void JKMod_ServerIdleCheck(void)
{
	if (level.numVotingClients == 0 && jkcvar_serverIdle.integer) 
	{
		if (level.jkmodLocals.idleTime > 0)
		{
			if (level.jkmodLocals.idleTime + jkcvar_serverIdle.integer*60000 < level.time + 15000)
			{
				trap_SendServerCommand(-1, "print \"Server idle, changing to defaults in 15 seconds...\n\"");
				level.jkmodLocals.idleTime =- level.jkmodLocals.idleTime;
			}
		}
		else 
		{
			if (-level.jkmodLocals.idleTime + jkcvar_serverIdle.integer*60000 < level.time)
			{
				trap_SendConsoleCommand(EXEC_APPEND, va("exec %s\n", jkcvar_serverIdleFile.string));
				level.jkmodLocals.idleTime = level.time;
			}
		}
	}
	else 
	{
		if (level.jkmodLocals.idleTime < 0) trap_SendServerCommand(-1, "print \"Server idle reset aborted!\n\"");
		level.jkmodLocals.idleTime = level.time;
	}
}

/*
=====================================================================
Custom shaders remaps
=====================================================================
*/
void JKMod_RemapShaders(void)
{
	char		*lineStart;

	lineStart = JKMod_ReadFile(va("maps/%s.remap", jkcvar_mapName.string));

	JKModRemapShadersFileCount = 0;

	if (VALIDSTRING(lineStart)) JKModRemapShadersFileCount += G_ParseInfos(lineStart, MAX_TOKEN_CHARS - JKModRemapShadersFileCount, &JKModRemapShadersFile[JKModRemapShadersFileCount]);

	if (JKModRemapShadersFile[0]) 
	{
		int		i = 0;
		char	target[MAX_TOKEN_CHARS] = { 0 };
		char	replace[MAX_TOKEN_CHARS] = { 0 };

		for (i = 0; i < JKModRemapShadersFileCount; i++)
		{
			float f = level.time * 0.001;

			strcpy(target, Info_ValueForKey(JKModRemapShadersFile[i], "target"));
			strcpy(replace, Info_ValueForKey(JKModRemapShadersFile[i], "replace"));

			AddRemap(target, replace, f);
			trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
		}
	}

	if (JKModRemapShadersFileCount) {
		G_Printf("%i shader remaps loaded\n", JKModRemapShadersFileCount);
	}
}

/*
=====================================================================
Custom gametype config
=====================================================================
*/
void JKMod_GameTypeConfig(void)
{
	if (jkcvar_gameTypeConfig.integer)
	{
		char *gametypeNames[] = {"ffa", "holocron", "jedimaster", "duel", "single", "team", "saga", "ctf", "cty"};
		fileHandle_t	f;

		if (level.newSession)
		{
			if (trap_FS_FOpenFile(va("configs/game_type/%s.cfg", gametypeNames[g_gametype.integer]), &f, FS_READ) >= 0) 
			{
				G_Printf("Loading custom gametype config for %s\n", gametypeNames[g_gametype.integer]);
				trap_SendConsoleCommand(EXEC_APPEND, va("exec configs/game_type/%s.cfg\n", gametypeNames[g_gametype.integer]));
				trap_FS_FCloseFile(f);
			}
			else
			{
				G_Printf("No custom gametype config for %s, loading default config\n", gametypeNames[g_gametype.integer]);
				trap_SendConsoleCommand(EXEC_APPEND, "exec configs/game_type/default.cfg\n");
			}
		}

		trap_SendConsoleCommand(EXEC_APPEND, "checkcvars\n");
	}
}

/*
=====================================================================
Main initialization functions
=====================================================================
*/
void JKMod_G_InitGame(int levelTime, int randomSeed, int restart) 
{
	G_Printf(S_COLOR_CYAN "----- JK+ Game Initialization -----\n");

	// Launch original init game function
	BaseJK2_G_InitGame(levelTime, randomSeed, restart);

	// Set random seed
	JKMod_RandSeed(randomSeed);

	// Set default timescale
	trap_Cvar_Set("timescale", "1");

	// Set server version
	level.jkmodLocals.serverVersion = VERSION_FLOAT(JK_MAJOR, JK_MINOR, JK_PATCH);

	// Set map restarted check
	level.jkmodLocals.mapRestarted = (qboolean)restart;

	// Set server idle time
	level.jkmodLocals.idleTime = levelTime;

	// Set server news time
	level.jkmodLocals.serverNewsTime = level.time + (jkcvar_serverNewsTime.integer * 1000);

	// Set jetpack active effect
	level.jkmodLocals.jetpackFxActive = G_EffectIndex("env/small_fire");

	// Set jetpack idle effect
	level.jkmodLocals.jetpackFxIdle = G_EffectIndex("env/thruster");

	// Set custom dimension data
	JKMod_DimensionLoad();

	// Set random begin message
	JKMod_RandomBeginLoad();

	// Set server news
	JKMod_ServerNewsLoad();

	// Set teleport chats
	JKMod_TeleportChatLoad();

	// Check gametype config
	JKMod_GameTypeConfig();

	// Check custom shader remaps
	JKMod_RemapShaders();

	// Check server idle
	if (jkcvar_serverIdle.integer)
	{
		G_Printf("%i minutes set for server idle\n", jkcvar_serverIdle.integer);
	}
	
	// Clear bots settings
	if (bot_forgimmick.integer) 
	{
		trap_Cvar_Set("bot_forGimmick", "0");
	}

	// Check server closed
	if (VALIDCVAR(jkcvar_serverClosed.string))
	{
		trap_Cvar_Set("g_needpass", "1");
	}

	// Precache join sound
	if (VALIDCVAR(jkcvar_serverJoinSound.string))
	{
		G_SoundIndex(jkcvar_serverJoinSound.string);
	}

	// Precache race sound
	if (jkcvar_altDimension.integer & DIMENSION_RACE)
	{
		G_SoundIndex("sound/movers/sec_panel_pass");
	}
}
