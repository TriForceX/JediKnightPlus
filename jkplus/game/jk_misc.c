/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Misc functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

extern qboolean SaberAttacking(gentity_t *self);

/*
=====================================================================
Custom force power valid check
=====================================================================
*/
qboolean JKMod_ForcePowerValid(forcePowers_t power, playerState_t *ps)
{
	gentity_t	*ent = &g_entities[ps->clientNum];

	if (!ent || !ent->client || ent->s.number > 31)
	{
		JKMod_Printf("Duelforce: Ent bug! %i\n", ps->clientNum);
		return qfalse;
	}
	if (ent->client->pers.jkmodPers.CustomDuel == 0)
	{
		return qfalse;
	}
	return qtrue;
}

/*
=====================================================================
Player moving check function
=====================================================================
*/
qboolean JKMod_PlayerMoving(gentity_t *ent, int move, int attack)
{
	qboolean checkMove = qfalse;
	qboolean checkAttack = qfalse;

	checkMove = (ent->client->ps.velocity[0] != 0 || ent->client->ps.velocity[1] != 0 || ent->client->ps.velocity[2] != 0) || BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim);
	checkAttack = ent->client->ps.saberInFlight || 
				ent->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN || 
				ent->client->ps.weapon == WP_SABER && SaberAttacking(ent) || 
				ent->client->ps.saberMove != LS_READY && ent->client->ps.saberMove != LS_NONE ||
				(ent->client->ps.eFlags & EF_FIRING) || 
				(ent->client->ps.eFlags & EF_ALT_FIRING);

	if (move && attack) {
		if (checkMove || checkAttack) return qtrue;
		else return qfalse;
	}
	else if (move && !attack) {
		if (checkMove) return qtrue;
		else return qfalse;
	}
	else if (!move && attack) {
		if (checkAttack) return qtrue;
		else return qfalse;
	}
	else {
		return qfalse;
	}
}

/*
=====================================================================
Player movements allowed
=====================================================================
*/
void JKMod_PlayerMovementCheck(gentity_t *ent)
{
	// JKA Jetpack Style
	if (jkcvar_playerMovement.integer & JK_JETPACK_JKA) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_JETPACK_JKA;
	else
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_JETPACK_JKA;
	
	// Weapon Stand Fix
	if (jkcvar_playerMovement.integer & JK_WEAPON_STAND) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_WEAPON_STAND;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_WEAPON_STAND;

	// Disruptor Zoom Always (Requires clientside)
	if ((jkcvar_playerMovement.integer & JK_DISRUPTOR_WALK) && ent->client->pers.jkmodPers.ClientPlugin) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_DISRUPTOR_WALK;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_DISRUPTOR_WALK;

	// Button Use Stand
	if (jkcvar_playerMovement.integer & JK_USE_STAND) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_USE_STAND;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_USE_STAND;

	JKMod_Printf(S_COLOR_YELLOW "Client %i movement checked\n", ent->client->ps.clientNum);
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
			tent = JKMod_G_TempEntity(player->client->ps.origin, EV_PLAYER_TELEPORT_OUT, player->s.number);
			tent->s.clientNum = player->s.clientNum;
			tent->s.otherEntityNum = player->s.clientNum;

			tent = JKMod_G_TempEntity(origin, EV_PLAYER_TELEPORT_IN, player->s.number);
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

				JKMod_G_PlayEffect_ID(G_EffectIndex(efxfile), player->client->ps.origin, temporigin, player->s.number);
				JKMod_G_SoundAtLoc(player->client->ps.origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/teleout")), player->s.number);

				JKMod_G_PlayEffect_ID(G_EffectIndex(efxfile), origin, temporigin, player->s.number);
				JKMod_G_SoundAtLoc(origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/telein")), player->s.number);
			}
		}
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	if (jkcvar_teleportFrag.integer) trap_UnlinkEntity(player); // Tr!Force: [TeleFrag] Allow kill and unlink

	VectorCopy(origin, player->client->ps.origin);
	player->client->ps.origin[2] += 2;

	if (spitplayer)
	{
		// spit the player out
		AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
		VectorScale(player->client->ps.velocity, spitspeed, player->client->ps.velocity);
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	}
	else
	{
		// clear player velocity
		VectorClear(player->client->ps.velocity);
	}

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// set angles
	SetClientViewAngle(player, angles);

	// kill anything at the destination
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		// Tr!Force: [TeleFrag] Allow kill and unlink
		if (jkcvar_teleportFrag.integer)
			G_KillBox(player);
		else
			JKMod_AntiStuckBox(player);
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
Custom & default game settings function
=====================================================================
*/
extern qboolean WP_HasForcePowers( const playerState_t *ps );

// Custom settings
void JKMod_CustomGameSettings(gentity_t *ent, int weapons, int forcepowers, int forcelevel, qboolean holdables, qboolean jetpack, qboolean invulnerability, qboolean passthrough, float speed, float gravity)
{
	int customSettings = ent->client->pers.jkmodPers.customSettings;
	int customSettingsCount = ent->client->pers.jkmodPers.customSettingsCount;

	// Reset?
	if (weapons == DEFAULT && forcepowers == DEFAULT && speed == DEFAULT && gravity == DEFAULT) { 
		ent->client->pers.jkmodPers.customSettings = 0;
		ent->client->pers.jkmodPers.customSettingsCount = 0;
	}

	// Default check
	if (weapons == DEFAULT)		weapons		= g_weaponDisable.integer;
	if (forcepowers == DEFAULT)	forcepowers	= g_forcePowerDisable.integer;
	if (speed == DEFAULT)		speed		= g_speed.value;
	if (gravity == DEFAULT)		gravity		= g_gravity.value;

	if (ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		// Force
		if (forcepowers != ent->client->ps.fd.forcePowersKnown && forcepowers != g_forcePowerDisable.integer)
		{
			int i;
			int customLevel, customCheck = -1;

			if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_SABER) {
				customCheck = 15; // Saber attack
				customLevel = FORCE_LEVEL_3;
			}

			i = 0;
			while (i < NUM_FORCE_POWERS)
			{
				if (forcepowers & (1 << i))
				{
					ent->client->ps.fd.forcePowerLevel[i] = 0;
					ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
				}
				else 
				{
					ent->client->ps.fd.forcePowersKnown |= (1 << i);
					if (forcelevel != DEFAULT) ent->client->ps.fd.forcePowerLevel[i] = customCheck == i ? customLevel : forcelevel;
				}
				i++;
			}
		}
		else
		{
			WP_InitForcePowers(ent);
		}

		// Weapons
		if (weapons != ent->client->ps.stats[STAT_WEAPONS] && weapons != g_weaponDisable.integer)
		{
			int i;
			int firstWeapon = 0;

			i = 0;
			while (i < WP_NUM_WEAPONS)
			{
				if (!(weapons & (1 << i)) && i != WP_NONE && i != WP_EMPLACED_GUN && i != WP_TURRET)
				{
					ent->client->ps.stats[STAT_WEAPONS] |= (1 << i);
					if (!firstWeapon) firstWeapon = i;
				}
				else 
				{
					ent->client->ps.stats[STAT_WEAPONS] &= ~(1 << i);
				}
				i++;
			}

			for (i = 0; i < AMMO_MAX; i++) ent->client->ps.ammo[i] = ammoData[i].max;

			ent->client->ps.weapon = firstWeapon;
		}
		else
		{
			ent->client->ps.stats[STAT_WEAPONS] = 0;

			if (WP_HasForcePowers(&ent->client->ps))
			{
				ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
				ent->client->ps.weapon = WP_SABER;
			}
			else if (!g_weaponDisable.integer || !(g_weaponDisable.integer & (1 << WP_BRYAR_PISTOL)))
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
		}

		// Holdables
		if (holdables)
		{
			int i;

			i = 0;
			while (i < HI_NUM_HOLDABLE)
			{
				if (i != HI_DATAPAD) ent->client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << i);
				i++;
			}
		}
		else
		{
			ent->client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
		}
	}

	// Other stuff
	if (customSettings != customSettingsCount)
	{
		int i;
		int removeSize;
		char *removeName[] = {
			// Projectiles
			"bryar_proj",
			"generic_proj",
			"blaster_proj",
			"emplaced_gun_proj",
			"bowcaster_proj",
			"bowcaster_alt_proj",
			"repeater_proj",
			"repeater_alt_proj",
			"demp2_proj",
			"demp2_alt_proj",
			"flech_proj",
			"flech_alt",
			"rocket_proj",
			"thermal_detonator",
			"detpack",
			"laserTrap",
			// Misc
			"sentryGun",
			"g2animent",
			// Items
			"item_shield",
			"item_ysalimari",
			"item_force_boon",
			"item_force_enlighten_dark",
			"item_force_enlighten_light",
			// Dropped
			"weapon_blaster",
			"weapon_disruptor",
			"weapon_bowcaster",
			"weapon_repeater",
			"weapon_demp2",
			"weapon_flechette",
			"weapon_rocket_launcher",
			"weapon_thermal",
			"weapon_trip_mine",
			"weapon_det_pack",
		};

		removeSize = sizeof(removeName) / sizeof(removeName[0]);

		i = 0;
		while (i < removeSize)
		{
			JKMod_RemoveByClass(ent, removeName[i]);
			i++;
		}

		if (ent->client->ps.eFlags & EF_SEEKERDRONE) 
		{
			ent->client->ps.eFlags -= EF_SEEKERDRONE;
			ent->client->ps.genericEnemyIndex = -1;
		}
	}
		
	// Jetpack
	if (jetpack) 
	{
		if (jkcvar_jetPack.integer) {
			ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
			if (!ent->client->ps.stats[JK_FUEL]) ent->client->ps.stats[JK_FUEL] = 100;
		}
	}
	else 
	{
		if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;
	}

	// Pass-through
	if (passthrough)
	{
		if (!(ent->client->ps.eFlags & JK_PASS_THROUGH)) ent->client->ps.eFlags |= JK_PASS_THROUGH;
		if (!ent->client->pers.jkmodPers.passThroughPerm) ent->client->pers.jkmodPers.passThroughPerm = qtrue;
	}
	else
	{
		if (ent->client->ps.eFlags & JK_PASS_THROUGH) ent->client->ps.eFlags &= ~JK_PASS_THROUGH;
		if (ent->client->pers.jkmodPers.passThroughPerm) ent->client->pers.jkmodPers.passThroughPerm = qfalse;
	}

	// Invulnerability
	if (invulnerability) {
		ent->client->pers.jkmodPers.invulnerability = qtrue;
	}
	else {
		ent->client->pers.jkmodPers.invulnerability = qfalse;
	}

	// Speed
	ent->client->pers.jkmodPers.customSpeed = speed;

	// Gravity
	ent->client->pers.jkmodPers.customGravity = gravity;

	// Cheats
	ent->client->ps.dualBlade = qfalse;
	ent->client->noclip = qfalse;
	ent->flags &= ~FL_GODMODE;

	// Timers
	ent->client->ps.duelTime = 0;
	ent->client->pers.jkmodPers.raceStartTime = 0;

	// Modification count
	if (customSettings != customSettingsCount) ent->client->pers.jkmodPers.customSettings = ent->client->pers.jkmodPers.customSettingsCount;

	JKMod_Printf(S_COLOR_YELLOW "Custom settings for client %i\n", ent->client->ps.clientNum);
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
	int currentAmmo;
	qboolean typeAmmo;
	qboolean typeHealth;
	qboolean typeShield;

	typeAmmo = (self->classname == "jkmod_ammo_power_converter");
	typeHealth = (self->classname == "jkmod_health_power_converter");
	typeShield = (self->classname == "jkmod_shield_power_converter");

	give = typeHealth ? 5 : 2;

	if (!activator || !activator->client)
	{
		return;
	}

	// Set ammo
	currentAmmo = weaponData[activator->client->ps.weapon].ammoIndex;

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
	gentity_t *effect = JKMod_G_Spawn(ent->s.number);
	int smoke;

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
	G_SpawnInt( "smoke", "0", &smoke );

	if (smoke)
	{
		JKMod_AddSpawnField("fxFile", "smoke");

		effect->s.origin[2] = (int)ent->s.origin[2] + 45;
		effect->s.origin[1] = (int)ent->s.origin[1];
		effect->s.origin[0] = (int)ent->s.origin[0];
		SP_fx_runner(effect);

		VectorCopy(ent->s.angles, angles);
		G_SetAngles(effect, angles);
	}
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
