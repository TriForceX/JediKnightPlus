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

vmCvar_t	jkplus_test1;
vmCvar_t	jkplus_test2;

vmCvar_t	jkplus_serverClosed;
vmCvar_t	jkplus_serverClosedIP;
vmCvar_t	jkplus_serverClosedBroadcast;

vmCvar_t	jkplus_allowBlackNames;
vmCvar_t	jkplus_dropFlag;
vmCvar_t	jkplus_pauseGame;
vmCvar_t	jkplus_pauseGameCenterPrint;

vmCvar_t	jkplus_emotesBreak;
vmCvar_t	jkplus_emotesFreeze;
vmCvar_t	jkplus_emotesEnabled;
vmCvar_t	jkplus_emotesPunchDamage;

static cvarTable_t	JKPlusCvarTable[] = {

	{ &jkplus_test1,					"jk_test1",					"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_test2,					"jk_test2",					"0",					CVAR_ARCHIVE,		0, qtrue },
	
	{ &jkplus_serverClosed,				"jk_serverClosed",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_serverClosedIP,			"jk_serverClosedIP",		"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_serverClosedBroadcast,	"jk_serverClosedBroadcast",	"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkplus_allowBlackNames,			"jk_allowBlackNames",		"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_dropFlag,					"jk_dropFlag",				"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_pauseGame,				"jk_pauseGame",				"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_pauseGameCenterPrint,		"jk_pauseGameCenterPrint",	"1",					CVAR_ARCHIVE,		0, qtrue },
	
	{ &jkplus_emotesBreak,				"jk_emotesBreak",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesFreeze,				"jk_emotesFreeze",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesEnabled,			"jk_emotesEnabled",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesPunchDamage,		"jk_emotesPunchDamage",		"0",					CVAR_ARCHIVE,		0, qtrue },

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
					if (cv->vmCvar != &jkplus_pauseGame)
					{
						trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string));
					}
				}

				// Pause game
				if (cv->vmCvar == &jkplus_pauseGame) 
				{
					int pause = cv->vmCvar->integer ? 1 : 0;
					int num;

					if (pause) 
					{
						gentity_t *ent;
						pauseGameStartTime = level.time;

						if (jkplus_pauseGameCenterPrint.integer == 1)
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

							if (jkplus_pauseGameCenterPrint.integer == 1)
							{
								trap_SendServerCommand(-1, va("cp \"Game unpaused after %s\n\"", JKPlus_MsToString(pausedGameTime)));
							}
							else
							{
								trap_SendServerCommand(-1, va("print \"Game unpaused after %s\n\"", JKPlus_MsToString(pausedGameTime)));
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
