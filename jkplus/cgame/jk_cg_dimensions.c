/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main dimension stuff...
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header

// Undo re-routing for calls made from here
#undef trap_S_StartSound
#undef trap_S_AddLoopingSound
#undef trap_S_AddRealLoopingSound

// Re-declare trace function
void trap_S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx);
void trap_S_AddLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx);
void trap_S_AddRealLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx);

/*
=====================================================================
Check player dimension
=====================================================================
*/
qboolean JKMod_CG_CheckDimension(int entNumber)
{
	// Check server cvar
	if (cgs.jkmodCvar.altDimensions)
	{
		// Duel dimension
		if (((cgs.jkmodCvar.altDimensions & (1 << DIMENSION_DUEL)) || jkcvar_cg_privateDuel.integer) && entNumber != cg.predictedPlayerState.clientNum && cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR)
		{
			if (cg.predictedPlayerState.stats[JK_DIMENSION] == JK_DUEL_IN)
			{
				if (!(entNumber != cg.snap->ps.clientNum && cg_entities[entNumber].currentState.bolt1 == JK_DUEL_IN))
					return qfalse;
			}
			else if (cg_entities[entNumber].currentState.bolt1 == JK_DUEL_IN)
				return qfalse;
		}
		// Guns dimension
		if (cgs.jkmodCvar.altDimensions & (1 << DIMENSION_GUNS) && entNumber != cg.predictedPlayerState.clientNum && cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR)
		{
			if (cg.predictedPlayerState.stats[JK_DIMENSION] == JK_GUNS_IN)
			{
				if (!(entNumber != cg.snap->ps.clientNum && cg_entities[entNumber].currentState.bolt1 == JK_GUNS_IN))
					return qfalse;
			}
			else if (cg_entities[entNumber].currentState.bolt1 == JK_GUNS_IN)
				return qfalse;
		}
		// Race dimension
		if (cgs.jkmodCvar.altDimensions & (1 << DIMENSION_RACE) && entNumber != cg.predictedPlayerState.clientNum && cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR)
		{
			if (cg.predictedPlayerState.stats[JK_DIMENSION] == JK_RACE_IN)
			{
				if (!(entNumber != cg.snap->ps.clientNum && cg_entities[entNumber].currentState.bolt1 == JK_RACE_IN))
					return qfalse;
			}
			else if (cg_entities[entNumber].currentState.bolt1 == JK_RACE_IN)
				return qfalse;
		}
	}

	// Final pass
	return qtrue;
}

/*
=====================================================================
Custom start sound function
=====================================================================
*/
void JKMod_trap_S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx) 
{
	if (!(entityNum >= 0 && entityNum < MAX_CLIENTS && entityNum != cg.snap->ps.clientNum && !JKMod_CG_CheckDimension(entityNum)) &&
		!(entityNum >= MAX_CLIENTS && cg_entities[entityNum].currentState.otherEntityNum != ENTITYNUM_NONE && !JKMod_CG_CheckDimension(cg_entities[entityNum].currentState.otherEntityNum))) 
	{
		trap_S_StartSound(origin, entityNum, entchannel, sfx);
	}
}

/*
=====================================================================
Custom add looping sound function
=====================================================================
*/
void JKMod_trap_S_AddLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx) 
{
	if (!(entityNum >= 0 && entityNum < MAX_CLIENTS && entityNum != cg.snap->ps.clientNum && !JKMod_CG_CheckDimension(entityNum)) &&
		!(entityNum >= MAX_CLIENTS && cg_entities[entityNum].currentState.otherEntityNum != ENTITYNUM_NONE && !JKMod_CG_CheckDimension(cg_entities[entityNum].currentState.otherEntityNum)))
	{
		trap_S_AddLoopingSound(entityNum, origin, velocity, sfx);
	}
}

/*
=====================================================================
Custom add real looping sound function
=====================================================================
*/
void JKMod_trap_S_AddRealLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx) 
{
	if (!(entityNum >= 0 && entityNum < MAX_CLIENTS && entityNum != cg.snap->ps.clientNum && !JKMod_CG_CheckDimension(entityNum)) &&
		!(entityNum >= MAX_CLIENTS && cg_entities[entityNum].currentState.otherEntityNum != ENTITYNUM_NONE && !JKMod_CG_CheckDimension(cg_entities[entityNum].currentState.otherEntityNum)))
	{
		trap_S_AddRealLoopingSound(entityNum, origin, velocity, sfx);
	}
}
