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

// Save entity owners nums
static int g_entities_dimension[MAX_GENTITIES];

/*
=====================================================================
Set dimension function
=====================================================================
*/
qboolean JKMod_SetDimension(char *dimension, gentity_t *ent, int clientNum)
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
	else
	{
		// Set guns dimension
		if (!Q_stricmp(dimension, "guns"))
		{
			int i;
			int wDisable = g_weaponDisable.integer;

			if (!(jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)))
			{
				trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
				return qfalse;
			}
			else if (ent->client->ps.stats[JK_DIMENSION] & (JK_DUEL_IN | JK_RACE_IN))
			{
				trap_SendServerCommand(ent - g_entities, "cp \"You need to leave the current dimension first\n\"");
				return qfalse;
			}
			else
			{
				// Disable
				if (ent->client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)
				{
					ent->client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_STUN_BATON)
						& ~(1 << WP_BRYAR_PISTOL)
						& ~(1 << WP_BLASTER)
						& ~(1 << WP_DISRUPTOR)
						& ~(1 << WP_BOWCASTER)
						& ~(1 << WP_REPEATER)
						& ~(1 << WP_DEMP2)
						& ~(1 << WP_FLECHETTE)
						& ~(1 << WP_ROCKET_LAUNCHER)
						& ~(1 << WP_THERMAL)
						& ~(1 << WP_TRIP_MINE)
						& ~(1 << WP_DET_PACK);
					ent->client->ps.stats[STAT_HOLDABLE_ITEMS] &= ~(1 << HI_BINOCULARS)
						& ~(1 << HI_SEEKER)
						& ~(1 << HI_MEDPAC)
						& ~(1 << HI_SHIELD)
						& ~(1 << HI_SENTRY_GUN);

					for (i = 0; i < MAX_WEAPONS; i++) ent->client->ps.ammo[i] = 0;

					if (WP_HasForcePowers(&ent->client->ps))
					{
						ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
						ent->client->ps.weapon = WP_SABER;
					}
					else if (!wDisable || !(wDisable & (1 << WP_BRYAR_PISTOL)))
					{
						ent->client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
						ent->client->ps.stats[STAT_WEAPONS] |= (1 << WP_BRYAR_PISTOL);
						ent->client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max;
						ent->client->ps.weapon = WP_BRYAR_PISTOL;
					}
					else
					{
						ent->client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
						ent->client->ps.weapon = WP_STUN_BATON;
					}

					ent->client->ps.forceRestricted = qfalse;
					ent->client->ps.fd.forcePowerLevel[FP_LEVITATION] = ent->client->pers.jkmodPers.customSavedJump;
					ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;

					memset(ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups));

					JKMod_RemoveByClass(ent, "bryar_proj");
					JKMod_RemoveByClass(ent, "generic_proj");
					JKMod_RemoveByClass(ent, "blaster_proj");
					JKMod_RemoveByClass(ent, "emplaced_gun_proj");
					JKMod_RemoveByClass(ent, "bowcaster_proj");
					JKMod_RemoveByClass(ent, "bowcaster_alt_proj");
					JKMod_RemoveByClass(ent, "repeater_proj");
					JKMod_RemoveByClass(ent, "repeater_alt_proj");
					JKMod_RemoveByClass(ent, "demp2_proj");
					JKMod_RemoveByClass(ent, "demp2_alt_proj");
					JKMod_RemoveByClass(ent, "flech_proj");
					JKMod_RemoveByClass(ent, "flech_alt");
					JKMod_RemoveByClass(ent, "rocket_proj");
					JKMod_RemoveByClass(ent, "thermal_detonator");
					JKMod_RemoveByClass(ent, "detpack");
					JKMod_RemoveByClass(ent, "laserTrap");
					JKMod_RemoveByClass(ent, "sentryGun");
					JKMod_RemoveByClass(ent, "item_shield");

					if (ent->client->ps.eFlags & EF_SEEKERDRONE) 
					{
						ent->client->ps.eFlags -= EF_SEEKERDRONE;
						ent->client->ps.genericEnemyIndex = -1;
					}

					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					ent->client->pers.jkmodPers.inDimension &= ~JK_GUNS_IN;
					ent->client->ps.stats[JK_DIMENSION] &= ~JK_GUNS_IN;

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns mode disabled\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					ent->client->pers.jkmodPers.inDimension |= JK_GUNS_IN;
					ent->client->ps.stats[JK_DIMENSION] |= JK_GUNS_IN;

					ent->client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
					ent->client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON)
						| (1 << WP_BRYAR_PISTOL)
						| (1 << WP_BLASTER)
						| (1 << WP_DISRUPTOR)
						| (1 << WP_BOWCASTER)
						| (1 << WP_REPEATER)
						| (1 << WP_DEMP2)
						| (1 << WP_FLECHETTE)
						| (1 << WP_ROCKET_LAUNCHER)
						| (1 << WP_THERMAL)
						| (1 << WP_TRIP_MINE)
						| (1 << WP_DET_PACK);
					ent->client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << HI_BINOCULARS)
						| (1 << HI_SEEKER)
						| (1 << HI_MEDPAC)
						| (1 << HI_SHIELD)
						| (1 << HI_SENTRY_GUN);

					for (i = 0; i < AMMO_MAX; i++) ent->client->ps.ammo[i] = ammoData[i].max;

					ent->client->ps.weapon = WP_STUN_BATON;
					ent->client->ps.forceRestricted = qtrue;
					ent->client->pers.jkmodPers.customSavedJump = ent->client->ps.fd.forcePowerLevel[FP_LEVITATION];
					ent->client->ps.fd.forcePowerLevel[FP_LEVITATION] = FORCE_LEVEL_1;

					if (jkcvar_jetPack.integer) 
					{
						ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
						if (!ent->client->ps.stats[JK_FUEL]) ent->client->ps.stats[JK_FUEL] = 100;
					}

					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					trap_SendServerCommand(ent - g_entities, va("cp \"Guns mode enabled\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3Guns ^7dimension\n\"", ent->client->pers.netname));
					return qtrue;
				}
			}
		}
		// Set race dimension
		else if (!Q_stricmp(dimension, "race"))
		{
			if (!(jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)))
			{
				trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
				return qfalse;
			}
			else if (ent->client->ps.stats[JK_DIMENSION] & (JK_DUEL_IN | JK_GUNS_IN))
			{
				trap_SendServerCommand(ent - g_entities, "cp \"You need to leave the current dimension first\n\"");
				return qfalse;
			}
			else
			{
				// Disable
				if (ent->client->ps.stats[JK_DIMENSION] & JK_RACE_IN)
				{
					ent->client->ps.forceRestricted = qfalse;
					ent->client->ps.fd.forcePowerLevel[FP_LEVITATION] = ent->client->pers.jkmodPers.customSavedJump;
					if (!ent->takedamage) ent->takedamage = qtrue;
					if (JKMod_OthersInBox(ent)) ent->client->ps.eFlags |= JK_PASS_THROUGH;

					ent->client->pers.jkmodPers.inDimension &= ~JK_RACE_IN;
					ent->client->ps.stats[JK_DIMENSION] &= ~JK_RACE_IN;

					trap_SendServerCommand(ent - g_entities, va("cp \"Race mode disabled\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " left the ^3Race ^7dimension\n\"", ent->client->pers.netname));
					return qfalse;
				}
				// Enable
				else
				{
					ent->client->pers.jkmodPers.inDimension |= JK_RACE_IN;
					ent->client->ps.stats[JK_DIMENSION] |= JK_RACE_IN;

					ent->client->ps.forceRestricted = qtrue;
					ent->client->pers.jkmodPers.customSavedJump = ent->client->ps.fd.forcePowerLevel[FP_LEVITATION];
					ent->client->ps.fd.forcePowerLevel[FP_LEVITATION] = FORCE_LEVEL_1;
					if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;

					trap_SendServerCommand(ent - g_entities, va("cp \"Race mode enabled\n\""));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " joined the ^3Race ^7dimension\n\"", ent->client->pers.netname));
					return qtrue;
				}
			}
		}
		else {
			trap_SendServerCommand(ent - g_entities, "print \"This dimension doesn't exist\n\"");
			return qfalse;
		}
	}
}

/*
=====================================================================
Trace parse and clean up function
=====================================================================
*/
static void trap_Trace_Parse(int entityNum, qboolean start)
{
	if (0 <= entityNum && entityNum < MAX_CLIENTS)
	{
		// Check duel dimension
		if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[entityNum].client && (level.clients[entityNum].ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack"))))) 
					{
						if (start) {
							g_entities_dimension[i] = g_entities[i].r.ownerNum;
							g_entities[i].r.ownerNum = entityNum;
						} else {
							g_entities[i].r.ownerNum = g_entities_dimension[i];
						}
					}
				}
			}
		}
		// Check guns dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN) != (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						if (start) {
							g_entities_dimension[i] = g_entities[i].r.ownerNum;
							g_entities[i].r.ownerNum = entityNum;
						} else {
							g_entities[i].r.ownerNum = g_entities_dimension[i];
						}
					}
				}
			}
		}
		// Check race dimension
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && g_entities[entityNum].client && (g_entities[entityNum].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) {
			int i;
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse &&
						(g_entities[i].s.eType == ET_PLAYER ||
						(g_entities[i].s.eType == ET_MOVER && ((!Q_stricmp(g_entities[i].classname, "func_door")))) ||
						(g_entities[i].s.eType == ET_GENERAL && (!Q_stricmp(g_entities[i].classname, "laserTrap")) || (!Q_stricmp(g_entities[i].classname, "detpack")))))
					{
						if (start) {
							g_entities_dimension[i] = g_entities[i].r.ownerNum;
							g_entities[i].r.ownerNum = entityNum;
						} else {
							g_entities[i].r.ownerNum = g_entities_dimension[i];
						}
					}
				}
			}
		}
		// Final check
		else {
			int i;
			if (g_entities[entityNum].inuse) {
				const int saberOwner = g_entities[entityNum].r.ownerNum;
				if (g_entities[saberOwner].client && ( // Dimensions with interaction
					((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[saberOwner].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
					((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[saberOwner].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) ))
				{
					return;
				}
			}
			for (i = 0; i < level.num_entities; i++) {
				if (i != entityNum) {
					if (g_entities[i].inuse && g_entities[i].client && (
						((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_DUEL_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_GUNS)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_GUNS_IN)) ||
						((jkcvar_altDimensions.integer & (1 << DIMENSION_RACE)) && (g_entities[i].client->ps.stats[JK_DIMENSION] & JK_RACE_IN)) ))
					{
						if (start) {
							g_entities_dimension[i] = g_entities[i].r.ownerNum;
							g_entities[i].r.ownerNum = entityNum;
						} else {
							g_entities[i].r.ownerNum = g_entities_dimension[i];
						}
					}
				}
			}
		}
	}
}

/*
=====================================================================
Dimensions trace functions
=====================================================================
*/
void JKMod_Dimensions(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	// Start trace parse
	trap_Trace_Parse(passEntityNum, qtrue);

	// Continue normal trace
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	// End clean up
	trap_Trace_Parse(passEntityNum, qfalse);
}
