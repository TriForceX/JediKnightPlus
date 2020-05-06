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
Play effect by ID function
=====================================================================
*/
gentity_t *JKMod_PlayEffect_ID(int fxID, vec3_t org, vec3_t ang)
{
	gentity_t	*te;

	te = G_TempEntity(org, EV_PLAY_EFFECT_ID);
	VectorCopy(ang, te->s.angles);
	VectorCopy(org, te->s.origin);
	te->s.eventParm = fxID;

	return te;
}

/*
=====================================================================
Push box function
=====================================================================
*/
void JKMod_PushBox(gentity_t *ent) {
	int			i, num;
	int			touch[MAX_GENTITIES];
	gentity_t	*hit;
	vec3_t		mins, maxs;
	vec3_t		temporigin, tempangles;

	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	num = trap_EntitiesInBox(mins, maxs, touch, MAX_GENTITIES);

	for (i = 0; i < num; i++) 
	{
		hit = &g_entities[touch[i]];
		if (!hit->client) {
			continue;
		}

		// push it
		VectorClear(temporigin);
		VectorClear(tempangles);

		tempangles[ROLL] = 0;
		tempangles[PITCH] = 0;
		tempangles[YAW] = hit->client->ps.viewangles[YAW];

		temporigin[0] = hit->client->ps.origin[0];
		temporigin[1] = hit->client->ps.origin[1];
		temporigin[2] = hit->client->ps.origin[2] + DEFAULT_MAXS_2;

		JKMod_TeleportPlayer(hit, temporigin, tempangles, qfalse, qfalse, 0, "none", NULL);
	}
}
/*
=====================================================================
Teleport player function
=====================================================================
*/
void JKMod_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean telefrag, qboolean spitplayer, int spitspeed, char *efxfile, char *efxsound)
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

			tent = G_TempEntity(origin, EV_PLAYER_TELEPORT_IN);
			tent->s.clientNum = player->s.clientNum;
		}
		else
		{
			if (strcmp(efxfile, "none") != 0)
			{
				vec3_t temporigin;

				temporigin[YAW] = 0;
				temporigin[PITCH] = -90;
				temporigin[ROLL] = 0;

				JKMod_PlayEffect_ID(G_EffectIndex(efxfile), player->client->ps.origin, temporigin);
				G_SoundAtLoc(player->client->ps.origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/teleout")));

				JKMod_PlayEffect_ID(G_EffectIndex(efxfile), origin, temporigin);
				G_SoundAtLoc(origin, CHAN_VOICE, G_SoundIndex((!efxsound[0] == '\0' ? efxsound : "sound/player/telein")));
			}
		}
	}

	if (telefrag)
	{
		// unlink to make sure it can't possibly interfere with G_KillBox
		trap_UnlinkEntity(player);
	}

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
	if (telefrag)
	{
		if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
			G_KillBox(player);
		}
	}
	// If not, push them
	else
	{
		if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
			JKMod_PushBox(player);
		}
	}

	// save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

	// use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	if (telefrag)
	{
		if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
			trap_LinkEntity(player);
		}
	}
}
