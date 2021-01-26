/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Misc functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Play effect by owner number function
=====================================================================
*/
gentity_t *JKMod_PlayEffect(int fxID, vec3_t org, vec3_t ang, int ownerNum)
{
	gentity_t	*te;

	te = G_TempEntity(org, EV_PLAY_EFFECT);
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;
	te->s.otherEntityNum = ownerNum;

	return te;
}

/*
=====================================================================
Play effect by ID function
=====================================================================
*/
gentity_t *JKMod_PlayEffect_ID(int fxID, vec3_t org, vec3_t ang, int ownerNum)
{
	gentity_t	*te;

	te = G_TempEntity(org, EV_PLAY_EFFECT_ID);
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;
	te->s.otherEntityNum = ownerNum;

	return te;
}

/*
=====================================================================
Predef sound by owner number function
=====================================================================
*/
gentity_t *JKMod_PreDefSound(vec3_t org, int pdSound, int ownerNum)
{
	gentity_t	*te;

	te = G_TempEntity(org, EV_PREDEFSOUND);
	te->s.eventParm = pdSound;
	te->s.otherEntityNum = ownerNum;
	VectorCopy(org, te->s.origin);

	return te;
}

/*
=====================================================================
Sound at loc by owner number function
=====================================================================
*/
void JKMod_SoundAtLoc(vec3_t loc, int channel, int soundIndex, int ownerNum)
{
	gentity_t	*te;

	te = G_TempEntity(loc, EV_GENERAL_SOUND);
	te->s.eventParm = soundIndex;
	te->s.otherEntityNum = ownerNum;
}

/*
=====================================================================
Check other clients in box
=====================================================================
*/
qboolean JKMod_OthersInBox(gentity_t *ent) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*other;
	vec3_t		mins, maxs;

	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	num = trap_EntitiesInBox(mins, maxs, touch, MAX_GENTITIES);

	for (i = 0; i < num; i++)
	{
		other = &g_entities[touch[i]];
		if (other->client && 
			other->client->ps.clientNum != ent->client->ps.clientNum && 
			!(((other->client->ps.stats[JK_PLAYER] & JK_CHAT_IN) && jkcvar_chatProtect.integer == 3) || other->client->ps.stats[JK_DIMENSION] == JK_RACE_IN)) 
		{
			return qtrue;
		}
	}
	return qfalse;
}

/*
=====================================================================
Pass-through box function
=====================================================================
*/
void JKMod_PassBox(gentity_t *ent) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*other;
	vec3_t		mins, maxs;

	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	num = trap_EntitiesInBox(mins, maxs, touch, MAX_GENTITIES);

	for (i = 0; i < num; i++)
	{
		other = &g_entities[touch[i]];
		if (other->client && other->client->ps.clientNum != ent->client->ps.clientNum) 
		{
			if (!(ent->client->ps.eFlags & JK_PASS_THROUGH)) ent->client->ps.eFlags |= JK_PASS_THROUGH;
			if (!(other->client->ps.eFlags & JK_PASS_THROUGH)) other->client->ps.eFlags |= JK_PASS_THROUGH;
		}
	}
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
		if (found->parent == ent)
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
		}
	}
}

/*
=====================================================================
Teleport player function
=====================================================================
*/
void JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, int spitspeed, char *efxfile, char *efxsound)
{
	gentity_t	*tent;

	if (!spitspeed) spitspeed = 400;

	if (player->client->sess.spectatorState == SPECTATOR_FOLLOW)
	{ // Follow spectators don't need to teleport. And calling BG_PlayerStateToEntityState on them corrupts their s.number.
		return;
	}

	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		if (efxfile[0] == '\0')
		{
			tent = G_TempEntity(player->client->ps.origin, EV_PLAYER_TELEPORT_OUT);
			tent->s.clientNum = player->s.clientNum;
			tent->s.otherEntityNum = player->s.clientNum;

			tent = G_TempEntity(origin, EV_PLAYER_TELEPORT_IN);
			tent->s.clientNum = player->s.clientNum;
			tent->s.otherEntityNum = player->s.clientNum;
		}
		else
		{
			if (strcmp(efxfile, "none") != 0)
			{
				vec3_t temporigin;

				temporigin[YAW] = 0;
				temporigin[PITCH] = -90;
				temporigin[ROLL] = 0;

				JKMod_PlayEffect_ID(G_EffectIndex(efxfile), player->client->ps.origin, temporigin, player->s.clientNum);
				JKMod_SoundAtLoc(player->client->ps.origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/teleout")), player->s.number); // Tr!Force: [Dimensions] Tag owner info into state for dimensions

				JKMod_PlayEffect_ID(G_EffectIndex(efxfile), origin, temporigin, player->s.clientNum);
				JKMod_SoundAtLoc(origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/telein")), player->s.number); // Tr!Force: [Dimensions] Tag owner info into state for dimensions
			}
		}
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	if (jkcvar_teleportFrag.integer) trap_UnlinkEntity(player); // Tr!Force: [TeleFrag] Allow kill and unlink

	VectorCopy(origin, player->client->ps.origin);
	player->client->ps.origin[2] += 1;

	if (spitplayer) 
	{
		// spit the player out
		AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
		VectorScale(player->client->ps.velocity, spitspeed, player->client->ps.velocity);
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

		// toggle the teleport bit so the client knows to not lerp
		player->client->ps.eFlags ^= EF_TELEPORT_BIT;
	}

	// set angles
	SetClientViewAngle(player, angles);

	// kill anything at the destination
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		// Tr!Force: [TeleFrag] Allow kill and unlink
		if (jkcvar_teleportFrag.integer)
			G_KillBox(player);
		else
			JKMod_PassBox(player);
	}

	// save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

	// use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		if (jkcvar_teleportFrag.integer) trap_LinkEntity(player); // Tr!Force: [TeleFrag] Allow kill and unlink
	}
}

/*
=====================================================================
Misc map entities resources
=====================================================================
*/

#define JK_STATION_RECHARGE_TIME	3000

// Effects usage
extern void SP_fx_runner(gentity_t *ent);

// Check recharge
void JKMod_EnergyStationCheck(gentity_t *ent)
{
	if (ent->fly_sound_debounce_time < level.time) {
		ent->s.loopSound = 0;
	}

	if (ent->count < ent->boltpoint4) {
		ent->count++;
	}

	ent->nextthink = level.time + ent->bolt_Head;
}

// Energy station settings function
void JKMod_EnergyStationSettings(gentity_t *ent, int count, qboolean chargeRate)
{
	G_SpawnInt("count", "0", &ent->count);

	if (!ent->count) {
		ent->count = count;
	}

	if (chargeRate)  {
		G_SpawnInt("chargerate", "0", &ent->bolt_Head);

		if (!ent->bolt_Head) {
			ent->bolt_Head = JK_STATION_RECHARGE_TIME;
		}
	}
}

// Energy station use function
void JKMod_EnergyStationUse(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	int dif, add, give;
	int stop = 1;
	int currentAmmo = weaponData[activator->client->ps.weapon].ammoIndex;
	qboolean typeAmmo = (self->classname == "jkmod_ammo_power_converter");
	qboolean typeHealth = (self->classname == "jkmod_health_power_converter");
	qboolean typeShield = (self->classname == "jkmod_shield_power_converter");

	give = typeHealth ? 5 : 2;

	if (!activator || !activator->client)
	{
		return;
	}

	if (jkcvar_pauseGame.integer) // Tr!Force: [Pause] Don't allow
	{
		return;
	}

	if (self->setTime < level.time)
	{
		// Sound check
		if (!self->s.loopSound)
		{
			if (typeAmmo)
				self->s.loopSound = G_SoundIndex("sound/interface/ammocon_run.wav");
			else if (typeHealth)
				self->s.loopSound = G_SoundIndex("sound/interface/ammocon_run.wav");
			else if (typeShield)
				self->s.loopSound = G_SoundIndex("sound/interface/shieldcon_run.wav");
		}

		self->setTime = level.time + 100;

		// Energy check
		if (typeHealth)
			dif = activator->client->ps.stats[STAT_MAX_HEALTH] - activator->health;
		else if (typeShield)
			dif = activator->client->ps.stats[STAT_MAX_HEALTH] - activator->client->ps.stats[STAT_ARMOR];

		if ((typeAmmo && self->count) || dif > 0)
		{
			if (typeAmmo) 
			{
				int i = AMMO_BLASTER;
				while (i < AMMO_MAX)
				{
					add = ammoData[i].max*0.1;

					if (add < 1) {
						add = 1;
					}
					if (activator->client->ps.ammo[i] < ammoData[i].max)
					{
						activator->client->ps.ammo[i] += add;
						if (activator->client->ps.ammo[i] > ammoData[i].max)
						{
							activator->client->ps.ammo[i] = ammoData[i].max;
						}
					}
					i++;
				}
			}
			else 
			{
				if (dif > give) {
					add = give;
				}
				else {
					add = dif;
				}
				if (self->count < add) {
					add = self->count;
				}
			}

			self->count -= add;

			if (self->count <= 0 && typeShield) {
				self->setTime = 0;
			}

			stop = 0;

			self->fly_sound_debounce_time = level.time + 50;

			if (typeHealth)
				activator->health += add;
			else if (typeShield)
				activator->client->ps.stats[STAT_ARMOR] += add;
		}

		// Stop check
		if ((typeHealth || typeAmmo) && stop)
		{
			self->s.loopSound = 0;
		}
		else if (typeShield && (stop || self->count <= 0))
		{
			if (self->s.loopSound && self->setTime < level.time) {
				G_Sound(self, CHAN_AUTO, G_SoundIndex("sound/interface/shieldcon_done.mp3"));
			}

			self->s.loopSound = 0;

			if (self->setTime < level.time) {
				self->setTime = level.time + self->bolt_Head + 100;
			}
		}

		// Show data
		if (typeAmmo) {
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Ammo\n^3%i\n\"", activator->client->ps.ammo[currentAmmo]));
		}
		else if (typeHealth) {
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Health\n^1%i\n\"", activator->client->ps.stats[STAT_HEALTH]));
		}
		else if (typeShield) {
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Shield\n^2%i\n\"", activator->client->ps.stats[STAT_ARMOR]));
		}
	}
}

/*
=====================================================================
Misc map entities functions
=====================================================================
*/

// Shield power converter function
void JKMod_SP_ShieldPowerConverter(gentity_t *ent)
{
	if (!ent->health)
	{
		ent->health = 999;
	}

	VectorSet(ent->r.mins, -16, -16, -40);
	VectorSet(ent->r.maxs, 16, 16, 40);

	ent->s.modelindex = G_ModelIndex("models/items/a_shield_converter.md3");

	ent->s.eFlags = 0;
	ent->r.svFlags |= SVF_PLAYER_USABLE;
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->classname = "jkmod_shield_power_converter";

	JKMod_EnergyStationSettings(ent, 50, qtrue);

	ent->boltpoint4 = ent->count; //initial value
	ent->think = JKMod_EnergyStationCheck;
	ent->nextthink = level.time + JK_STATION_RECHARGE_TIME;

	ent->use = JKMod_EnergyStationUse;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);
	trap_LinkEntity(ent);

	G_SoundIndex("sound/movers/objects/useshieldstation.wav");

	ent->s.modelindex2 = G_ModelIndex("/models/items/psd_big.md3");	// Precache model
}

// Health power converter function
void JKMod_SP_HealthPowerConverter(gentity_t *ent)
{
	vec3_t angles;
	gentity_t *effect = G_Spawn();

	if (!ent->health)
	{
		ent->health = 999;
	}

	VectorSet(ent->r.mins, -8, -8, -50);
	VectorSet(ent->r.maxs, 8, 8, 50);

	ent->s.modelindex = G_ModelIndex("models/map_objects/imp_mine/tank.md3");

	ent->s.eFlags = 0;
	ent->r.svFlags |= SVF_PLAYER_USABLE;
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->classname = "jkmod_health_power_converter";
	ent->use = JKMod_EnergyStationUse;

	JKMod_EnergyStationSettings(ent, 100, qfalse);

	ent->boltpoint4 = ent->count; //initial value
	ent->think = JKMod_EnergyStationCheck;
	ent->nextthink = level.time + JK_STATION_RECHARGE_TIME;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);
	trap_LinkEntity(ent);

	G_SoundIndex("sound/movers/objects/useshieldstation.wav");

	JKMod_AddSpawnField("fxFile", "smoke");

	effect->s.origin[2] = (int)ent->s.origin[2] + 45;
	effect->s.origin[1] = (int)ent->s.origin[1];
	effect->s.origin[0] = (int)ent->s.origin[0];
	SP_fx_runner(effect);

	VectorCopy(ent->s.angles, angles);
	G_SetAngles(effect, angles);
}

// Ammo power converter function
void JKMod_SP_AmmoPowerConverter(gentity_t *ent)
{
	if (!ent->health)
	{
		ent->health = 999;
	}

	VectorSet(ent->r.mins, -16, -16, -40);
	VectorSet(ent->r.maxs, 16, 16, 40);

	ent->s.modelindex = G_ModelIndex("models/items/a_pwr_converter.md3");

	ent->s.eFlags = 0;
	ent->r.svFlags |= SVF_PLAYER_USABLE;
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->classname = "jkmod_ammo_power_converter";
	ent->use = JKMod_EnergyStationUse;

	JKMod_EnergyStationSettings(ent, 100, qfalse);

	ent->boltpoint4 = ent->count; //initial value
	ent->think = JKMod_EnergyStationCheck;
	ent->nextthink = level.time + JK_STATION_RECHARGE_TIME;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);
	trap_LinkEntity(ent);

	G_SoundIndex("sound/movers/objects/useshieldstation.wav");
}
