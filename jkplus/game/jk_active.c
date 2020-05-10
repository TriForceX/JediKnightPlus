/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Client active module
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Client timer actions function
=====================================================================
*/
void JKMod_ClientTimerActions(gentity_t *ent, int msec) 
{
	gclient_t	*client;
	qtime_t		serverTime;
	char		*serverTimeType;
	char		serverMotd[MAX_STRING_CHARS];

	trap_RealTime(&serverTime);
	serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

	client = ent->client;
	client->jkmodClient.TimeResidual += msec;

	// Don't allow in pause mode
	if (jkcvar_pauseGame.integer) return;

	// Launch original client timer actions function
	BaseJK2_ClientTimerActions(ent, msec);

	// Custom time actions
	while (client->jkmodClient.TimeResidual >= 1000)
	{
		client->jkmodClient.TimeResidual -= 1000;

		// Drop flag check
		if (client->jkmodClient.DropFlagTime)
		{
			if (client->jkmodClient.DropFlagTime > 0) client->jkmodClient.DropFlagTime--;
			else client->jkmodClient.DropFlagTime = 0;
		}

		// Call vote check
		if (client->jkmodClient.VoteWaitTime)
		{
			if (client->jkmodClient.VoteWaitTime > 0) client->jkmodClient.VoteWaitTime--;
			else client->jkmodClient.VoteWaitTime = 0;
		}

		// Chat protect check
		if (jkcvar_chatProtect.integer && (client->ps.eFlags & EF_TALK))
		{
			if (client->jkmodClient.ChatTime >= jkcvar_chatProtectTime.integer)
			{
				client->jkmodClient.ChatTime = jkcvar_chatProtectTime.integer;
				client->ps.eFlags |= JK_CHAT_PROTECT;
				ent->takedamage = qfalse;
			}
			else
			{
				client->jkmodClient.ChatTime++;
			}
		}
		else
		{
			if (client->jkmodClient.ChatTime != 0) client->jkmodClient.ChatTime = 0;
			if (client->ps.eFlags & JK_CHAT_PROTECT) client->ps.eFlags &= ~JK_CHAT_PROTECT;
			if (!ent->health <= 0) ent->takedamage = qtrue;
		}

		// Show server motd
		if (client->jkmodClient.MotdTime && *jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
		{
			JKMod_stringEscape(jkcvar_serverMotd.string, serverMotd, MAX_STRING_CHARS);
			G_CenterPrint(client->ps.clientNum, 3, va("%s\nTime: %d\n", serverMotd, client->jkmodClient.MotdTime));
			client->jkmodClient.MotdTime--;
		}

		// Server news
		if (Q_stricmp(jkcvar_serverNews.string, "1") == 0 && !Q_stricmp(level.jkmodLevel.ServerNews[0], "") == 0 && g_gametype.integer != GT_TOURNAMENT)
		{
			int i;
			int total = level.jkmodLevel.ServerNewsCount;
			
			level.jkmodLevel.ServerNewsNum++;

			for (i = 1; i < (jkcvar_serverNewsTime.integer * total); i++)
			{
				if (level.jkmodLevel.ServerNewsNum == (jkcvar_serverNewsTime.integer * i))
				{
					trap_SendServerCommand(client->ps.clientNum, va("print \"Server News ^5(^7%02i^5:^7%02i%s^5)^7: %s\n\"", serverTime.tm_hour, serverTime.tm_min, serverTimeType, level.jkmodLevel.ServerNews[(i-1)]));
					// Reset
					if (level.jkmodLevel.ServerNewsNum == ((jkcvar_serverNewsTime.integer * total))) {
						level.jkmodLevel.ServerNewsNum = 0;
					}
				}
			}
		}
	}
}

/*
=====================================================================
Client think real function
=====================================================================
*/
void JKMod_ClientThink_real(gentity_t *ent)
{
	// Already in an emote
	if (JKMod_emoteIn(ent, -1))
	{
		if (ent->client->ps.pm_type == PM_DEAD)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKMod_emoteIn(ent, 0))
		{	
			// In a frozen emote
			if (ent->client->pers.cmd.upmove > 0)
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
				ent->client->ps.forceDodgeAnim = 0;
				ent->client->ps.forceHandExtendTime = 0;
			}
			else
			{
				ent->client->ps.forceHandExtend = HANDEXTEND_DODGE;
				ent->client->ps.forceDodgeAnim = (ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT);
				ent->client->ps.forceHandExtendTime = level.time + INFINITE;
			}
		}
		else if (JKMod_emoteIn(ent, 1) && ent->client->pers.cmd.upmove > 0)
		{	
			// In an animation emote
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKMod_emoteIn(ent, 2))
		{	
			// In a special emote (walkable)
			ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
			ent->client->ps.forceDodgeAnim = (ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT);
			ent->client->ps.forceHandExtendTime = level.time + INFINITE;
		}

		// Keep the client informed about to predict the emote leg timers
		if (!(ent->client->ps.eFlags & JK_EMOTE_IN))
		{
			ent->client->ps.eFlags |= JK_EMOTE_IN;
		}
	}
	else if (ent->client->ps.eFlags & JK_EMOTE_IN)
	{
		ent->client->ps.eFlags &= ~JK_EMOTE_IN;
	}

	// Launch original client think real function
	BaseJK2_ClientThink_real(ent);
}

/*
=====================================================================
Run client function
=====================================================================
*/
void JKMod_RunClient(gentity_t *ent) 
{
	if (jkcvar_antiWarp.integer)
	{
		gclient_t	*client = ent->client;
		usercmd_t	*cmd = &client->pers.cmd;

		if (jkcvar_pauseGame.integer) // Don't allow in pause mode
		{ 
			return;
		}
		else if (ent->client->pers.botDelayed) // Call ClientBegin for delayed bots now
		{ 
			ClientBegin(ent - g_entities, qtrue);
			ent->client->pers.botDelayed = qfalse;
		}
		else if ((ent->r.svFlags & SVF_BOT) || g_synchronousClients.integer)
		{
			cmd->serverTime = level.time;
			ClientThink_real(ent);
		}
		else if (client->lastCmdTime > 0 &&
			client->lastCmdTime < level.time - (jkcvar_antiWarpTime.value*1000) &&
			client->pers.connected == CON_CONNECTED &&
			client->sess.spectatorState == SPECTATOR_NOT &&
			client->ps.pm_type != PM_DEAD)
		{
			client->ps.eFlags |= EF_CONNECTION;

			if (jkcvar_antiWarp.integer == 2)
			{
				// Create a fake user command to make him move, causing client prediction error for a warping player
				cmd->serverTime = level.time + (cmd->serverTime - client->lastCmdTime);
				cmd->buttons = 0;
				cmd->generic_cmd = 0; // Let go any force power
				cmd->forwardmove = 0;
				cmd->rightmove = 0;
				cmd->upmove = 0;

				ClientThink_real(ent);
			}
		}
		else
		{
			client->ps.eFlags &= ~EF_CONNECTION;
		}
	}
	else
	{
		if (!(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer || jkcvar_pauseGame.integer) { // Don't allow in pause mode
			return;
		}

		if (ent->client->pers.botDelayed) // Call ClientBegin for delayed bots now
		{ 
			ClientBegin(ent - g_entities, qtrue);
			ent->client->pers.botDelayed = qfalse;
		}

		ent->client->pers.cmd.serverTime = level.time;
		ClientThink_real(ent);
	}
}
