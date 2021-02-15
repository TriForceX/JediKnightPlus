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
Dimension settings function
=====================================================================
*/
void JKMod_DimensionSettings(gentity_t *ent, unsigned dimension)
{
	switch(dimension)
	{
		case DIMENSION_GUNS:
		{
			JKMod_CustomGameSettings(ent, 
				6,				// Weapons
				262141,			// Force
				FORCE_LEVEL_1,	// Force level
				qtrue,			// Holdable items
				qtrue,			// Jetpack
				qfalse,			// Invulnerability
				g_speed.value,	// Speed
				g_gravity.value	// Gravity
			);
		}
		break;
		case DIMENSION_RACE:
		{
			JKMod_CustomGameSettings(ent, 
				65531,			// Weapons
				229373,			// Force
				FORCE_LEVEL_1,	// Force level
				qfalse,			// Holdable items
				qfalse,			// Jetpack
				qtrue,			// Invulnerability
				g_speed.value,	// Speed
				g_gravity.value	// Gravity
			);
		}
		break;
		case DIMENSION_FREE:
		{
			JKMod_CustomGameSettings(ent, 
				g_weaponDisable.integer,		// Weapons
				g_forcePowerDisable.integer,	// Force
				qfalse,							// Force level
				qfalse,							// Holdable items
				qfalse,							// Jetpack
				qfalse,							// Invulnerability
				g_speed.value,					// Speed
				g_gravity.value					// Gravity
			);
		}
	}
}

/*
=====================================================================
Set dimension function
=====================================================================
*/
qboolean JKMod_DimensionCmd(gentity_t *ent, char *dimension)
{
	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Join the game first before switch dimension\n\"");
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
	if (ent->client->jkmodClient.DimensionTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds before change dimension\n\"", ent->client->jkmodClient.DimensionTime));
		return qfalse;
	}
	else
	{
		// Guns Arena
		if (!Q_stricmp(dimension, "guns"))
		{
			if (!(jkcvar_altDimension.integer & DIMENSION_GUNS))
			{
				trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
				return qfalse;
			}
			else
			{
				// Delay
				ent->client->jkmodClient.DimensionTime = jkcvar_altDimensionTime.integer;

				// Disable
				if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_GUNS)
				{
					JKMod_DimensionSet(ent, DIMENSION_FREE);
					JKMod_DimensionSettings(ent, DIMENSION_FREE);

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns dimension left\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					JKMod_DimensionSet(ent, DIMENSION_GUNS);
					JKMod_DimensionSettings(ent, DIMENSION_GUNS);

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns dimension\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qtrue;
				}
			}
		}
		// Race Arena
		else if (!Q_stricmp(dimension, "race"))
		{
			if (!(jkcvar_altDimension.integer & DIMENSION_RACE))
			{
				trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
				return qfalse;
			}
			else
			{
				// Delay
				ent->client->jkmodClient.DimensionTime = jkcvar_altDimensionTime.integer;

				// Disable
				if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
				{
					JKMod_DimensionSet(ent, DIMENSION_FREE);
					JKMod_DimensionSettings(ent, DIMENSION_FREE);

					trap_SendServerCommand(ent - g_entities, va("cp \"Race dimension left\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Race ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					JKMod_DimensionSet(ent, DIMENSION_RACE);
					JKMod_DimensionSettings(ent, DIMENSION_RACE);

					if (ent->r.contents & CONTENTS_BODY) ent->r.contents &= ~CONTENTS_BODY; // Check body

					trap_SendServerCommand(ent - g_entities, va("cp \"Race dimension\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3Race ^7dimension\n\"", ent->client->pers.netname));
					return qtrue;
				}
			}
		}
		// No dimensions
		else 
		{
			trap_SendServerCommand(ent - g_entities, "print \"This dimension doesn't exist\n\"");
			return qfalse;
		}
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
	if (jkcvar_altDimension.integer && mvapi) 
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
	if (jkcvar_altDimension.integer && mvapi)
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
	if (jkcvar_altDimension.integer && mvapi)
	{
		int	i;
		int	clientNum = ent->s.number;

		ent->client->ps.stats[JK_DIMENSION] = ent->client->ps.duelInProgress ? DIMENSION_DUEL : dimension;
		ent->jkmodEnt.dimensionNumber = dimension;

		if (ent->client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			// Check body
			if (!(ent->r.contents & CONTENTS_BODY)) ent->r.contents = CONTENTS_BODY;

			// Check stuck
			if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_ANTI_STUCK;

			// Check saber
			if (ent->client->ps.saberEntityNum != ENTITYNUM_NONE) g_entities[ent->client->ps.saberEntityNum].jkmodEnt.dimensionNumber = dimension;
		}

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
}

/*
=====================================================================
Dimension check collide
=====================================================================
*/
qboolean JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2)
{
	if (jkcvar_altDimension.integer && mvapi)
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

	if (jkcvar_altDimension.integer && mvapi)
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
	if (jkcvar_altDimension.integer && mvapi)
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
