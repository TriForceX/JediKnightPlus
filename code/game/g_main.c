// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"

#include "mvsdk_setup.h"

level_locals_t	level;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	    // track this variable, and announce if changed
  qboolean teamShader;        // track and if changed, update shader state
} cvarTable_t;

gentity_t		g_entities[MAX_GENTITIES];
gclient_t		g_clients[MAX_CLIENTS];

mvsharedEntity_t	mv_entities[MAX_GENTITIES];
mvclientSession_t	mv_clientSessions[MAX_CLIENTS];

qboolean gDuelExit = qfalse;

vmCvar_t	g_trueJedi;

vmCvar_t	g_gametype;
vmCvar_t	g_MaxHolocronCarry;
vmCvar_t	g_ff_objectives;
vmCvar_t	g_autoMapCycle;
vmCvar_t	g_dmflags;
vmCvar_t	g_maxForceRank;
vmCvar_t	g_forceBasedTeams;
vmCvar_t	g_privateDuel;
vmCvar_t	g_saberLocking;
vmCvar_t	g_saberLockFactor;
vmCvar_t	g_saberTraceSaberFirst;

#ifdef G2_COLLISION_ENABLED
vmCvar_t	g_saberGhoul2Collision;
#endif
vmCvar_t	g_saberAlwaysBoxTrace;
vmCvar_t	g_saberBoxTraceSize;

vmCvar_t	g_logClientInfo;

vmCvar_t	g_slowmoDuelEnd;

vmCvar_t	g_saberDamageScale;

vmCvar_t	g_useWhileThrowing;

vmCvar_t	g_forceRegenTime;
vmCvar_t	g_spawnInvulnerability;
vmCvar_t	g_forcePowerDisable;
vmCvar_t	g_weaponDisable;
vmCvar_t	g_duelWeaponDisable;
vmCvar_t	g_allowDuelSuicide;
vmCvar_t	g_fraglimitVoteCorrection;
vmCvar_t	g_fraglimit;
vmCvar_t	g_duel_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_capturelimit;
vmCvar_t	g_saberInterpolate;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_friendlySaber;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_quadfactor;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_weaponTeamRespawn;
vmCvar_t	g_adaptRespawn;
vmCvar_t	g_motd;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_log;
vmCvar_t	g_logSync;
vmCvar_t	g_statLog;
vmCvar_t	g_statLogFile;
vmCvar_t	g_blood;
vmCvar_t	g_podiumDist;
vmCvar_t	g_podiumDrop;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_debugForward;
vmCvar_t	g_debugRight;
vmCvar_t	g_debugUp;
vmCvar_t	g_smoothClients;
vmCvar_t	g_pmove_fixed;
vmCvar_t	g_pmove_msec;
vmCvar_t	g_rankings;
vmCvar_t	g_listEntity;
vmCvar_t	g_redteam;
vmCvar_t	g_blueteam;
vmCvar_t	g_singlePlayer;
vmCvar_t	g_enableDust;
vmCvar_t	g_enableBreath;
vmCvar_t	g_dismember;
vmCvar_t	g_forceDodge;
vmCvar_t	g_timeouttospec;

vmCvar_t	g_saberDmgVelocityScale;
vmCvar_t	g_saberDmgDelay_Idle;
vmCvar_t	g_saberDmgDelay_Wound;

vmCvar_t	g_saberDebugPrint;

vmCvar_t	g_austrian;

vmCvar_t	g_gamename;
vmCvar_t	g_gamedate;

// Fixes and multiversion cvars
vmCvar_t	g_mv_fixgalaking;
vmCvar_t	g_mv_fixbrokenmodels;
vmCvar_t	g_mv_blockchargejump;
vmCvar_t	g_mv_blockspeedhack;
vmCvar_t	g_mv_fixturretcrash;
vmCvar_t	g_connectionlimit;
vmCvar_t	g_connectinglimit;
vmCvar_t	g_mv_forcePowerDisableMode;

// New cvars
vmCvar_t	g_submodelWorkaround;
vmCvar_t	g_botTeamAutoBalance;

vmCvar_t	g_MVSDK;

int gDuelist1 = -1;
int gDuelist2 = -1;

// bk001129 - made static to avoid aliasing
static cvarTable_t		gameCvarTable[] = {
	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ &g_gamename, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ &g_gamedate, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ NULL, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_MaxHolocronCarry, "g_MaxHolocronCarry", "3", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
	{ &g_ff_objectives, "g_ff_objectives", "0", /*CVAR_SERVERINFO |*/  CVAR_NORESTART, 0, qtrue },

	{ &g_trueJedi, "g_jediVmerc", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qtrue },

	{ &g_autoMapCycle, "g_autoMapCycle", "0", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	
	{ &g_maxForceRank, "g_maxForceRank", "6", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_forceBasedTeams, "g_forceBasedTeams", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_privateDuel, "g_privateDuel", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_saberLocking, "g_saberLocking", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_saberLockFactor, "g_saberLockFactor", "6", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_saberTraceSaberFirst, "g_saberTraceSaberFirst", "1", CVAR_ARCHIVE, 0, qtrue  },

#ifdef G2_COLLISION_ENABLED
	{ &g_saberGhoul2Collision, "g_saberGhoul2Collision", "0", 0, 0, qtrue  },
#endif
	{ &g_saberAlwaysBoxTrace, "g_saberAlwaysBoxTrace", "0", 0, 0, qtrue  },
	{ &g_saberBoxTraceSize, "g_saberBoxTraceSize", "2", 0, 0, qtrue  },

	{ &g_logClientInfo, "g_logClientInfo", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_slowmoDuelEnd, "g_slowmoDuelEnd", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_saberDamageScale, "g_saberDamageScale", "1", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_useWhileThrowing, "g_useWhileThrowing", "1", 0, 0, qtrue  },

	{ &g_forceRegenTime, "g_forceRegenTime", "200", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },

	{ &g_spawnInvulnerability, "g_spawnInvulnerability", "3000", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_forcePowerDisable, "g_forcePowerDisable", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_LATCH, 0, qtrue  },
	{ &g_weaponDisable, "g_weaponDisable", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_LATCH, 0, qtrue  },
	{ &g_duelWeaponDisable, "g_duelWeaponDisable", "1", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_LATCH, 0, qtrue  },

	{ &g_allowDuelSuicide, "g_allowDuelSuicide", "0", CVAR_ARCHIVE, 0, qtrue },

	{ &g_fraglimitVoteCorrection, "g_fraglimitVoteCorrection", "1", CVAR_ARCHIVE, 0, qtrue },

	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_duel_fraglimit, "duel_fraglimit", "10", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },

	{ &g_saberInterpolate, "g_saberInterpolate", "1", CVAR_ARCHIVE, 0, qtrue },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_friendlySaber, "g_friendlySaber", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE  },
	{ &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },

	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_doWarmup, "g_doWarmup", "0", 0, 0, qtrue  },
	{ &g_log, "g_log", "games.log", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logSync, "g_logSync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_statLog, "g_statLog", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &g_statLogFile, "g_statLogFile", "statlog.log", CVAR_ARCHIVE, 0, qfalse },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "250", 0, 0, qtrue  },
	{ &g_gravity, "g_gravity", "800", 0, 0, qtrue  },
	{ &g_knockback, "g_knockback", "1000", 0, 0, qtrue  },
	{ &g_quadfactor, "g_quadfactor", "3", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },
	{ &g_weaponTeamRespawn, "g_weaponTeamRespawn", "5", 0, 0, qtrue },
	{ &g_adaptRespawn, "g_adaptrespawn", "1", 0, 0, qtrue  },		// Make weapons respawn faster with a lot of players.
	{ &g_forcerespawn, "g_forcerespawn", "60", 0, 0, qtrue },		// One minute force respawn.  Give a player enough time to reallocate force.
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },
	{ &g_blood, "com_blood", "1", 0, 0, qfalse },

	{ &g_podiumDist, "g_podiumDist", "80", 0, 0, qfalse },
	{ &g_podiumDrop, "g_podiumDrop", "70", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_listEntity, "g_listEntity", "0", 0, 0, qfalse },

#if 0
	{ &g_debugForward, "g_debugForward", "0", 0, 0, qfalse },
	{ &g_debugRight, "g_debugRight", "0", 0, 0, qfalse },
	{ &g_debugUp, "g_debugUp", "0", 0, 0, qfalse },
#endif

	{ &g_redteam, "g_redteam", "Empire", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue },
	{ &g_blueteam, "g_blueteam", "Rebellion", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue  },
	{ &g_singlePlayer, "ui_singlePlayerActive", "", 0, 0, qfalse, qfalse  },

	{ &g_enableDust, "g_enableDust", "0", 0, 0, qtrue, qfalse },
	{ &g_enableBreath, "g_enableBreath", "0", 0, 0, qtrue, qfalse },
	{ &g_smoothClients, "g_smoothClients", "1", 0, 0, qfalse},
	{ &g_pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO, 0, qfalse},
	{ &g_pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, 0, qfalse},

	{ &g_rankings, "g_rankings", "0", 0, 0, qfalse},

	{ &g_dismember, "g_dismember", "0", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_forceDodge, "g_forceDodge", "1", 0, 0, qtrue  },

	{ &g_timeouttospec, "g_timeouttospec", "70", CVAR_ARCHIVE, 0, qfalse },

	{ &g_saberDmgVelocityScale, "g_saberDmgVelocityScale", "0", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_saberDmgDelay_Idle, "g_saberDmgDelay_Idle", "350", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_saberDmgDelay_Wound, "g_saberDmgDelay_Wound", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_saberDebugPrint, "g_saberDebugPrint", "0", CVAR_CHEAT, 0, qfalse  },

	{ &g_austrian, "g_austrian", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_mv_fixgalaking, "mv_fixgalaking", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_mv_fixbrokenmodels, "mv_fixbrokenmodels", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_mv_blockchargejump, "mv_blockchargejump", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_mv_blockspeedhack, "mv_blockspeedhack", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_mv_fixturretcrash, "mv_fixturretcrash", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_connectinglimit, "g_connectinglimit", "3", CVAR_ARCHIVE, 0, qfalse },
	{ &g_connectionlimit, "g_connectionlimit", "0", CVAR_ARCHIVE, 0, qfalse },

	// The 1.02 client doesn't show the force selection menu if ANY force power is disabled. And the basejk 1.02 server sets saber attack to level 3,
	// saber defense to level 3 and force jump to level 1, if ANY force power is disabled. This leads to some differences in g_forcePowerDisable
	// behaviour between 1.02 and 1.03+. For instance disabling all powers except jump, saber attack and saber defense on 1.02 behaves the same as
	// disabling all force powers on 1.03+. To stay compatible with existing configs mvsdk is going to handle g_forcePowerDisable like 1.02, when
	// loaded in 1.02 mode and mv_forcePowerDisableMode is set. By disabling this cvar 1.03+ behaviour is going to be enabled in 1.02 mode, but
	// base clients might have disadvantages, cause their ui doesn't allow them to assign force jump points, even when force jump is enabled.
	// This cvar only has an effect when the startversion is 1.02.
	{ &g_mv_forcePowerDisableMode, "mv_forcePowerDisableMode", "1", CVAR_ARCHIVE, 0, qfalse },

	// g_submodelWorkaround was technically just setting a flag for mvsdk clients to apply the clientside workaround
	// The cvar might seem more appropriate on cgame, but defaulting it to "0" on the client would make the workaround hardly usable
	// and defaulting it to "1" on the the client might lead to mappers not realising they exceeded basejk limits when testing their maps.
	// So we have a cvar in the game module to let servers enable the clientside workaround for bigger maps, defaulting to "0".
	// Clients supporting the workaround are going to inform the server about it in their userinfo, no matter what this cvar is set to.
	// By now g_submodelWorkaround also supports the value "2", which leads to the modelindex of submodel entities being copied to their
	// time2 fields. This way we can use more than 8 bit for the modelindex. The time2 modelindex is only useful in combination with an
	// engine that supports more than the default 256 submodels.
	{ &g_submodelWorkaround, "g_submodelWorkaround", "0", CVAR_ARCHIVE, 0, qtrue },

	// Bots reset their teams on map_restart and map change on basejk. This is often undesired, so let the host decide.
	{ &g_botTeamAutoBalance, "g_botTeamAutoBalance", "1", CVAR_ARCHIVE, 0, qtrue },

	{ &g_MVSDK, "g_MVSDK", MVSDK_VERSION, CVAR_ROM | CVAR_SERVERINFO, 0, qfalse },
};

// bk001129 - made static to avoid aliasing
static int gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );


void G_InitGame					( int levelTime, int randomSeed, int restart );
void G_RunFrame					( int levelTime );
void G_ShutdownGame				( int restart );
void CheckExitRules				( void );
void G_ROFF_NotetrackCallback	( gentity_t *cent, const char *notetrack);


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
int mvapi = 0;
qboolean mvStructConversionDisabled = qfalse;

int Init_levelTime;
int Init_randomSeed;
int Init_restart;

intptr_t JK2_vmMain( intptr_t command, intptr_t arg0, intptr_t arg1, intptr_t arg2, intptr_t arg3, intptr_t arg4, intptr_t arg5, intptr_t arg6, intptr_t arg7, intptr_t arg8, intptr_t arg9, intptr_t arg10, intptr_t arg11  );
LIBEXPORT intptr_t vmMain( intptr_t command, intptr_t arg0, intptr_t arg1, intptr_t arg2, intptr_t arg3, intptr_t arg4, intptr_t arg5, intptr_t arg6, intptr_t arg7, intptr_t arg8, intptr_t arg9, intptr_t arg10, intptr_t arg11  )
{ // Wrapper for vmMain, to apply version-specifc adjustments at the beginning and the end of every VM_Call without compleltly changing the vmMain function.
	intptr_t retValue;
	static int activeVMCalls = 0;

	if ( !activeVMCalls ) // If we're not using any wrapper functions it can happen that a syscall triggers a VM_Call and we would try to convert data that has been converted already. So we need to keep track of this...
		MV_VersionMagic( qfalse );
	activeVMCalls++;

	retValue = JK2_vmMain( command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );

	activeVMCalls--;
	if ( !activeVMCalls ) 
		MV_VersionMagic( qtrue );

	return retValue;
}
intptr_t JK2_vmMain( intptr_t command, intptr_t arg0, intptr_t arg1, intptr_t arg2, intptr_t arg3, intptr_t arg4, intptr_t arg5, intptr_t arg6, intptr_t arg7, intptr_t arg8, intptr_t arg9, intptr_t arg10, intptr_t arg11  ) {
int requestedMvApi = 0;
	switch ( command ) {
	case GAME_INIT:
		requestedMvApi = MVAPI_Init(arg11);
		if ( !requestedMvApi )
		{ // Only call G_InitGame if we haven't got access to the MVAPI. If we can use the MVAPI we delay the Init until the "MVAPI_AFTER_INIT" command is sent. That allows us use the MVAPI in the actual init.
			G_InitGame( arg0, arg1, arg2 );
		}
		else
		{ // Store the values that were meant for G_InitGame to use them later, when MVAPIR_AFTER_INIT is called.
			Init_levelTime = arg0;
			Init_randomSeed = arg1;
			Init_restart = arg2;
		}
		return requestedMvApi;
	case MVAPI_AFTER_INIT:
		MVAPI_AfterInit();
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (intptr_t)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 );
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0, qtrue );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		return ConsoleCommand();
	case BOTAI_START_FRAME:
		return BotAIStartFrame( arg0 );
	case GAME_ROFF_NOTETRACK_CALLBACK:
		G_ROFF_NotetrackCallback( &g_entities[arg0], (const char *)arg1 );
	}

	return -1;
}


void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Printf( text );
}

Q_NORETURN void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Error( text );
}

#define GAME_MV_MIN_APILEVEL 1
#define GAME_MV_MIN_VERSION "1.1"
int MVAPI_Init(int apilevel)
{
	if (!trap_Cvar_VariableIntegerValue("mv_apienabled"))
	{
		G_Printf("Game: MVAPI is not supported at all or has been disabled.\n");
		G_Printf("Game: You need at least JK2MV " GAME_MV_MIN_VERSION ".\n");
		return 0;
	}

	if (apilevel < GAME_MV_MIN_APILEVEL)
	{
		G_Printf("Game: MVAPI level %i not supported.\n", GAME_MV_MIN_APILEVEL);
		G_Printf("Game: You need at least JK2MV " GAME_MV_MIN_VERSION ".\n");
		return 0;
	}

	if (apilevel < MV_APILEVEL)
	{
		G_Printf("Game: MVAPI level %i not supported (using level %i instead).\n", MV_APILEVEL, apilevel);
		G_Printf("Game: You need at least JK2MV " MV_MIN_VERSION " to enable all API features.\n");
	}

	mvapi = apilevel;
	if ( mvapi > MV_APILEVEL ) mvapi = MV_APILEVEL;

	G_Printf("Game: Using MVAPI level %i (%i supported).\n", mvapi, apilevel);
	return mvapi;
}

void MVAPI_AfterInit(void)
{
	if ( mvapi >= 3 )
	{ // If the apilevel supports it tell the engine that we're using 1.04 structs etc. internally
		// Get the inital version
		jk2startversion = trap_MVAPI_GetVersion();
		// Set the version to 1.04
		trap_MVAPI_SetVersion( VERSION_1_04 );
		// Get the current version (should always be 1.04)
		jk2version = trap_MVAPI_GetVersion();

		// Set gameplay and version
		MV_SetGameVersion( jk2version, qfalse );
		MV_SetGamePlay( jk2startversion );
	}
	else if ( mvapi >= 2 )
	{ // If the mvapi supports it tell the engine that we are using the post 1.02 structs internally and don't waste any time converting structs
		mvStructConversionDisabled = qtrue;
		trap_MVAPI_DisableStructConversion( mvStructConversionDisabled );
	}

	// Let the engine know we support more than 256 submodels
	if ( mvapi >= 4 ) trap_MVAPI_EnableSubmodelBypass( qtrue );

	// Call G_InitGame now, because we delayed it earilier
	G_InitGame( Init_levelTime, Init_randomSeed, Init_restart );

	// Disable those JK2MV Engine fixes we can take care of in the VM
	trap_MVAPI_ControlFixes( MVFIX_NAMECRASH | MVFIX_FORCECRASH | MVFIX_GALAKING | MVFIX_BROKENMODEL | MVFIX_TURRETCRASH | MVFIX_CHARGEJUMP | MVFIX_SPEEDHACK | MVFIX_SABERSTEALING | MVFIX_PLAYERGHOSTING );

	// Inform JK2MV that we can handle level.time resetting on mapchanges
	if ( mvapi >= 4 ) trap_MVAPI_ResetServerTime( qtrue );
}

/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=MAX_CLIENTS, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}

	G_Printf ("%i teams with %i entities\n", c, c2);
}

void G_RemapTeamShaders( void ) {
#if 0
	char string[1024];
	float f = level.time * 0.001;
	Com_sprintf( string, sizeof(string), "team_icon/%s_red", g_redteam.string );
	AddRemap("textures/ctf2/redteam01", string, f); 
	AddRemap("textures/ctf2/redteam02", string, f); 
	Com_sprintf( string, sizeof(string), "team_icon/%s_blue", g_blueteam.string );
	AddRemap("textures/ctf2/blueteam01", string, f); 
	AddRemap("textures/ctf2/blueteam02", string, f); 
	trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
#endif
}


/*
=================
G_RegisterCvars
=================
*/
void BaseJK2_G_RegisterCvars( void ) { // Tr!Force: [BaseJK2] Register cvars function
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {

		// Tr!Force: [GameTypeConfig] Cvar latch temp unlock toggle
		if (cv->vmCvar != &g_gametype && cv->cvarFlags & (CVAR_LATCH | CVAR_TEMP) && jkcvar_gameTypeConfig.integer) 
		{
			cv->cvarFlags ^= CVAR_LATCH;
			cv->cvarFlags ^= CVAR_TEMP;
			JKMod_Printf("%s latched from basejk %s\n", cv->cvarName, (cv->cvarFlags & CVAR_LATCH ? "^5locked" : "^3unlocked"));
		}

		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;

		if (cv->teamShader) {
			remapped = qtrue;
		}
	}

	// Tr!Force: [GameTypeConfig] Cvar latch temp unlock finish
	if (jkcvar_gameTypeConfig.integer && level.jkmodLocals.cvarTempUnlock == 1) 
	{
		level.jkmodLocals.cvarTempUnlock = 2;
		if (level.newSession) JKMod_CheckValidMapItems();
		return;
	}

	if ( strcmp(g_gamename.string, GAMEVERSION) || strcmp(g_gamedate.string, __DATE__) ) {
		// Inform the host about the unexpected change
		G_Printf( S_COLOR_YELLOW "WARNING: The gamename or gamedate changed after mapchange.\n"
		          S_COLOR_YELLOW "         This could indiciate unexpected side-effects due to module updates at runtime.\n"
		          S_COLOR_YELLOW "         You might want to restart the server.\n" );

		trap_Cvar_Set( "gamename", GAMEVERSION );
		trap_Cvar_Set( "gamedate", __DATE__ );
	}

	if (remapped) {
		G_RemapTeamShaders();
	}

	// check some things
	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
		G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	level.warmupModificationCount = g_warmup.modificationCount;

	MV_UpdateSvFlags();
}

/*
=================
G_UpdateCvars
=================
*/
void BaseJK2_G_UpdateCvars( void ) { // Tr!Force: [BaseJK2] Update cvars function
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount ) {
				cv->modificationCount = cv->vmCvar->modificationCount;

				if ( cv->trackChange && !level.jkmodLocals.cvarToggleMod ) { // Tr!Force: [JKMod] Toggle mod check
					trap_SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", 
						cv->cvarName, cv->vmCvar->string ) );
				}

				if (cv->teamShader) {
					remapped = qtrue;
				}

				// mvsdk_svFlags
				if ( cv->vmCvar == &g_submodelWorkaround )
					MV_UpdateSvFlags();
			}
		}
	}

	if (remapped) {
		G_RemapTeamShaders();
	}
}

/*
=================
MV_UpdateMvsdkConfigstring

=================
*/
void MV_UpdateMvsdkConfigstring( char *key, char *value )
{
	char csString[MAX_INFO_STRING];

	trap_GetConfigstring( CS_MVSDK, csString, sizeof(csString) );
	Info_SetValueForKey( csString, key, value );
	trap_SetConfigstring( CS_MVSDK, csString );
}

/*
=================
MV_UpdateSvFlags

Called when registering cvars and updating specific cvars and updates the mvsdk_svFlags according to the current settings
=================
*/
void MV_UpdateSvFlags( void )
{
	// mvsdk_svFlags - Used to inform clients about additional mvsdk serverside-features or the compatibility to clientside-features
	static int lastValue = 0;
	int intValue = 0;

	// Check for the features and determine the flags
	if ( level.bboxEncoding )               intValue |= MVSDK_SVFLAG_BBOX;
	if ( g_submodelWorkaround.integer & 1 ) intValue |= MVSDK_SVFLAG_SUBMODEL_WORKAROUND;
	if ( level.modelindexTime2 )            intValue |= MVSDK_SVFLAG_SUBMODEL_TIME2;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!! Forks of MVSDK should NOT modify the mvsdk_svFlags                              !!!
	// !!! Removal, replacement or adding of new flags might lead to incompatibilities     !!!
	// !!! Forks should define their own infostring, but they can send it through CS_MVSDK !!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Check if we have to update anything or if we can return already
	if ( intValue == lastValue ) return;

	// Update the configstring
	MV_UpdateMvsdkConfigstring( "mvsdk_svFlags", va("%i", intValue) );

	// Remember the old value
	lastValue = intValue;
}

/*
============
G_InitGame

============
*/
void BaseJK2_G_InitGame( int levelTime, int randomSeed, int restart ) { // Tr!Force: BaseJk2 Init game function
	int					i;
	unsigned			jkmod_dimensionBase; // Tr!Force: [Dimensions] Global base dimension

	B_InitAlloc(); //make sure everything is clean

	G_Printf ("------- Game Initialization -------\n");
	G_Printf ("gamename: %s\n", GAMEVERSION);
	G_Printf ("gamedate: %s\n", __DATE__);
	
	if ( jk2version == VERSION_UNDEF )
	{ // We don't know the version of the server, yet...
		// JK2MV with api?
		if ( mvapi ) jk2version = trap_MVAPI_GetVersion();

		if ( jk2version == VERSION_UNDEF )
		{
			char version[128];

			trap_Cvar_VariableStringBuffer("version", version, sizeof(version));

			// Not checking for exact strings, as those are different on every build. Instead we check if the version is in the string.
			if ( strstr(version, "JK2MP") )
			{ // Seems to be JK2MP or JK2MV > 1.1
					 if ( strstr(version, "1.02") ) jk2version = VERSION_1_02;
				else if ( strstr(version, "1.03") ) jk2version = VERSION_1_03;
				else if ( strstr(version, "1.04") ) jk2version = VERSION_1_04;
				else
				{
					jk2version = VERSION_1_04;
					G_Printf("MVSDK: Unable to detect jk2mp version, setting to 1.04 compatibility.\n");
				}
			}
			else if ( strstr(version, "JK2MV") )
			{ // Seems to be jk2mv, but an old version, try to find the version by reading the mv_serverversion cvar
				trap_Cvar_VariableStringBuffer("mv_serverversion", version, sizeof(version));
					 if ( !Q_stricmp(version, "1.02") ) jk2version = VERSION_1_02;
				else if ( !Q_stricmp(version, "1.03") ) jk2version = VERSION_1_03;
				else if ( !Q_stricmp(version, "1.04") ) jk2version = VERSION_1_04;
			}
		}

		if ( jk2version == VERSION_UNDEF ) G_Error("MVSDK: Unable to detect jk2version [Game].");
		jk2startversion = jk2version;
		MV_SetGameVersion(jk2version, qtrue);
	}
	G_Printf("jk2version [Game]: 1.0%i\n", jk2startversion);

	srand( randomSeed );
	mysrand( randomSeed ); // On linux rand() behaves different than on Winodws or in a qvm, ...

	G_RegisterCvars();

	G_ProcessIPBans();

	G_InitMemory();

	// set some level globals
	jkmod_dimensionBase = level.jkmodLocals.dimensionBase ? level.jkmodLocals.dimensionBase : DIMENSION_FREE; // Tr!Force: [Dimensions] Global base dimension
	memset( &level, 0, sizeof( level ) );
	level.time = levelTime;
	level.startTime = levelTime;
	level.jkmodLocals.dimensionBase = g_gametype.integer == GT_FFA ? jkmod_dimensionBase : DIMENSION_FREE; // Tr!Force: [Dimensions] Global base dimension
	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	//trap_SP_RegisterServer("mp_svgame");

	if ( g_log.string[0] ) {
		if ( g_logSync.integer ) {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND_SYNC );
		} else {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND );
		}
		if ( !level.logFile ) {
			G_Printf( "WARNING: Couldn't open logfile: %s\n", g_log.string );
		} else {
			char	serverinfo[MAX_INFO_STRING];

			trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

			G_LogPrintf("------------------------------------------------------------\n" );
			G_LogPrintf("InitGame: %s\n", serverinfo );
		}
	} else {
		G_Printf( "Not logging to disk.\n" );
	}

	G_LogWeaponInit();

	G_InitWorldSession();

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;
	
	// We can initialise this even without the JK2MV API and use it in the VM, but we can only share it with the engine, if the API is available
	memset( &mv_entities, 0, sizeof(mv_entities) );

	// initialize all clients for this game
	level.maxclients = g_maxclients.integer;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}

	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

	// let the server system know where the entites are
	if ( jk2version == VERSION_1_02 && !mvStructConversionDisabled )
	{ // 1.02
		// initialize all clients for this game
		memset( g_ps, 0, MAX_CLIENTS * sizeof(g_ps[0]) );

		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
			(playerState_t*)&g_ps[0], sizeof( g_ps[0] ) );
	}
	else
	{
		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
			&level.clients[0].ps, sizeof( level.clients[0] ) );
	}
	
	// Inform the engine about our mv_entities
	if ( mvapi ) trap_MVAPI_LocateGameData( mv_entities, level.num_entities, sizeof( mvsharedEntity_t ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// initialize saga mode before spawning entities so we know
	// if we should remove any saga-related entities on spawn
	InitSagaMode();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	if( g_gametype.integer >= GT_TEAM ) {
		G_CheckTeamItems();
	}
	else if ( g_gametype.integer == GT_JEDIMASTER )
	{
		trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, "-1" );
	}

	trap_SetConfigstring ( CS_CLIENT_DUELISTS, va("-1|-1") );
	trap_SetConfigstring ( CS_CLIENT_DUELWINNER, va("-1") );
	gDuelist1 = -1;
	gDuelist2 = -1;

	SaveRegisteredItems();

	G_Printf ("-----------------------------------\n");

	if( g_gametype.integer == GT_SINGLE_PLAYER || trap_Cvar_VariableIntegerValue( "com_buildScript" ) ) {
		G_ModelIndex( SP_PODIUM_MODEL );
		G_SoundIndex( "sound/player/gurp1.wav" );
		G_SoundIndex( "sound/player/gurp2.wav" );
	}

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_InitBots( restart );
	}

	G_RemapTeamShaders();

	if ( g_gametype.integer == GT_TOURNAMENT )
	{
		G_LogPrintf("Duel Tournament Begun: kill limit %d, win limit: %d\n", g_fraglimit.integer, g_duel_fraglimit.integer );
	}
}



/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
	G_Printf ("==== ShutdownGame ====\n");

	G_LogWeaponOutput();

	if ( level.logFile ) {
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();

	trap_ROFF_Clean();

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}

	B_CleanupAlloc(); //clean up all allocations made with B_Alloc

	// Some builds of jk2mv don't reset the vm->gameversion on map_restart, so reset the gameversion now
	if ( mvapi >= 3 && jk2version != jk2startversion )
	{
		// Reset the gameversion in the engine
		trap_MVAPI_SetVersion( jk2startversion );

		// If we are not called again it shouldn't matter, but in case some modified engine version decides to do
		// additional vmCalls after the shutdown we want to know what version it expects
		jk2version = trap_MVAPI_GetVersion();
		MV_SetGameVersion( jk2version, qfalse );

		// Replace the gamedata (the g_ps array gets valid data at the end of the vmCall if we returned into 1.02 mode)
		if ( jk2version == VERSION_1_02 && !mvStructConversionDisabled )
		{
			memset( g_ps, 0, MAX_CLIENTS * sizeof(g_ps[0]) );
			trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), (playerState_t*)&g_ps[0], sizeof( g_ps[0] ) );
		}
		else
		{
			trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), &level.clients[0].ps, sizeof( level.clients[0] ) );
		}
	}
}



//===================================================================

Q_NORETURN void QDECL Com_Error ( errorParm_t level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	Q_vsnprintf (text, sizeof(text), error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	G_Printf ("%s", text);
}

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/

/*
=============
AddTournamentPlayer

If there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	// never change during intermission
//	if ( level.intermissiontime ) {
//		return;
//	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD || 
			client->sess.spectatorClient < 0  ) {
			continue;
		}
		// Tr!Force: [Plugin] Don't allow
		if (jkcvar_pluginRequired.integer == 2 && !client->pers.jkmodPers.clientPlugin) {
			continue;
		}

		if ( !nextInLine || client->sess.spectatorOrder > nextInLine->sess.spectatorOrder ) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}

/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

void RemoveDuelDrawLoser(void)
{
	int clFirst = 0;
	int clSec = 0;
	int clFailure = 0;

	if ( level.clients[ level.sortedClients[0] ].pers.connected != CON_CONNECTED )
	{
		return;
	}
	if ( level.clients[ level.sortedClients[1] ].pers.connected != CON_CONNECTED )
	{
		return;
	}

	clFirst = level.clients[ level.sortedClients[0] ].ps.stats[STAT_HEALTH] + level.clients[ level.sortedClients[0] ].ps.stats[STAT_ARMOR];
	clSec = level.clients[ level.sortedClients[1] ].ps.stats[STAT_HEALTH] + level.clients[ level.sortedClients[1] ].ps.stats[STAT_ARMOR];

	if (clFirst > clSec)
	{
		clFailure = 1;
	}
	else if (clSec > clFirst)
	{
		clFailure = 0;
	}
	else
	{
		clFailure = 2;
	}

	if (clFailure != 2)
	{
		SetTeam( &g_entities[ level.sortedClients[clFailure] ], "s" );
	}
	else
	{ //we could be more elegant about this, but oh well.
		SetTeam( &g_entities[ level.sortedClients[1] ], "s" );
	}
}

/*
=======================
RemoveTournamentWinner
=======================
*/
void RemoveTournamentWinner( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[0];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
AdjustTournamentScores
=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	if (level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE] ==
		level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE] &&
		level.clients[level.sortedClients[0]].pers.connected == CON_CONNECTED &&
		level.clients[level.sortedClients[1]].pers.connected == CON_CONNECTED)
	{
		int clFirst = level.clients[ level.sortedClients[0] ].ps.stats[STAT_HEALTH] + level.clients[ level.sortedClients[0] ].ps.stats[STAT_ARMOR];
		int clSec = level.clients[ level.sortedClients[1] ].ps.stats[STAT_HEALTH] + level.clients[ level.sortedClients[1] ].ps.stats[STAT_ARMOR];
		int clFailure = 0;
		int clSuccess = 0;

		if (clFirst > clSec)
		{
			clFailure = 1;
			clSuccess = 0;
		}
		else if (clSec > clFirst)
		{
			clFailure = 0;
			clSuccess = 1;
		}
		else
		{
			clFailure = 2;
			clSuccess = 2;
		}

		if (clFailure != 2)
		{
			clientNum = level.sortedClients[clSuccess];

			level.clients[ clientNum ].sess.wins++;
			ClientUserinfoChanged( clientNum );
			trap_SetConfigstring ( CS_CLIENT_DUELWINNER, va("%i", clientNum ) );

			// Tr!Force: [PlayerStatus] Announce score
			if (jkcvar_chatAutoStatus.integer && level.clients[clientNum].sess.jkmodSess.playerStatus) JKMod_playerStatus(&g_entities[clientNum], qtrue);

			clientNum = level.sortedClients[clFailure];

			level.clients[ clientNum ].sess.losses++;
			ClientUserinfoChanged( clientNum );
		}
		else
		{
			clSuccess = 0;
			clFailure = 1;

			clientNum = level.sortedClients[clSuccess];

			level.clients[ clientNum ].sess.wins++;
			ClientUserinfoChanged( clientNum );
			trap_SetConfigstring ( CS_CLIENT_DUELWINNER, va("%i", clientNum ) );

			// Tr!Force: [PlayerStatus] Announce score
			if (jkcvar_chatAutoStatus.integer && level.clients[clientNum].sess.jkmodSess.playerStatus) JKMod_playerStatus(&g_entities[clientNum], qtrue);

			clientNum = level.sortedClients[clFailure];

			level.clients[ clientNum ].sess.losses++;
			ClientUserinfoChanged( clientNum );
		}
	}
	else
	{
		clientNum = level.sortedClients[0];
		if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
			level.clients[ clientNum ].sess.wins++;
			ClientUserinfoChanged( clientNum );

			trap_SetConfigstring ( CS_CLIENT_DUELWINNER, va("%i", clientNum ) );

			// Tr!Force: [PlayerStatus] Announce score
			if (jkcvar_chatAutoStatus.integer && level.clients[clientNum].sess.jkmodSess.playerStatus) JKMod_playerStatus(&g_entities[clientNum], qtrue);
		}

		clientNum = level.sortedClients[1];
		if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
			level.clients[ clientNum ].sess.losses++;
			ClientUserinfoChanged( clientNum );
		}
	}
}

/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorOrder > cb->sess.spectatorOrder ) {
			return -1;
		}
		if ( ca->sess.spectatorOrder < cb->sess.spectatorOrder ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

	// then sort by score
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	return 0;
}

qboolean gQueueScoreMessage = qfalse;
int gQueueScoreMessageTime = 0;

/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void CalculateRanks( void ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	// int		preNumSpec = 0;
	// int		nonSpecIndex = -1;
	gclient_t	*cl;

	// preNumSpec = level.numNonSpectatorClients;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for ( i = 0; i < /*TEAM_NUM_TEAMS*/2; i++ ) { // TEAM_NUM_TEAMS is 4, numteamVotingClients has a size of [2]
		level.numteamVotingClients[i] = 0;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
				level.numNonSpectatorClients++;
				// nonSpecIndex = i;
			
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
						if ( level.clients[i].sess.sessionTeam == TEAM_RED )
							level.numteamVotingClients[0]++;
						else if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							level.numteamVotingClients[1]++;
					}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	if (!g_warmup.integer)
	{
		level.warmupTime = 0;
	}

	/*
	if (level.numNonSpectatorClients == 2 && preNumSpec < 2 && nonSpecIndex != -1 && g_gametype.integer == GT_TOURNAMENT && !level.warmupTime)
	{
		gentity_t *currentWinner = G_GetDuelWinner(&level.clients[nonSpecIndex]);

		if (currentWinner && currentWinner->client)
		{
			G_CenterPrint( -1, 3, va("%s" S_COLOR_WHITE " %s %s\n",
			currentWinner->client->pers.netname, G_GetStripEdString("SVINGAME", "VERSUS"), level.clients[nonSpecIndex].pers.netname));
		}
	}
	*/
	//NOTE: for now not doing this either. May use later if appropriate.

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer >= GT_TEAM ) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = 0;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}

		if (g_gametype.integer != GT_TOURNAMENT)
		{ //when not in duel, use this configstring to pass the index of the player currently in first place
			if ( level.numConnectedClients >= 1 )
			{
				trap_SetConfigstring ( CS_CLIENT_DUELWINNER, va("%i", level.sortedClients[0] ) );
			}
			else
			{
				trap_SetConfigstring ( CS_CLIENT_DUELWINNER, "-1" );
			}
		}
	}

	// see if it is time to end the level
	CheckExitRules();

	// if we are at the intermission or in multi-frag Duel game mode, send the new info to everyone
	if ( level.intermissiontime || g_gametype.integer == GT_TOURNAMENT ) {
		gQueueScoreMessage = qtrue;
		gQueueScoreMessageTime = level.time + 500;
		//SendScoreboardMessageToAllClients();
		//rww - Made this operate on a "queue" system because it was causing large overflows
	}
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DeathmatchScoreboardMessage( g_entities + i );
		}
	}
}

/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}


	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, ent->client->ps.origin );
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pm_type = PM_INTERMISSION;

	// clean up powerup info
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	ent->client->ps.eFlags = 0;
	ent->s.eFlags = 0;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

qboolean DuelLimitHit(void);

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;

	if ( level.intermissiontime ) {
		return;		// already active
	}

	// if in tournement mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		trap_SetConfigstring ( CS_CLIENT_DUELWINNER, "-1" );

		AdjustTournamentScores();
		if (DuelLimitHit())
		{
			gDuelExit = qtrue;
		}
		else
		{
			gDuelExit = qfalse;
		}
	}

	level.intermissiontime = level.time;
	FindIntermissionPoint();

	if (g_singlePlayer.integer) {
		trap_Cvar_Set("ui_singlePlayerActive", "0");
		UpdateTournamentInfo();
	}

	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		if (client->health <= 0) {
			respawn(client);
		}
		MoveClientToIntermission( client );
	}

	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();

}

qboolean DuelLimitHit(void)
{
	int i;
	gclient_t *cl;

	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}

		if ( g_duel_fraglimit.integer && cl->sess.wins >= g_duel_fraglimit.integer )
		{
			return qtrue;
		}
	}

	return qfalse;
}

void DuelResetWinsLosses(void)
{
	int i;
	gclient_t *cl;

	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}

		cl->sess.wins = 0;
		cl->sess.losses = 0;
	}
}

/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar 

=============
*/
void ExitLevel (void) {
	int		i;
	gclient_t *cl;

	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT  ) {
		if (!DuelLimitHit())
		{
			if ( !level.restarted ) {
				trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
				level.restarted = qtrue;
				level.changemap = NULL;
				level.intermissiontime = 0;
			}
			return;	
		}

		DuelResetWinsLosses();
	}


	trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );
	level.changemap = NULL;
	level.intermissiontime = 0;

	// reset all the scores so we don't enter the intermission again
	level.teamScores[TEAM_RED] = 0;
	level.teamScores[TEAM_BLUE] = 0;
	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.persistant[PERS_SCORE] = 0;
	}

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< g_maxclients.integer ; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}

/*
=================
G_LogPrintf

Print to the logfile with a time stamp if it is open
=================
*/
void QDECL G_LogPrintf( const char *fmt, ... ) {
	va_list		argptr;
	char		string[1024];
	int			min, tens, sec;

	sec = level.time / 1000;

	min = sec / 60;
	sec -= min * 60;
	tens = sec / 10;
	sec -= tens * 10;

	Com_sprintf( string, sizeof(string), "%3i:%i%i ", min, tens, sec );

	va_start( argptr, fmt );
	Q_vsnprintf( string +7, sizeof(string)-7 , fmt,argptr );
	va_end( argptr );

	if ( g_dedicated.integer ) {
		G_Printf( "%s", string + 7 );
	}

	if ( !level.logFile ) {
		return;
	}

	trap_FS_Write( string, strlen( string ), level.logFile );
}

/*
================
LogExit

Append information about this game to the log file
================
*/
void LogExit( const char *string ) {
	int				i, numSorted;
	gclient_t		*cl;
	qboolean		won = qtrue;
	G_LogPrintf( "Exit: %s\n", string );

	level.intermissionQueued = level.time;

	// this will keep the clients from playing any voice sounds
	// that will get cut off when the queued intermission starts
	trap_SetConfigstring( CS_INTERMISSION, "1" );

	// don't send more than 32 scores (FIXME?)
	numSorted = level.numConnectedClients;
	if ( numSorted > 32 ) {
		numSorted = 32;
	}

	if ( g_gametype.integer >= GT_TEAM ) {
		G_LogPrintf( "red:%i  blue:%i\n",
			level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE] );
	}

	for (i=0 ; i < numSorted ; i++) {
		int		ping;

		cl = &level.clients[level.sortedClients[i]];

		if ( cl->sess.sessionTeam == TEAM_SPECTATOR ) {
			continue;
		}
		if ( cl->pers.connected == CON_CONNECTING ) {
			continue;
		}

		ping = cl->ps.ping < 999 ? cl->ps.ping : 999;

		G_LogPrintf( "score: %i  ping: %i  client: %i %s\n", cl->ps.persistant[PERS_SCORE], ping, level.sortedClients[i],	cl->pers.netname );
		if (g_singlePlayer.integer && g_gametype.integer == GT_TOURNAMENT) {
			if (g_entities[cl - level.clients].r.svFlags & SVF_BOT && cl->ps.persistant[PERS_RANK] == 0) {
				won = qfalse;
			}
		}
	}

	if (g_singlePlayer.integer) {
		if (g_gametype.integer >= GT_CTF) {
			won = level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE];
		}
		trap_SendConsoleCommand( EXEC_APPEND, (won) ? "spWin\n" : "spLose\n" );
	}
}

qboolean gDidDuelStuff = qfalse; //gets reset on game reinit

/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
wait 10 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady;
	int			i;
	gclient_t	*cl;
	int			readyMask;

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	if ( g_gametype.integer == GT_TOURNAMENT && !gDidDuelStuff &&
		(level.time > level.intermissiontime + 2000) )
	{
		gDidDuelStuff = qtrue;

		if ( g_austrian.integer )
		{
			G_LogPrintf("Duel Results:\n");
			//G_LogPrintf("Duel Time: %d\n", level.time );
			G_LogPrintf("winner: %s, score: %d, wins/losses: %d/%d\n", 
				level.clients[level.sortedClients[0]].pers.netname,
				level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE],
				level.clients[level.sortedClients[0]].sess.wins,
				level.clients[level.sortedClients[0]].sess.losses );
			G_LogPrintf("loser: %s, score: %d, wins/losses: %d/%d\n", 
				level.clients[level.sortedClients[1]].pers.netname,
				level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE],
				level.clients[level.sortedClients[1]].sess.wins,
				level.clients[level.sortedClients[1]].sess.losses );
		}
		// if we are running a tournement map, kick the loser to spectator status,
		// which will automatically grab the next spectator and restart
		if (!DuelLimitHit())
		{
			if (level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE] ==
				level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE] &&
				level.clients[level.sortedClients[0]].pers.connected == CON_CONNECTED &&
				level.clients[level.sortedClients[1]].pers.connected == CON_CONNECTED)
			{
				RemoveDuelDrawLoser();
			}
			else
			{
				RemoveTournamentLoser();
			}

			AddTournamentPlayer();

			if ( g_austrian.integer )
			{
				G_LogPrintf("Duel Initiated: %s %d/%d vs %s %d/%d, kill limit: %d\n", 
					level.clients[level.sortedClients[0]].pers.netname,
					level.clients[level.sortedClients[0]].sess.wins,
					level.clients[level.sortedClients[0]].sess.losses,
					level.clients[level.sortedClients[1]].pers.netname,
					level.clients[level.sortedClients[1]].sess.wins,
					level.clients[level.sortedClients[1]].sess.losses,
					g_fraglimit.integer );
			}
			
			if (level.numPlayingClients >= 2)
			{
				trap_SetConfigstring ( CS_CLIENT_DUELISTS, va("%i|%i", level.sortedClients[0], level.sortedClients[1] ) );
				trap_SetConfigstring ( CS_CLIENT_DUELWINNER, "-1" );

				gDuelist1 = level.sortedClients[0];
				gDuelist2 = level.sortedClients[1];
			}

			return;	
		}

		if ( g_austrian.integer )
		{
			G_LogPrintf("Duel Tournament Winner: %s wins/losses: %d/%d\n", 
				level.clients[level.sortedClients[0]].pers.netname,
				level.clients[level.sortedClients[0]].sess.wins,
				level.clients[level.sortedClients[0]].sess.losses );
		}
		//this means we hit the duel limit so reset the wins/losses
		//but still push the loser to the back of the line, and retain the order for
		//the map change
		if (level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE] ==
			level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE] &&
			level.clients[level.sortedClients[0]].pers.connected == CON_CONNECTED &&
			level.clients[level.sortedClients[1]].pers.connected == CON_CONNECTED)
		{
			RemoveDuelDrawLoser();
		}
		else
		{
			RemoveTournamentLoser();
		}

		AddTournamentPlayer();

		if (level.numPlayingClients >= 2)
		{
			trap_SetConfigstring ( CS_CLIENT_DUELISTS, va("%i|%i", level.sortedClients[0], level.sortedClients[1] ) );
			trap_SetConfigstring ( CS_CLIENT_DUELWINNER, "-1" );

			gDuelist1 = level.sortedClients[0];
			gDuelist2 = level.sortedClients[1];
		}
	}

	if (g_gametype.integer == GT_TOURNAMENT && !gDuelExit)
	{ //in duel, we have different behaviour for between-round intermissions
		if ( level.time > level.intermissiontime + 4000 )
		{ //automatically go to next after 4 seconds
			ExitLevel();
			return;
		}

		for (i=0 ; i< g_maxclients.integer ; i++)
		{ //being in a "ready" state is not necessary here, so clear it for everyone
		  //yes, I also thinking holding this in a ps value uniquely for each player
		  //is bad and wrong, but it wasn't my idea.
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED )
			{
				continue;
			}
			cl->ps.stats[STAT_CLIENTS_READY] = 0;
		}
		return;
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// if nobody wants to go, clear timer
	if ( !ready ) {
		level.readyToExit = qfalse;
		return;
	}

	// if everyone wants to go, go now
	if ( !notReady ) {
		ExitLevel();
		return;
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited ten seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + 10000 ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}
	
	if ( g_gametype.integer >= GT_TEAM ) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void CheckExitRules( void ) {
 	int			i;
	gclient_t	*cl;
	// if at the intermission, wait for all non-bots to
	// signal ready, then go to next level
	if ( level.intermissiontime ) {
		CheckIntermissionExit ();
		return;
	}

	if (gDoSlowMoDuel)
	{ //don't go to intermission while in slow motion
		return;
	}

	if (gEscaping)
	{
		int i = 0;
		int numLiveClients = 0;

		while (i < MAX_CLIENTS)
		{
			if (g_entities[i].inuse && g_entities[i].client && g_entities[i].health > 0)
			{
				if (g_entities[i].client->sess.sessionTeam != TEAM_SPECTATOR &&
					!(g_entities[i].client->ps.pm_flags & PMF_FOLLOW))
				{
					numLiveClients++;
				}
			}

			i++;
		}
		if (gEscapeTime < level.time)
		{
			gEscaping = qfalse;
			LogExit( "Escape time ended." );
			return;
		}
		if (!numLiveClients)
		{
			gEscaping = qfalse;
			LogExit( "Everyone failed to escape." );
			return;
		}
	}

	if ( level.intermissionQueued ) {
		int time = (g_singlePlayer.integer) ? SP_INTERMISSION_DELAY_TIME : INTERMISSION_DELAY_TIME;
		if ( level.time - level.intermissionQueued >= time ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
		return;
	}

	// check for sudden death
	if ( ScoreIsTied() ) {
		// always wait for sudden death
		if (g_gametype.integer != GT_TOURNAMENT || !g_timelimit.integer)
		{
			return;
		}
	}

	if ( g_timelimit.integer && !level.warmupTime ) {
		if ( level.time - level.startTime >= g_timelimit.integer*60000 ) {
//			trap_SendServerCommand( -1, "print \"Timelimit hit.\n\"");
			trap_SendServerCommand( -1, va("print \"%s.\n\"",G_GetStripEdString("SVINGAME", "TIMELIMIT_HIT")));
			LogExit( "Timelimit hit." );
			return;
		}
	}

	if ( level.numPlayingClients < 2 ) {
		return;
	}

	if ( g_gametype.integer < GT_CTF && g_fraglimit.integer ) {
		if ( level.teamScores[TEAM_RED] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, va("print \"Red %s\n\"", G_GetStripEdString("SVINGAME", "HIT_THE_KILL_LIMIT")) );
			LogExit( "Kill limit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, va("print \"Blue %s\n\"", G_GetStripEdString("SVINGAME", "HIT_THE_KILL_LIMIT")) );
			LogExit( "Kill limit hit." );
			return;
		}

		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( cl->sess.sessionTeam != TEAM_FREE ) {
				continue;
			}

			if ( g_gametype.integer == GT_TOURNAMENT && g_duel_fraglimit.integer && cl->sess.wins >= g_duel_fraglimit.integer )
			{
				LogExit( "Duel limit hit." );
				gDuelExit = qtrue;
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " hit the win limit.\n\"",
					cl->pers.netname ) );
				return;
			}

			if ( cl->ps.persistant[PERS_SCORE] >= g_fraglimit.integer ) {
				LogExit( "Kill limit hit." );
				gDuelExit = qfalse;
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"",
												cl->pers.netname,
												G_GetStripEdString("SVINGAME", "HIT_THE_KILL_LIMIT")
												) 
										);
				return;
			}
		}
	}

	if ( g_gametype.integer >= GT_CTF && g_capturelimit.integer ) {

		if ( level.teamScores[TEAM_RED] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}
	}
}



/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/

/*
=============
CheckTournament

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournament( void ) {
	// check because we run 3 game frames before calling Connect and/or ClientBegin
	// for clients on a map_restart
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		// pull in a spectator if needed
		if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();

			if (level.numPlayingClients >= 2)
			{
				trap_SetConfigstring ( CS_CLIENT_DUELISTS, va("%i|%i", level.sortedClients[0], level.sortedClients[1] ) );
				gDuelist1 = level.sortedClients[0];
				gDuelist2 = level.sortedClients[1];
			}
		}

		if (level.numPlayingClients >= 2)
		{
			if (gDuelist1 == -1 ||
				gDuelist2 == -1)
			{
				trap_SetConfigstring ( CS_CLIENT_DUELISTS, va("%i|%i", level.sortedClients[0], level.sortedClients[1] ) );
				gDuelist1 = level.sortedClients[0];
				gDuelist2 = level.sortedClients[1];
				if ( g_austrian.integer )
				{
					G_LogPrintf("Duel Initiated: %s %d/%d vs %s %d/%d, kill limit: %d\n", 
						level.clients[level.sortedClients[0]].pers.netname,
						level.clients[level.sortedClients[0]].sess.wins,
						level.clients[level.sortedClients[0]].sess.losses,
						level.clients[level.sortedClients[1]].pers.netname,
						level.clients[level.sortedClients[1]].sess.wins,
						level.clients[level.sortedClients[1]].sess.losses,
						g_fraglimit.integer );
				}
				//trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
				//FIXME: This seems to cause problems. But we'd like to reset things whenever a new opponent is set.
			}
		}

		//rww - It seems we have decided there will be no warmup in duel.
		//if (!g_warmup.integer)
		{ //don't care about any of this stuff then, just add people and leave me alone
			level.warmupTime = 0;
			return;
		}
#if 0
		// if we don't have two players, go back to "waiting for players"
		if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return;
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			if ( level.numPlayingClients == 2 ) {
				// fudge by -1 to account for extra delays
				level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;

				if (level.warmupTime < (level.time + 3000))
				{ //rww - this is an unpleasent hack to keep the level from resetting completely on the client (this happens when two map_restarts are issued rapidly)
					level.warmupTime = level.time + 3000;
				}
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
#endif
	} else if ( level.warmupTime != 0 ) {
		int		counts[TEAM_NUM_TEAMS];
		qboolean	notEnough = qfalse;

		if ( g_gametype.integer > GT_TEAM ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

			if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}

		if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return; // still waiting for team members
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	}
}


/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( level.voteExecuteTime && level.voteExecuteTime < level.time ) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );

		if (level.votingGametype)
		{
			if (trap_Cvar_VariableIntegerValue("g_gametype") != level.votingGametypeTo)
			{ //If we're voting to a different game type, be sure to refresh all the map stuff
				const char *nextMap = G_RefreshNextMap(level.votingGametypeTo, qtrue);

				if (nextMap && nextMap[0])
				{
					trap_SendConsoleCommand( EXEC_APPEND, va("map %s\n", nextMap ) );
				}

			}
			else
			{ //otherwise, just leave the map until a restart
				G_RefreshNextMap(level.votingGametypeTo, qfalse);
			}

			if (g_fraglimitVoteCorrection.integer)
			{ //This means to auto-correct fraglimit when voting to and from duel.
				int currentGT = trap_Cvar_VariableIntegerValue("g_gametype");
				int currentFL = trap_Cvar_VariableIntegerValue("fraglimit");

				if (level.votingGametypeTo == GT_TOURNAMENT && currentGT != GT_TOURNAMENT)
				{
					if (currentFL > 3 || !currentFL)
					{ //if voting to duel, and fraglimit is more than 3 (or unlimited), then set it down to 3
						trap_SendConsoleCommand(EXEC_APPEND, "fraglimit 3\n");
					}
				}
				else if (level.votingGametypeTo != GT_TOURNAMENT && currentGT == GT_TOURNAMENT)
				{
					if (currentFL && currentFL < 20)
					{ //if voting from duel, an fraglimit is less than 20, then set it up to 20
						trap_SendConsoleCommand(EXEC_APPEND, "fraglimit 20\n");
					}
				}
			}

			level.votingGametype = qfalse;
			level.votingGametypeTo = 0;
		}
	}
	if ( !level.voteTime || level.jkmodLocals.pauseTime > level.time ) { // Tr!Force: [Pause] Check vote time
		return;
	}
	// Tr!Force: [Vote] Check min/max vote pass
	if ( jkcvar_voteMaxPass.integer )
	{
		if ( level.time - level.voteTime >= VOTE_TIME || (level.voteYes + level.voteNo) == level.numVotingClients )
		{
			if ( level.voteYes > level.voteNo )
			{
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEPASSED")) );
				level.voteExecuteTime = level.time + 3000;
			}
			else
			{
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEFAILED")) );
			}
		} 
		else 
		{
			if ( level.voteYes > level.numVotingClients )
			{
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEPASSED")) );
				level.voteExecuteTime = level.time + 3000;
			}
			else if ( level.voteNo >= level.numVotingClients )
			{
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEFAILED")) );
			}
			else 
			{
				return;
			}
		}
	}
	else
	{
		if ( level.time - level.voteTime >= VOTE_TIME ) {
			trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEFAILED")) );
		} else {
			if ( level.voteYes > level.numVotingClients/2 ) {
				// execute the command, then remove the vote
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEPASSED")) );
				level.voteExecuteTime = level.time + 3000;
			} else if ( level.voteNo >= level.numVotingClients/2 ) {
				// same behavior as a timeout
				trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEFAILED")) );
			} else {
				// still waiting for a majority
				return;
			}
		}
	}

	// Tr!Force: [Vote] Show vote results
	if (strstr(level.voteDisplayString, "Poll:"))
	{
		trap_SendServerCommand(-1, va("cp \"%s\n^7Yes: ^2%d ^7No: ^1%d\"", level.voteDisplayString, level.voteYes, level.voteNo));
	}
	if (jkcvar_voteResults.integer)
	{
		trap_SendServerCommand(-1, va("print \"Vote results: Yes: ^2%d ^7No: ^1%d\n\"", level.voteYes, level.voteNo));
	}

	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );

}

/*
==================
PrintTeam
==================
*/
void PrintTeam(team_t team, char *message) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		trap_SendServerCommand( i, message );
	}
}

/*
==================
SetLeader
==================
*/
void SetLeader(team_t team, int client) {
	int i;

	if ( level.clients[client].pers.connected == CON_DISCONNECTED ) {
		PrintTeam(team, va("print \"%s" S_COLOR_WHITE " is not connected\n\"", level.clients[client].pers.netname) );
		return;
	}
	if (level.clients[client].sess.sessionTeam != team) {
		PrintTeam(team, va("print \"%s" S_COLOR_WHITE " is not on the team anymore\n\"", level.clients[client].pers.netname) );
		return;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader) {
			level.clients[i].sess.teamLeader = qfalse;
			ClientUserinfoChanged(i);
		}
	}
	level.clients[client].sess.teamLeader = qtrue;
	ClientUserinfoChanged( client );
	PrintTeam(team, va("print \"%s" S_COLOR_WHITE " %s\n\"", level.clients[client].pers.netname, G_GetStripEdString("SVINGAME", "NEWTEAMLEADER")) );
}

/*
==================
CheckTeamLeader
==================
*/
void CheckTeamLeader( team_t team ) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader)
			break;
	}
	if (i >= level.maxclients) {
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			if (!(g_entities[i].r.svFlags & SVF_BOT)) {
				level.clients[i].sess.teamLeader = qtrue;
				break;
			}
		}
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			level.clients[i].sess.teamLeader = qtrue;
			break;
		}
	}
}

/*
==================
CheckTeamVote
==================
*/
void CheckTeamVote( team_t team ) {
	int cs_offset;

	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !level.teamVoteTime[cs_offset] ) {
		return;
	}
	if ( level.time - level.teamVoteTime[cs_offset] >= VOTE_TIME ) {
		trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "TEAMVOTEFAILED")) );
	} else {
		if ( level.teamVoteYes[cs_offset] > level.numteamVotingClients[cs_offset]/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "TEAMVOTEPASSED")) );
			//
			if ( !Q_strncmp( "leader", level.teamVoteString[cs_offset], 6) ) {
				//set the team leader
				SetLeader(team, atoi(level.teamVoteString[cs_offset] + 7));
			}
			else {
				trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.teamVoteString[cs_offset] ) );
			}
		} else if ( level.teamVoteNo[cs_offset] >= level.numteamVotingClients[cs_offset]/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "TEAMVOTEFAILED")) );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.teamVoteTime[cs_offset] = 0;
	trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset, "" );

}


/*
==================
CheckCvars
==================
*/
void CheckCvars( void ) {
	static int lastMod = -1;
	
	if ( VALIDCVAR(jkcvar_serverClosed.string) ) return; // Tr!Force: [JKMod] Server closed on top

	if ( g_password.modificationCount != lastMod ) {
		lastMod = g_password.modificationCount;
		if ( *g_password.string && Q_stricmp( g_password.string, "none" ) ) {
			trap_Cvar_Set( "g_needpass", "1" );
		} else {
			trap_Cvar_Set( "g_needpass", "0" );
		}
	}
}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}
	
	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}
	ent->think (ent);
}

int g_LastFrameTime = 0;
int g_TimeSinceLastFrame = 0;

qboolean gDoSlowMoDuel = qfalse;
int gSlowMoDuelTime = 0;

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/

void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	int			msec;
	int start, end;

	if (gDoSlowMoDuel)
	{
		if (level.restarted)
		{
			char buf[128];
			float tFVal = 0;

			trap_Cvar_VariableStringBuffer("timescale", buf, sizeof(buf));

			tFVal = atof(buf);

			trap_Cvar_Set("timescale", "1");
			if (tFVal == 1.0f)
			{
				gDoSlowMoDuel = qfalse;
			}
		}
		else
		{
			float timeDif = (level.time - gSlowMoDuelTime); //difference in time between when the slow motion was initiated and now
			float useDif = 0; //the difference to use when actually setting the timescale

			// Tr!Force: [JKMod] Slow-mo duel end workaround
			if (jkcvar_duelEndSlow.integer)
			{
				char buf[128];
				float tFVal = 0;
				float tScale = jkcvar_duelEndSlowScale.value;

				trap_Cvar_VariableStringBuffer("timescale", buf, sizeof(buf));
				tFVal = atof(buf);

				if (tScale < 0.1 || tScale > 0.9) tScale = 0.5f;

				if (timeDif < 1150)
				{
					if (tFVal != tScale) trap_Cvar_Set("timescale", va("%f", tScale));
				}
				else
				{
					if (tFVal != 1.0f) trap_Cvar_Set("timescale", "1");
					if (timeDif > 1500 && tFVal == 1.0f) gDoSlowMoDuel = qfalse;
				}
			}
			else
			{
				if (timeDif < 150)
				{
					trap_Cvar_Set("timescale", "0.1f");
				}
				else if (timeDif < 1150)
				{
					useDif = (timeDif/1000); //scale from 0.1 up to 1
					if (useDif < 0.1)
					{
						useDif = 0.1;
					}
					if (useDif > 1.0)
					{
						useDif = 1.0;
					}
					trap_Cvar_Set("timescale", va("%f", useDif));
				}
				else
				{
					char buf[128];
					float tFVal = 0;

					trap_Cvar_VariableStringBuffer("timescale", buf, sizeof(buf));

					tFVal = atof(buf);

					trap_Cvar_Set("timescale", "1");
					if (timeDif > 1500 && tFVal == 1.0f)
					{
						gDoSlowMoDuel = qfalse;
					}
				}
			}
		}
	}

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	msec = level.time - level.previousTime;

	g_TimeSinceLastFrame = (level.time - g_LastFrameTime);

	// Tr!Force: [Pause] Check paused frame
	if (JKMod_PauseFrameCheck(levelTime)) {
		JKMod_PauseFrameRun();
		return;
	}

	// get any cvar changes
	G_UpdateCvars();

	//
	// go through all allocated objects
	//
	start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( ent->s.event ) {
				ent->s.event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ent->client->ps.externalEvent = 0;
					// predicted events should never be set to zero
					//ent->client->ps.events[0] = 0;
					//ent->client->ps.events[1] = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				if (ent->s.eFlags & EF_SOUNDTRACKER)
				{ //don't trigger the event again..
					ent->s.event = 0;
					ent->s.eventParm = 0;
					ent->s.eType = 0;
					ent->eventTime = 0;
				}
				else
				{
					G_FreeEntity( ent );
					continue;
				}
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

		if ( ent->s.eType == ET_MISSILE ) {
			G_RunMissile( ent );
			continue;
		}

		if ( ent->s.eType == ET_ITEM || ent->physicsObject ) {
			G_RunItem( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER ) {
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) 
		{
			G_CheckClientTimeouts ( ent );
			
			if((!level.intermissiontime)&&!(ent->client->ps.pm_flags&PMF_FOLLOW) && ent->client->sess.sessionTeam != TEAM_SPECTATOR)
			{
				WP_ForcePowersUpdate(ent, &ent->client->pers.cmd );
				WP_SaberPositionUpdate(ent, &ent->client->pers.cmd);
			}
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}
end = trap_Milliseconds();

	trap_ROFF_UpdateEntities();

start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
end = trap_Milliseconds();

	// Tr!Force: [JKMod] Check server news
	JKMod_ServerNewsCheck();

	// Tr!Force: [JKMod] Check server idle
	JKMod_ServerIdleCheck();

	// see if it is time to do a tournement restart
	CheckTournament();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// cancel vote if timed out
	CheckVote();

	// check team votes
	CheckTeamVote( TEAM_RED );
	CheckTeamVote( TEAM_BLUE );

	// for tracking changes
	CheckCvars();

	if (g_listEntity.integer) {
		for (i = 0; i < MAX_GENTITIES; i++) {
			G_Printf("%4i: %s\n", i, g_entities[i].classname);
		}
		trap_Cvar_Set("g_listEntity", "0");
	}

	//At the end of the frame, send out the ghoul2 kill queue, if there is one
	G_SendG2KillQueue();


	if (gQueueScoreMessage)
	{
		if (gQueueScoreMessageTime < level.time)
		{
			SendScoreboardMessageToAllClients();

			gQueueScoreMessageTime = 0;
			gQueueScoreMessage = 0;
		}
	}

	g_LastFrameTime = level.time;

	// suppress unused-but-set warnings
	(void)start;
	(void)end;
	(void)msec;
}

const char *G_GetStripEdString(char *refSection, char *refName)
{
	/*
	static char text[1024]={0};
	trap_SP_GetStringTextString(va("%s_%s", refSection, refName), text, sizeof(text));
	return text;
	*/

	//Well, it would've been lovely doing it the above way, but it would mean mixing
	//languages for the client depending on what the server is. So we'll mark this as
	//a striped reference with @@@ and send the refname to the client, and when it goes
	//to print it will get scanned for the striped reference indication and dealt with
	//properly.
	static char text[1024]={0};
	Com_sprintf(text, sizeof(text), "@@@%s", refName);
	return text;
}

// On linux rand() behaves different than on Winodws or in a qvm, ...
static int myRandSeed = 0;
void	mysrand( unsigned seed ) {
	myRandSeed = seed;
}

int		myrand( void ) {
	myRandSeed = (69069 * myRandSeed + 1);
	return myRandSeed & 0x7fff;
}

void G_StringAppendSubstring( char *dst, size_t dstSize, const char *src, size_t srcLen )
{
	Q_strcat( dst, strlen(dst)+srcLen+1 >= dstSize ? dstSize : strlen(dst)+srcLen+1, src );
}

/*
================
MV_BBoxToTime2

This function uses an entity state's time2 value to encode the missing 3 bbox values that are not encoded by trap_LinkEntity already.
This function is supposed to be called for solid entities with a non-symmetric bbox after the entity was linked.
================
*/
void MV_BBoxToTime2( gentity_t *ent )
{
	int maxs1, mins0, mins1;

	// Only do this if the entity is solid (same condition as in the engine when calling trap_LinkEntity)
	if ( !(ent->r.contents & (CONTENTS_SOLID|CONTENTS_BODY)) )
		return;

	if ( !level.bboxEncoding )
	{ // Okay, from now on we're encoding maxs[1], mins[0] and mins[1] in s.time2; let the clients know about it
		level.bboxEncoding = qtrue;
		MV_UpdateSvFlags();
	}

	// When we call trap_LinkEntity the engine encodes maxs[0], mins[2] and maxs[2] as r.solid
	// However as that's not enough to properly predict bboxes on the client we additionally encode
	// the missing three values (maxs[1], mins[0] and mins[1]) as s.time2 for mvsdk clients to use in
	// prediction. The values are encoded similar to the way the engine encodes them (>= 1).

	maxs1 = ent->r.maxs[1];
	if ( maxs1 < 1 ) maxs1 = 1;
	if ( maxs1 > 255 ) maxs1 = 255;

	mins0 = -(ent->r.mins[0]);
	if ( mins0 < 1 ) mins0 = 1;
	if ( mins0 > 255 ) mins0 = 255;

	mins1 = -(ent->r.mins[1]);
	if ( mins1 < 1 ) mins1 = 1;
	if ( mins1 > 255 ) mins1 = 255;

	// Encode the values for prediction
	ent->s.time2 = (mins1 << 16) | (mins0 << 8) | maxs1;
}

/*
================
MV_ModelindexToTime2

This function uses an entity state's time2 value to transmit the modelindex to work around the 8 bit modelindex limit.
This function is supposed to be called for all entities after assigning the brushmodel.
================
*/
void MV_ModelindexToTime2( gentity_t *ent )
{
	// Don't do this if the server hasn't enabled it.
	if ( !(g_submodelWorkaround.integer & 2) )
		return;

	if ( !level.modelindexTime2 )
	{ // Let clients know that a modelindex can be found in the time2 value now.
		level.modelindexTime2 = qtrue;
		MV_UpdateSvFlags();
	}

	// The original idea was to only store modelindex >= 255 in time2 and signal this by setting modelindex to 255, however
	// as the serverside engine still needs to know the correct modelindex we just copy the modelindex over to time2 and
	// work with that.
	ent->s.time2 = ent->s.modelindex;
}

