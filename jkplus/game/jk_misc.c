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
Teleport player function
=====================================================================
*/
void JKPlus_TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean spitplayer, qboolean killbox)
{
	gentity_t	*tent;

	if (player->client->sess.spectatorState == SPECTATOR_FOLLOW)
	{ // Follow spectators don't need to teleport. And calling BG_PlayerStateToEntityState on them corrupts their s.number.
		return;
	}

	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		tent = G_TempEntity(player->client->ps.origin, EV_PLAYER_TELEPORT_OUT);
		tent->s.clientNum = player->s.clientNum;

		tent = G_TempEntity(origin, EV_PLAYER_TELEPORT_IN);
		tent->s.clientNum = player->s.clientNum;
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	trap_UnlinkEntity(player);

	VectorCopy(origin, player->client->ps.origin);

	// spit the player out
	if (spitplayer) 
	{
		player->client->ps.origin[2] += 1;

		AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
		VectorScale(player->client->ps.velocity, 400, player->client->ps.velocity);
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	}

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// set angles
	SetClientViewAngle(player, angles);

	// kill anything at the destination
	if (killbox)
	{
		if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
			G_KillBox(player);
		}
	}

	// save results of pmove
	BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

	// use the precise origin for linking
	VectorCopy(player->client->ps.origin, player->r.currentOrigin);

	if (player->client->sess.sessionTeam != TEAM_SPECTATOR) {
		trap_LinkEntity(player);
	}
}
