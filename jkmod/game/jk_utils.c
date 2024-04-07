/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Utils functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern void G_TestLine(vec3_t start, vec3_t end, int color, int time);

/*
=====================================================================
Draw box lines (Due clientside: 255 = red, 1 = black, 0 = white)
=====================================================================
*/
void JKMod_DrawBoxLines(vec3_t orig, vec3_t mins, vec3_t maxs, int color, int duration)
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

		G_TestLine(point1, point2, color, duration);
		G_TestLine(point2, point3, color, duration);
		G_TestLine(point1, point4, color, duration);
		G_TestLine(point4, point3, color, duration);

		//+ face
		point1[vec[0]] = point2[vec[0]] = point3[vec[0]] = point4[vec[0]] = bmaxs[vec[0]];

		G_TestLine(point1, point2, color, duration);
		G_TestLine(point2, point3, color, duration);
		G_TestLine(point1, point4, color, duration);
		G_TestLine(point4, point1, color, duration);
	}
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
		if (developer.integer) JKMod_DrawBoxLines(dest, mins, maxs, 255, 500);
		JKMod_Printf(S_COLOR_YELLOW "Can't spawn here, we are in solid\n");
		return qfalse;
	}

	if (developer.integer) JKMod_DrawBoxLines(dest, mins, maxs, 0, 500);
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
