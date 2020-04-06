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

vmCvar_t	jkcvar_serverClosed;
vmCvar_t	jkcvar_serverClosedIP;
vmCvar_t	jkcvar_serverClosedBroadcast;

vmCvar_t	jkcvar_allowBlackNames;
vmCvar_t	jkcvar_allowMultiDuel;
vmCvar_t	jkcvar_allowDuelChat;
vmCvar_t	jkcvar_noDuplicatedNames;

vmCvar_t	jkcvar_voteCustomMap;
vmCvar_t	jkcvar_voteGameplay;
vmCvar_t	jkcvar_itemForcePhysics;
vmCvar_t	jkcvar_duelStartHealth;
vmCvar_t	jkcvar_duelStartShield;
vmCvar_t	jkcvar_duelEndStats;

vmCvar_t	jkcvar_dropFlag;
vmCvar_t	jkcvar_dropFlagTime;
vmCvar_t	jkcvar_damagePlums;
vmCvar_t	jkcvar_pauseGame;
vmCvar_t	jkcvar_pauseGameCenterPrint;

vmCvar_t	jkcvar_emotesEnabled;
vmCvar_t	jkcvar_emotesBreak;
vmCvar_t	jkcvar_emotesFreeze;
vmCvar_t	jkcvar_emotesPunchDamage;

static cvarTable_t	JKPlusCvarTable[] = {

	{ &jkcvar_test1,					"jk_test1",					"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_test2,					"jk_test2",					"0",					CVAR_ARCHIVE,		0, qtrue },
	
	{ &jkcvar_serverClosed,				"jk_serverClosed",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_serverClosedIP,			"jk_serverClosedIP",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_serverClosedBroadcast,	"jk_serverClosedBroadcast",	"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkcvar_allowBlackNames,			"jk_allowBlackNames",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_allowMultiDuel,			"jk_allowMultiDuel",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_allowDuelChat,			"jk_allowDuelChat",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_noDuplicatedNames,		"jk_noDuplicatedNames",		"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkcvar_voteCustomMap,			"jk_voteCustomMap",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_voteGameplay,				"jk_voteGameplay",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_itemForcePhysics,			"jk_itemForcePhysics",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_duelStartHealth,			"jk_duelStartHealth",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_duelStartShield,			"jk_duelStartShield",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_duelEndStats,				"jk_duelEndStats",			"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkcvar_dropFlag,					"jk_dropFlag",				"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_dropFlagTime,				"jk_dropFlagTime",			"15",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_damagePlums,				"jk_damagePlums",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_pauseGame,				"jk_pauseGame",				"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_pauseGameCenterPrint,		"jk_pauseGameCenterPrint",	"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkcvar_emotesEnabled,			"jk_emotesEnabled",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_emotesBreak,				"jk_emotesBreak",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_emotesFreeze,				"jk_emotesFreeze",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkcvar_emotesPunchDamage,		"jk_emotesPunchDamage",		"0",					CVAR_ARCHIVE,		0, qtrue },

};

static int JKPlusCvarTableSize = sizeof(JKPlusCvarTable) / sizeof(JKPlusCvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

void JKPlus_G_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for(i = 0, cv = JKPlusCvarTable; i < JKPlusCvarTableSize; i++, cv++)
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

void JKPlus_G_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for(i = 0, cv = JKPlusCvarTable; i < JKPlusCvarTableSize; i++, cv++)
	{
		if(cv->vmCvar)
		{
			trap_Cvar_Update(cv->vmCvar);

			if(cv->modificationCount != cv->vmCvar->modificationCount)
			{
				cv->modificationCount = cv->vmCvar->modificationCount;

				if(cv->trackChange)
				{
					if (cv->vmCvar != &jkcvar_pauseGame)
					{
						trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string));
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

						if (jkcvar_pauseGameCenterPrint.integer == 1)
						{
							trap_SendServerCommand(-1, "cp \"Game paused by the server\n\"");
						}
						else
						{
							trap_SendServerCommand(-1, "print \"Game paused by the server\n\"");
						}

						// Save player viewangles
						for (num = 0, ent = g_entities; num < MAX_CLIENTS; ++num, ++ent) 
						{
							if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED) 
							{
								VectorCopy(ent->client->ps.viewangles, ent->client->JKPlusPauseSavedView);
							}
						}
					}
					else 
					{
						// Set unpause
						level.JKPlusUnpauseClient = -1;

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

							if (jkcvar_pauseGameCenterPrint.integer == 1)
							{
								trap_SendServerCommand(-1, va("cp \"Game unpaused after %s\n\"", JKPlus_msToString(pausedGameTime)));
							}
							else
							{
								trap_SendServerCommand(-1, va("print \"Game unpaused after %s\n\"", JKPlus_msToString(pausedGameTime)));
							}

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
									if (ent->client->pers.enterTime > level.time)
									{
										ent->client->pers.enterTime = level.time;
									}

									if (ent->client->pers.teamState.flagsince) 
									{
										// If holding a flag update the timer so its not counting pause time
										ent->client->pers.teamState.flagsince += pausedGameTime;

										if (ent->client->pers.teamState.flagsince > level.time)
										{
											ent->client->pers.teamState.flagsince = level.time;
										}
									}
									if (ent->client->pers.teamState.lastreturnedflag) 
									{
										ent->client->pers.teamState.lastreturnedflag += pausedGameTime;
									}
									if (ent->client->pers.teamState.lastfraggedcarrier) 
									{
										ent->client->pers.teamState.lastfraggedcarrier += pausedGameTime;
									}
									if (ent->client->pers.teamState.lasthurtcarrier) 
									{
										ent->client->pers.teamState.lasthurtcarrier += pausedGameTime;
									}
									if (ent->client->sess.sessionTeam != TEAM_SPECTATOR) 
									{
										// Restore this player viewangles
										SetClientViewAngle(ent, ent->client->JKPlusPauseSavedView);
									}
								}
							}
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
Main initialization functions
=====================================================================
*/
void JKPlus_G_InitGame(int levelTime, int randomSeed, int restart) {
	int					i;

	G_Printf(S_COLOR_CYAN "------ JKPlus Initialization ------\n");

	// Launch original init game function
	BaseJK2_G_InitGame(levelTime, randomSeed, restart);

	// More stuff ...
}
