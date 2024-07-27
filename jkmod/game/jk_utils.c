/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Utils functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern void WP_AddAsMindtricked(forcedata_t *fd, int entNum);

/*
=====================================================================
Draw box lines (Due clientside: 255 = red, 1 = black, 0 = white)
=====================================================================
*/
void JKMod_DrawBoxLines(vec3_t orig, vec3_t mins, vec3_t maxs, int color, int duration, int dimensionOwner)
{
	vec3_t	point1, point2, point3, point4;
	vec3_t	bmins, bmaxs;
	int		vec[3];
	int		axis, i;

	VectorAdd(orig, mins, bmins);
	VectorAdd(orig, maxs, bmaxs);

	for (axis = 0, vec[0] = 0, vec[1] = 1, vec[2] = 2; axis < 3; axis++, vec[0]++, vec[1]++, vec[2]++)
	{
		for (i = 0; i < 3; i++)
		{
			if (vec[i] > 2)
			{
				vec[i] = 0;
			}
		}

		point1[vec[1]] = bmins[vec[1]];
		point1[vec[2]] = bmins[vec[2]];

		point2[vec[1]] = bmins[vec[1]];
		point2[vec[2]] = bmaxs[vec[2]];

		point3[vec[1]] = bmaxs[vec[1]];
		point3[vec[2]] = bmaxs[vec[2]];
		
		point4[vec[1]] = bmaxs[vec[1]];
		point4[vec[2]] = bmins[vec[2]];

		//- face
		point1[vec[0]] = point2[vec[0]] = point3[vec[0]] = point4[vec[0]] = bmins[vec[0]];

		JKMod_G_TestLine(point1, point2, color, duration, dimensionOwner);
		JKMod_G_TestLine(point2, point3, color, duration, dimensionOwner);
		JKMod_G_TestLine(point1, point4, color, duration, dimensionOwner);
		JKMod_G_TestLine(point4, point3, color, duration, dimensionOwner);

		//+ face
		point1[vec[0]] = point2[vec[0]] = point3[vec[0]] = point4[vec[0]] = bmaxs[vec[0]];

		JKMod_G_TestLine(point1, point2, color, duration, dimensionOwner);
		JKMod_G_TestLine(point2, point3, color, duration, dimensionOwner);
		JKMod_G_TestLine(point1, point4, color, duration, dimensionOwner);
		JKMod_G_TestLine(point4, point1, color, duration, dimensionOwner);
	}
}

/*
=====================================================================
Entity scan function
=====================================================================
*/
void JKMod_EntityScan(gentity_t *ent, int distance, int boxdelay, int linedelay)
{
	trace_t		tr;
	vec3_t		fwd, dest, orig;
	vec3_t		mins = { -5, -5, -5 }, maxs = { 5, 5, 5 };

	if (!distance) distance = 200;
	if (!boxdelay) boxdelay = 500;
	if (!linedelay) linedelay = 500;

	AngleVectors(ent->client->ps.viewangles, fwd, NULL, NULL);

	VectorCopy(ent->client->ps.origin, orig);
	orig[2] += (ent->client->ps.weapon == WP_SABER ? ent->r.maxs[2] : ent->r.maxs[2] / 2);
	VectorMA(orig, distance, fwd, dest);

	trap_Trace(&tr, orig, mins, maxs, dest, ent->s.number, MASK_ALL);

	if (tr.allsolid || tr.startsolid || tr.fraction != 1.0f)
	{
		gentity_t	*found = &g_entities[tr.entityNum];
		char		 found_classname[40];
		char		 found_model[44];
		char		 found_origin[43];
		char		 found_target[29];
		char		 found_targetname[25];
		char		 found_spawnflags[25];
		char		 found_angles[16];
		char		 found_mins[18];
		char		 found_maxs[18];

		JKMod_G_TestLine(orig, dest, 255, linedelay, ent->s.number);

		if (!found->inuse) return;
		
		Q_strncpyz(found_classname, va("%s", found->classname), sizeof(found_classname));
		Q_strncpyz(found_model, va("%s", found->model), sizeof(found_model));
		Q_strncpyz(found_origin, va("%.0f %.0f %.0f", found->s.origin[0], found->s.origin[1], found->s.origin[2]), sizeof(found_origin));
		Q_strncpyz(found_target, va("%s", found->target), sizeof(found_target));
		Q_strncpyz(found_targetname, va("%s", found->targetname), sizeof(found_targetname));
		Q_strncpyz(found_spawnflags, va("%i", found->spawnflags), sizeof(found_spawnflags));
		Q_strncpyz(found_mins, va("%.0f %.0f %.0f", found->r.mins[0], found->r.mins[1], found->r.mins[2]), sizeof(found_mins));
		Q_strncpyz(found_maxs, va("%.0f %.0f %.0f", found->r.maxs[0], found->r.maxs[1], found->r.maxs[2]), sizeof(found_maxs));
		Q_strncpyz(found_angles, va("%.0f %.0f %.0f", found->s.angles[0], found->s.angles[1], found->s.angles[2]), sizeof(found_angles));

		trap_SendServerCommand(ent - g_entities, va("print \"\n"
			"^7Classname: ^3%-40s ^7Target: ^3%-29s ^7Angles: ^3%-16s\n"
			"^7Model: ^3%-44s ^7Targetname: ^3%-25s ^7Mins: ^3%-18s\n"
			"^7Origin: ^3%-43s ^7Spawnflags: ^3%-25s ^7Maxs: ^3%-18s\n\"", 
			found_classname,
			found_target,
			found_angles,
			found_model,
			found_targetname,
			found_mins,
			found_origin,
			found_spawnflags,
			found_maxs));

		JKMod_DrawBoxLines(found->s.origin, found->r.mins,  found->r.maxs, 255, boxdelay, ent->s.number);
		return;
	}

	JKMod_G_TestLine(orig, dest, 0, linedelay, ent->s.number);
	return;
}

/*
=====================================================================
Temp effect function
=====================================================================
*/
void JKMod_TempEffect(gentity_t *ent, int alignment, int rotation, char *efxfile, int angle, qboolean serverside)
{
	vec3_t origin;
	vec3_t angles;

	if (!VALIDSTRING(efxfile)) return;

	VectorCopy(ent->client->ps.origin, origin);
	if (alignment == 2) { 
		origin[2] += ent->r.maxs[2];
	} else if (alignment == 1) { 
		origin[2] += ent->r.maxs[2]/2;
	} else if (!alignment) { 
		origin[2] += DEFAULT_MINS_2;
	}

	VectorCopy(ent->client->ps.viewangles, angles);
	if (rotation) angles[PITCH] = -90;
	if (angle) angles[YAW] += angle;

	JKMod_G_PlayEffect_ID(G_EffectIndex(efxfile), origin, angles, ent->s.number, serverside);
}

/*
=====================================================================
Temp model add function
=====================================================================
*/
void JKMod_TempModelAdd(gentity_t *ent, int alignment, int rotation, char *modelname, int angle, qboolean playersolid)
{
	gentity_t *model;

	if (!ent->client->pers.jkmodPers.tempModelNum && VALIDSTRING(modelname))
	{
		vec3_t origin;
		vec3_t angles;
		int glm = 0;

		model = JKMod_G_Spawn(ent->s.number); // Tag owner info 
		model->s.modelindex = G_ModelIndex(modelname);
		model->model = modelname;
 
		VectorSet(model->r.mins, 0, 0, 0);
		VectorSet(model->r.maxs, 0, 0, 0);
 
		if (strstr(modelname, ".glm"))
		{	
			model->s.g2radius = 100;
			model->s.modelGhoul2 = 1;
			glm = 1;
		}

		VectorCopy(ent->client->ps.origin, origin);
		if (alignment == 2) { 
			origin[2] += (glm ? ent->r.maxs[2] + abs(DEFAULT_MINS_2) : ent->r.maxs[2]);
		} else if (alignment == 1) { 
			origin[2] += (glm ? ent->r.maxs[2]/2 : ent->r.maxs[2]/2 - DEFAULT_MAXS_2/2);
		} else if (!alignment && !glm) { 
			origin[2] += DEFAULT_MINS_2;
		}
		G_SetOrigin(model, origin);
		
		VectorCopy(ent->client->ps.viewangles, angles);
		if (!glm) angles[YAW] -= 90;
		if (angle) angles[YAW] += angle;
		if (angle) angles[ROLL] = 0.0f;
		if (rotation) angles[PITCH] = 0.0f;
		if (!rotation) angles[PITCH] = (angle && !glm) || (!angle && glm) ? ent->client->ps.viewangles[PITCH] : 0.0f;
		if (!rotation && !angle && !glm) angles[ROLL] -= ent->client->ps.viewangles[PITCH];
		if (!rotation && angle && glm) angles[ROLL] = ent->client->ps.viewangles[PITCH];
		G_SetAngles(model, angles);

		trap_LinkEntity(model);

		ent->client->pers.jkmodPers.tempModelNum = model->s.number;
		ent->client->pers.jkmodPers.tempModelSettings[0] = alignment;
		ent->client->pers.jkmodPers.tempModelSettings[1] = rotation;
		ent->client->pers.jkmodPers.tempModelSettings[2] = angle;
		ent->client->pers.jkmodPers.tempModelSettings[3] = glm;
		trap_SendServerCommand(ent - g_entities, va("print \"Temporary model %i created (%s)\n\"", model->s.number, modelname));
		JKMod_GhostAdd(ent, playersolid);
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Temporary model %i deleted\n\"", ent->client->pers.jkmodPers.tempModelNum));
		JKMod_TempModelRemove(ent, ent->client->pers.jkmodPers.tempModelNum);
	}
}

/*
=====================================================================
Temp model remove function
=====================================================================
*/
void JKMod_TempModelRemove(gentity_t *ent, int number)
{
	if (number > MAX_CLIENTS) {
		gentity_t *model = &g_entities[number];
		if (model->inuse) {
			JKMod_Printf(S_COLOR_MAGENTA "Removing temp model %i\n", model->s.number);
			trap_UnlinkEntity(model);
			G_FreeEntity(model);
		}
	}
	if (ent && ent->client) {
		ent->client->pers.jkmodPers.tempModelNum = 0;
		ent->client->pers.jkmodPers.tempModelSettings[0] = 0;
		ent->client->pers.jkmodPers.tempModelSettings[1] = 0;
		JKMod_GhostRemove(ent);
	}
}

/*
=====================================================================
Player ghost add function
=====================================================================
*/
void JKMod_GhostAdd(gentity_t* ent, qboolean solid)
{
	if (!(ent->client->ps.fd.forcePowersActive & (1 << FP_TELEPATHY)))
	{
		ent->client->ps.fd.forcePowersActive |= ( 1 << FP_TELEPATHY );
		ent->client->ps.fd.forcePowerDuration[FP_TELEPATHY] = level.time + INFINITE;
	}

	WP_AddAsMindtricked(&ent->client->ps.fd, ent->s.number);
	ent->r.svFlags |= SVF_SINGLECLIENT;
	ent->r.singleClient = ent->s.number;
	if (!solid) {
		ent->client->ps.eFlags |= JK_PASS_THROUGH;
		ent->client->pers.jkmodPers.passThroughPerm = qtrue;
	}
	ent->client->pers.jkmodPers.ghostPlayer = qtrue;
}

/*
=====================================================================
Player ghost remove function
=====================================================================
*/
void JKMod_GhostRemove(gentity_t* ent)
{
	if (ent->client->ps.fd.forcePowersActive & (1 << FP_TELEPATHY))
	{
		ent->client->ps.fd.forcePowersActive &= ~(1 << FP_TELEPATHY);
		ent->client->ps.fd.forceMindtrickTargetIndex = 0;
		ent->client->ps.fd.forceMindtrickTargetIndex2 = 0;
		ent->client->ps.fd.forceMindtrickTargetIndex3 = 0;
		ent->client->ps.fd.forceMindtrickTargetIndex4 = 0;
	}

	ent->r.svFlags &= ~SVF_SINGLECLIENT;
	ent->r.singleClient = 0;
	if (ent->client->pers.jkmodPers.passThroughPerm)
	{
		ent->client->ps.eFlags &= ~JK_PASS_THROUGH;
		ent->client->pers.jkmodPers.passThroughPerm = qfalse;
		if (JKMod_OthersInBox(ent)) JKMod_AntiStuckBox(ent);
	}
	ent->client->pers.jkmodPers.ghostPlayer = qfalse;
}

/*
=====================================================================
Player ghost collide check
=====================================================================
*/
qboolean JKMod_GhostCollide(gentity_t* ent1, gentity_t* ent2)
{
	if (ent1 && ent1->client && ent2 && ent2->client) {
		if (ent2->client->pers.jkmodPers.ghostPlayer && ent2->client->pers.jkmodPers.passThroughPerm) {
			return qfalse;
		}
	}
	return qtrue;
}
/*
=====================================================================
Check other clients in box
=====================================================================
*/
qboolean JKMod_OthersInBox(gentity_t *ent) 
{
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*other;
	vec3_t		mins, maxs;

	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	num = JKMod_DimensionEntitiesInBox(mins, maxs, touch, MAX_GENTITIES, ent->s.number);

	for (i = 0; i < num; i++)
	{
		other = &g_entities[touch[i]];
		if (trap_EntityContact(mins, maxs, other) && other->client && other->client->ps.clientNum != ent->client->ps.clientNum && !(other->client->ps.eFlags & JK_PASS_THROUGH))
		{
			return qtrue;
		}
	}

	JKMod_Printf(S_COLOR_CYAN "Client %i checking others in box\n", ent->client->ps.clientNum);
	return qfalse;
}

/*
=====================================================================
Player anti stuck box function
=====================================================================
*/
void JKMod_AntiStuckBox(gentity_t *ent) 
{
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*other;
	vec3_t		mins, maxs;

	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	num = JKMod_DimensionEntitiesInBox(mins, maxs, touch, MAX_GENTITIES, ent->s.number);

	for (i = 0; i < num; i++)
	{
		other = &g_entities[touch[i]];
		if (other->client && other->client->ps.clientNum != ent->client->ps.clientNum && !(other->client->ps.eFlags & JK_PASS_THROUGH)) 
		{
			if (!(ent->client->ps.stats[JK_PLAYER] & JK_ANTI_STUCK)) ent->client->ps.stats[JK_PLAYER] |= JK_ANTI_STUCK;
			if (!(other->client->ps.stats[JK_PLAYER] & JK_ANTI_STUCK)) other->client->ps.stats[JK_PLAYER] |= JK_ANTI_STUCK;
		}
	}

	JKMod_Printf(S_COLOR_YELLOW "Client %i checking pass box\n", ent->client->ps.clientNum);
}

/*
=====================================================================
Check solid terrain from client view/origin
=====================================================================
*/
qboolean JKMod_CheckSolid(gentity_t *ent, int distance, vec3_t mins, vec3_t maxs, qboolean elevation)
{
	trace_t tr;
	vec3_t fwd, dest, orig;

	AngleVectors(ent->client->ps.viewangles, fwd, NULL, NULL);

	VectorCopy(ent->client->ps.origin, orig);
	if (!elevation) fwd[2] = 0;
	VectorMA(orig, distance, fwd, dest);
	trap_Trace(&tr, orig, mins, maxs, dest, ent->s.number, MASK_PLAYERSOLID);

	if (tr.allsolid || tr.startsolid || tr.fraction != 1.0f)
	{
		if (developer.integer) JKMod_DrawBoxLines(dest, mins, maxs, 255, 500, ent->s.number);
		JKMod_Printf(S_COLOR_YELLOW "Can't spawn here, we are in solid\n");
		return qfalse;
	}

	if (developer.integer) JKMod_DrawBoxLines(dest, mins, maxs, 0, 500, ent->s.number);
	return qtrue;
}

/*
=====================================================================
Remove entity by classname
=====================================================================
*/
void JKMod_RemoveByClass(gentity_t *ent, char *name)
{
	gentity_t *found = NULL;

	while ((found = G_Find(found, FOFS(classname), name)) != NULL)
	{
		if (found->parent == ent || (!Q_stricmp(name, "g2animent") && found->s.owner == ent->s.number))
		{
			if (!Q_stricmp(name, "sentryGun") && ent->client->ps.fd.sentryDeployed) {
				ent->client->ps.fd.sentryDeployed = qfalse;
			}
			if (!Q_stricmp(name, "detpack") && ent->client->ps.hasDetPackPlanted) {
				ent->client->ps.hasDetPackPlanted = qfalse;
			}

			VectorCopy(found->r.currentOrigin, found->s.origin);
			found->think = G_FreeEntity;
			found->nextthink = level.time;

			JKMod_Printf(S_COLOR_MAGENTA "Check remove %s from client %i\n", name, ent->s.number);
		}
	}
}
/*
=====================================================================
Jetpack use button action
=====================================================================
*/
void JKMod_JetpackTryUse(gentity_t *ent)
{
	if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE)
	{
		if (ent->client->ps.eFlags & JK_JETPACK_FLAMING && ent->client->pers.jkmodPers.jetpackUseDelay <= level.time) {
			// Disable
			ent->client->ps.eFlags &= ~JK_JETPACK_FLAMING;
			ent->client->pers.jkmodPers.jetpackUseDelay = level.time + 800;
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, va("print \"Jetpack off\n\""));
		}
		else if (ent->client->ps.groundEntityNum == ENTITYNUM_NONE &&
			ent->client->pers.jkmodPers.jetpackUseDelay <= level.time &&
			!(BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim) || JKMod_PlayerMoving(ent, qfalse, qtrue))) {
			// Enable
			ent->client->pers.jkmodPers.jetpackUseDelay = level.time + 800;
			ent->client->ps.eFlags |= JK_JETPACK_FLAMING;
			if (!ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, va("print \"Jetpack on\n\""));
		}
	}
}

/*
=====================================================================
Get private duel partner
=====================================================================
*/
gentity_t* JKMod_DuelGetPartner(gentity_t* ent)
{
	const int partnerID = ent->client->ps.duelIndex;
	gentity_t* partner = &g_entities[ent->client->ps.duelIndex];

	if (!ent->client->ps.duelInProgress) return NULL;
	if (partnerID == ENTITYNUM_NONE) return NULL;
	if (!partner->client->ps.duelInProgress) return NULL;
	if (partner->client->ps.duelIndex != ent->s.number) return NULL;

	return partner;
}

/*
=====================================================================
Check players dueling each other
=====================================================================
*/
qboolean JKMod_DuelEachOther(gentity_t* ent1, gentity_t* ent2)
{
	const gentity_t* ent1Partner = JKMod_DuelGetPartner(ent1);
	const gentity_t* ent2Partner = JKMod_DuelGetPartner(ent2);

	if (!ent1->client->ps.duelInProgress || !ent2->client->ps.duelInProgress) return qfalse;
	if (!ent1Partner || !ent2Partner) return qfalse;
	if (ent1Partner != ent2 || ent2Partner != ent1) return qfalse;

	return qtrue;
}

/*
=====================================================================
Check players duel isolation
=====================================================================
*/
qboolean JKMod_DuelIsolationCheck(gentity_t* ent1, gentity_t* ent2)
{
	if (jkcvar_duelPassThrough.integer) {
		if (ent1 && ent1->client && ent2 && ent2->client) {
			if (ent1->client->ps.duelInProgress || ent2->client->ps.duelInProgress) {
				return JKMod_DuelEachOther(ent1, ent2);
			}
		}
	}
	return qtrue;
}

/*
=====================================================================
Remove private duel status
=====================================================================
*/
void JKMod_DuelRemove(gentity_t *ent)
{
	ent->client->ps.duelInProgress = 0;
	ent->client->pers.jkmodPers.customDuel = 0;
	ent->client->pers.jkmodPers.duelHitCount = 0;
	G_AddEvent(ent, EV_PRIVATE_DUEL, 0);
	JKMod_DimensionSettings(ent, DIMENSION_FREE);
	JKMod_Printf(S_COLOR_MAGENTA "Client %i stopped duel\n", ent - g_entities);
}

/*
=====================================================================
Init game entity with owner info
=====================================================================
*/
void JKMod_G_InitGentity( gentity_t *e, int dimensionOwner ) 
{
	e->inuse = qtrue;
	e->classname = "noclass";
	e->s.number = e - g_entities;
	e->r.ownerNum = ENTITYNUM_NONE;
	e->s.modelGhoul2 = 0; //assume not
	e->freetime = 0;

	JKMod_DimensionOwnerCheck( dimensionOwner, e );
}

/*
=====================================================================
Play effect function with owner info
=====================================================================
*/
gentity_t *JKMod_G_PlayEffect(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner)
{
	gentity_t	*te;

	te = JKMod_G_TempEntity( org, EV_PLAY_EFFECT, dimensionOwner );
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;

	return te;
}

/*
=====================================================================
Play effect by ID function
=====================================================================
*/
gentity_t *JKMod_G_PlayEffect_ID(effectTypes_t fxID, const vec3_t org, const vec3_t ang, int dimensionOwner, qboolean serverSide)
{
	gentity_t	*te;
	gentity_t	*efxOwner = &g_entities[dimensionOwner];

	te = JKMod_G_TempEntity( org, EV_PLAY_EFFECT_ID, dimensionOwner );
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;
	
	if (serverSide) te->s.generic1 = GENERIC_SERVERSIDE; // Don't render on clientside

	return te;
}

/*
=====================================================================
Game spawn with owner info
=====================================================================
*/
gentity_t *JKMod_G_Spawn( int dimensionOwner ) 
{
	int			i, force;
	gentity_t	*e;

	e = NULL;	// shut up warning
	i = 0;		// shut up warning
	for ( force = 0 ; force < 2 ; force++ ) {
		// if we go through all entities and can't find one to free,
		// override the normal minimum times before use
		e = &g_entities[MAX_CLIENTS];
		for ( i = MAX_CLIENTS ; i<level.num_entities ; i++, e++) {
			if ( e->inuse ) {
				continue;
			}

			// the first couple seconds of server time can involve a lot of
			// freeing and allocating, so relax the replacement policy
			if ( !force && e->freetime > level.startTime + 2000 && level.time - e->freetime < 1000 ) {
				continue;
			}

			// reuse this slot
			JKMod_G_InitGentity( e, dimensionOwner );
			return e;
		}
		if ( i != ENTITYNUM_MAX_NORMAL ) {
			break;
		}
	}
	if ( i == ENTITYNUM_MAX_NORMAL )
	{
		gentity_t *found = NULL;
		if ( g_mv_fixturretcrash.integer )
		{ // TurretCrashFix - One last try!
			G_Printf("G_Spawn: no free entities, trying to make room by deleting temp entities and missiles\n");
			for ( i = MAX_CLIENTS; i < MAX_GENTITIES; i++ )
			{
				e = &g_entities[i];

				if ( e && (e->s.eType == ET_EVENTS + EV_SABER_BLOCK || ((e->s.weapon == WP_TURRET || g_mv_fixturretcrash.integer == 2) && e->s.eType == ET_MISSILE)) )
				{ // Delete all saber blocks and missiles...
					// g_mv_fixturretcrash == 1 -> only missiles from the turret will be removed
					// g_mv_fixturretcrash == 2 -> any missile will be removed
					if ( !found ) found = e;
					G_FreeEntity(e);
				}
			}
		}

		if ( !found )
		{
			for (i = 0; i < MAX_GENTITIES; i++) {
				G_Printf("%4i: %s\n", i, g_entities[i].classname);
			}
			G_Error( "G_Spawn: no free entities" );
		}
	}
	
	// open up a new slot
	level.num_entities++;

	// let the server system know that there are more entities
	if ( jk2version == VERSION_1_02 && !mvStructConversionDisabled )
	{ // 1.02
		// initialize all clients for this game
		memset( g_ps, 0, MAX_CLIENTS * sizeof(g_ps[0]) );

		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
			(playerState_t*)&g_ps[0], sizeof( g_ps[0] ) );
	}
	else
	{
		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
			&level.clients[0].ps, sizeof( level.clients[0] ) );
	}

	if ( mvapi )
	{
		trap_MVAPI_LocateGameData( mv_entities, level.num_entities, sizeof( mvsharedEntity_t ) );
	}

	JKMod_G_InitGentity( e, dimensionOwner );
	return e;
}

/*
=====================================================================
Game temp entity with owner info
=====================================================================
*/
gentity_t *JKMod_G_TempEntity( const vec3_t origin, entity_event_t event, int dimensionOwner ) 
{
	gentity_t		*e;
	vec3_t		snapped;
	
	assert(0 <= (int)(event & ~EV_EVENT_BITS) && (event & ~EV_EVENT_BITS) < EV_MAX);

	e = JKMod_G_Spawn( dimensionOwner );
	e->s.eType = ET_EVENTS + event;

	e->classname = "tempEntity";
	e->eventTime = level.time;
	e->freeAfterEvent = qtrue;

	VectorCopy( origin, snapped );
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( e, snapped );

	// find cluster for PVS
	trap_LinkEntity( e );

	return e;
}

/*
=====================================================================
Game sound temp entity with owner info
=====================================================================
*/
gentity_t *JKMod_G_SoundTempEntity( const vec3_t origin, int event, int channel, int dimensionOwner ) 
{
	gentity_t		*e;
	vec3_t		snapped;

	e = JKMod_G_Spawn( dimensionOwner );

	e->s.eType = ET_EVENTS + event;

	e->classname = "tempEntity";
	e->eventTime = level.time;
	e->freeAfterEvent = qtrue;

	VectorCopy( origin, snapped );
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( e, snapped );

	// find cluster for PVS
	trap_LinkEntity( e );

	return e;
}

/*
=====================================================================
Game sound at location entity owner info
=====================================================================
*/
gentity_t *JKMod_G_PreDefSound(vec3_t org, pdSounds_t pdSound, int dimensionOwner)
{
	gentity_t	*te;

	te = JKMod_G_TempEntity( org, EV_PREDEFSOUND, dimensionOwner );
	te->s.eventParm = pdSound;
	VectorCopy(org, te->s.origin);

	return te;
}

/*
=====================================================================
Game sound at location entity with owner info
=====================================================================
*/
void JKMod_G_SoundAtLoc(vec3_t loc, soundChannel_t channel, int soundIndex, int dimensionOwner) 
{
	gentity_t	*te;

	te = JKMod_G_TempEntity( loc, EV_GENERAL_SOUND, dimensionOwner );
	te->s.eventParm = soundIndex;
}

/*
=====================================================================
Test line event with owner info
=====================================================================
*/
void JKMod_G_TestLine(vec3_t start, vec3_t end, int color, int time, int dimensionOwner)
{
	gentity_t *te;

	te = JKMod_G_TempEntity( start, EV_TESTLINE, dimensionOwner );
	VectorCopy(start, te->s.origin);
	VectorCopy(end, te->s.origin2);
	te->s.time2 = time;
	te->s.weapon = color;
	te->r.svFlags |= SVF_BROADCAST;
}

/*
=====================================================================
Spawns an item and tosses it forward with owner info
=====================================================================
*/
extern gentity_t *droppedRedFlag;
extern gentity_t *droppedBlueFlag;
extern qboolean	itemRegistered[MAX_ITEMS];

gentity_t *JKMod_LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity, int dimensionOwner ) {
	gentity_t	*dropped;
	
	JKMod_Printf(S_COLOR_MAGENTA "Dropped %s from client %i\n", item->classname, dimensionOwner);

	dropped = JKMod_G_Spawn( dimensionOwner );
	dropped->parent = &g_entities[dimensionOwner]; // store parent number
	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - bg_itemlist;	// store item number in modelindex
	if (dropped->s.modelindex < 0)
	{
		dropped->s.modelindex = 0;
	}
	dropped->s.modelindex2 = 1; // This is non-zero is it's a dropped item

	dropped->classname = item->classname;
	dropped->item = item;

	if (!itemRegistered[dropped->s.modelindex]) { // Save item registered
		RegisterItem(item); 
		SaveRegisteredItems();
	}

	VectorSet (dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet (dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);

	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin( dropped, origin );
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy( velocity, dropped->s.pos.trDelta );

	dropped->s.eFlags |= EF_BOUNCE_HALF;
	if ((g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) && item->giType == IT_TEAM) { // Special case for CTF flags
		dropped->think = Team_DroppedFlagThink;
		dropped->nextthink = level.time + 30000;
		Team_CheckDroppedItem( dropped );

		//rww - so bots know
		if (strcmp(dropped->classname, "team_CTF_redflag") == 0)
		{
			droppedRedFlag = dropped;
		}
		else if (strcmp(dropped->classname, "team_CTF_blueflag") == 0)
		{
			droppedBlueFlag = dropped;
		}
	} else { // auto-remove after 30 seconds
		dropped->think = G_FreeEntity;
		dropped->nextthink = level.time + 30000;
	}

	dropped->flags = FL_DROPPED_ITEM;

	if (item->giType == IT_WEAPON || item->giType == IT_POWERUP)
	{
		dropped->s.eFlags |= EF_DROPPEDWEAPON;
	}

	vectoangles(velocity, dropped->s.angles);
	dropped->s.angles[PITCH] = 0;

	if (item->giTag == WP_TRIP_MINE ||
		item->giTag == WP_DET_PACK)
	{
		dropped->s.angles[PITCH] = -90;
	}

	if (item->giTag != WP_BOWCASTER &&
		item->giTag != WP_DET_PACK &&
		item->giTag != WP_THERMAL)
	{
		dropped->s.angles[ROLL] = -90;
	}

	dropped->physicsObject = qtrue;

	trap_LinkEntity (dropped);

	return dropped;
}
