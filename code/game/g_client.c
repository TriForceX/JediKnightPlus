// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"
#include "../ghoul2/g2.h"

// g_client.c -- client functions that don't happen every frame

static vec3_t	playerMins = {-15, -15, DEFAULT_MINS_2};
static vec3_t	playerMaxs = {15, 15, DEFAULT_MAXS_2};

forcedata_t Client_Force[MAX_CLIENTS];

/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
potential spawning position for deathmatch games.
The first time a player enters the game, they will be at an 'initial' spot.
Targets will be fired when someone spawns in on them.
"nobots" will prevent bots from using this spot.
"nohumans" will prevent non-bots from using this spot.
*/
void SP_info_player_deathmatch( gentity_t *ent ) {
	int		i;

	G_SpawnInt( "nobots", "0", &i);
	if ( i ) {
		ent->flags |= FL_NO_BOTS;
	}
	G_SpawnInt( "nohumans", "0", &i );
	if ( i ) {
		ent->flags |= FL_NO_HUMANS;
	}
}

/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
equivelant to info_player_deathmatch
*/
void SP_info_player_start(gentity_t *ent) {
	ent->classname = "info_player_deathmatch";
	SP_info_player_deathmatch( ent );
}

/*QUAKED info_player_imperial (1 0 0) (-16 -16 -24) (16 16 32)
saga start point - imperial
*/
void SP_info_player_imperial(gentity_t *ent) {
	if (g_gametype.integer != GT_SAGA)
	{ //turn into a DM spawn if not in saga game mode
		ent->classname = "info_player_deathmatch";
		SP_info_player_deathmatch( ent );
	}
}

/*QUAKED info_player_rebel (1 0 0) (-16 -16 -24) (16 16 32)
saga start point - rebel
*/
void SP_info_player_rebel(gentity_t *ent) {
	if (g_gametype.integer != GT_SAGA)
	{ //turn into a DM spawn if not in saga game mode
		ent->classname = "info_player_deathmatch";
		SP_info_player_deathmatch( ent );
	}
}

/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
The intermission will be viewed from this point.  Target an info_notnull for the view direction.
*/
void SP_info_player_intermission( gentity_t *ent ) {

}

#define JMSABER_RESPAWN_TIME 20000 //in case it gets stuck somewhere no one can reach

void ThrowSaberToAttacker(gentity_t *self, gentity_t *attacker)
{
	gentity_t *ent = &g_entities[self->client->ps.saberIndex];
	vec3_t a;
	int altVelocity = 0;

	if (!ent || ent->enemy != self)
	{ //something has gone very wrong (this should never happen)
		//but in case it does.. find the saber manually
#ifdef _DEBUG
		Com_Printf("Lost the saber! Attempting to use global pointer..\n");
#endif
		ent = gJMSaberEnt;

		if (!ent)
		{
#ifdef _DEBUG
			Com_Printf("The global pointer was NULL. This is a bad thing.\n");
#endif
			return;
		}

#ifdef _DEBUG
		Com_Printf("Got it (%i). Setting enemy to client %i.\n", ent->s.number, self->s.number);
#endif

		ent->enemy = self;
		self->client->ps.saberIndex = ent->s.number;
	}

	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, "-1" );

	if (attacker && attacker->client && self->client->ps.saberInFlight)
	{ //someone killed us and we had the saber thrown, so actually move this saber to the saber location
	  //if we killed ourselves with saber thrown, however, same suicide rules of respawning at spawn spot still
	  //apply.
		gentity_t *flyingsaber = &g_entities[self->client->ps.saberEntityNum];

		if (flyingsaber && flyingsaber->inuse)
		{
			VectorCopy(flyingsaber->s.pos.trBase, ent->s.pos.trBase);
			VectorCopy(flyingsaber->s.pos.trDelta, ent->s.pos.trDelta);
			VectorCopy(flyingsaber->s.apos.trBase, ent->s.apos.trBase);
			VectorCopy(flyingsaber->s.apos.trDelta, ent->s.apos.trDelta);

			VectorCopy(flyingsaber->r.currentOrigin, ent->r.currentOrigin);
			VectorCopy(flyingsaber->r.currentAngles, ent->r.currentAngles);
			altVelocity = 1;
		}
	}

	self->client->ps.saberInFlight = qtrue; //say he threw it anyway in order to properly remove from dead body

	ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
	ent->s.eFlags &= ~(EF_NODRAW);
	ent->s.modelGhoul2 = 1;
	ent->s.eType = ET_MISSILE;
	ent->enemy = NULL;

	if (!attacker || !attacker->client)
	{
		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
		VectorCopy(ent->s.origin2, ent->s.origin);
		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
		ent->jkmodEnt.time1/*pos2[0]*/ = 0; // Tr!Force: [Pause] Think time
		trap_LinkEntity(ent);
		return;
	}

	if (!altVelocity)
	{
		VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
		VectorCopy(self->s.pos.trBase, ent->s.origin);
		VectorCopy(self->s.pos.trBase, ent->r.currentOrigin);

		VectorSubtract(attacker->client->ps.origin, ent->s.pos.trBase, a);

		VectorNormalize(a);

		ent->s.pos.trDelta[0] = a[0]*256;
		ent->s.pos.trDelta[1] = a[1]*256;
		ent->s.pos.trDelta[2] = 256;
	}

	trap_LinkEntity(ent);
}

void JMSaberThink(gentity_t *ent)
{
	gJMSaberEnt = ent;

	if (ent->enemy)
	{
		if (!ent->enemy->client || !ent->enemy->inuse)
		{ //disconnected?
			VectorCopy(ent->enemy->s.pos.trBase, ent->s.pos.trBase);
			VectorCopy(ent->enemy->s.pos.trBase, ent->s.origin);
			VectorCopy(ent->enemy->s.pos.trBase, ent->r.currentOrigin);
			ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
			ent->s.eFlags &= ~(EF_NODRAW);
			ent->s.modelGhoul2 = 1;
			ent->s.eType = ET_MISSILE;
			ent->enemy = NULL;

			// ent->pos2[0] = 1; // Tr!Force: [Pause] Skip this
			ent->jkmodEnt.time1/*pos2[1]*/ = 0; //respawn next think // Tr!Force: [Pause] Think time
			trap_LinkEntity(ent);
		}
		else
		{
			ent->jkmodEnt.time1/*pos2[1]*/ = level.time + JMSABER_RESPAWN_TIME; // Tr!Force: [Pause] Think time
		}
	}
	else if (ent->jkmodEnt.time1/*pos2[0]*/ && ent->jkmodEnt.time1/*pos2[1]*/ < level.time) // Tr!Force: [Pause] Think time
	{
		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
		VectorCopy(ent->s.origin2, ent->s.origin);
		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
		ent->jkmodEnt.time1/*pos2[0]*/ = 0; // Tr!Force: [Pause] Think time
		trap_LinkEntity(ent);
	}

	ent->nextthink = level.time + 50;
	G_RunObject(ent);
}

void JMSaberTouch(gentity_t *self, gentity_t *other, trace_t *trace)
{
	int i = 0;
//	gentity_t *te;

	if (!other || !other->client || other->health < 1)
	{
		return;
	}

	if (self->enemy)
	{
		return;
	}

	if (!self->s.modelindex)
	{
		return;
	}

	if (other->client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER))
	{
		return;
	}

	if (other->client->ps.isJediMaster)
	{
		return;
	}

	self->enemy = other;
	other->client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
	other->client->ps.weapon = WP_SABER;
	other->s.weapon = WP_SABER;
	G_AddEvent(other, EV_BECOME_JEDIMASTER, 0);

	// Track the jedi master 
	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, va("%i", other->s.number ) );

	if (g_spawnInvulnerability.integer)
	{
		other->client->ps.eFlags |= EF_INVULNERABLE;
		other->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
	}

	G_CenterPrint( -1, 3, va("%s" S_COLOR_WHITE " %s\n", other->client->pers.netname, G_GetStripEdString("SVINGAME", "BECOMEJM")) );

	other->client->ps.isJediMaster = qtrue;
	other->client->ps.saberIndex = self->s.number;

	if (other->health < 200 && other->health > 0)
	{ //full health when you become the Jedi Master
		other->client->ps.stats[STAT_HEALTH] = other->health = 200;
	}

	if (other->client->ps.fd.forcePower < 100)
	{
		other->client->ps.fd.forcePower = 100;
	}

	while (i < NUM_FORCE_POWERS)
	{
		other->client->ps.fd.forcePowersKnown |= (1 << i);
		other->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;

		i++;
	}

	// self->pos2[0] = 1; // Tr!Force: [Pause] Skip this
	self->jkmodEnt.time1/*pos2[1]*/ = level.time + JMSABER_RESPAWN_TIME; // Tr!Force: [Pause] Think time

	self->s.modelindex = 0;
	self->s.eFlags |= EF_NODRAW;
	self->s.modelGhoul2 = 0;
	self->s.eType = ET_GENERAL;

	/*
	te = G_TempEntity( vec3_origin, EV_DESTROY_GHOUL2_INSTANCE );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = self->s.number;
	*/
	G_KillG2Queue(self->s.number);

	return;
}

gentity_t *gJMSaberEnt = NULL;

/*QUAKED info_jedimaster_start (1 0 0) (-16 -16 -24) (16 16 32)
"jedi master" saber spawn point
*/
void SP_info_jedimaster_start(gentity_t *ent)
{
	if (g_gametype.integer != GT_JEDIMASTER)
	{
		gJMSaberEnt = NULL;
		G_FreeEntity(ent);
		return;
	}

	ent->enemy = NULL;

	ent->s.eFlags = EF_BOUNCE_HALF;

	ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
	ent->s.modelGhoul2 = 1;
	ent->s.g2radius = 20;
	//ent->s.eType = ET_GENERAL;
	ent->s.eType = ET_MISSILE;
	ent->s.weapon = WP_SABER;
	ent->s.pos.trType = TR_GRAVITY;
	ent->s.pos.trTime = level.time;
	VectorSet( ent->r.maxs, 3, 3, 3 );
	VectorSet( ent->r.mins, -3, -3, -3 );
	ent->r.contents = CONTENTS_TRIGGER;
	ent->clipmask = MASK_SOLID;

	ent->isSaberEntity = qtrue;

	ent->bounceCount = -5;

	ent->physicsObject = qtrue;

	VectorCopy(ent->s.pos.trBase, ent->s.origin2); //remember the spawn spot

	ent->touch = JMSaberTouch;

	trap_LinkEntity(ent);

	ent->think = JMSaberThink;
	ent->nextthink = level.time + 50;
}

/*
=======================================================================

  SelectSpawnPoint

=======================================================================
*/

/*
================
SpotWouldTelefrag

================
*/
qboolean SpotWouldTelefrag( gentity_t *spot ) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;

	VectorAdd( spot->s.origin, playerMins, mins );
	VectorAdd( spot->s.origin, playerMaxs, maxs );
	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for (i=0 ; i<num ; i++) {
		hit = &g_entities[touch[i]];
		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
		if ( hit->client) {
			return qtrue;
		}

	}

	return qfalse;
}

/*
================
SelectNearestDeathmatchSpawnPoint

Find the spot that we DON'T want to use
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist, nearestDist;
	gentity_t	*nearestSpot;

	nearestDist = 999999;
	nearestSpot = NULL;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {

		VectorSubtract( spot->s.origin, from, delta );
		dist = VectorLength( delta );
		if ( dist < nearestDist ) {
			nearestDist = dist;
			nearestSpot = spot;
		}
	}

	return nearestSpot;
}


/*
================
SelectRandomDeathmatchSpawnPoint

go to a random point that doesn't telefrag
================
*/
#define	MAX_SPAWN_POINTS	128
gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
	gentity_t	*spot;
	int			count;
	int			selection;
	gentity_t	*spots[MAX_SPAWN_POINTS];

	count = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		spots[ count ] = spot;
		count++;
	}

	if ( !count ) {	// no spots that won't telefrag
		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
	}

	selection = rand() % count;
	return spots[ selection ];
}

/*
===========
SelectRandomFurthestSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectRandomFurthestSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
	gentity_t	*spot;
	vec3_t		delta;
	float		dist;
	float		list_dist[64];
	gentity_t	*list_spot[64];
	int			numSpots, rnd, i, j;

	numSpots = 0;
	spot = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( SpotWouldTelefrag( spot ) ) {
			continue;
		}
		VectorSubtract( spot->s.origin, avoidPoint, delta );
		dist = VectorLength( delta );
		for (i = 0; i < numSpots; i++) {
			if ( dist > list_dist[i] ) {
				if ( numSpots >= 64 )
					numSpots = 64-1;
				for (j = numSpots; j > i; j--) {
					list_dist[j] = list_dist[j-1];
					list_spot[j] = list_spot[j-1];
				}
				list_dist[i] = dist;
				list_spot[i] = spot;
				numSpots++;
				if (numSpots > 64)
					numSpots = 64;
				break;
			}
		}
		if (i >= numSpots && numSpots < 64) {
			list_dist[numSpots] = dist;
			list_spot[numSpots] = spot;
			numSpots++;
		}
	}
	if (!numSpots) {
		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
		if (!spot)
		{
			G_Error( "Couldn't find a spawn point" );
		}
		VectorCopy (spot->s.origin, origin);
		origin[2] += 9;
		VectorCopy (spot->s.angles, angles);
		return spot;
	}

	// select a random spot from the spawn points furthest away
	rnd = random() * (numSpots / 2);

	VectorCopy (list_spot[rnd]->s.origin, origin);
	origin[2] += 9;
	VectorCopy (list_spot[rnd]->s.angles, angles);

	return list_spot[rnd];
}

/*
===========
SelectSpawnPoint

Chooses a player start, deathmatch start, etc
============
*/
gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
	return SelectRandomFurthestSpawnPoint( avoidPoint, origin, angles );

	/*
	gentity_t	*spot;
	gentity_t	*nearestSpot;

	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );

	spot = SelectRandomDeathmatchSpawnPoint ( );
	if ( spot == nearestSpot ) {
		// roll again if it would be real close to point of death
		spot = SelectRandomDeathmatchSpawnPoint ( );
		if ( spot == nearestSpot ) {
			// last try
			spot = SelectRandomDeathmatchSpawnPoint ( );
		}		
	}

	// find a single player start spot
	if (!spot) {
		G_Error( "Couldn't find a spawn point" );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
	*/
}

/*
===========
SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles ) {
	gentity_t	*spot;

	spot = NULL;
	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if ( spot->spawnflags & 1 ) {
			break;
		}
	}

	if ( !spot || SpotWouldTelefrag( spot ) ) {
		return SelectSpawnPoint( vec3_origin, origin, angles );
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);

	return spot;
}

/*
===========
SelectSpectatorSpawnPoint

============
*/
gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
	FindIntermissionPoint();

	VectorCopy( level.intermission_origin, origin );
	VectorCopy( level.intermission_angle, angles );

	return NULL;
}

/*
=======================================================================

BODYQUE

=======================================================================
*/

/*
=======================================================================

BODYQUE

=======================================================================
*/

#define BODY_SINK_TIME		45000

/*
===============
InitBodyQue
===============
*/
void InitBodyQue (void) {
	int		i;
	gentity_t	*ent;

	level.bodyQueIndex = 0;
	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
		ent = JKMod_G_Spawn( ENTITYNUM_NONE ); // Tr!Force: [Dimensions] Tag owner info
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		level.bodyQue[i] = ent;
	}
}

/*
=============
BodySink

After sitting around for five seconds, fall into the ground and dissapear
=============
*/
void BodySink( gentity_t *ent ) {
	if ( level.time - ent->timestamp > BODY_SINK_TIME + 1500 ) {
		// the body ques are never actually freed, they are just unlinked
		trap_UnlinkEntity( ent );
		ent->physicsObject = qfalse;
		return;	
	}
	ent->nextthink = level.time + 100;
	ent->s.pos.trBase[2] -= 1;
}

/*
=============
CopyToBodyQue

A player is respawning, so make an entity that looks
just like the existing corpse to leave behind.
=============
*/
void CopyToBodyQue( gentity_t *ent ) {
	gentity_t		*body;
	int			contents;

	if (level.intermissiontime)
	{
		return;
	}

	if ( !ent || !ent->client ) return;

	trap_UnlinkEntity (ent);

	// if client is in a nodrop area, don't leave the body
	contents = trap_PointContents( ent->s.origin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		return;
	}

	if (ent->client->ps.eFlags & EF_DISINTEGRATION)
	{ //for now, just don't spawn a body if you got disint'd
		return;
	}

	// grab a body que and cycle to the next one
	body = level.bodyQue[ level.bodyQueIndex ];
	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;

	trap_UnlinkEntity (body);
	body->s = ent->s;

	//avoid oddly angled corpses floating around
	body->s.angles[PITCH] = body->s.angles[ROLL] = body->s.apos.trBase[PITCH] = body->s.apos.trBase[ROLL] = 0;

	body->s.g2radius = 100;

	body->s.eType = ET_BODY;
	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc

	if (ent->client->ps.eFlags & EF_DISINTEGRATION)
	{
		body->s.eFlags |= EF_DISINTEGRATION;
	}

	VectorCopy(ent->client->ps.lastHitLoc, body->s.origin2);

	body->s.powerups = 0;	// clear powerups
	body->s.loopSound = 0;	// clear lava burning
	body->s.number = body - g_entities;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->physicsBounce = 0;		// don't bounce
	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
		body->s.pos.trType = TR_GRAVITY;
		body->s.pos.trTime = level.time;
		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
	} else {
		body->s.pos.trType = TR_STATIONARY;
	}
	body->s.event = 0;

	body->s.weapon = ent->s.bolt2;

	if (body->s.weapon == WP_SABER && ent->client->ps.saberInFlight)
	{
		body->s.weapon = WP_BLASTER; //lie to keep from putting a saber on the corpse, because it was thrown at death
	}

	G_AddEvent(body, EV_BODY_QUEUE_COPY, ent->s.clientNum);

	body->r.svFlags = ent->r.svFlags | SVF_BROADCAST;
	VectorCopy (ent->r.mins, body->r.mins);
	VectorCopy (ent->r.maxs, body->r.maxs);
	VectorCopy (ent->r.absmin, body->r.absmin);
	VectorCopy (ent->r.absmax, body->r.absmax);

	body->s.torsoAnim = body->s.legsAnim = ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT;

	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_CORPSE;
	body->r.ownerNum = ent->s.number;

	body->nextthink = level.time + BODY_SINK_TIME;
	body->think = BodySink;

	body->die = body_die;

	// don't take more damage if already gibbed
	if ( ent->health <= GIB_HEALTH ) {
		body->takedamage = qfalse;
	} else {
		body->takedamage = qtrue;
	}

	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
	JKMod_DimensionOwnerCheck( ent->s.number, body ); // Tr!Force: [Dimensions] Entity owner check
	trap_LinkEntity (body);
}

//======================================================================


/*
==================
SetClientViewAngle

==================
*/
void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
	int			i;

	// set the delta angle
	for (i=0 ; i<3 ; i++) {
		int		cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy( angle, ent->s.angles );
	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
}

/*
================
respawn
================
*/
void respawn( gentity_t *ent ) {
	gentity_t	*tent;

	CopyToBodyQue (ent);

	if (gEscaping)
	{
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
		ent->client->sess.spectatorState = SPECTATOR_FREE;
		ent->client->sess.spectatorClient = 0;

		ent->client->pers.teamState.state = TEAM_BEGIN;
	}

	trap_UnlinkEntity (ent);
	ClientSpawn(ent);

	// add a teleportation effect
	tent = JKMod_G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN, ent->s.number ); // Tr!Force: [Dimensions] Tag owner info
	tent->s.clientNum = ent->s.clientNum;
}

/*
================
TeamCount

Returns number of players on a team
================
*/
team_t TeamCount( int ignoreClientNum, team_t team ) {
	int		i;
	int		count = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( i == ignoreClientNum ) {
			continue;
		}
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( level.clients[i].sess.sessionTeam == team ) {
			count++;
		}
	}

	return count;
}

/*
================
TeamLeader

Returns the client number of the team leader
================
*/
int TeamLeader( team_t team ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( level.clients[i].sess.sessionTeam == team ) {
			if ( level.clients[i].sess.teamLeader )
				return i;
		}
	}

	return -1;
}


/*
================
PickTeam

================
*/
team_t PickTeam( int ignoreClientNum ) {
	int		counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );

	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
		return TEAM_RED;
	}
	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
		return TEAM_BLUE;
	}
	// equal team count, so join the team with the lowest score
	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
		return TEAM_RED;
	}
	return TEAM_BLUE;
}

/*
===========
ForceClientSkin

Forces a client's skin (for teamplay)
===========
*/
/*
static void ForceClientSkin( gclient_t *client, char *model, const char *skin ) {
	char *p;

	if ((p = Q_strrchr(model, '/')) != 0) {
		*p = 0;
	}

	Q_strcat(model, MAX_QPATH, "/");
	Q_strcat(model, MAX_QPATH, skin);
}
*/

/*
===========
ClientCheckName
============
*/
static void ClientCleanName( const char *in, char *out, int outSize ) {
	int		len, colorlessLen;
	char	ch;
	char	*p;
	int		spaces;

	//save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while( 1 ) {
		ch = *in++;
		if( !ch ) {
			break;
		}

		// don't allow leading spaces
		if( !*p && ch == ' ' ) {
			continue;
		}

		// check colors
		if( ch == Q_COLOR_ESCAPE ) {
			// solo trailing carat is not a color prefix
			if( !*in ) {
				break;
			}

			// don't allow black in a name, period
			if( ColorIndex(*in) == 0 ) {
				in++;
				continue;
			}

			// make sure room in dest for both chars
			if( len > outSize - 2 ) {
				break;
			}

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// don't allow too many consecutive spaces
		if( ch == ' ' ) {
			spaces++;
			if( spaces > 3 ) {
				continue;
			}
		}
		else {
			spaces = 0;
		}

		if( len > outSize - 1 ) {
			break;
		}

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = 0;

	// don't allow empty names
	if( *p == 0 || colorlessLen == 0 ) {
		Q_strncpyz( p, "Padawan", outSize );
	}
}

#ifdef _DEBUG
void G_DebugWrite(const char *path, const char *text)
{
	fileHandle_t f;

	trap_FS_FOpenFile( path, &f, FS_APPEND );
	trap_FS_Write(text, strlen(text), f);
	trap_FS_FCloseFile(f);
}
#endif

/*
===========
SetupGameGhoul2Model

There are two ghoul2 model instances per player (actually three).  One is on the clientinfo (the base for the client side 
player, and copied for player spawns and for corpses).  One is attached to the centity itself, which is the model acutally 
animated and rendered by the system.  The final is the game ghoul2 model.  This is animated by pmove on the server, and
is used for determining where the lightsaber should be, and for per-poly collision tests.
===========
*/
void *g2SaberInstance = NULL;
void SetupGameGhoul2Model(gclient_t *client, char *modelname)
{
	int handle;
	char		afilename[MAX_QPATH];
	char		/**GLAName,*/ *slash;
	char		GLAName[MAX_QPATH];
	vec3_t	tempVec = {0,0,0};

	// First things first.  If this is a ghoul2 model, then let's make sure we demolish this first.
	if (client->ghoul2 && trap_G2_HaveWeGhoul2Models(client->ghoul2))
	{
		trap_G2API_CleanGhoul2Models(&(client->ghoul2));
	}

	/*
	Com_sprintf( afilename, sizeof( afilename ), "models/players/%s/model.glm", modelname );
	handle = trap_G2API_InitGhoul2Model(&client->ghoul2, afilename, 0, 0, -20, 0, 0);
	if (handle<0)
	{
		Com_sprintf( afilename, sizeof( afilename ), "models/players/kyle/model.glm" );
		handle = trap_G2API_InitGhoul2Model(&client->ghoul2, afilename, 0, 0, -20, 0, 0);

		if (handle<0)
		{
			return;
		}
	}
	*/

	//rww - just load the "standard" model for the server"
	if (!precachedKyle)
	{
		Com_sprintf( afilename, sizeof( afilename ), "models/players/kyle/model.glm" );
		handle = trap_G2API_InitGhoul2Model(&precachedKyle, afilename, 0, 0, -20, 0, 0);

		if (handle<0)
		{
			return;
		}
	}

	if (precachedKyle && trap_G2_HaveWeGhoul2Models(precachedKyle))
	{
		trap_G2API_DuplicateGhoul2Instance(precachedKyle, &client->ghoul2);
	}
	else
	{
		return;
	}

	// The model is now loaded.

	GLAName[0] = 0;

	if (!BGPAFtextLoaded)
	{
		//get the location of the animation.cfg
		//GLAName = trap_G2API_GetGLAName( client->ghoul2, 0);
		trap_G2API_GetGLAName( client->ghoul2, 0, GLAName);

		if (!GLAName[0])
		{
			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
			{
				Com_Printf( "Failed to load animation file %s\n", afilename );
				return;
			}
			return;
		}
		Q_strncpyz( afilename, GLAName, sizeof( afilename ));
		slash = Q_strrchr( afilename, '/' );
		if ( slash )
		{
			strcpy(slash, "/animation.cfg");
		}	// Now afilename holds just the path to the animation.cfg
		else 
		{	// Didn't find any slashes, this is a raw filename right in base (whish isn't a good thing)
			return;
		}

		// Try to load the animation.cfg for this model then.
		if ( !BG_ParseAnimationFile( afilename ) )
		{	// The GLA's animations failed
			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
			{
				Com_Printf( "Failed to load animation file %s\n", afilename );
				return;
			}
		}
	}

	trap_G2API_AddBolt(client->ghoul2, 0, "*r_hand");
	trap_G2API_AddBolt(client->ghoul2, 0, "*l_hand");

	// NOTE - ensure this sequence of bolt and bone accessing are always the same because the client expects them in a certain order
	trap_G2API_SetBoneAnim(client->ghoul2, 0, "model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, level.time, -1, -1);
	trap_G2API_SetBoneAngles(client->ghoul2, 0, "upper_lumbar", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time);
	trap_G2API_SetBoneAngles(client->ghoul2, 0, "cranium", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, level.time);

	if (!g2SaberInstance)
	{
		trap_G2API_InitGhoul2Model(&g2SaberInstance, "models/weapons2/saber/saber_w.glm", 0, 0, -20, 0, 0);

		if (g2SaberInstance)
		{
			// indicate we will be bolted to model 0 (ie the player) on bolt 0 (always the right hand) when we get copied
			trap_G2API_SetBoltInfo(g2SaberInstance, 0, 0);
			// now set up the gun bolt on it
			trap_G2API_AddBolt(g2SaberInstance, 0, "*flash");
		}
	}

	if (g2SaberInstance)
	{
		trap_G2API_CopySpecificGhoul2Model(g2SaberInstance, 0, client->ghoul2, 1); 
	}
}




/*
===========
ClientUserInfoChanged

Called from ClientConnect when the player first connects and
directly by the server system when the player updates a userinfo variable.

The game can override any of the settings and call trap_SetUserinfo
if desired.
============
*/
void ClientUserinfoChanged( int clientNum ) {
	gentity_t *ent;
	int		teamTask, teamLeader, team, health;
	char	*s;
	char	model[MAX_QPATH];
	//char	headModel[MAX_QPATH];
	char	forcePowers[MAX_QPATH];
	char	oldname[MAX_STRING_CHARS];
	gclient_t	*client;
	char	c1[MAX_INFO_STRING];
	char	c2[MAX_INFO_STRING];
	char	redTeam[MAX_INFO_STRING];
	char	blueTeam[MAX_INFO_STRING];
	char	userinfo[MAX_INFO_STRING];
	int		jkmod_hat;		// Tr!Force: [General] Custom client info
	int		jkmod_race;		// Tr!Force: [General] Custom client info

	// NameCrashFix (whitelisted characters)
	static const char	validChars[]  = " ~QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm1234567890<>?,./';:][{}`-=!@#$^&*()_+|";
	int					i, j, isValidChar;
	char				*ptr;

	ent = g_entities + clientNum;
	client = ent->client;

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	// check for malformed or illegal info strings
	if ( !Info_Validate(userinfo) ) {
		strcpy (userinfo, "\\name\\badinfo");
	}

	// check for local client
	s = Info_ValueForKey( userinfo, "ip" );
	if ( !strcmp( s, "localhost" ) ) {
		client->pers.localClient = qtrue;
	}

	// Tr!Force: [Plugin] Check plugin
	s = Info_ValueForKey( userinfo, "jkmod_client" );
	if ( strstr( s, JK_SHORTNAME ) ) {
		float clientVer = JKMod_GetVersion(s + 9);
		client->pers.jkmodPers.clientPlugin = clientVer >= level.jkmodLocals.serverVersion;
		client->pers.jkmodPers.clientVersion = clientVer;
	} else {
		client->pers.jkmodPers.clientPlugin = qfalse;
		client->pers.jkmodPers.clientVersion = qfalse;
	}

	// Tr!Force: [Duel] Private duel check
	if (client->pers.jkmodPers.clientPlugin)
	{
		s = Info_ValueForKey( userinfo, "jk_cg_privateDuel" );
		if ( !atoi( s ) ) {
			client->sess.jkmodSess.privateDuel = qfalse;
		} else {
			client->sess.jkmodSess.privateDuel = qtrue;
		}
	}

	// Tr!Force: [Duel] Auto duel check
	if (client->pers.jkmodPers.clientPlugin)
	{
		s = Info_ValueForKey( userinfo, "jk_cg_autoDuel" );
		if ( !atoi( s ) ) {
			client->sess.jkmodSess.autoDuel = qfalse;
		} else {
			client->sess.jkmodSess.autoDuel = qtrue;
		}
	}

	// Tr!Force: [PlayerStatus] Auto status check
	if (client->pers.jkmodPers.clientPlugin)
	{
		s = Info_ValueForKey( userinfo, "jk_cg_chatAutoStatus" );
		if ( !atoi( s ) ) {
			client->sess.jkmodSess.playerStatus = qfalse;
		} else {
			client->sess.jkmodSess.playerStatus = qtrue;
		}
	}

	// check the item prediction
	s = Info_ValueForKey( userinfo, "cg_predictItems" );
	if ( !atoi( s ) ) {
		client->pers.predictItemPickup = qfalse;
	} else {
		client->pers.predictItemPickup = qtrue;
	}

	// set name
	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
	s = Info_ValueForKey (userinfo, "name");
	
	// NameCrashFix
	for ( i = 0; i < (int)strlen(s); i++ )
	{
		isValidChar = 0;

		for ( j = 0; validChars[j]; j++ )
		{
			if ( s[i] == validChars[j] ) isValidChar = 1; //The char is on the whitelist - it's a valid Char...
		}

		if ( !isValidChar )	s[i] = '.';
	}
	
	// Don't let players use @@@ in their names (multi-language strings)
	ptr = strstr( s, "@@@" );
	while ( ptr )
	{
		memset( ptr, '.', 3 );
		ptr = strstr( s, "@@@" );
	}
	
	JKMod_ClientCleanName( s, ent->client->pers.netname, sizeof(ent->client->pers.netname), ent); // Tr!Force: [General] Client clean name
	Info_RemoveKey( userinfo, "name" );
	Info_SetValueForKey( userinfo, "name", ent->client->pers.netname );
	trap_SetUserinfo( clientNum, userinfo );

	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
		}
	}

	if ( client->pers.connected == CON_CONNECTED ) {
		if ( strcmp( oldname, client->pers.netname ) ) {
			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s %s\n\"", oldname, G_GetStripEdString("SVINGAME", "PLRENAME"),
				client->pers.netname) );
		}
	}

	// set max health
	health = 100; //atoi( Info_ValueForKey( userinfo, "handicap" ) );
	client->pers.maxHealth = health;
	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
		client->pers.maxHealth = 100;
	}
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;

	// set model
	if( g_gametype.integer >= GT_TEAM ) {
		Q_strncpyz( model, Info_ValueForKey (userinfo, "team_model"), sizeof( model ) );
		//Q_strncpyz( headModel, Info_ValueForKey (userinfo, "team_headmodel"), sizeof( headModel ) );
	} else {
		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
		//Q_strncpyz( headModel, Info_ValueForKey (userinfo, "headmodel"), sizeof( headModel ) );
	}
	
	// GalakingFix
	if ( g_mv_fixgalaking.integer && (!Q_stricmp(model, "galak_mech") || !Q_stricmpn(model, "galak_mech/", strlen("galak_mech/"))) )
	{
		Q_strncpyz( model, "galak/default", sizeof(model) );
	}
	
	if ( g_mv_fixbrokenmodels.integer && (!Q_stricmpn(model, "kyle/fpls", strlen("kyle/fpls")) || !Q_stricmp(model, "morgan") || (!Q_stricmpn(model, "morgan/", strlen("morgan/")) && (Q_stricmp(model, "morgan/default_mp") && Q_stricmp(model, "morgan/red") && Q_stricmp(model, "morgan/blue")))) )
	{
		Q_strncpyz( model, "kyle/default", sizeof(model) );
	}

	JKMod_PlayerModelInfo(ent, model); // Tr!Force: [GameGeneral] Update model and skin name

	Q_strncpyz( forcePowers, Info_ValueForKey (userinfo, "forcepowers"), sizeof( forcePowers ) );

	team = client->sess.sessionTeam;

	// Tr!Force: [GameGeneral] Update instant force change
	if (ent->client->pers.connected == CON_CONNECTED && ent->client->sess.sessionTeam != TEAM_SPECTATOR && client->jkmodClient.teamChangeDelay <= level.time)
	{
		if ((ent->client->pers.jkmodPers.customDuel == DUEL_FORCE || (ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_FORCE | DIMENSION_PRIVATE)) || jkcvar_forceChangeInstant.integer) && Q_stricmp(forcePowers, ent->client->pers.jkmodPers.forcePowers))
		{
			if (JKMod_PlayerMoving(ent, qtrue, qtrue)) {
				trap_SendServerCommand(ent - g_entities, "print \"You can't change force powers while moving\n\"");
			}
			else if (!jkcvar_forceChangeTime.integer || jkcvar_forceChangeInstant.integer == 1 && (ent->client->pers.jkmodPers.customDuel == DUEL_FORCE || ent->client->ps.stats[JK_DIMENSION] == DIMENSION_FORCE)) {
				JKMod_ForcePowerChange(ent, DIMENSION_FORCE);
			}
			else if (jkcvar_forceChangeInstant.integer == 1) {
				JKMod_ForcePowerChange(ent, DIMENSION_FREE);
			}
			else if (jkcvar_forceChangeTime.integer && jkcvar_forceChangeInstant.integer != 1 && !ent->client->jkmodClient.forceChangeDelay) {
				trap_SendServerCommand(ent - g_entities, va("print \"Applying force changes in %d seconds...\n\"", jkcvar_forceChangeTime.integer));
				ent->client->jkmodClient.forceChangeDelay = jkcvar_forceChangeTime.integer + 1;
			}
		}
	}

/*	NOTE: all client side now

	// team
	switch( team ) {
	case TEAM_RED:
		ForceClientSkin(client, model, "red");
//		ForceClientSkin(client, headModel, "red");
		break;
	case TEAM_BLUE:
		ForceClientSkin(client, model, "blue");
//		ForceClientSkin(client, headModel, "blue");
		break;
	}
	// don't ever use a default skin in teamplay, it would just waste memory
	// however bots will always join a team but they spawn in as spectator
	if ( g_gametype.integer >= GT_TEAM && team == TEAM_SPECTATOR) {
		ForceClientSkin(client, model, "red");
//		ForceClientSkin(client, headModel, "red");
	}
*/

	if (g_gametype.integer >= GT_TEAM) {
		client->pers.teamInfo = qtrue;
	} else {
		s = Info_ValueForKey( userinfo, "teamoverlay" );
		if ( ! *s || atoi( s ) != 0 ) {
			client->pers.teamInfo = qtrue;
		} else {
			client->pers.teamInfo = qfalse;
		}
	}
	/*
	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
	if ( !*s || atoi( s ) == 0 ) {
		client->pers.pmoveFixed = qfalse;
	}
	else {
		client->pers.pmoveFixed = qtrue;
	}
	*/

	// Tr!Force: [General] Custom user info
	s = Info_ValueForKey( userinfo, "jk_cg_customHats" );
	if ( ! *s || atoi( s ) != 0 ) {
		jkmod_hat = atoi( s );
	} else {
		jkmod_hat = 0;
	}

	// Tr!Force: [General] Custom user info
	jkmod_race = client->pers.jkmodPers.raceBestTime;

	// team task (0 = none, 1 = offence, 2 = defence)
	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
	// team Leader (1 = leader, 0 is normal player)
	teamLeader = client->sess.teamLeader;

	// colors
	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));

	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));

	// send over a subset of the userinfo keys so other clients can
	// print scoreboards, display models, and play custom sounds
	if ( ent->r.svFlags & SVF_BOT ) {
		s = va("n\\%s\\t\\%i\\model\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d\\mvgp\\%i\\jkhat\\%i\\jkrace\\%i",
			client->pers.netname, team, model,  c1, c2, 
			client->pers.maxHealth, client->sess.wins, client->sess.losses,
			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader, jk2gameplay, 
			// Tr!Force: [General] Custom user info
			jkmod_hat,
			jkmod_race
		);
	} else {
		s = va("n\\%s\\t\\%i\\model\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d\\mvgp\\%i\\jkhat\\%i\\jkrace\\%i",
			client->pers.netname, client->sess.sessionTeam, model, redTeam, blueTeam, c1, c2, 
			client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader, jk2gameplay, 
			// Tr!Force: [General] Custom user info
			jkmod_hat,
			jkmod_race
		);
	}
	
	trap_SetConfigstring( CS_PLAYERS+clientNum, s );

	if (g_logClientInfo.integer)
	{
		G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
	}
}


/*
===========
ClientConnect

Called when a player begins connecting to the server.
Called again for every map change or tournement restart.

The session information will be valid after exit.

Return NULL if the client should be allowed, otherwise return
a string with the reason for denial.

Otherwise, the client will be sent the current gamestate
and will eventually get to ClientBegin.

firstTime will be qtrue the very first time a client connects
to the server machine, but qfalse on map changes and tournement
restarts.
============
*/
qboolean MV_SetClientIP( int clientNum, char *value )
{
	mvclientSession_t *mvSess = &mv_clientSessions[clientNum];
	int i, j = 0, step = 0;
	char temp[4];

	if ( !value || !strlen(value) ) return qfalse;
	
	memset( temp, 0, sizeof(temp) );

	for ( i = 0; i < (int)strlen(value); i++ )
	{
		if ( value[i] == '.' || (value[i] == ':' && step == 3) )
		{
			mvSess->clientIP[step] = atoi(temp);

			memset( temp, 0, sizeof(temp) );
			step++;
			j = 0;

			if ( step == 4 ) break;
		}
		else if ( value[i] >= '0' && value[i] <= '9' && j < (int)sizeof(temp)-1 )
		{
			temp[j] = value[i];
			j++;
		}
		else return qfalse;
	}
	mvSess->localClient = qfalse;
	return qtrue;
}

char *BaseJK2_ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) { // Tr!Force: [BaseJK2] Client connect function
	char		*value;
//	char		*areabits;
	gclient_t	*client;
	char		userinfo[MAX_INFO_STRING];
	gentity_t	*ent;
	gentity_t	*te;
	mvclientSession_t *mvSess = &mv_clientSessions[clientNum];

	ent = &g_entities[ clientNum ];

	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );

	// check to see if they are on the banned IP list
	value = Info_ValueForKey (userinfo, "ip");
	if ( G_FilterPacket( value ) ) {
		return "Banned.";
	}

	memset( &mv_clientSessions[clientNum], 0, sizeof(mv_clientSessions[clientNum]) );
	if ( (ent->r.svFlags & SVF_BOT) || isBot || !Q_stricmp(value, "localhost") )
	{ // Bots and localhost get 127.0.0.1
		mvSess->clientIP[0] = 127;
		mvSess->clientIP[1] = 0;
		mvSess->clientIP[2] = 0;
		mvSess->clientIP[3] = 1;

		mvSess->localClient = qtrue;
	}
	else if ( firstTime && !MV_SetClientIP( clientNum, value ) ) return "Please wait...";

	if ( !firstTime ) MV_ReadSessionData( clientNum ); // If this isn't a "firstTime" read the stored IPs...
	if ( mvSess->clientIP[0] == 0 && mvSess->clientIP[1] == 0 && mvSess->clientIP[2] == 0 && mvSess->clientIP[3] == 0 ) return "was dropped due to an internal error."; // Should never happen, but just in case...
	
	if ( g_connectionlimit.integer && firstTime && !mvSess->localClient )
	{
		mvclientSession_t	*mvSessOther;
		gentity_t			*other;
		int					sameip;
		int					i;

		sameip = 0;

		for ( i = 0; i < MAX_CLIENTS; i++ )
		{
			if ( i == clientNum ) continue;

			other = &g_entities[i];
			mvSessOther = &mv_clientSessions[i];

			if ( other && other->client && (other->client->pers.connected == CON_CONNECTING || other->client->pers.connected == CON_CONNECTED || other->client->pers.connected != CON_DISCONNECTED)/*&& other->inuse*/ )
			{
				if ( ((mvSessOther->clientIP[0] == mvSess->clientIP[0]) && (mvSessOther->clientIP[1] == mvSess->clientIP[1]) && (mvSessOther->clientIP[2] == mvSess->clientIP[2]) && (mvSessOther->clientIP[3] == mvSess->clientIP[3])))
				{
					sameip++;
				}
			}
		}
		if ( sameip >= g_connectionlimit.integer )
		{
			return "Too many connections from your IP.";
		}
	}

	if ( g_connectinglimit.integer != 0 && firstTime && !mvSess->localClient )
	{
		mvclientSession_t	*mvSessOther;
		gentity_t			*other;
		int					alreadyConnecting;
		int					i;

		alreadyConnecting = 0;

		for ( i = 0; i < MAX_CLIENTS; i++ )
		{
			if ( i == clientNum ) continue;

			other = &g_entities[i];
			mvSessOther = &mv_clientSessions[i];

			if ( other && other->client && other->client->pers.connected == CON_CONNECTING )
			{
				if ( ((mvSessOther->clientIP[0] == mvSess->clientIP[0]) && (mvSessOther->clientIP[1] == mvSess->clientIP[1]) && (mvSessOther->clientIP[2] == mvSess->clientIP[2]) && (mvSessOther->clientIP[3] == mvSess->clientIP[3])) )
				{
					alreadyConnecting++;
				}
			}
		}
		if ( alreadyConnecting >= g_connectinglimit.integer )
		{
			return "Too many players from your IP are trying to connect at the same time.";
		}
	}

	if ( !( ent->r.svFlags & SVF_BOT ) && !isBot && g_needpass.integer ) {
		// check for a password
		value = Info_ValueForKey (userinfo, "password");
		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) &&
			strcmp( g_password.string, value) != 0) {
			static char sTemp[1024];
			Q_strncpyz(sTemp, G_GetStripEdString("SVINGAME","INVALID_PASSWORD"), sizeof (sTemp) );
			return sTemp;// return "Invalid password";
		}
	}

	// they can connect
	ent->client = level.clients + clientNum;
	client = ent->client;

//	areabits = client->areabits;

	memset( client, 0, sizeof(*client) );

	client->pers.connected = CON_CONNECTING;

	// read or initialize the session data
	if ( firstTime || level.newSession ) {
		G_InitSessionData( client, userinfo, isBot );
	}
	G_ReadSessionData( client );

	if( isBot ) {
		ent->r.svFlags |= SVF_BOT;
		ent->inuse = qtrue;
		if( !G_BotConnect( clientNum, !firstTime ) ) {
			return "BotConnectfailed";
		}
	}

	// get and distribute relevent paramters
	G_LogPrintf( "ClientConnect: %i\n", clientNum );
	ClientUserinfoChanged( clientNum );

	// don't do the "xxx connected" messages if they were caried over from previous level
	// Tr!Force: [Welcome] Don't show on client begin
	/*if ( firstTime ) {
		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLCONNECT")) );
	}*/

	if ( g_gametype.integer >= GT_TEAM &&
		client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BroadcastTeamChange( client, -1 );
	}

	// count current clients and rank for scoreboard
	CalculateRanks();

	te = JKMod_G_TempEntity( vec3_origin, EV_CLIENTJOIN, ENTITYNUM_WORLD ); // Tr!Force: [Dimensions] Tag owner info
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = clientNum;

	memset( &ent->client->ps, 0, sizeof(ent->client->ps) ); // Make sure we always use a fresh playerState for new clients (this allows us to check for powerups in the playerState to prevent flagEating when calling ClientBegin)
	// for statistics
//	client->areabits = areabits;
//	if ( !client->areabits )
//		client->areabits = G_Alloc( (trap_AAS_PointReachabilityAreaIndex( NULL ) + 7) / 8 );

	return NULL;
}

void G_WriteClientSessionData( gclient_t *client );

/*
===========
ClientBegin

called when a client has finished connecting, and is ready
to be placed into the level.  This will happen every level load,
and on transition between teams, but doesn't happen on respawns
============
*/
void BaseJK2_ClientBegin( int clientNum, qboolean allowTeamReset ) { // Tr!Force: [BaseJK2] Client begin function
	gentity_t	*ent;
	gclient_t	*client;
	gentity_t	*tent;
	int			flags, i;
	char		userinfo[MAX_INFO_VALUE], *modelname;
	int			jksave_player;		// Tr!Force: [General] Don't remove flags
	int			jksave_dimension;	// Tr!Force: [Dimensions] Don't remove flags
	int			jksave_tweaks;		// Tr!Force: [General] Don't remove flags

	ent = g_entities + clientNum;
	
	// FlagEatingFix - We must ensure that powerups are cleared on ClientConnect and before team changes. Otherwise we might accidently trigger a flag duplication here.
	for ( i = PW_REDFLAG; i <= PW_NEUTRALFLAG; i++ )
	{
		if ( ent->client->ps.powerups[i] )
		{
			gitem_t		*item;
			gentity_t	*drop;

			item = BG_FindItemForPowerup( i );
			if ( item )
			{
				drop = Drop_Item( ent, item, 45 );
				// decide how many seconds it has left
				drop->count = ( ent->client->ps.powerups[ i ] - level.time ) / 1000;
				if ( drop->count < 1 ) {
					drop->count = 1;
				}
			}

			ent->client->ps.powerups[i] = 0;
		}
	}

	if ((ent->r.svFlags & SVF_BOT) && g_gametype.integer >= GT_TEAM)
	{
		if (allowTeamReset && g_botTeamAutoBalance.integer)
		{
			const char *team = "Red";
			int preSess;

			if ( !ent->client->pers.botDelayed )
			{ // Delay bots until all clients are connected
				ent->client->pers.botDelayed = qtrue;
				return;
			}

			//SetTeam(ent, "");
			ent->client->sess.sessionTeam = PickTeam(clientNum);
			trap_GetUserinfo(clientNum, userinfo, MAX_INFO_STRING);

			if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
			{
				ent->client->sess.sessionTeam = TEAM_RED;
			}

			if (ent->client->sess.sessionTeam == TEAM_RED)
			{
				team = "Red";
			}
			else
			{
				team = "Blue";
			}

			Info_SetValueForKey( userinfo, "team", team );

			trap_SetUserinfo( clientNum, userinfo );

			ent->client->ps.persistant[ PERS_TEAM ] = ent->client->sess.sessionTeam;

			preSess = ent->client->sess.sessionTeam;
			G_ReadSessionData( ent->client );
			ent->client->sess.sessionTeam = preSess;
			G_WriteClientSessionData(ent->client);
			ClientUserinfoChanged( clientNum );
			ClientBegin(clientNum, qfalse);
			return;
		}
	}

	client = level.clients + clientNum;

	if ( ent->r.linked ) {
		trap_UnlinkEntity( ent );
	}
	JKMod_G_InitGentity( ent, clientNum ); // Tr!Force: [Dimensions] Tag owner info
	ent->touch = 0;
	ent->pain = 0;
	ent->client = client;

	client->pers.connected = CON_CONNECTED;
	client->pers.enterTime = level.time;
	client->pers.teamState.state = TEAM_BEGIN;

	// Tr!Force: [GameGeneral] Apply team change delay for certain cases
	if (!client->jkmodClient.teamChangeDelay) client->jkmodClient.teamChangeDelay = level.time + TEAM_CHANGE_DELAY;

	// save eflags around this, because changing teams will
	// cause this to happen with a valid entity, and we
	// want to make sure the teleport bit is set right
	// so the viewpoint doesn't interpolate through the
	// world to the new position
	flags = client->ps.eFlags;

	jksave_player = client->ps.stats[JK_PLAYER];		// Tr!Force: [General] Don't remove flags
	jksave_dimension = client->ps.stats[JK_DIMENSION];	// Tr!Force: [Dimensions] Don't remove flags
	jksave_tweaks = client->ps.stats[JK_TWEAKS];		// Tr!Force: [General] Don't remove flags

	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		if (ent->client->ps.fd.forcePowersActive & (1 << i))
		{
			WP_ForcePowerStop(ent, i);
		}
		i++;
	}

	i = TRACK_CHANNEL_1;

	while (i < NUM_TRACK_CHANNELS)
	{
		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0)
		{
			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
		}
		i++;
	}
	i = 0;

	memset( &client->ps, 0, sizeof( client->ps ) );
	client->ps.eFlags = flags;

	client->ps.stats[JK_PLAYER] = jksave_player;		// Tr!Force: [General] Don't remove flags
	client->ps.stats[JK_DIMENSION] = jksave_dimension;	// Tr!Force: [Dimensions] Don't remove flags
	client->ps.stats[JK_TWEAKS] = jksave_tweaks;		// Tr!Force: [General] Don't remove flags

	client->ps.hasDetPackPlanted = qfalse;

	//first-time force power initialization
	WP_InitForcePowers( ent );

	//init saber ent
	WP_SaberInitBladeData( ent );

	// First time model setup for that player.
	trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
	modelname = Info_ValueForKey (userinfo, "model");
	SetupGameGhoul2Model(client, modelname);

	if (ent->client->ghoul2)
	{
		ent->bolt_Head = trap_G2API_AddBolt(ent->client->ghoul2, 0, "cranium");
		ent->bolt_Waist = trap_G2API_AddBolt(ent->client->ghoul2, 0, "thoracic");
		ent->bolt_LArm = trap_G2API_AddBolt(ent->client->ghoul2, 0, "lradius");
		ent->bolt_RArm = trap_G2API_AddBolt(ent->client->ghoul2, 0, "rradius");
		ent->bolt_LLeg = trap_G2API_AddBolt(ent->client->ghoul2, 0, "ltibia");
		ent->bolt_RLeg = trap_G2API_AddBolt(ent->client->ghoul2, 0, "rtibia");
		ent->bolt_Motion = trap_G2API_AddBolt(ent->client->ghoul2, 0, "Motion");
	}

	// locate ent at a spawn point
	ClientSpawn( ent );

	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
		// send event
		tent = JKMod_G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN, clientNum ); // Tr!Force: [DImensions] Tag owner info
		tent->s.clientNum = ent->s.clientNum;

		// Tr!Force: [Welcome] Don't show on client begin
		/*if ( g_gametype.integer != GT_TOURNAMENT  ) {
			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLENTER")) );
		}*/
	}
	G_LogPrintf( "ClientBegin: %i\n", clientNum );

	// count current clients and rank for scoreboard
	CalculateRanks();

	G_ClearClientLog(clientNum);
}

static qboolean AllForceDisabled(int force)
{
	int i;

	if (force)
	{
		for (i=0;i<NUM_FORCE_POWERS;i++)
		{
			if (!(force & (1<<i)))
			{
				return qfalse;
			}
		}

		return qtrue;
	}

	return qfalse;
}

/*
===========
ClientSpawn

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState
============
*/
extern qboolean WP_HasForcePowers( const playerState_t *ps );
void ClientSpawn(gentity_t *ent) {
	int		index;
	vec3_t	spawn_origin, spawn_angles;
	gclient_t	*client;
	int		i;
	clientPersistant_t	saved;
	clientSession_t		savedSess;
	int		persistant[MAX_PERSISTANT];
	gentity_t	*spawnPoint;
	int		flags;
	int		savedPing;
//	char	*savedAreaBits;
	int		accuracy_hits, accuracy_shots;
	int		eventSequence;
//	char	userinfo[MAX_INFO_STRING];
	forcedata_t			savedForce;
	void		*ghoul2save;
	int		saveSaberNum = ENTITYNUM_NONE;
	int		wDisable = 0;
	int		jksave_player;		// Tr!Force: [General] Don't remove flags
	int		jksave_dimension;	// Tr!Force: [Dimensions] Don't remove flags
	int		jksave_tweaks;		// Tr!Force: [General] Don't remove flags

	index = ent - g_entities;
	client = ent->client;

	if ( ent->client->ps.saberInFlight && ent->client->ps.saberEntityNum >= MAX_CLIENTS && ent->client->ps.saberEntityNum < MAX_GENTITIES && !(ent->client->ps.stats[JK_TWEAKS] & JK_GHOST_SABERS)) // Tr!Force: [PlayerTweaks] Ghost Sabers
	{
		gentity_t *saberent = &g_entities[ent->client->ps.saberEntityNum];

		if ( saberent && saberent->inuse && saberent->r.ownerNum == ent-g_entities && saberent->touch == thrownSaberTouch )
		{
			saberent->touch = SaberGotHit;
			saberent->think = SaberUpdateSelf;
			/*if ( jk2gameplay == VERSION_1_04 )*/ saberent->bolt_Head = 0; // MVSDK: This shouldn't affect gameplay.
			saberent->nextthink = level.time;

			MakeDeadSaber(saberent);

			saberent->r.svFlags |= (SVF_NOCLIENT);
			saberent->r.contents = CONTENTS_LIGHTSABER;
			VectorSet( saberent->r.mins, -SABER_BOX_SIZE, -SABER_BOX_SIZE, -SABER_BOX_SIZE );
			VectorSet( saberent->r.maxs, SABER_BOX_SIZE, SABER_BOX_SIZE, SABER_BOX_SIZE );
			saberent->s.loopSound = 0;

			ent->client->ps.saberInFlight = qfalse;
			ent->client->ps.saberThrowDelay = level.time + 500;
			ent->client->ps.saberCanThrow = qfalse;
		}
	}

	if (client->ps.fd.forceDoInit)
	{ //force a reread of force powers
		WP_InitForcePowers( ent );
		client->ps.fd.forceDoInit = 0;
	}
	// find a spawn point
	// do it before setting health back up, so farthest
	// ranging doesn't count this client
	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
		spawnPoint = SelectSpectatorSpawnPoint ( 
						spawn_origin, spawn_angles);
	} else if (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) {
		// all base oriented team games use the CTF spawn points
		spawnPoint = SelectCTFSpawnPoint ( 
						client->sess.sessionTeam, 
						client->pers.teamState.state, 
						spawn_origin, spawn_angles);
	}
	else if (g_gametype.integer == GT_SAGA)
	{
		spawnPoint = SelectSagaSpawnPoint ( 
						client->sess.sessionTeam, 
						client->pers.teamState.state, 
						spawn_origin, spawn_angles);
	}
	else {
		do {
			// the first spawn should be at a good looking spot
			if ( !client->pers.initialSpawn && client->pers.localClient ) {
				client->pers.initialSpawn = qtrue;
				spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles );
			} else {
				// don't spawn near existing origin if possible
				spawnPoint = SelectSpawnPoint ( 
					client->ps.origin, 
					spawn_origin, spawn_angles);
			}

			// Tim needs to prevent bots from spawning at the initial point
			// on q3dm0...
			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}
			// just to be symetric, we have a nohumans option...
			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
				continue;	// try again
			}

			break;

		} while ( 1 );
	}
	client->pers.teamState.state = TEAM_ACTIVE;

	// toggle the teleport bit so the client knows to not lerp
	// and never clear the voted flag
	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT | EF_VOTED | EF_TEAMVOTED);
	flags ^= EF_TELEPORT_BIT;

	// clear everything but the persistant data

	saved = client->pers;
	savedSess = client->sess;
	savedPing = client->ps.ping;
//	savedAreaBits = client->areabits;
	accuracy_hits = client->accuracy_hits;
	accuracy_shots = client->accuracy_shots;
	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		persistant[i] = client->ps.persistant[i];
	}
	eventSequence = client->ps.eventSequence;

	savedForce = client->ps.fd;

	ghoul2save = client->ghoul2;

	saveSaberNum = client->ps.saberEntityNum;

	jksave_player = client->ps.stats[JK_PLAYER];		// Tr!Force: [General] Don't remove flags
	jksave_dimension = client->ps.stats[JK_DIMENSION];	// Tr!Force: [Dimensions] Don't remove flags
	jksave_tweaks = client->ps.stats[JK_TWEAKS];		// Tr!Force: [General] Don't remove flags

	memset (client, 0, sizeof(*client)); // bk FIXME: Com_Memset?

	client->ps.stats[JK_PLAYER] = jksave_player;		// Tr!Force: [General] Don't remove flags
	client->ps.stats[JK_DIMENSION] = jksave_dimension;	// Tr!Force: [Dimensions] Don't remove flags
	client->ps.stats[JK_TWEAKS] = jksave_tweaks;		// Tr!Force: [General] Don't remove flags

	//rww - Don't wipe the ghoul2 instance or the animation data
	client->ghoul2 = ghoul2save;

	//or the saber ent num
	client->ps.saberEntityNum = saveSaberNum;

	client->ps.fd = savedForce;

	client->ps.duelIndex = ENTITYNUM_NONE;

	client->pers = saved;
	client->sess = savedSess;
	client->ps.ping = savedPing;
//	client->areabits = savedAreaBits;
	client->accuracy_hits = accuracy_hits;
	client->accuracy_shots = accuracy_shots;
	client->lastkilled_client = -1;

	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
		client->ps.persistant[i] = persistant[i];
	}
	client->ps.eventSequence = eventSequence;
	// increment the spawncount so the client will detect the respawn
	client->ps.persistant[PERS_SPAWN_COUNT]++;
	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;

	client->airOutTime = level.time + 12000;

//	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );
	// set max health
	client->pers.maxHealth = 100;//atoi( Info_ValueForKey( userinfo, "handicap" ) );
	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
		client->pers.maxHealth = 100;
	}
	// clear entity values
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
	client->ps.eFlags = flags;

	ent->s.groundEntityNum = ENTITYNUM_NONE;
	ent->client = &level.clients[index];
	ent->takedamage = qtrue;
	ent->inuse = qtrue;
	ent->classname = "player";
	ent->r.contents = CONTENTS_BODY;
	ent->clipmask = MASK_PLAYERSOLID;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags = 0;
	
	VectorCopy (playerMins, ent->r.mins);
	VectorCopy (playerMaxs, ent->r.maxs);

	client->ps.clientNum = index;
	//give default weapons
	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_NONE );

	if (g_gametype.integer == GT_TOURNAMENT)
	{
		wDisable = g_duelWeaponDisable.integer;
	}
	else
	{
		wDisable = g_weaponDisable.integer;
	}

	if ( jk2gameplay == VERSION_1_02 )
	{
		if (g_gametype.integer == GT_HOLOCRON)
		{
			//always get free saber level 1 in holocron
			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
		}
		else
		{
			if (client->ps.fd.forcePowerLevel[FP_SABERATTACK])
			{
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
			}
			else
			{ //if you don't have saber attack rank then you don't get a saber
				client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
			}
		}
		
		if (!wDisable || !(wDisable & (1 << WP_BRYAR_PISTOL)))
		{
			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
		}
		else if (g_gametype.integer == GT_JEDIMASTER)
		{
			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
		}

		if (g_gametype.integer == GT_JEDIMASTER)
		{
			client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
			client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
		}

		if (client->ps.stats[STAT_WEAPONS] & (1 << WP_BRYAR_PISTOL))
		{
			client->ps.weapon = WP_BRYAR_PISTOL;
		}
		else if (client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER))
		{
			client->ps.weapon = WP_SABER;
		}
		else
		{
			client->ps.weapon = WP_STUN_BATON;
		}
	}
	else
	{
		if ( g_gametype.integer != GT_HOLOCRON 
			&& g_gametype.integer != GT_JEDIMASTER 
			&& !HasSetSaberOnly()
			&& !AllForceDisabled( g_forcePowerDisable.integer )
			&& g_trueJedi.integer )
		{
			if ( jk2gameplay == VERSION_1_04 && g_gametype.integer >= GT_TEAM && (client->sess.sessionTeam == TEAM_BLUE || client->sess.sessionTeam == TEAM_RED) )
			{//In Team games, force one side to be merc and other to be jedi
				if ( level.numPlayingClients > 0 )
				{//already someone in the game
					int		i;
					team_t	forceTeam = TEAM_SPECTATOR;
					for ( i = 0 ; i < level.maxclients ; i++ ) 
					{
						if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
							continue;
						}
						if ( level.clients[i].sess.sessionTeam == TEAM_BLUE || level.clients[i].sess.sessionTeam == TEAM_RED ) 
						{//in-game
							if ( WP_HasForcePowers( &level.clients[i].ps ) )
							{//this side is using force
								forceTeam = level.clients[i].sess.sessionTeam;
							}
							else
							{//other team is using force
								if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
								{
									forceTeam = TEAM_RED;
								}
								else
								{
									forceTeam = TEAM_BLUE;
								}
							}
							break;
						}
					}
					if ( WP_HasForcePowers( &client->ps ) && client->sess.sessionTeam != forceTeam )
					{//using force but not on right team, switch him over
						const char *teamName = TeamName( forceTeam );
						//client->sess.sessionTeam = forceTeam;
						SetTeam( ent, (char *)teamName );
						return;
					}
				}
			}

			if ( WP_HasForcePowers( &client->ps ) )
			{
				client->ps.trueNonJedi = qfalse;
				client->ps.trueJedi = qtrue;
				//make sure they only use the saber
				client->ps.weapon = WP_SABER;
				client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
			}
			else
			{//no force powers set
				client->ps.trueNonJedi = qtrue;
				client->ps.trueJedi = qfalse;
				if (!wDisable || !(wDisable & (1 << WP_BRYAR_PISTOL)))
				{
					client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
				}
				if ( jk2gameplay == VERSION_1_04 )
				{
					if (!wDisable || !(wDisable & (1 << WP_BLASTER)))
					{
						client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BLASTER );
					}
					if (!wDisable || !(wDisable & (1 << WP_BOWCASTER)))
					{
						client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BOWCASTER );
					}
				}
				client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
				client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
				client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max;
				client->ps.weapon = WP_BRYAR_PISTOL;
			}
		}
		else
		{//jediVmerc is incompatible with this gametype, turn it off!
			if ( jk2gameplay == VERSION_1_04 ) trap_Cvar_Set( "g_jediVmerc", "0" ); // MVSDK: I don't know what happens if you try jediVmerc with one of the incompatible gametypes, but maybe you end up with some special kind of jedi-master gametype... // FIXME: Check if jediVmerc with incompatible gametypes has some bad side-effects.
			if (g_gametype.integer == GT_HOLOCRON)
			{
				//always get free saber level 1 in holocron
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
			}
			else
			{
				if (client->ps.fd.forcePowerLevel[FP_SABERATTACK])
				{
					client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
				}
				else
				{ //if you don't have saber attack rank then you don't get a saber
					client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
				}
			}

			if (!wDisable || !(wDisable & (1 << WP_BRYAR_PISTOL)))
			{
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
			}
			else if (g_gametype.integer == GT_JEDIMASTER)
			{
				client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
			}

			if (g_gametype.integer == GT_JEDIMASTER)
			{
				client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
				client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
			}

			if (client->ps.stats[STAT_WEAPONS] & (1 << WP_BRYAR_PISTOL))
			{
				client->ps.weapon = WP_BRYAR_PISTOL;
			}
			else if (client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER))
			{
				client->ps.weapon = WP_SABER;
			}
			else
			{
				client->ps.weapon = WP_STUN_BATON;
			}
		}
	}

	/*
	client->ps.stats[STAT_HOLDABLE_ITEMS] |= ( 1 << HI_BINOCULARS );
	client->ps.stats[STAT_HOLDABLE_ITEM] = BG_GetItemIndexByTag(HI_BINOCULARS, IT_HOLDABLE);
	*/

	client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
	client->ps.stats[STAT_HOLDABLE_ITEM] = 0;

	if ( client->sess.sessionTeam == TEAM_SPECTATOR )
	{
		client->ps.stats[STAT_WEAPONS] = 0;
		client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	}

	client->ps.ammo[AMMO_BLASTER] = 100; //ammoData[AMMO_BLASTER].max; //100 seems fair.
//	client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max;
//	client->ps.ammo[AMMO_FORCE] = ammoData[AMMO_FORCE].max;
//	client->ps.ammo[AMMO_METAL_BOLTS] = ammoData[AMMO_METAL_BOLTS].max;
//	client->ps.ammo[AMMO_ROCKETS] = ammoData[AMMO_ROCKETS].max;
/*
	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_BRYAR_PISTOL);
	if ( g_gametype.integer == GT_TEAM ) {
		client->ps.ammo[WP_BRYAR_PISTOL] = 50;
	} else {
		client->ps.ammo[WP_BRYAR_PISTOL] = 100;
	}
*/
	client->ps.rocketLockIndex = MAX_CLIENTS;
	client->ps.rocketLockTime = 0;

	//rww - Set here to initialize the circling seeker drone to off.
	//A quick note about this so I don't forget how it works again:
	//ps.genericEnemyIndex is kept in sync between the server and client.
	//When it gets set then an entitystate value of the same name gets
	//set along with an entitystate flag in the shared bg code. Which
	//is why a value needs to be both on the player state and entity state.
	//(it doesn't seem to just carry over the entitystate value automatically
	//because entity state value is derived from player state data or some
	//such)
	client->ps.genericEnemyIndex = -1;

	client->ps.isJediMaster = qfalse;

	client->ps.fallingToDeath = 0;

	//Do per-spawn force power initialization
	WP_SpawnInitForcePowers( ent );

	// health will count down towards max_health
	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] * 1.25;

	// Start with a small amount of armor as well.
	client->ps.stats[STAT_ARMOR] = client->ps.stats[STAT_MAX_HEALTH] * 0.25;

	// Tr!Force: [GameGeneral] Custom spawn point
	if (ent->client->pers.jkmodPers.customSpawnCheck) 
	{
		spawn_origin[0] = ent->client->pers.jkmodPers.customSpawn[0];
		spawn_origin[1] = ent->client->pers.jkmodPers.customSpawn[1];
		spawn_origin[2] = ent->client->pers.jkmodPers.customSpawn[2];
		spawn_angles[0] = ent->client->pers.jkmodPers.customSpawn[3];
		spawn_angles[1] = ent->client->pers.jkmodPers.customSpawn[4];
	}

	G_SetOrigin( ent, spawn_origin );
	VectorCopy( spawn_origin, client->ps.origin );

	// the respawned flag will be cleared after the attack and jump keys come up
	client->ps.pm_flags |= PMF_RESPAWNED;

	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
	SetClientViewAngle( ent, spawn_angles );

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {

	} else {
		// Tr!Force: [Dimensions] Don't kill racers on spawn
		if (client->ps.stats[JK_DIMENSION] != DIMENSION_RACE)
		{
			// Tr!Force: [TeleFrag] Allow kill and unlink
			if (jkcvar_teleportFrag.integer) {
				G_KillBox(ent);
				trap_LinkEntity(ent);
			}
			else {
				JKMod_AntiStuckBox(ent);
			}
		}

		// force the base weapon up
		client->ps.weapon = WP_BRYAR_PISTOL;
		client->ps.weaponstate = FIRST_WEAPON;

	}

	// don't allow full run speed for a bit
	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	client->ps.pm_time = 100;

	client->respawnTime = level.time;
	client->inactivityTime = level.time + g_inactivity.integer * 1000;
	client->latched_buttons = 0;

	// set default animations
	client->ps.torsoAnim = WeaponReadyAnim[client->ps.weapon];
	client->ps.legsAnim = WeaponReadyAnim[client->ps.weapon];

	if ( level.intermissiontime ) {
		MoveClientToIntermission( ent );
	} else {
		// fire the targets of the spawn point
		G_UseTargets( spawnPoint, ent );

		// select the highest weapon number available, after any
		// spawn given items have fired
		client->ps.weapon = 1;
		for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
				client->ps.weapon = i;
				break;
			}
		}
	}

	// Tr!Force: [GameGeneral] Activate saber on spawn
	if (!(jkcvar_spawnActivateSaber.integer & (1 << g_gametype.integer)) && client->ps.weapon == WP_SABER) ent->client->ps.saberHolstered = qtrue;

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	client->ps.commandTime = level.time - 100;
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink( ent-g_entities );

	// positively link the client, even if the command times are weird
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
		trap_LinkEntity( ent );
	}

	if (g_spawnInvulnerability.integer && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_INSTA))) // Tr!Force: [Dimensions] Don't set invulnerable flag
	{
		ent->client->ps.eFlags |= EF_INVULNERABLE;
		ent->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
	}

	// Tr!Force: [Dimensions] Set dimension settings
	if (ent->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE && client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		JKMod_DimensionSettings(ent, ent->jkmodEnt.dimensionNumber);
	}

	// Tr!Force: [DualSaber] Check enable
	if (client->ps.weapon == WP_SABER) client->ps.dualBlade = client->pers.jkmodPers.dualSaber;

	// run the presend to set anything else
	if ( ent->client->sess.spectatorState != SPECTATOR_FOLLOW )
	{ // Only do this if we're not dealing with follow spectators to prevent two bugs:
	  // 1) follow spectators turning into free spectators at map_restart, because the client they were following has a higher client number and isn't ingame, yet
	  // 2) follow spectators corrupting their s.number in BG_PlayerStateToEntityState, cause they get the other client's playerState in ClientEndFrame
		ClientEndFrame( ent );
	}

	// clear entity state values
	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
}


/*
===========
ClientDisconnect

Called when a player drops from the server.
Will not be called between levels.

This should NOT be called directly by any game logic,
call trap_DropClient(), which will call this and do
server system housekeeping.
============
*/
void ClientDisconnect( int clientNum ) {
	gentity_t	*ent;
	gentity_t	*tent;
	int			i;

	// cleanup if we are kicking a bot that
	// hasn't spawned yet
	G_RemoveQueuedBotBegin( clientNum );

	ent = g_entities + clientNum;
	if ( !ent->client ) {
		return;
	}

	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		if (ent->client->ps.fd.forcePowersActive & (1 << i))
		{
			WP_ForcePowerStop(ent, i);
		}
		i++;
	}

	i = TRACK_CHANNEL_1;

	while (i < NUM_TRACK_CHANNELS)
	{
		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0)
		{
			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
		}
		i++;
	}
	i = 0;

	// stop any following clients
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
			&& level.clients[i].sess.spectatorClient == clientNum ) {
			StopFollowing( &g_entities[i] );
		}
	}

	// send effect if they were completely connected
	if ( ent->client->pers.connected == CON_CONNECTED 
		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		tent = JKMod_G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT, clientNum ); // Tr!Force: [Dimensions] Tag owner info
		tent->s.clientNum = ent->s.clientNum;

		// They don't get to take powerups with them!
		// Especially important for stuff like CTF flags
		TossClientItems( ent );
	}

	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );

	// if we are playing in tourney mode, give a win to the other player and clear his frags for this round
	if ( (g_gametype.integer == GT_TOURNAMENT )
		&& !level.intermissiontime
		&& !level.warmupTime ) {
		if ( level.sortedClients[1] == clientNum ) {
			level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] = 0;
			level.clients[ level.sortedClients[0] ].sess.wins++;
			ClientUserinfoChanged( level.sortedClients[0] );

			// Tr!Force: [PlayerStatus] Announce score
			if (jkcvar_chatAutoStatus.integer && level.clients[level.sortedClients[0]].sess.jkmodSess.playerStatus) JKMod_playerStatus(&g_entities[level.sortedClients[0]], qtrue);
		}
		else if ( level.sortedClients[0] == clientNum ) {
			level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] = 0;
			level.clients[ level.sortedClients[1] ].sess.wins++;
			ClientUserinfoChanged( level.sortedClients[1] );
		}
	}

	trap_UnlinkEntity (ent);
	ent->s.modelindex = 0;
	ent->inuse = qfalse;
	ent->classname = "disconnected";
	ent->client->pers.connected = CON_DISCONNECTED;
	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
	ent->client->sess.sessionTeam = TEAM_FREE;

	// Tr!Force: [Emotes] Chair emote disable
	if (ent->client->jkmodClient.chairModelUsed) JKMod_ChairModelDisable(ent);
	
	// Tr!Force: [General] Check for reconnect
	Q_strncpyz(level.jkmodLocals.reconnectedIP, ent->client->sess.jkmodSess.clientIP, sizeof(level.jkmodLocals.reconnectedIP));

	// Tr!Force: [General] Clear ignored client
	JKMod_IgnoreClientClear(clientNum);
	
	trap_SetConfigstring( CS_PLAYERS + clientNum, "");

	CalculateRanks();

	if ( ent->r.svFlags & SVF_BOT ) {
		BotAIShutdownClient( clientNum, qfalse );
	}

	G_ClearClientLog(clientNum);

	// Tr!Force: [Dimensions] Entity owner check
	for ( i = 0; i < level.num_entities; i++ ) {
		if ( g_entities[i].inuse && g_entities[i].jkmodEnt.dimensionOwner == clientNum ) {
			JKMod_DimensionOwnerCheck( ENTITYNUM_NONE, &g_entities[i] );
		}
	}
}

#define MAX_CLIENT_CENTERPRINT_LINELENGTH 50
#define MAX_CLIENT_CENTERPRINT_LENGTH 1024
void G_CenterPrint( int targetNum, int autoLineWraps, const char *message )
{
	if ( !autoLineWraps )
		trap_SendServerCommand( targetNum, va("cp \"%s\"", message) );
	else
	{
		char newMessage[MAX_CLIENT_CENTERPRINT_LENGTH];

		const char *lineStart = message;
		const char *lineEnd = lineStart;
		const char *wordStart, *wordEnd;
		const char *ptr;

		int lineLength;
		int wordLength;
		int curLength;
		int reset;
		int isMultiLang;

		*newMessage = 0;

		while ( *lineStart && (size_t)(lineStart-message) < strlen(message) )
		{
			if ( *newMessage ) Q_strcat( newMessage, sizeof(newMessage), "\n" );

			while ( *lineEnd && *lineEnd != '\n' ) lineEnd++;
			lineLength = lineEnd - lineStart;

			isMultiLang = 0;
			ptr = lineStart;
			while ( ptr < lineEnd-2 )
			{
				if ( *ptr == '@' && *(ptr+1) == '@' && *(ptr+2) == '@' )
				{
					isMultiLang = 1;
					break;
				}
				ptr++;
			}

			if ( lineLength > MAX_CLIENT_CENTERPRINT_LINELENGTH || isMultiLang )
			{ // Now we have to cut the line.
				wordStart = wordEnd = lineStart;
				curLength = 0;
				reset = 0;

				while ( lineStart < lineEnd )
				{
					isMultiLang = 0;
					if ( reset )
					{
						curLength = 0;
						Q_strcat( newMessage, sizeof(newMessage), "\n" );

						ptr = wordStart;
						while ( *ptr && (*ptr == ' ' || *ptr == '\t') ) ptr++;

						if ( !Q_IsColorString_1_02(ptr) ) // CenterPrint interprets colors like the 1.02 console
						{
							// Find the old color if we don't start with a new color on the next line
							ptr = wordStart - 1;
							while ( *ptr && ptr >= lineStart )
							{
								if ( Q_IsColorString_1_02(ptr) )
								{
									if ( *(ptr+1) == '7' ) break; // Don't redo white

									Q_strcat( newMessage, sizeof(newMessage), va("^%c", *(ptr+1)) );
									curLength += 2;
									reset = 2;
									break;
								}
								ptr--;
							}
						}
					}
					wordEnd = wordStart;
					while ( *wordEnd != ' ' && *wordEnd != '\t' && *wordEnd != '\n' && wordEnd < lineEnd ) wordEnd++;
					wordLength = wordEnd - wordStart;

					if ( wordEnd > lineEnd ) // Make sure the word is still in our line and doesn't exceed it
						break;

					if ( wordLength >= 3 && *wordStart == '@' && *(wordStart+1) == '@' && *(wordStart+2) == '@' )
						isMultiLang = 1;

					// & 1: move whole words to the next line (prefered if combined with 2)
					// & 2: cut words into pieces
					if ( curLength + wordLength > MAX_CLIENT_CENTERPRINT_LINELENGTH || isMultiLang )
					{ // The next word would make the line too long
						if ( (autoLineWraps & 2) && (!curLength || !(autoLineWraps & 1)) && !isMultiLang )
						{ // We want to cut the word into pieces
							if ( curLength && !reset ) Q_strcat( newMessage, sizeof(newMessage), " " );

							wordEnd = ptr = wordStart + (MAX_CLIENT_CENTERPRINT_LINELENGTH - curLength);
							ptr = wordEnd - 1;

							// Make sure we don't accidently split a colorcode
							while ( *ptr && ptr > wordStart && *ptr == '^' ) ptr--;
							if ( ptr == wordStart ) ptr = wordEnd - 1;

							wordEnd = ptr + 1;

							G_StringAppendSubstring( newMessage, sizeof(newMessage), wordStart, wordEnd-wordStart );
							wordStart = wordEnd;
						}
						else if ( !curLength || reset == 2 )
						{ // We don't to split it, so just append the whole thing and let the client cut it off
							G_StringAppendSubstring( newMessage, sizeof(newMessage), wordStart, wordLength );
							wordStart = wordEnd + 1;
						}

						// Reset the counters
						reset = 1;
					}
					else
					{ // Append the word
						// If this isn't the first word add a space
						if ( curLength && !reset ) Q_strcat( newMessage, sizeof(newMessage), " " );
						G_StringAppendSubstring( newMessage, sizeof(newMessage), wordStart, wordLength );
						curLength += wordLength + 1;
						wordStart = wordEnd + 1;

						reset = 0;
					}

					// Shouldn't get here
					if ( !*wordEnd ) break;
				}
			}
			else G_StringAppendSubstring( newMessage, sizeof(newMessage), lineStart, lineLength );

			// Next line
			lineStart = lineEnd + 1;
			lineEnd = lineStart;
		}
		trap_SendServerCommand( targetNum, va("cp \"%s\"", newMessage) );
	}
}


