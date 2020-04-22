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

void JKPlus_ClientTimerActions(gentity_t *ent, int msec) 
{
	gclient_t	*client;
	qtime_t		serverTime;
	char		*serverTimeType;
	char		serverMotd[MAX_STRING_CHARS];

	trap_RealTime(&serverTime);
	serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

	client = ent->client;
	client->JKPlusTimeResidual += msec;

	// Don't allow in pause mode
	if (jkcvar_pauseGame.integer) return;

	// Launch original client timer actions function
	BaseJK2_ClientTimerActions(ent, msec);

	// Custom time actions
	while (client->JKPlusTimeResidual >= 1000)
	{
		client->JKPlusTimeResidual -= 1000;

		// Drop flag check
		if (client->JKPlusDropFlagTime)
		{
			if (client->JKPlusDropFlagTime > 0)
			{
				client->JKPlusDropFlagTime--;
			}
			else
			{
				client->JKPlusDropFlagTime = 0;
			}
		}

		// Chat protect check
		if (jkcvar_chatProtect.integer && (client->ps.eFlags & EF_TALK))
		{
			if (client->JKPlusChatTime >= jkcvar_chatProtectTime.integer)
			{
				client->JKPlusChatTime = jkcvar_chatProtectTime.integer;
				client->ps.eFlags |= JK_CHAT_PROTECT;
				ent->takedamage = qfalse;
			}
			else
			{
				client->JKPlusChatTime++;
			}
		}
		else
		{
			if (client->JKPlusChatTime != 0) client->JKPlusChatTime = 0;
			if (client->ps.eFlags & JK_CHAT_PROTECT) client->ps.eFlags &= ~JK_CHAT_PROTECT;
			if (!ent->health <= 0) ent->takedamage = qtrue;
		}

		// Show server motd
		if (client->JKPlusMotdTime && *jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
		{
			JKPlus_stringEscape(jkcvar_serverMotd.string, serverMotd, MAX_STRING_CHARS);
			trap_SendServerCommand(client->ps.clientNum, va("cp \"%s\nTime: %d\"", serverMotd, client->JKPlusMotdTime));
			client->JKPlusMotdTime--;
		}

		// Server news
		if (Q_stricmp(jkcvar_serverNews.string, "1") == 0 && !Q_stricmp(level.JKPlusServerNews[0], "") == 0)
		{
			int i;
			int total = level.JKPlusServerNewsCount;
			
			level.JKPlusServerNewsNum++;

			for (i = 1; i < (jkcvar_serverNewsTime.integer * total); i++)
			{
				if (level.JKPlusServerNewsNum == (jkcvar_serverNewsTime.integer * i))
				{
					trap_SendServerCommand(client->ps.clientNum, va("print \"Server News ^5(^7%02i^5:^7%02i%s^5)^7: %s\n\"", serverTime.tm_hour, serverTime.tm_min, serverTimeType, level.JKPlusServerNews[(i-1)]));
					// Reset
					if (level.JKPlusServerNewsNum == ((jkcvar_serverNewsTime.integer * total))) {
						level.JKPlusServerNewsNum = 0;
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

void JKPlus_ClientThink_real(gentity_t *ent)
{
	// Already in an emote
	if (JKPlus_emoteIn(ent, -1))
	{
		if (ent->client->ps.pm_type == PM_DEAD)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKPlus_emoteIn(ent, 0))
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
		else if (JKPlus_emoteIn(ent, 1) && ent->client->pers.cmd.upmove > 0)
		{	
			// In an animation emote
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKPlus_emoteIn(ent, 2))
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
