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
Race trigger functions
=====================================================================
*/

// Start race timer
void JKMod_TimerStart(gentity_t *ent, gentity_t *other, gentity_t *activator) 
{
	int	i;
	int	clientNum = -1;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Find client
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (activator->client == &level.clients[i]) {
			clientNum = i;
			break;
		}
	}
	if (clientNum == -1) {
		G_Printf("Couldn't find client\n");
		return;
	}

	// Set Info
	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer started!\""));
	level.jkmodLevel.racerStartTime[clientNum] = trap_Milliseconds();
}

// Stop race timer
void JKMod_TimerStop(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	i;
	int	clientNum = -1;
	int	timeSec, timeMin, timeMsec;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Find client
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (activator->client == &level.clients[i]) {
			clientNum = i;
			break;
		}
	}
	if (clientNum == -1) {
		G_Printf("Couldn't find client\n");
		return;
	}

	// Check timer
	if (!level.jkmodLevel.racerStartTime[clientNum]) return;

	// Show info
	timeMsec = trap_Milliseconds() - level.jkmodLevel.racerStartTime[clientNum];
	timeSec = timeMsec / 1000;
	timeMsec -= timeSec * 1000;
	timeMin = timeSec / 60;
	timeSec -= timeMin * 60;

	trap_SendServerCommand(-1, va("print \"%s " S_COLOR_WHITE "has finished the race in [^2%02i:%02i:%03i^7]\n\"", activator->client->pers.netname, timeMin, timeSec, timeMsec));
	trap_SendServerCommand(activator - g_entities, va("cp \"Race timer finished!\""));

	// Reset timer
	level.jkmodLevel.racerStartTime[clientNum] = 0;
}

// Checkpoint race timer
void JKMod_TimerCheckpoint(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	int	i;
	int	clientNum = -1;
	int	timeSec, timeMin, timeMsec;

	// Check dimension
	if (!(jkcvar_altDimension.integer & DIMENSION_RACE)) return;

	// Check client
	if (!activator->client) return;

	// Check mode
	if (activator->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) return;

	// Find client
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (activator->client == &level.clients[i]) {
			clientNum = i;
			break;
		}
	}
	if (clientNum == -1) {
		G_Printf("Couldn't find client\n");
		return;
	}

	// Check timer
	if (!level.jkmodLevel.racerStartTime[clientNum]) return;

	// Show info
	timeMsec = trap_Milliseconds() - level.jkmodLevel.racerStartTime[clientNum];
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
