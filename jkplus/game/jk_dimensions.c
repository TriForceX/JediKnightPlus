/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main dimension stuff...
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
jkmod_dimension_data_t jkmod_dimension_data[] =
{
	// dimension		weapons		forcepowers	forcelevel		holdables	jetpack		invulnerability	passthrough	speed	 gravity	command		name
    { DIMENSION_FREE,	DEFAULT,	DEFAULT,	DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		DEFAULT, DEFAULT,	"normal",	"Normal" },
    { DIMENSION_GUNS,	6,			262141,		FORCE_LEVEL_1,	qtrue,		qtrue,		qfalse,			qfalse,		250,	 800,		"guns",		"Guns Arena" },
	{ DIMENSION_RACE,	65531,		229373,		FORCE_LEVEL_1,	qfalse,		qfalse,		qtrue,			qtrue,		250,	 800,		"race",		"Race Defrag" },
	{ DIMENSION_SABER,	65531,		229373,		FORCE_LEVEL_1,	qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"saber",	"Saber Only" },
	{ DIMENSION_INSTA,	65503,		262141,		FORCE_LEVEL_3,	qfalse,		qfalse,		qfalse,			qfalse,		415,	 800,		"insta",	"Insta Kill" },
	{ DIMENSION_CHEAT,	DEFAULT,	DEFAULT,	DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"cheats",	"Cheats Mode" },
};

/*
=====================================================================
Dimension index from table
=====================================================================
*/
int JKMod_DimensionIndex(unsigned dimension)
{
	int	num;

	for(num = 0; num < sizeof(jkmod_dimension_data) / sizeof(jkmod_dimension_data[0]); num++)
	{
		if (jkmod_dimension_data[num].dimension == dimension) {
			return num;
		}
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
	if (jkcvar_altDimension.integer)
	{
		int num = JKMod_DimensionIndex(dimension);

		JKMod_CustomGameSettings(ent, 
			jkmod_dimension_data[num].weapons,			// Weapons
			jkmod_dimension_data[num].forcepowers,		// Force
			jkmod_dimension_data[num].forcelevel,		// Force level
			jkmod_dimension_data[num].holdables,		// Holdable items
			jkmod_dimension_data[num].jetpack,			// Jetpack
			jkmod_dimension_data[num].invulnerability,	// Invulnerability
			jkmod_dimension_data[num].passthrough,		// Pass-through
			jkmod_dimension_data[num].speed,			// Speed
			jkmod_dimension_data[num].gravity			// Gravity
		);
	}
}

/*
=====================================================================
Set dimension function
=====================================================================
*/
qboolean JKMod_DimensionCmd(gentity_t *ent, char *dimension, qboolean say)
{
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
	else if (level.jkmodLevel.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension during pause mode\n\"");
		return qfalse;
	}
	else if (ent->client->jkmodClient.DimensionTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds before change dimension\n\"", ent->client->jkmodClient.DimensionTime));
		return qfalse;
	}
	else
	{
		int	num;

		// Start
		for (num = 0; num < sizeof(jkmod_dimension_data) / sizeof(jkmod_dimension_data[0]); num++)
		{
			if (!Q_stricmp(dimension, jkmod_dimension_data[num].command))
			{
				qboolean	dimensionFree = jkmod_dimension_data[num].dimension == DIMENSION_FREE;
				int			dimensionIndex = jkmod_dimension_data[num].dimension;
				char		*dimensionName = jkmod_dimension_data[num].name;

				// Delay
				ent->client->jkmodClient.DimensionTime = jkcvar_altDimensionTime.integer;

				// Check
				if (!(jkcvar_altDimension.integer & dimensionIndex) && !dimensionFree)
				{
					trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
					return qfalse;
				}
				// Enable
				else if (ent->client->ps.stats[JK_DIMENSION] != dimensionIndex && !dimensionFree)
				{
					JKMod_DimensionSet(ent, dimensionIndex);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\n\"", dimensionName));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3%s ^7dimension\n\"", ent->client->pers.netname, dimensionName));
					return qtrue;
				}
				// Disable
				else if (ent->client->ps.stats[JK_DIMENSION] > 1)
				{
					JKMod_DimensionSet(ent, DIMENSION_FREE);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\n\"", jkmod_dimension_data[0].name));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s ^3%s ^7dimension\n\"", ent->client->pers.netname, (dimensionFree ? "back to" : "left the"), dimensionName));
					return dimensionFree;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, "print \"You are already in this dimension\n\"");
					return qfalse;
				}
			}
		}

		// Not found
		if (!say)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Usage: dimension <option>\nSee ^3/help dimensions ^7for more information\n\""));
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, va("cp \""
				"^7Usage: ^2!dimension <option>\n"
				"^7Option list:\n"
				"^3normal\n"
				"^3guns\n"
				"^3race\n"
				"^3saber\n"
				"^3insta\n"
				"^3cheats\n"
				"^7\""));
		}
		
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
	if (mvapi) 
	{
		uint8_t	*snapshotIgnore = mv_entities[ent->s.number].snapshotIgnore;
		int	i;

		assert(owner >= 0 && owner < MAX_GENTITIES);

		if (owner >= MAX_CLIENTS && owner < ENTITYNUM_MAX_NORMAL) 
		{
			owner = g_entities[owner].jkmodEnt.dimensionOwner;
			assert(owner < MAX_CLIENTS || owner == ENTITYNUM_NONE || owner == ENTITYNUM_WORLD);
		}

		ent->jkmodEnt.dimensionOwner = owner;

		if (ent - g_entities < MAX_CLIENTS) {
			ent->jkmodEnt.dimensionNumber = DIMENSION_FREE;
		} else {
			ent->jkmodEnt.dimensionNumber = DIMENSION_ALL;
		}

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
	if (mvapi && jkcvar_altDimension.integer)
	{
		unsigned dimension;
		qboolean free;
		int i;

		for (dimension = 1 << 15; dimension != 0; dimension <<= 1) {
			free = qtrue;

			for (i = 0; i < level.maxclients; i++) {
				if (!g_entities[i].inuse) {
					continue;
				}
				if ((g_entities[i].jkmodEnt.dimensionNumber & dimension) != 0) {
					free = qfalse;
					break;
				}
			}
			if (free) {
				return dimension;
			}
		}
		assert(0);
		return DIMENSION_FREE;
	}
	else
	{
		return 0;
	}
}

/*
=====================================================================
Dimension snapshot ignore apply
=====================================================================
*/
void JKMod_DimensionSet(gentity_t *ent, unsigned dimension)
{
	if (mvapi && jkcvar_altDimension.integer)
	{
		int	i;
		int	clientNum = ent->s.number;

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

		JKMod_Printf(S_COLOR_YELLOW "Client %i changed dimension to %i\n", clientNum, dimension);
	}
}

/*
=====================================================================
Dimension check collide
=====================================================================
*/
qboolean JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2)
{
	if (mvapi && jkcvar_altDimension.integer)
	{
		int owner1 = g_entities[ent1->s.number].jkmodEnt.dimensionOwner;
		int owner2 = g_entities[ent2->s.number].jkmodEnt.dimensionOwner;

		if (g_entities[owner1].jkmodEnt.dimensionNumber == DIMENSION_RACE && 
			g_entities[owner2].jkmodEnt.dimensionNumber == DIMENSION_RACE)
		{
			return (qboolean)!(JKMod_DimensionCheck(owner1, owner2));
		}
		else
		{
			return (qboolean)!!(JKMod_DimensionCheck(owner1, owner2));
		}
	}
	else
	{
		return qtrue;
	}
}

/*
=====================================================================
Dimensions main trace function
=====================================================================
*/
void JKMod_DimensionTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	if (mvapi && jkcvar_altDimension.integer)
	{
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
}

/*
=====================================================================
Dimensions check entities in box
=====================================================================
*/
int JKMod_DimensionEntitiesInBox(const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount, int entityNum)
{
	if (mvapi && jkcvar_altDimension.integer)
	{
		gentity_t	*passEnt = g_entities + entityNum;
		int	fullCount;
		int	count;
		int	i;

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
	else
	{
		return trap_EntitiesInBox(mins, maxs, entityList, maxcount);
	}
}
