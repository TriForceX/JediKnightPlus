/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Misc functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// External stuff
extern qboolean SaberAttacking(gentity_t *self);
extern qboolean WP_HasForcePowers(const playerState_t *ps);

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
	const char	*currentmap = jkcvar_mapName.string;
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
Custom force power change
=====================================================================
*/
void JKMod_ForcePowerChange(gentity_t *ent, unsigned dimension)
{
	WP_InitForcePowers(ent);
	JKMod_DimensionSettings(ent, dimension);
	trap_SendServerCommand(ent - g_entities, va("print \"" S_COLOR_GREEN "%s\n\"", G_GetStripEdString("SVINGAME", "FORCEAPPLIED")));
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
	if (ent->client->pers.jkmodPers.customDuel == DUEL_SABER)
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
Player movements and tweaks check
=====================================================================
*/
void JKMod_PlayerTweaksCheck(gentity_t *ent)
{
	int i;
	jkmod_tweaks_t playerTweaks[] =
	{
		// feature					plugin required
		{ JK_JETPACK_JKA,			qfalse },
		{ JK_WEAPON_STAND,			qfalse },
		{ JK_DISRUPTOR_WALK,		qtrue },
		{ JK_USE_STAND,				qfalse },
		{ JK_SPECTATOR_NOCLIP,		qfalse },
		{ JK_RESIST_PUSH,			qfalse },
		{ JK_DUAL_MOVES,			qfalse },
		{ JK_GHOST_SABERS,			qfalse },
		{ JK_CHAIR_FORCE,			qfalse },
	};

	for (i = 0; i < ARRAY_LEN(playerTweaks); i++)
	{
		if ((jkcvar_playerTweaks.integer & playerTweaks[i].feature) && (!playerTweaks[i].plugin ? qtrue : ent->client->pers.jkmodPers.clientPlugin)) {
			ent->client->ps.stats[JK_TWEAKS] |= playerTweaks[i].feature;
		} else {
			ent->client->ps.stats[JK_TWEAKS] &= ~playerTweaks[i].feature;
		}
	}

	JKMod_Printf(S_COLOR_YELLOW "Client %i tweaks checked\n", ent->client->ps.clientNum);
}

/*
=====================================================================
Player model, skin and sounds info
=====================================================================
*/
void JKMod_PlayerModelInfo(gentity_t *ent, const char* modelname)
{
	int				i;
	int				fLen = 0;
	char			soundpath[MAX_QPATH];
	char			soundfull[MAX_QPATH];
	fileHandle_t	f;
	char			model[MAX_QPATH];
	char			*skin;

	// Get model and skin
	Q_strncpyz( model, modelname, sizeof( model ) );

	skin = Q_strrchr( model, '/' );
	if ( skin ) {
		*skin++ = '\0';
	} else {
		skin = "default";
	}

	// Get sound
	if (skin[0] == '\0' || !Q_stricmp( "default", skin)) {	
		fLen = trap_FS_FOpenFile(va("models/players/%s/sounds.cfg", model), &f, FS_READ);
		if (!f) {
			fLen = trap_FS_FOpenFile(va("models/players/%s/sounds_default.cfg", model), &f, FS_READ);
		}
	} else {
		fLen = trap_FS_FOpenFile(va("models/players/%s/sounds_%s.cfg", model, skin), &f, FS_READ);
		if (!f) {
			fLen = trap_FS_FOpenFile(va("models/players/%s/sounds.cfg", model), &f, FS_READ);
		}
	}

	soundpath[0] = 0;

	if (f) {
		trap_FS_Read(soundpath, fLen, f);
		i = fLen;
		while (i > 0) {
			i--;
			if (soundpath[i] == '\n') {
				if (i > 0) i--; 
				break;
			}
		}
		soundpath[i] = '\0';
		trap_FS_FCloseFile(f);
	}

	// Check sound
	if (soundpath[0]) {
		Q_strncpyz(soundfull, va("sound/%s", soundpath), sizeof(soundfull));
	} else {
		Q_strncpyz(soundfull, va("sound/chars/%s/misc", model), sizeof(soundfull));
	}

	// Check path
	if (trap_FS_FOpenFile(va("%s/taunt.mp3", soundfull), &f, FS_READ) >= 0) {
		trap_FS_FCloseFile(f);
		Q_strncpyz(ent->client->pers.jkmodPers.modelSounds, soundfull, sizeof(ent->client->pers.jkmodPers.modelSounds));
	} else {
		Q_strncpyz(ent->client->pers.jkmodPers.modelSounds, "sound/chars/mp_generic_male/misc", sizeof(ent->client->pers.jkmodPers.modelSounds));
	}

	Q_strncpyz(ent->client->pers.jkmodPers.modelName, model, sizeof(ent->client->pers.jkmodPers.modelName));
	Q_strncpyz(ent->client->pers.jkmodPers.skinName, skin, sizeof(ent->client->pers.jkmodPers.skinName));
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
		trap_SendServerCommand(-1, va("cp \"Gameplay changed to 1.0%i\"", version));
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
			JKMod_G_PlayEffect_ID(G_EffectIndex((!strcmp(efxfile, "default") ? "mp/spawn" : efxfile)), player->client->ps.origin, temporigin, player->s.number, qfalse);
			JKMod_G_PlayEffect_ID(G_EffectIndex((!strcmp(efxfile, "default") ? "mp/spawn" : efxfile)), origin, temporigin, player->s.number, qfalse);
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
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR && jkcvar_teleportFrag.integer) 
	{
		G_KillBox(player);
	}

	// Save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

	// Use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	// Link entity
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) trap_LinkEntity(player);

	// Chair emote disable
	if (player->client->jkmodClient.chairModelUsed) JKMod_ChairModelDisable(player);

	// Check stuck
	if (!jkcvar_teleportFrag.integer && JKMod_OthersInBox(player)) JKMod_AntiStuckBox(player);
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
void JKMod_CustomGameSettings(gentity_t *ent, int weapondisable, int forcedisable, int forcelevel, qboolean holdables, qboolean jetpack, qboolean invulnerability, qboolean passthrough, float speed, float gravity)
{
	int customSettings = ent->client->pers.jkmodPers.customSettings;
	int customSettingsCount = ent->client->pers.jkmodPers.customSettingsCount;

	// Reset?
	if (customSettings == customSettingsCount && customSettings >= MAX_CLIENTS) 
	{ 
		ent->client->pers.jkmodPers.customSettings = 0;
		ent->client->pers.jkmodPers.customSettingsCount = 0;
	}

	// Default check
	if (weapondisable == DEFAULT)	weapondisable	= g_weaponDisable.integer;
	if (forcedisable == DEFAULT)	forcedisable	= g_forcePowerDisable.integer;
	if (speed == DEFAULT)			speed			= g_speed.value;
	if (gravity == DEFAULT)			gravity			= g_gravity.value;

	if (ent->client->sess.sessionTeam != TEAM_SPECTATOR && g_gametype.integer != GT_JEDIMASTER && g_gametype.integer != GT_HOLOCRON)
	{
		// Force
		if (forcedisable != ent->client->ps.fd.forcePowersKnown && forcedisable != g_forcePowerDisable.integer)
		{
			int i;
			int checkLevel = forcelevel == DEFAULT ? FORCE_LEVEL_0 : forcelevel;
			int checkDimension = ent->client->ps.stats[JK_DIMENSION];
			
			i = 0;
			while (i < NUM_FORCE_POWERS)
			{
				if (ent->client->ps.fd.forcePowersActive & (1 << i)) WP_ForcePowerStop(ent, i);

				if ((checkDimension & (DIMENSION_FORCE | DIMENSION_PRIVATE)) || ent->client->pers.jkmodPers.customDuel == DUEL_FORCE)
				{
					ent->client->ps.fd.forcePowerLevel[i] = checkLevel;

					if (!forcePowerDarkLight[i] || ent->client->ps.fd.forceSide == forcePowerDarkLight[i])
					{
						if (!(forcedisable & (1 << i)))
						{
							if (forcelevel == DEFAULT && !(weapondisable & (1 << WP_SABER)) && i == FP_SABERATTACK) checkLevel = FORCE_LEVEL_1;
							ent->client->ps.fd.forcePowerLevel[i] = checkLevel;
							ent->client->ps.fd.forcePowersKnown |= (1 << i);
						}
					}
				}
				else
				{
					if (forcedisable & (1 << i))
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
				}
				
				i++;
			}
		}
		else
		{
			WP_InitForcePowers(ent);
		}

		// Weapons
		if (weapondisable != ent->client->ps.stats[STAT_WEAPONS] && weapondisable != g_weaponDisable.integer)
		{
			int i;
			int firstWeapon = 0;

			i = 0;
			while (i < WP_NUM_WEAPONS)
			{
				if (!(weapondisable & (1 << i)) && i != WP_NONE && i != WP_EMPLACED_GUN && i != WP_TURRET)
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
		ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
		ent->client->ps.eFlags &= ~JK_JETPACK_FLAMING;
		ent->client->ps.stats[JK_FUEL] = 100;
		ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
	}
	else if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) 
	{
		ent->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
		ent->client->ps.stats[JK_FUEL] = 0;
		ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
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
extern void StandardSetBodyAnim(gentity_t *self, int anim, int flags);
extern jkmod_emotes_data_t JKModEmotesData[];
extern void Blocked_Door(gentity_t *ent, gentity_t *other);
extern void InitMover(gentity_t *ent);
extern void Think_MatchTeam(gentity_t *ent);
extern void Think_SpawnNewDoorTrigger(gentity_t *ent);

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
		if (!activator->client->pers.jkmodPers.clientPlugin) {
			if (typeAmmo)
				trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Ammo: ^3%i\"", (getAmount > 999 ? 999 : getAmount)));
			else if (typeHealth)
				trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Health: ^1%i\"", getAmount));
			else if (typeShield)
				trap_SendServerCommand(activator->client->ps.clientNum, va("cp \"Shield: ^2%i\"", getAmount));
		}
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

	mins = ent->watertype == STATION_HEALTH_TYPE ? "-8 -8 -50" : "-16 -16 -40";
	maxs = ent->watertype == STATION_HEALTH_TYPE ? "8 8 50" : "16 16 40";

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
	ent->s.bolt2 = ent->watertype;
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

// Chair model disable function
void JKMod_ChairModelDisable(gentity_t *ent)
{
	// Unset
	ent->client->ps.pm_type = PM_NORMAL;
	ent->client->ps.forceRestricted = qfalse;
	ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
	ent->client->ps.forceDodgeAnim = 0;
	ent->client->ps.forceHandExtendTime = 0;
	StandardSetBodyAnim(ent, BOTH_STAND1, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);

	// Adjust
	if (ent->client->jkmodClient.chairModelUsed != 1) ent->client->ps.origin[2] += 5;
	
	// Clear
	ent->client->jkmodClient.chairModelUsed = 0;
	ent->client->jkmodClient.chairModelDelay = level.time + 1000;
	VectorClear(ent->client->ps.velocity);
	level.jkmodLocals.chairModelCheck[ent->client->jkmodClient.chairModelNum][ent->jkmodEnt.dimensionNumber] = qfalse;
}

// Chair model use function
void JKMod_ChairModelUse(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	int emote = EMOTE_SIT;

	if (!activator || !activator->client) return;
	if (activator->client->ps.duelInProgress) return;
	if (self->count > level.time) return;
	if (activator->client->jkmodClient.chairModelDelay > level.time) return;
	
	// Check user
	if (level.jkmodLocals.chairModelCheck[self->s.number][activator->jkmodEnt.dimensionNumber]) {
		trap_SendServerCommand(activator->client->ps.clientNum, "cp \"Take another seat\"");
		return;
	}

	// Set emote
	if (self->watertype == 2) emote = EMOTE_SIT2;
	if (self->watertype == 3) emote = EMOTE_SIT3;

	// Delay
	self->count = level.time + 1000;

	// Disable emote
	if (activator->client->jkmodClient.chairModelUsed)
	{
		JKMod_ChairModelDisable(activator);
	}
	// Enable emote
	else if (JKMod_EmoteCheck(JKModEmotesData[emote].cmd, activator))
	{
		vec3_t	startspot, endspot, angles;
		int		i = 0;

		// Remove force powers
		while (i < NUM_FORCE_POWERS) {
			if (activator->client->ps.fd.forcePowersActive & (1 << i)) {
				WP_ForcePowerStop(activator, i);
			}
			i++;
		}

		// Get top
		VectorCopy(self->s.origin, startspot);
		startspot[2] += self->r.maxs[2] + 4;
		VectorCopy(startspot, endspot);

		// Set anim
		StandardSetBodyAnim(activator, JKModEmotesData[emote].startAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
		activator->client->ps.pm_type = PM_FREEZE;
		activator->client->ps.forceRestricted = qtrue;
		activator->client->jkmodClient.chairModelNum = self->s.number;
		activator->client->jkmodClient.chairModelUsed = self->watertype;
		activator->client->jkmodClient.chairModelDelay = level.time + 1000;

		// Move
		VectorCopy(endspot, activator->client->ps.origin);

		if (self->watertype == 1) activator->client->ps.origin[2] += activator->r.maxs[2] / 2;
		if (self->watertype != 1) activator->client->ps.origin[2] += 5;

		// Custom pos
		if (self->bolt_Head) activator->client->ps.origin[0] += self->bolt_Head;
		if (self->bolt_Waist) activator->client->ps.origin[1] += self->bolt_Waist;

		// View
		VectorCopy(self->s.angles, angles);
		angles[YAW] = self->s.angles[YAW];
		if (self->bolt_Motion) angles[YAW] += self->bolt_Motion;
		SetClientViewAngle(activator, angles);

		// Check stuck
		if (JKMod_OthersInBox(activator)) JKMod_AntiStuckBox(activator);
		
		// Lock user
		level.jkmodLocals.chairModelCheck[self->s.number][activator->jkmodEnt.dimensionNumber] = qtrue;
	}
}

// Chair model
void JKMod_SP_ChairModel(gentity_t* ent)
{
	G_SpawnInt("type", "", &ent->watertype);
	G_SpawnInt("offsetX", "0", &ent->bolt_Head);
	G_SpawnInt("offsetY", "0", &ent->bolt_Waist);
	G_SpawnInt("viewAngles", "0", &ent->bolt_Motion);
	G_SpawnString("model", "", &ent->model);

	if (ent->watertype != 1 && ent->watertype != 2 && ent->watertype != 3)
	{
		G_Printf(S_COLOR_RED "ERROR: jkmod_chair_model doesn't have a valid type (%i)\n", ent->watertype);
		G_FreeEntity(ent);
		return;
	}

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

	ent->r.svFlags |= SVF_PLAYER_USABLE;
	ent->s.generic1 |= GENERIC_USABLE; // Tr!Force: [IdentifyObjects] Check usable hint
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;
	ent->use = JKMod_ChairModelUse;
	ent->count = 0;

	G_SetOrigin(ent, ent->s.origin);
	VectorCopy(ent->s.angles, ent->s.apos.trBase);

	trap_LinkEntity(ent);
}

// Door model
void JKMod_SP_DoorModel(gentity_t* ent)
{
	vec3_t	abs_movedir;
	float	distance;
	vec3_t	size;
	float	lip;
	char	*sound;
	int		soundon = 0;
	char	*model;
	float	modelAngle;
	float	modelPitch;
	float	modelRoll;

	G_SpawnInt("sound", "1", &soundon);
	G_SpawnFloat("lip", "8", &lip);
	G_SpawnFloat("modelAngle", "0", &modelAngle);
	G_SpawnFloat("modelPitch", "0", &modelPitch);
	G_SpawnFloat("modelRoll", "0", &modelRoll);

	if (soundon)
	{
		G_SpawnString("soundstart", "sound/movers/doors/door1start.wav", &sound);
		ent->sound1to2 = ent->sound2to1 = G_SoundIndex(sound);

		G_SpawnString("soundstop", "sound/movers/doors/door1stop.wav", &sound);
		ent->soundPos1 = ent->soundPos2 = G_SoundIndex(sound);

		G_SpawnString("soundmove", "sound/movers/doors/door1move.wav", &sound);
		ent->soundLoop = G_SoundIndex(sound);
	}
	else
	{
		ent->sound1to2 = ent->sound2to1 = ent->soundPos1 = ent->soundPos2 = 0;
	}

	ent->blocked = Blocked_Door;

	// default speed of 400
	if (!ent->speed) ent->speed = 400;

	// default wait of 2 seconds
	if (!ent->wait) ent->wait = 2;

	// default damage of 2 points
	G_SpawnInt("dmg", "2", &ent->damage);

	// first position at start
	VectorCopy(ent->s.origin, ent->pos1);

	// calculate second position
	G_SpawnString( "model", "", &model );
	
	ent->s.modelindex = G_ModelIndex( model );
	ent->s.modelindex2 = G_ModelIndex( model );

	if (!Q_stricmp(ent->model,".glm"))
	{	
		ent->s.g2radius = 100;
		ent->s.modelGhoul2 = 1;
	}
	
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;

	if (ent->r.maxs[2] == 0 && ent->r.mins[2] == 0)
	{
		JKMod_Printf(S_COLOR_YELLOW "Door spawned without collision box.\n");
		VectorSet(ent->r.maxs, 25, 25, 21);
		VectorScale(ent->r.maxs, -1, ent->r.mins);
	}

	trap_LinkEntity(ent);
	G_SetMovedir (ent->s.angles, ent->movedir);
	ent->s.angles[YAW] = modelAngle;
	ent->s.angles[PITCH] = modelPitch;
	ent->s.angles[ROLL] = modelRoll;
	G_SetAngles(ent, ent->s.angles);
	abs_movedir[0] = fabs(ent->movedir[0]);
	abs_movedir[1] = fabs(ent->movedir[1]);
	abs_movedir[2] = fabs(ent->movedir[2]);
	VectorSubtract(ent->r.maxs, ent->r.mins, size);
	distance = DotProduct( abs_movedir, size) + lip;
	VectorMA(ent->pos1, distance, ent->movedir, ent->pos2);

	// if "start_open", reverse position 1 and 2
	if (ent->spawnflags & 1) {
		vec3_t	temp;

		VectorCopy(ent->pos2, temp);
		VectorCopy(ent->s.origin, ent->pos2);
		VectorCopy(temp, ent->pos1);
	}

	// Allow door use button
	if ((jkcvar_mapFixes.integer & JK_MAP_SPDOORUSE) && JKMod_SPMapCheck(jkcvar_mapName.string))
	{
		 if (ent->spawnflags & 64) ent->boltpoint1 = 1;
	}

	InitMover(ent);

	ent->s.modelindex = G_ModelIndex(model);
	ent->s.modelindex2 = G_ModelIndex(model);
	ent->r.contents = CONTENTS_SOLID;
	ent->clipmask = MASK_SOLID;

	trap_LinkEntity(ent);

	ent->nextthink = level.time + FRAMETIME;

	if (!(ent->flags & FL_TEAMSLAVE)) 
	{
		int health;

		G_SpawnInt("health", "0", &health);

		if (health) ent->takedamage = qtrue;

		if (ent->targetname || health) {
			// non touch/shoot doors
			ent->think = Think_MatchTeam;
		} else {
			// Fix doors trigger spawn
			if ((jkcvar_mapFixes.integer & JK_MAP_SPDOORTRIGGER) && JKMod_SPMapCheck(jkcvar_mapName.string)) {
				JKMod_DoorFix(ent);
			}
			ent->think = Think_SpawnNewDoorTrigger;
		}
	}
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
