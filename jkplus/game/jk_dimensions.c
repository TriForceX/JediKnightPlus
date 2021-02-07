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
void JKMod_DimensionSettings(gentity_t *ent, int dimension)
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
			if (!(jkcvar_altDimension.integer & (1 << DIMENSION_GUNS)))
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
					ent->client->ps.stats[JK_DIMENSION] = DIMENSION_FREE;
					JKMod_DimensionSettings(ent, DIMENSION_FREE);

					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns dimension left\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					ent->client->ps.stats[JK_DIMENSION] = DIMENSION_GUNS;
					JKMod_DimensionSettings(ent, DIMENSION_GUNS);

					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns dimension\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qtrue;
				}
			}
		}
		// Race Arena
		else if (!Q_stricmp(dimension, "race"))
		{
			if (!(jkcvar_altDimension.integer & (1 << DIMENSION_RACE)))
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
					ent->client->ps.stats[JK_DIMENSION] = DIMENSION_FREE;
					JKMod_DimensionSettings(ent, DIMENSION_FREE);

					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					trap_SendServerCommand(ent - g_entities, va("cp \"Race dimension left\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Race ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					ent->client->ps.stats[JK_DIMENSION] = DIMENSION_RACE;
					JKMod_DimensionSettings(ent, DIMENSION_RACE);

					// if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

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
Dimensions trace functions
=====================================================================
*/
void JKMod_DimensionsTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	// Temp...
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);
}
