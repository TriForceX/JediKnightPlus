/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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
	static const vec3_t	pmins = {-15, -15, DEFAULT_MINS_2};
	static const vec3_t	pmaxs = {15, 15, DEFAULT_MAXS_2};

	VectorAdd( interpOrigin, pmins, mins );
	VectorAdd( interpOrigin, pmaxs, maxs );

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
	int	clientNum = -1;
	int	lessTime;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Set info
	lessTime = JKMod_InterpolateTouchTime(activator, ent);

	activator->client->ps.duelTime = level.time - lessTime;
	activator->client->pers.jkmodPers.raceStartTime = trap_Milliseconds() - lessTime;

	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer started!\""));
}

// Stop race timer
void JKMod_TimerStop(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	clientNum = -1;
	int	timeSec, timeMin, timeMsec;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Check timer
	if (!activator->client->pers.jkmodPers.raceStartTime) return;

	// Show info
	timeMsec = trap_Milliseconds() - activator->client->pers.jkmodPers.raceStartTime;
	timeSec = timeMsec / 1000;
	timeMsec -= timeSec * 1000;
	timeMin = timeSec / 60;
	timeSec -= timeMin * 60;

	trap_SendServerCommand(-1, va("print \"%s " S_COLOR_WHITE "has finished the race in [^2%02i:%02i:%03i^7]\n\"", activator->client->pers.netname, timeMin, timeSec, timeMsec));
	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer finished!\""));

	// Reset timer
	activator->client->ps.duelTime = 0;
	activator->client->pers.jkmodPers.raceStartTime = 0;
}

// Checkpoint race timer
void JKMod_TimerCheckpoint(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	clientNum = -1;
	int	timeSec, timeMin, timeMsec;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Check timer
	if (!activator->client->pers.jkmodPers.raceStartTime) return;

	// Show info
	timeMsec = trap_Milliseconds() - activator->client->pers.jkmodPers.raceStartTime;
	timeSec = timeMsec / 1000;
	timeMsec -= timeSec * 1000;
	timeMin = timeSec / 60;
	timeSec -= timeMin * 60;

	trap_SendServerCommand(activator - g_entities, va("cp \"Checkpoint!\n^3%02i:%02i:%03i\"", timeMin, timeSec, timeMsec));
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
