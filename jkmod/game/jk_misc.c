/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Misc functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// External stuff
extern qboolean SaberAttacking(gentity_t *self);
extern qboolean WP_HasForcePowers( const playerState_t *ps );

/*
=====================================================================
Get current server map
=====================================================================
*/
const char *JKMod_GetCurrentMap(void)
{
	vmCvar_t	mapname;
	static char	*currentmap;

	trap_Cvar_Register(&mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM);
	currentmap = mapname.string;

	return currentmap;
}

/*
=====================================================================
Get first music from folder
=====================================================================
*/
const char *JKMod_GetMapMusic(void)
{
	int			numFiles;
	int			filelen;
	char		filelist[4096];
	static char	filename[MAX_QPATH];
	char		*fileptr;
	// int		i;
	const char	*currentmap = JKMod_GetCurrentMap();
	static char	*defaultmusic = jkcvar_mapDefaultMusic.string;

	numFiles = trap_FS_GetFileList(va("music/%s", currentmap), ".mp3", filelist, sizeof(filelist));
	fileptr = filelist;

	// for (i = 0; i < numFiles; i++, fileptr += filelen+1)
	// {
		filelen = strlen(fileptr);
		Q_strncpyz(filename, va("music/%s/", currentmap), sizeof(filename));
		Q_strcat(filename, sizeof(filename), fileptr);
	// }
		
	return VALIDSTRING(fileptr) ? filename : defaultmusic;
}

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
	if (ent->client->pers.jkmodPers.customDuel == 0)
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
	if ((jkcvar_playerMovement.integer & JK_DISRUPTOR_WALK) && ent->client->pers.jkmodPers.clientPlugin) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_DISRUPTOR_WALK;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_DISRUPTOR_WALK;

	// Button Use Stand
	if (jkcvar_playerMovement.integer & JK_USE_STAND) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_USE_STAND;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_USE_STAND;

	// Spectator no-clip (Requires clientside)
	if ((jkcvar_playerMovement.integer & JK_SPECTATOR_NOCLIP) && ent->client->pers.jkmodPers.clientPlugin) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_SPECTATOR_NOCLIP;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_SPECTATOR_NOCLIP;

	// Resist Push/Pull animation
	if (jkcvar_playerMovement.integer & JK_RESIST_PUSH) 
		ent->client->ps.stats[JK_MOVEMENT] |= JK_RESIST_PUSH;
	else 
		ent->client->ps.stats[JK_MOVEMENT] &= ~JK_RESIST_PUSH;

	JKMod_Printf(S_COLOR_YELLOW "Client %i movement checked\n", ent->client->ps.clientNum);
}

/*
=====================================================================
Gameplay changer
=====================================================================
*/
void JKMod_SetGamePlay(char *option)
{
	int version;

	if (!Q_stricmp(option, "2") || !Q_stricmp(option, "1.02")) version = VERSION_1_02;
	else if (!Q_stricmp(option, "3") || !Q_stricmp(option, "1.03")) version = VERSION_1_03;
	else if (!Q_stricmp(option, "4") || !Q_stricmp(option, "1.04")) version = VERSION_1_04;
	else if (!Q_stricmp(option, "0")) version = jk2startversion;
	else version = jk2gameplay;

	if (version != jk2gameplay) 
	{
		MV_SetGamePlay(version);
		trap_SendServerCommand(-1, va("print \"Server gameplay changed to 1.0%i\n\"", version));
		trap_SendServerCommand(-1, va("cp \"Gameplay changed to 1.0%i\n\"", version));
	}

	trap_Cvar_Set("jk_gamePlay", va("%i", version == jk2startversion ? 0 : version));
}

/*
=====================================================================
Teleport player function
=====================================================================
*/
void JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, int spitspeed, char *efxfile, char *efxsound)
{
	gentity_t	*tent;

	// Default spit speed
	if (!spitspeed) spitspeed = 400;

	// Follow spectators don't need to teleport. And calling BG_PlayerStateToEntityState on them corrupts their s.number.
	if (player->client->sess.spectatorState == SPECTATOR_FOLLOW) return;

	// Use temp events at source and destination to prevent the effect from getting dropped by a second player event
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) 
	{
		vec3_t temporigin;

		temporigin[YAW] = 0;
		temporigin[PITCH] = -90;
		temporigin[ROLL] = 0;

		if (efxfile != NULL) 
		{
			JKMod_G_PlayEffect_ID(G_EffectIndex((!strcmp(efxfile, "default") ? "mp/spawn" : efxfile)), player->client->ps.origin, temporigin, player->s.number);
			JKMod_G_PlayEffect_ID(G_EffectIndex((!strcmp(efxfile, "default") ? "mp/spawn" : efxfile)), origin, temporigin, player->s.number);
		}
		
		if (efxsound != NULL) 
		{
			JKMod_G_SoundAtLoc(player->client->ps.origin, CHAN_VOICE, G_SoundIndex((!strcmp(efxsound, "default") ? "sound/player/teleout" : efxsound)), player->s.number);
			JKMod_G_SoundAtLoc(origin, CHAN_VOICE, G_SoundIndex((!strcmp(efxsound, "default") ? "sound/player/telein" : efxsound)), player->s.number);
		}
	}

	// Unlink to make sure it can't possibly interfere with G_KillBox
	if (jkcvar_teleportFrag.integer) trap_UnlinkEntity(player);

	VectorCopy(origin, player->client->ps.origin);
	player->client->ps.origin[2] += 2;

	if (spitplayer)
	{
		// Spit the player out
		AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
		VectorScale(player->client->ps.velocity, spitspeed, player->client->ps.velocity);
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	}
	else
	{
		// Clear player velocity
		VectorClear(player->client->ps.velocity);
	}

	// Toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// Set angles
	SetClientViewAngle(player, angles);

	// Kill anything at the destination
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) 
	{
		if (jkcvar_teleportFrag.integer) {
			G_KillBox(player);
		} else {
			JKMod_AntiStuckBox(player);
		}
	}

	// Save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

	// Use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	// Link entity
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR && jkcvar_teleportFrag.integer) trap_LinkEntity(player);
}

/*
=====================================================================
Respawn time calculation for item throwing
=====================================================================
*/
#define	ITEM_RESPAWN_ARMOR			20
#define	ITEM_RESPAWN_HEALTH			30
#define	ITEM_RESPAWN_AMMO			40
#define	ITEM_RESPAWN_HOLDABLE		60

int JKMod_ItemRespawnTime(gentity_t *ent)
{
	float respawnTime;

	// Respawn time based on giType
	if (ent->item->giType == IT_ARMOR)
	{
		respawnTime = ITEM_RESPAWN_ARMOR;
	}
	else if (ent->item->giType == IT_HOLDABLE)
	{
		respawnTime = ITEM_RESPAWN_HOLDABLE;
	}
	else if (ent->item->giType == IT_HEALTH)
	{
		respawnTime = ITEM_RESPAWN_HEALTH;
	}
	else if (ent->item->giType == IT_AMMO)
	{
		respawnTime = ITEM_RESPAWN_AMMO;
	}

	// Adaption disabled so no wait time/random set
	if (!g_adaptRespawn.integer && !ent->wait && !ent->random)
	{
		return((int)respawnTime);
	}
	else
	{
		// Scale respawn time based on the current playing clients
		if (level.numPlayingClients > 4)
		{
			if (level.numPlayingClients > 32)
			{
				respawnTime *= 0.25;
			}
			else if (level.numPlayingClients > 12)
			{
				respawnTime *= 20.0 / (float)(level.numPlayingClients + 8);
			}
			else
			{
				respawnTime *= 8.0 / (float)(level.numPlayingClients + 4);
			}
		}

		// Check wait
		if (ent->wait)
			respawnTime = ent->wait;

		// Check random
		if (ent->random)
			respawnTime += crandom() * ent->random;

		// Don't allow below 1
		if (respawnTime < 1.0)
			respawnTime = 1.0;

		return ((int)respawnTime);
	}
}

/*
=====================================================================
Custom & default game settings function
=====================================================================
*/
void JKMod_CustomGameSettings(gentity_t *ent, int weapons, int forcepowers, int forcelevel, qboolean holdables, qboolean jetpack, qboolean invulnerability, qboolean passthrough, float speed, float gravity)
{
	int customSettings = ent->client->pers.jkmodPers.customSettings;
	int customSettingsCount = ent->client->pers.jkmodPers.customSettingsCount;

	// Reset?
	if (weapons == DEFAULT && forcepowers == DEFAULT && speed == DEFAULT && gravity == DEFAULT) 
	{ 
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
			int checkLevel = forcelevel;
			int checkDimension = ent->client->ps.stats[JK_DIMENSION];
			
			i = 0;
			while (i < NUM_FORCE_POWERS)
			{
				if (ent->client->ps.fd.forcePowersActive & (1 << i)) WP_ForcePowerStop(ent, i);

				if (forcepowers & (1 << i))
				{
					ent->client->ps.fd.forcePowerLevel[i] = 0;
					ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
				}
				else 
				{
					ent->client->ps.fd.forcePowersKnown |= (1 << i);
					if (checkDimension == DIMENSION_SABER && (i == FP_SABERATTACK || i == FP_SABERDEFEND)) checkLevel = FORCE_LEVEL_3;
					if (checkDimension == DIMENSION_SABER && (i == FP_LEVITATION && jk2gameplay == VERSION_1_04)) checkLevel = FORCE_LEVEL_3;
					if (forcelevel != DEFAULT) ent->client->ps.fd.forcePowerLevel[i] = checkLevel;
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

			if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_INSTA) 
				for (i = 0; i < AMMO_MAX; i++) ent->client->ps.ammo[i] = INFINITE_AMMO;
			else
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
		gentity_t *found = NULL;
		int i = MAX_CLIENTS;
		int foundType = 0;
		int foundCheck = (1 << ET_GENERAL) | (1 << ET_ITEM) | (1 << ET_MISSILE) | (1 << ET_SPECIAL) | (1 << ET_GRAPPLE);

		while (i < MAX_GENTITIES)
		{
			found = &g_entities[i];
			foundType = (1 << found->s.eType);

			if ((foundType & foundCheck) && (found->parent == ent || (!Q_stricmp(found->classname, "g2animent") && found->s.owner == ent->s.number)) && Q_stricmp(found->classname, "lightsaber")) // Skip this
			{
				if (!Q_stricmp(found->classname, "sentryGun") && ent->client->ps.fd.sentryDeployed) {
					ent->client->ps.fd.sentryDeployed = qfalse;
				}
				if (!Q_stricmp(found->classname, "detpack") && ent->client->ps.hasDetPackPlanted) {
					ent->client->ps.hasDetPackPlanted = qfalse;
				}

				JKMod_Printf(S_COLOR_MAGENTA "Check remove %s from client %i\n", found->classname, ent->s.number);
				G_FreeEntity(found);
			}
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
			ent->client->ps.stats[JK_FUEL] = 100;
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
	ent->client->pers.jkmodPers.invulnerability = invulnerability;

	// Speed
	ent->client->pers.jkmodPers.customSpeed = speed;

	// Gravity
	ent->client->pers.jkmodPers.customGravity = gravity;

	// Dual saber
	ent->client->pers.jkmodPers.dualSaber = qfalse;

	// Cheats
	ent->client->ps.dualBlade = qfalse;
	ent->client->noclip = qfalse;
	ent->flags &= ~(FL_GODMODE | FL_NOTARGET);

	// Timers
	ent->client->ps.duelTime = 0;
	ent->client->pers.jkmodPers.raceStartTime = 0;

	// Modification count
	if (customSettings != customSettingsCount) ent->client->pers.jkmodPers.customSettings = ent->client->pers.jkmodPers.customSettingsCount;

	JKMod_Printf(S_COLOR_YELLOW "Client %i custom settings\n", ent->client->ps.clientNum);
}

/*
=====================================================================
Check if is single player map
=====================================================================
*/
int JKMod_SPMapCheck(const char *mapname)
{
	int i;
	char *maps[] = {
		"artus_topside",
		"cairn_assembly",
		"cairn_reactor",
		"doom_shields",
		"bespin_streets",
		"bespin_platform",
		"yavin_temple",
		"yavin_swamp",
		"yavin_trial",
		"yavin_canyon",
		"yavin_courtyard",
		"yavin_final",
		"valley",
		"pit",
	};
	char *special[] = {
		"kejim_base",
		"kejim_post",
		"artus_detention",
		"artus_mine",
		"bespin_undercity",
		"cairn_bay",
		"cairn_dock1",
		"doom_comm",
		"doom_detention",
		"ns_hideout",
		"ns_starpad",
		"ns_streets",
	};

	// Requires serverside or bsp modification
	for (i = 0; i < ARRAY_LEN(maps); i++)
	{
		if (!Q_stricmp(mapname, maps[i])) return 1;
	}

	// Requires engine and bsp modification
	for (i = 0; i < ARRAY_LEN(special); i++)
	{
		if (!Q_stricmp(mapname, special[i])) return 2;
	}
	
	return 0;
}

/*
=====================================================================
Check for valid player models
=====================================================================
*/
qboolean JKMod_ValidPlayerModel(const char* modelname)
{
	int i;
	char *models[] = {
		"bespin_cop",
		"chiss",
		"desann",
		"galak",
		"gran",
		"imperial",
		"imperial_worker",
		"jan",
		"jedi",
		"jeditrainer",
		"kyle",
		"lando",
		"luke",
		"monmothma",
		"morgan",
		"prisoner",
		"rebel",
		"reborn",
		"reelo",
		"rodian",
		"shadowtrooper",
		"stormpilot",
		"stormtrooper",
		"swamptrooper",
		"tavion",
		"trandoshan",
		"ugnaught",
		"weequay",
	};

	for (i = 0; i < ARRAY_LEN(models); i++)
	{
		if (!Q_stricmp(modelname, va("models/players/%s/model.glm", models[i]))) return qtrue;
	}
	
	return qfalse;
}

/*
=====================================================================
Misc map entities resources
=====================================================================
*/
#define STATION_RECHARGE_TIME			3000
#define STATION_SHIELD_TYPE				1
#define STATION_HEALTH_TYPE				2
#define STATION_AMMO_TYPE				3
#define DEBRIS_SPECIALCASE_ROCK			-1
#define DEBRIS_SPECIALCASE_CHUNKS		-2
#define DEBRIS_SPECIALCASE_WOOD			-3
#define DEBRIS_SPECIALCASE_GLASS		-4

extern int gExplSound;
extern void BreakableBrushDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);
extern void BreakableBrushPain(gentity_t *self, gentity_t *attacker, int damage);
extern void BreakableBrushUse(gentity_t *self, gentity_t *other, gentity_t *activator);
extern void SP_fx_runner(gentity_t *ent);

// Check recharge
void JKMod_EnergyStationCheck(gentity_t *ent)
{
	if (ent->fly_sound_debounce_time < level.time) ent->s.loopSound = 0;
	ent->nextthink = level.time + STATION_RECHARGE_TIME;
}

// Energy station use function
void JKMod_EnergyStationUse(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	int addAmount = 0, getAmount = 0, maxAmount = 0, currentAmmo = 0;
	qboolean typeAmmo;
	qboolean typeHealth;
	qboolean typeShield;

	if (!activator || !activator->client) return;
	if (activator->client->ps.duelInProgress) return;

	// Set type
	typeAmmo = self->watertype == STATION_AMMO_TYPE;
	typeHealth =  self->watertype == STATION_HEALTH_TYPE;
	typeShield = self->watertype == STATION_SHIELD_TYPE;

	// Check weapon
	if (typeAmmo && activator->client->ps.weapon < WP_BRYAR_PISTOL) 
	{
		trap_SendServerCommand(activator->client->ps.clientNum, "cp \"This weapon does not use ammo\"");
		return;
	}

	// Get data
	currentAmmo = weaponData[activator->client->ps.weapon].ammoIndex;
	addAmount = self->boltpoint4;
	maxAmount = typeAmmo ? ammoData[currentAmmo].max : self->count;

	// Set data
	if (typeAmmo)
		getAmount = activator->client->ps.ammo[currentAmmo];
	else if (typeHealth)
		getAmount = activator->health;
	else if (typeShield)
		getAmount = activator->client->ps.stats[STAT_ARMOR];
	

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

		self->setTime = level.time + self->bolt_Head;

		// Amount check
		if (getAmount < maxAmount)
		{
			if (typeAmmo)
				activator->client->ps.ammo[currentAmmo] += ((getAmount + addAmount) > maxAmount ? 1 : addAmount);
			else if (typeHealth)
				activator->health += ((getAmount + addAmount) > maxAmount ? 1 : addAmount);
			else if (typeShield)
				activator->client->ps.stats[STAT_ARMOR] += ((getAmount + addAmount) > maxAmount ? 1 : addAmount);

			self->fly_sound_debounce_time = level.time + 50;
		}
		else
		{
			self->setTime = 0;
			if (self->s.loopSound && self->setTime < level.time) G_Sound(self, CHAN_AUTO, G_SoundIndex("sound/interface/shieldcon_done.mp3"));

			self->s.loopSound = 0;
			if (self->setTime < level.time) self->setTime = level.time + STATION_RECHARGE_TIME + self->bolt_Head;
		}

		// Show data
		if (typeAmmo)
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Ammo: ^3%i\"", (getAmount > 999 ? 999 : getAmount)));
		else if (typeHealth)
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Health: ^1%i\"", getAmount));
		else if (typeShield)
			trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Shield: ^2%i\"", getAmount));
	}
}

/*
=====================================================================
Misc map entities functions
=====================================================================
*/

// Misc power converter
void JKMod_SP_MiscPowerConverter(gentity_t *ent)
{
	vec3_t		angles;
	gentity_t	*effect;
	char		*fxFile;
	char		*mins, *maxs;

	G_SpawnInt("type", "1", &ent->watertype);
	G_SpawnInt("chargemax", "0", &ent->count);
	G_SpawnInt("chargerate", "0", &ent->bolt_Head);
	G_SpawnInt("chargeamount", "0", &ent->boltpoint4);
	G_SpawnString("model", "", &ent->model);

	if (ent->watertype != STATION_SHIELD_TYPE && ent->watertype != STATION_HEALTH_TYPE && ent->watertype != STATION_AMMO_TYPE)
	{
		G_Printf(S_COLOR_RED "ERROR: jkmod_misc_power_converter doesn't have a valid type (%i)\n", ent->watertype);
		G_FreeEntity(ent);
		return;
	}

	if (!VALIDSTRING(ent->model))
	{
		if (ent->watertype == STATION_SHIELD_TYPE) ent->model = "models/items/a_shield_converter.md3";
		if (ent->watertype == STATION_HEALTH_TYPE) ent->model = "models/map_objects/imp_mine/tank.md3";
		if (ent->watertype == STATION_AMMO_TYPE) ent->model = "models/items/a_pwr_converter.md3";
	}

	ent->s.modelindex = G_ModelIndex(ent->model);
	ent->s.modelindex2 = G_ModelIndex(ent->model);

	mins = ent->watertype == STATION_HEALTH_TYPE ? "-8, -8, -50" : "-16 -16 -40";
	maxs = ent->watertype == STATION_HEALTH_TYPE ? "8, 8, 50" : "16 16 40";

	G_SpawnVector("mins", mins, ent->r.mins);
	G_SpawnVector("maxs", maxs, ent->r.maxs);

	if (!Q_stricmp(ent->model,".glm"))
	{	
		ent->s.g2radius = 100;
		ent->s.modelGhoul2 = 1;
	}

	if (!ent->health) ent->health = 999;
	if (!ent->count) ent->count = 100;
	if (!ent->bolt_Head) ent->bolt_Head = 100;
	if (!ent->boltpoint4) ent->boltpoint4 = 5;

	ent->s.eFlags = 0;
	ent->r.svFlags |= SVF_PLAYER_USABLE;
	ent->s.generic1 |= GENERIC_USABLE | GENERIC_CONVERTER; // Tr!Force: [IdentifyObjects] Check usable hint
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->use = JKMod_EnergyStationUse;
	ent->think = JKMod_EnergyStationCheck;
	ent->nextthink = level.time + STATION_RECHARGE_TIME;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);
	trap_LinkEntity(ent);
	
	// Precache
	G_SoundIndex("sound/interface/ammocon_run.wav");
	G_SoundIndex("sound/interface/shieldcon_run.wav");
	G_SoundIndex("sound/interface/shieldcon_done.mp3");

	if (G_SpawnString("fxFile", "", &fxFile))
	{
		if (VALIDSTRING(fxFile))
		{
			effect = JKMod_G_Spawn(ent->s.number);

			effect->s.origin[0] = (int)ent->s.origin[0];
			effect->s.origin[1] = (int)ent->s.origin[1];
			effect->s.origin[2] = (int)ent->s.origin[2] + ent->r.maxs[2];
			SP_fx_runner(effect);

			VectorCopy(ent->s.angles, angles);
			G_SetAngles(effect, angles);
		}
	}
}

// Misc model
void JKMod_SP_MiscModel(gentity_t *ent)
{
	G_SpawnString( "model", "", &ent->model );

	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = G_ModelIndex(ent->model);
	ent->s.modelindex2 = G_ModelIndex(ent->model);

	G_SpawnVector("mins", "-16 -16 -16", ent->r.mins);
	G_SpawnVector("maxs", "16 16 16", ent->r.maxs);

	if (!Q_stricmp(ent->model,".glm"))
	{	
		ent->s.g2radius = 100;
		ent->s.modelGhoul2 = 1;
	}

    ent->r.contents = CONTENTS_SOLID;
    ent->clipmask = MASK_SOLID;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);

	trap_LinkEntity( ent );
}

// Misc model breakable
void JKMod_SP_MiscModelBreakable(gentity_t *ent)
{
	char *model, *sound, *debrissound;
	
	ent->s.modelindex = G_ModelIndex(ent->model);
	ent->s.modelindex2 = G_ModelIndex(ent->model);

	if (ent->spawnflags & 1)
	{
		ent->r.contents = CONTENTS_SOLID|CONTENTS_OPAQUE|CONTENTS_BODY|CONTENTS_MONSTERCLIP|CONTENTS_BOTCLIP;
	}
	else if (ent->health)
	{
		ent->r.contents = CONTENTS_SHOTCLIP;
	}
		
	G_SpawnVector("mins", "-16 -16 -16", ent->r.mins);
	G_SpawnVector("maxs", "16 16 16", ent->r.maxs);

	if (!Q_stricmp(ent->model,".glm"))
	{	
		ent->s.g2radius = 100;
		ent->s.modelGhoul2 = 1;
	}

	G_SetOrigin(ent, ent->s.origin);
	G_SetAngles(ent, ent->s.angles);

	if (ent->health)
	{
		ent->takedamage = qtrue;
		ent->pain = BreakableBrushPain;
		ent->die = BreakableBrushDie;
	}

	if (ent->spawnflags & 128)
	{
		ent->r.svFlags |= SVF_PLAYER_USABLE;
		ent->s.generic1 |= GENERIC_USABLE; // Tr!Force: [IdentifyObjects] Check usable hint
	}
	
	ent->use = BreakableBrushUse;
	ent->s.teamowner = 0;

	G_SpawnString("debrismodel", "rock", &model);
	G_SpawnString("debrissound", "", &debrissound);
	G_SpawnString("breaksound", "sound/movers/objects/objectBreak.wav", &sound);

	ent->boltpoint3 = G_SoundIndex(sound);

	gExplSound = G_SoundIndex("sound/weapons/explosions/cargoexplode.wav");

	if (VALIDSTRING(debrissound))
	{
		ent->boltpoint1 = G_SoundIndex(debrissound);
	}
	else
	{
		ent->boltpoint1 = 0;
	}

	ent->boltpoint4 = 1;

	if (strcmp(model, "rock") == 0)
	{
		G_ModelIndex("models/chunks/rock/rock1_1.md3");
		G_ModelIndex("models/chunks/rock/rock1_2.md3");
		G_ModelIndex("models/chunks/rock/rock1_3.md3");
		G_ModelIndex("models/chunks/rock/rock1_4.md3");

		ent->boltpoint2 = DEBRIS_SPECIALCASE_ROCK;
	}
	else if (strcmp(model, "chunks") == 0)
	{
		G_ModelIndex("models/chunks/generic/chunks_1.md3");
		G_ModelIndex("models/chunks/generic/chunks_2.md3");

		ent->boltpoint2 = DEBRIS_SPECIALCASE_CHUNKS;
	}
	else if (strcmp(model, "wood") == 0)
	{
		G_ModelIndex("models/chunks/crate/crate1_1.md3");
		G_ModelIndex("models/chunks/crate/crate1_2.md3");
		G_ModelIndex("models/chunks/crate/crate1_3.md3");
		G_ModelIndex("models/chunks/crate/crate1_4.md3");
		G_ModelIndex("models/chunks/crate/crate2_1.md3");
		G_ModelIndex("models/chunks/crate/crate2_2.md3");
		G_ModelIndex("models/chunks/crate/crate2_3.md3");
		G_ModelIndex("models/chunks/crate/crate2_4.md3");

		ent->boltpoint2 = DEBRIS_SPECIALCASE_WOOD;
	}
	else if (strcmp(model, "glass") == 0)
	{
		G_ModelIndex("models/chunks/metal/metal1_1.md3");
		G_ModelIndex("models/chunks/metal/metal1_2.md3");
		G_ModelIndex("models/chunks/metal/metal1_3.md3");
		G_ModelIndex("models/chunks/metal/metal1_4.md3");
		G_ModelIndex("models/chunks/metal/metal2_1.md3");
		G_ModelIndex("models/chunks/metal/metal2_2.md3");
		G_ModelIndex("models/chunks/metal/metal2_3.md3");
		G_ModelIndex("models/chunks/metal/metal2_4.md3");

		ent->boltpoint2 = DEBRIS_SPECIALCASE_GLASS;
	}
	else if (strcmp(model, "none") == 0)
	{
		ent->boltpoint2 = 0;
	}
	else
	{
		ent->boltpoint2 = G_ModelIndex(model);
	}

	trap_LinkEntity(ent);
}

// Drain model
void JKMod_SP_DrainModel(gentity_t* ent)
{
	G_SpawnString("model", "", &ent->model);

	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = G_ModelIndex(ent->model);
	ent->s.modelindex2 = G_ModelIndex(ent->model);

	G_SpawnVector("mins", "-16 -16 -16", ent->r.mins);
	G_SpawnVector("maxs", "16 16 16", ent->r.maxs);

	if (!Q_stricmp(ent->model, ".glm"))
	{
		ent->s.g2radius = 100;
		ent->s.modelGhoul2 = 1;
	}

	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);

	trap_LinkEntity(ent);
}

// Misc ion cannon
void JKMod_SP_MiscIonCannon(gentity_t *ent)
{
	char *model;

	model = "models/map_objects/imp_mine/ion_cannon.glm";

	ent->s.eType = ET_GENERAL;
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->bolt_Head = 0;
	ent->s.modelindex = G_ModelIndex(model);
	ent->s.modelGhoul2 = 1;
	ent->s.g2radius = 110;

	G_SetOrigin(ent, ent->s.origin);

	VectorCopy(ent->s.angles, ent->pos1 );
	VectorCopy(ent->s.angles, ent->r.currentAngles);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);

	VectorSet(ent->r.mins, -141.0f, -148.0f, 0.0f);
	VectorSet(ent->r.maxs,  142.0f, 135.0f, 245.0f);

	ent->s.pos.trType = TR_STATIONARY;
	trap_LinkEntity(ent);
}
