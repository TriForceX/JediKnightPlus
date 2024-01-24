/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Trigger functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Check trigger contact
=====================================================================
*/
qboolean JKMod_InTrigger(vec3_t interpOrigin, gentity_t *trigger)
{
	vec3_t	mins, maxs;
	vec3_t	playerMins, playerMaxs;

	VectorSet(playerMins, -15, -15, DEFAULT_MINS_2);
	VectorSet(playerMaxs, 15, 15, DEFAULT_MAXS_2);

	VectorAdd(interpOrigin, playerMins, mins);
	VectorAdd(interpOrigin, playerMaxs, maxs);

	if (trap_EntityContact(mins, maxs, trigger)) return qtrue;
	
	return qfalse;
}

/*
=====================================================================
Check interpolate touch time
=====================================================================
*/
int JKMod_InterpolateTouchTime(gentity_t *activator, gentity_t *trigger)
{
	vec3_t	interpOrigin, delta;
	int lessTime = -1;

	qboolean touched = qfalse;
	qboolean inTrigger;

	VectorCopy(activator->client->ps.origin, interpOrigin);
	VectorScale(activator->s.pos.trDelta, 0.001f, delta);

	while ((inTrigger = JKMod_InTrigger(interpOrigin, trigger)) || !touched) 
	{
		if (inTrigger) touched = qtrue;

		lessTime++;
		VectorSubtract(interpOrigin, delta, interpOrigin);
		
		if (lessTime >= 250) break; 
	}

	return lessTime;
}

/*
=====================================================================
Race trigger functions
=====================================================================
*/

// Start race timer
void JKMod_TimerStart(gentity_t *ent, gentity_t *other, gentity_t *activator) 
{
	int	timeStart;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Set info
	timeStart = JKMod_InterpolateTouchTime(activator, ent);

	// Set timers
	activator->client->ps.duelTime = level.time - timeStart;
	activator->client->pers.jkmodPers.raceStartTime = trap_Milliseconds() - timeStart;

	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer started!\""));
}

// Stop race timer
void JKMod_TimerStop(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	timeLast, timeBest;
	char timeLastStr[32], timeBestStr[32];

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Check timer
	if (!activator->client->pers.jkmodPers.raceStartTime) return;

	// Set info
	timeLast = trap_Milliseconds() - activator->client->pers.jkmodPers.raceStartTime;
	timeBest = !activator->client->pers.jkmodPers.raceBestTime ? timeLast : activator->client->pers.jkmodPers.raceBestTime;

	Q_strncpyz(timeLastStr, JKMod_MsToString(timeLast), sizeof(timeLastStr));
	Q_strncpyz(timeBestStr, JKMod_MsToString(timeBest), sizeof(timeBestStr));

	// Show info
	if (timeLast == timeBest) {
		trap_SendServerCommand(-1, va("print \"%s " S_COLOR_WHITE "has finished the race in [^2%s^7]\n\"", activator->client->pers.netname, timeLastStr));
	} else if (timeLast < timeBest) {
		trap_SendServerCommand(-1, va("print \"%s " S_COLOR_WHITE "has finished the race in [^5%s^7] which is a new personal record!\n\"", activator->client->pers.netname, timeLastStr));
	} else {
		trap_SendServerCommand(-1, va("print \"%s " S_COLOR_WHITE "has finished the race in [^2%s^7] and his record was [^5%s^7]\n\"", activator->client->pers.netname, timeLastStr, timeBestStr));
	}

	// Play sound
	if (timeLast < timeBest) G_Sound(activator, CHAN_AUTO, G_SoundIndex("sound/movers/sec_panel_pass"));

	// Show info
	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer finished!\""));

	// Update timers
	activator->client->pers.jkmodPers.raceLastTime = timeLast;
	activator->client->pers.jkmodPers.raceBestTime = timeLast > timeBest ? timeBest : timeLast;

	// Update client
	ClientUserinfoChanged(activator - g_entities);
	
	// Reset timers
	activator->client->ps.duelTime = 0;
	activator->client->pers.jkmodPers.raceStartTime = 0;
}

// Checkpoint race timer
void JKMod_TimerCheckpoint(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	timeCheck;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Check timer
	if (!activator->client->pers.jkmodPers.raceStartTime) return;

	// Set info
	timeCheck = trap_Milliseconds() - activator->client->pers.jkmodPers.raceStartTime;

	// Show info
	trap_SendServerCommand(activator - g_entities, va("cp \"Checkpoint!\n^3%s\"", JKMod_MsToString(timeCheck)));
}

/*
=====================================================================
Race spawn functions
=====================================================================
*/

// Start race timer
void JKMod_SP_TimerStart(gentity_t *self)
{
	self->use = JKMod_TimerStart;
}
// Stop race timer
void JKMod_SP_TimerStop(gentity_t *self)
{
	self->use = JKMod_TimerStop;
}
// Checkpoint race timer
void JKMod_SP_TimerCheckpoint(gentity_t *self)
{
	self->use = JKMod_TimerCheckpoint;
}
