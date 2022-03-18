/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Main dimension stuff
=====================================================================
*/

#include "../../code/game/g_local.h" // Main header

// Undo re-routing for calls made from here
#undef trap_Trace

// Re-declare trace function
void trap_Trace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

/*
=====================================================================
Dimension data table
=====================================================================
*/
jkmod_dimension_data_t JKModDimensionData[] =
{
	// dimension		weapons		forcepowers	forcelevel		holdables	jetpack		invulnerability	passthrough	speed	 gravity	command		name
    { DIMENSION_FREE,	DEFAULT,	DEFAULT,	DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		DEFAULT, DEFAULT,	"normal",	"Normal" },
    { DIMENSION_GUNS,	6,			262141,		FORCE_LEVEL_1,	qtrue,		qtrue,		qfalse,			qfalse,		250,	 800,		"guns",		"Guns Arena" },
	{ DIMENSION_RACE,	65531,		229373,		FORCE_LEVEL_1,	qfalse,		qfalse,		qtrue,			qtrue,		250,	 800,		"race",		"Race Defrag" },
	{ DIMENSION_SABER,	65531,		163837,		FORCE_LEVEL_1,	qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"saber",	"Saber Only" },
	{ DIMENSION_INSTA,	65503,		262141,		FORCE_LEVEL_3,	qfalse,		qfalse,		qfalse,			qfalse,		415,	 800,		"insta",	"Insta Kill" },
	{ DIMENSION_CHEAT,	DEFAULT,	DEFAULT,	DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"cheats",	"Cheats Mode" },
};

int JKModDimensionDataSize = ARRAY_LEN(JKModDimensionData);

/*
=====================================================================
Dimension index from table
=====================================================================
*/
int JKMod_DimensionIndex(unsigned dimension)
{
	int	i;

	for (i = 0; i < JKModDimensionDataSize; i++)
	{
		if (JKModDimensionData[i].dimension == dimension) return i;
	}
	
	return 0;
}

/*
=====================================================================
Dimension settings function
=====================================================================
*/
void JKMod_DimensionSettings(gentity_t *ent, unsigned dimension)
{
	if (g_gametype.integer == GT_FFA)
	{
		int i = JKMod_DimensionIndex(dimension);

		JKMod_CustomGameSettings(ent, 
			JKModDimensionData[i].weapons,			// Weapons
			JKModDimensionData[i].forcepowers,		// Force
			JKModDimensionData[i].forcelevel,		// Force level
			JKModDimensionData[i].holdables,		// Holdable items
			JKModDimensionData[i].jetpack,			// Jetpack
			JKModDimensionData[i].invulnerability,	// Invulnerability
			JKModDimensionData[i].passthrough,		// Pass-through
			JKModDimensionData[i].speed,			// Speed
			JKModDimensionData[i].gravity			// Gravity
		);
	}
}

/*
=====================================================================
Set dimension change function
=====================================================================
*/
qboolean JKMod_DimensionChange(gentity_t *ent, char *dimension, qboolean say)
{
	int			i;
	char		*print = say ? "cp" : "print";
	char		message[MAX_STRING_CHARS] = { 0 };
	qboolean	baseCmd = qfalse;

	if (!jkcvar_altDimension.integer)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Dimensions are disabled by the server\n\"");
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Join the game first before switch dimension\n\"");
		return qfalse;
	}
	else if (ent->client->ps.pm_type == PM_DEAD)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension while dead\n\"");
		return qfalse;
	}
	else if (ent->client->ps.duelInProgress)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in a private duel\n\"");
		return qfalse;
	}
	else if (g_gametype.integer != GT_FFA)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in this game type\n\"");
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension during pause mode\n\"");
		return qfalse;
	}
	else if (ent->client->jkmodClient.dimensionTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds before change dimension\n\"", ent->client->jkmodClient.dimensionTime));
		return qfalse;
	}
	else
	{
		// Message
		Q_strcat(message, sizeof(message), "^7Usage: ^2!dimension <option>\n^7Option list:\n");

		// Base?
		if (!Q_stricmp(dimension, "base")) baseCmd = qtrue;

		// Start
		for (i = 0; i < JKModDimensionDataSize; i++)
		{
			// Add to message
			Q_strcat(message, sizeof(message), va("^3%s\n", JKModDimensionData[i].command));

			// Command
			if (!Q_stricmp(dimension, JKModDimensionData[i].command) || baseCmd)
			{
				int		dimensionBase = jkcvar_altDimensionBase.integer ? jkcvar_altDimensionBase.integer : DIMENSION_FREE;
				int		dimensionBaseIndex = JKMod_DimensionIndex(dimensionBase);
				int		dimensionIndex = JKModDimensionData[baseCmd ? dimensionBaseIndex : i].dimension;
				char	*dimensionName = JKModDimensionData[baseCmd ? dimensionBaseIndex : i].name;

				// Delay
				ent->client->jkmodClient.dimensionTime = jkcvar_altDimensionTime.integer;

				// Check
				if (!(jkcvar_altDimension.integer & dimensionIndex))
				{
					trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
					return qfalse;
				}
				// Enable
				else if (ent->client->ps.stats[JK_DIMENSION] != dimensionIndex)
				{
					JKMod_DimensionSet(ent, dimensionIndex);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\n\"", dimensionName));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s ^3%s ^7dimension\n\"", ent->client->pers.netname, (dimensionIndex != dimensionBase ? "joined the" : "back to"), dimensionName));
					return qtrue;
				}
				// Disable
				else if (dimensionIndex != dimensionBase)
				{
					JKMod_DimensionSet(ent, dimensionBase);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\n\"", JKModDimensionData[dimensionBaseIndex].name));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " back to ^3%s ^7dimension\n\"", ent->client->pers.netname, JKModDimensionData[dimensionBaseIndex].name));
					return qtrue;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, "print \"You are already in this dimension\n\"");
					return qfalse;
				}
			}
		}

		// Not found
		trap_SendServerCommand(ent - g_entities, va("%s \"%s\n\"", print, (say ? message : "Usage: dimension <option>\nSee ^3/help dimensions ^7for more information")));
		return qfalse;
	}
}

/*
=====================================================================
Dimension basic player/entity check
=====================================================================
*/
qboolean JKMod_DimensionCheck(int ent1, int ent2)
{
	int num1 = g_entities[ent1].jkmodEnt.dimensionNumber;
	int num2 = g_entities[ent2].jkmodEnt.dimensionNumber;
	
	if (ent1 == ENTITYNUM_WORLD) return qtrue;
	if (ent2 == ENTITYNUM_WORLD) return qtrue;
	if (ent1 == ent2) return qtrue;

	return (num1 & num2);
}

/*
=====================================================================
Dimension snapshot ignore and owner check
=====================================================================
*/
void JKMod_DimensionOwnerCheck(int owner, gentity_t *ent)
{
	assert(owner >= 0 && owner < MAX_GENTITIES);

	if (owner >= MAX_CLIENTS && owner < ENTITYNUM_MAX_NORMAL) {
		owner = g_entities[owner].jkmodEnt.dimensionOwner;
		assert(owner < MAX_CLIENTS || owner == ENTITYNUM_NONE || owner == ENTITYNUM_WORLD);
	}

	ent->jkmodEnt.dimensionOwner = owner;

	if (ent - g_entities < MAX_CLIENTS) {
		ent->jkmodEnt.dimensionNumber = jkcvar_altDimensionBase.integer ? jkcvar_altDimensionBase.integer : DIMENSION_FREE;
	} else {
		ent->jkmodEnt.dimensionNumber = DIMENSION_ALL;
	}

	if (mvapi)
	{
		uint8_t *snapshotIgnore = mv_entities[ent->s.number].snapshotIgnore;
		int	i;

		if (owner == ENTITYNUM_WORLD) {
			memset(snapshotIgnore, 0, sizeof(snapshotIgnore[0]) * MAX_CLIENTS);
		} else {
			for (i = 0; i < level.maxclients; i++) {
				if (g_entities[i].inuse) {
					snapshotIgnore[i] = !JKMod_DimensionCheck(owner, i);
				}
			}
		}
	}
}

/*
=====================================================================
Dimension get free number
=====================================================================
*/
unsigned JKMod_DimensionGetFree(void)
{
	unsigned dimension;
	qboolean free;
	int i;

	for (dimension = 1 << DIMENSION_MAX; dimension != 0; dimension <<= 1) 
	{
		free = qtrue;

		for (i = 0; i < level.maxclients; i++) 
		{
			if (!g_entities[i].inuse) continue;

			if ((g_entities[i].jkmodEnt.dimensionNumber & dimension) != 0) 
			{
				free = qfalse;
				break;
			}
		}

		if (free) return dimension;
	}

	assert(0);
	return DIMENSION_FREE;
}

/*
=====================================================================
Dimension snapshot ignore apply
=====================================================================
*/
void JKMod_DimensionSet(gentity_t *ent, unsigned dimension)
{
	int	i;
	int	clientNum = ent->s.number;
	vec3_t spawnOrigin, spawnAngles;

	ent->client->ps.stats[JK_DIMENSION] = ent->client->ps.duelInProgress ? DIMENSION_DUEL : dimension;
	ent->jkmodEnt.dimensionNumber = dimension;

	// Check pass-trough
	if (ent->client->ps.eFlags & JK_PASS_THROUGH) ent->client->ps.eFlags &= ~JK_PASS_THROUGH;

	// Check stuck
	if (JKMod_OthersInBox(ent)) JKMod_AntiStuckBox(ent);

	// Check saber
	if (ent->client->ps.saberEntityNum != ENTITYNUM_NONE) g_entities[ent->client->ps.saberEntityNum].jkmodEnt.dimensionNumber = dimension;

	// Update settings
	ent->client->pers.jkmodPers.customSettingsCount++;

	// Set settings
	JKMod_DimensionSettings(ent, dimension);

	// Get random spawn point
	SelectSpawnPoint(ent->client->ps.origin, spawnOrigin, spawnAngles);
	
	// Set new spawn point
	JKMod_TeleportPlayer(ent, spawnOrigin, spawnAngles, qfalse, 0, NULL, "sound/interface/secret_area");

	if (mvapi)
	{
		for (i = 0; i < level.num_entities; i++) {
			if (g_entities[i].inuse) {
				int owner = g_entities[i].jkmodEnt.dimensionOwner;

				if (JKMod_DimensionCheck(owner, clientNum)) {
					mv_entities[i].snapshotIgnore[clientNum] = 0;
				} else {
					mv_entities[i].snapshotIgnore[clientNum] = 1;
				}
			}
		}

		for (i = 0; i < level.maxclients; i++) {
			if (g_entities[i].inuse) {
				int owner = g_entities[i].jkmodEnt.dimensionOwner;

				if (JKMod_DimensionCheck(owner, clientNum)) {
					mv_entities[clientNum].snapshotIgnore[i] = 0;
				} else {
					mv_entities[clientNum].snapshotIgnore[i] = 1;
				}
			}
		}
	}

	JKMod_Printf(S_COLOR_YELLOW "Client %i changed dimension to %i\n", clientNum, dimension);
}

/*
=====================================================================
Dimension check collide
=====================================================================
*/
qboolean JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2)
{
	int owner1 = ent1->jkmodEnt.dimensionOwner;
	int owner2 = ent2->jkmodEnt.dimensionOwner;

	if (ent1->jkmodEnt.dimensionNumber == DIMENSION_RACE && ent2->jkmodEnt.dimensionNumber == DIMENSION_RACE)
	{
		return (qboolean)!(JKMod_DimensionCheck(owner1, owner2));
	}
	if (ent1->jkmodEnt.dimensionNumber == DIMENSION_FREE && ent2->jkmodEnt.dimensionNumber == DIMENSION_FREE && jkcvar_duelPassThrough.integer)
	{
		return (qboolean)!!(JKMod_DuelIsolationCheck(ent1, ent2));
	}

	return (qboolean)!!(JKMod_DimensionCheck(owner1, owner2));
}

/*
=====================================================================
Dimensions main trace function
=====================================================================
*/
void JKMod_DimensionTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	if (results->entityNum < ENTITYNUM_MAX_NORMAL) 
	{
		gentity_t	*passEnt = g_entities + passEntityNum;
		gentity_t	*ent = g_entities + results->entityNum;

		if (!JKMod_DimensionCollide(ent, passEnt)) 
		{
			int contents;

			contents = ent->r.contents;
			ent->r.contents = 0;
			JKMod_DimensionTrace(results, start, mins, maxs, end, passEntityNum, contentmask);
			ent->r.contents = contents;

			return;
		}
	}

	if (results->startsolid && start != end) 
	{
		trace_t tw;

		JKMod_DimensionTrace(&tw, start, mins, maxs, start, passEntityNum, contentmask);
		results->startsolid = tw.startsolid;
	}
}

/*
=====================================================================
Dimensions check entities in box
=====================================================================
*/
int JKMod_DimensionEntitiesInBox(const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount, int entityNum)
{
	gentity_t	*passEnt = g_entities + entityNum;
	int			fullCount;
	int			count;
	int			i;

	fullCount = trap_EntitiesInBox(mins, maxs, entityList, maxcount);

	for (i = 0, count = 0; i < fullCount; i++) 
	{
		if (JKMod_DimensionCollide(g_entities + entityList[i], passEnt)) 
		{
			entityList[count] = entityList[i];
			count++;
		}
	}

	return count;
}
