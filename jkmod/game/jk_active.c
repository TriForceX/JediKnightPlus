/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
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
	gclient_t *client;
	client = ent->client;

	// Drop flag check
	if (client->jkmodClient.dropFlagTime)
	{
		if (client->jkmodClient.dropFlagTime > 0) client->jkmodClient.dropFlagTime--;
		else client->jkmodClient.dropFlagTime = 0;
	}

	// Call vote check
	if (client->jkmodClient.voteWaitTime)
	{
		if (client->jkmodClient.voteWaitTime > 0) client->jkmodClient.voteWaitTime--;
		else client->jkmodClient.voteWaitTime = 0;
	}

	// Dimension time check
	if (client->jkmodClient.dimensionTime)
	{
		if (client->jkmodClient.dimensionTime > 0) client->jkmodClient.dimensionTime--;
		else client->jkmodClient.dimensionTime = 0;
	}

	// Teleport chat time check
	if (client->jkmodClient.teleportChatTime)
	{
		if (client->jkmodClient.teleportChatTime > 0) client->jkmodClient.teleportChatTime--;
		else client->jkmodClient.teleportChatTime = 0;
	}

	// Force change time check
	if (client->jkmodClient.forceChangeDelay && jkcvar_forceChangeTime.integer && jkcvar_forceChangeInstant.integer != 1 && ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		if (client->jkmodClient.forceChangeDelay > 1) {
			client->jkmodClient.forceChangeDelay--;
			trap_SendServerCommand(ent - g_entities, va("cp \"Applying force changes in %d\"", ent->client->jkmodClient.forceChangeDelay));
		} else {
			trap_SendServerCommand(ent - g_entities, "cp \"Force updated!\"");
			if (ent->client->pers.jkmodPers.customDuel == DUEL_FORCE || (ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_FORCE | DIMENSION_PRIVATE))) {
				JKMod_ForcePowerChange(ent, DIMENSION_FORCE);
			} else {
				JKMod_ForcePowerChange(ent, DIMENSION_FREE);
			}
			client->jkmodClient.forceChangeDelay = 0;
		}
	}

	// Private remaining time check
	if (client->pers.jkmodPers.privateRoom[PRIVATE_TIME])
	{
		if (client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] > 0) 
		{
			trap_SendServerCommand(ent - g_entities, va("cp \"You have been invited by\n%s" S_COLOR_WHITE "\nto create a private room\nsay ^2!accept^7 in chat to join\nTime: %i\"", g_entities[client->pers.jkmodPers.privateRoom[PRIVATE_INDEX]].client->pers.netname, client->pers.jkmodPers.privateRoom[PRIVATE_INVITE]));
			client->pers.jkmodPers.privateRoom[PRIVATE_INVITE]--;
		} 
		else if (client->pers.jkmodPers.privateRoom[PRIVATE_TIME] < level.time) 
		{
			trap_SendServerCommand(ent - g_entities, "print \"Private room invitation expired\n\"");

			client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = 0;
			client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = 0;
			client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = 0;
		}
	}

	// Private check for bots leave
	if (client->pers.jkmodPers.privateRoom[PRIVATE_NUM] && (ent->r.svFlags & SVF_BOT))
	{
		if (!JKMod_PlayersPrivate(client->pers.jkmodPers.privateRoom[PRIVATE_NUM], qtrue))
		{
			int inactivityTime = jkcvar_privateInactiveBots.integer >= MIN_PRIVATE_TIME ? jkcvar_privateInactiveBots.integer : MIN_PRIVATE_TIME;
			if (client->pers.jkmodBots.inactivityTime == 0) client->pers.jkmodBots.inactivityTime = level.time + (inactivityTime*1000);
			if (client->pers.jkmodBots.inactivityTime < level.time)
			{
				trap_EA_Say(ent->s.number, "!dimension leave");
				trap_EA_Say(ent->s.number, va("I left private rooms after %i seconds of inactivity", inactivityTime));
				client->pers.jkmodBots.inactivityTime = 0;
				client->pers.jkmodBots.actionFlags = 0;
			}
		}
		else if (client->pers.jkmodBots.inactivityTime != 0) 
		{
			client->pers.jkmodBots.inactivityTime = 0;
		}
	}

	// Chat protect check
	if (jkcvar_chatProtect.integer && (client->ps.eFlags & EF_TALK) && !(g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY))
	{
		if (client->jkmodClient.chatTime >= jkcvar_chatProtectTime.integer)
		{
			client->jkmodClient.chatTime = jkcvar_chatProtectTime.integer;

			if (!(client->ps.stats[JK_PLAYER] & JK_CHAT_IN)) client->ps.stats[JK_PLAYER] |= JK_CHAT_IN;
			if (!(client->ps.eFlags & JK_PASS_THROUGH) && jkcvar_chatProtect.integer == 3) client->ps.eFlags |= JK_PASS_THROUGH;
			if (!client->pers.jkmodPers.invulnerability) client->pers.jkmodPers.invulnerability = qtrue;
			if (!client->ps.saberHolstered && !JKMod_EmoteIn(ent, -1)) Cmd_ToggleSaber_f(ent);
		}
		else
		{
			client->jkmodClient.chatTime++;
		}
	}

	// Show server motd
	if (client->jkmodClient.motdTime)
	{
		if (client->jkmodClient.motdTime <= jkcvar_serverMotdTime.integer)
		{
			char serverMotd[MAX_STRING_CHARS];
			JKMod_StringEscape(jkcvar_serverMotd.string, serverMotd, MAX_STRING_CHARS);

			// Check map teleports
			if (jkcvar_serverMotdTele.integer && jkcvar_teleportChat.integer)
			{
				int			i = 0;
				char		command[MAX_TOKEN_CHARS];
				char		map[MAX_TOKEN_CHARS];
				char		message[MAX_STRING_CHARS] = { 0 };
				qboolean	found = qfalse;

				Q_strcat(message, sizeof(message), va("\n^7Teleports: %s", jkcvar_serverMotdTele.integer == 2 ? "\n" : ""));

				for (i = 0; i < level.jkmodLocals.teleportChatsCount; i++)
				{
					strcpy(command, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "command"));
					strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));

					if (Q_stricmp(map, jkcvar_mapName.string) == 0)
					{
						Q_strcat(message, sizeof(message), va("^3%s%s", command, jkcvar_serverMotdTele.integer == 2 ? "\n" : ", "));
						found = qtrue;
					}
				}

				if (found) 
				{
					message[strlen(message) - (jkcvar_serverMotdTele.integer == 2 ? 1 : 2)] = '\0';
					Q_strcat(serverMotd, sizeof(serverMotd), message);
				}
			}

			if (jkcvar_serverMotdTimePrint.integer) {
				G_CenterPrint(client->ps.clientNum, 3, va("%s\nTime: %d\n", serverMotd, client->jkmodClient.motdTime));
			} else {
				G_CenterPrint(client->ps.clientNum, 3, va("%s\n", serverMotd));
			}
		}

		client->jkmodClient.motdTime--;
	}

	// Check jetpack fuel
	if (jkcvar_jetPack.integer != 2 && (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) && !(ent->client->ps.stats[JK_DIMENSION] & DIMENSION_CHEAT))
	{
		int amount = 5;

		if (ent->client->ps.eFlags & JK_JETPACK_FLAMING)
		{
			if (!ent->client->pers.jkmodPers.clientPlugin) {
				// Show fuel
				trap_SendServerCommand(client->ps.clientNum, va("cp \"" NEWLINES "Fuel: ^3%i", ent->client->ps.stats[JK_FUEL]));
			}

			if (ent->client->ps.stats[JK_FUEL] > 0) {
				// Reduce
				ent->client->ps.stats[JK_FUEL] -= amount;
			} else {
				// Turn off
				ent->client->ps.eFlags &= ~JK_JETPACK_FLAMING;
			}
		}
		else if (ent->client->ps.stats[JK_FUEL] < 100) 
		{
			// Recharge
			ent->client->ps.stats[JK_FUEL] += amount;
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
	usercmd_t	*cmd;

	// Checks
	if (!ent || !ent->client) return;
	if (ent->client->pers.connected != CON_CONNECTED) return;
	
	cmd = &ent->client->pers.cmd;

	// Already in an emote
	if (JKMod_EmoteIn(ent, -1))
	{
		if (ent->client->ps.pm_type == PM_DEAD)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKMod_EmoteIn(ent, 0))
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
		else if (JKMod_EmoteIn(ent, 1) && ent->client->pers.cmd.upmove > 0)
		{	
			// In an animation emote
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		else if (JKMod_EmoteIn(ent, 2))
		{	
			// In a special emote (walkable)
			ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
			ent->client->ps.forceDodgeAnim = (ent->client->ps.torsoAnim & ~ANIM_TOGGLEBIT);
			ent->client->ps.forceHandExtendTime = level.time + INFINITE;
		}

		// Keep the client informed about to predict the emote leg timers
		ent->client->pers.jkmodPers.emoteLastTime = level.time;
	}
	// Leaving an emote
	else if (ent->client->pers.jkmodPers.emoteLastTime && ent->client->pers.jkmodPers.emoteLastTime != level.time)
	{
		if (!JKMod_EmoteOut(ent))
		{
			if (JKMod_OthersInBox(ent)) JKMod_AntiStuckBox(ent);
			ent->client->ps.stats[JK_PLAYER] &= ~JK_EMOTE_IN;
			ent->client->pers.jkmodPers.emoteLastTime = 0;
		}
	}

	// Check player pass-through
	if (ent->client->ps.eFlags & JK_PASS_THROUGH)
	{
		if (ent->r.contents & CONTENTS_BODY) ent->r.contents &= ~CONTENTS_BODY;
		if (!ent->client->pers.jkmodPers.passThrough) ent->client->pers.jkmodPers.passThrough = qtrue;
	}
	else if (ent->client->pers.jkmodPers.passThrough) {
		if (!(ent->r.contents & CONTENTS_BODY)) ent->r.contents = CONTENTS_BODY;
		ent->client->pers.jkmodPers.passThrough = qfalse;
	}

	// Check player anti-stuck
	if ((ent->client->ps.stats[JK_PLAYER] & JK_ANTI_STUCK) && !(ent->client->ps.eFlags & JK_PASS_THROUGH))
	{
		if (JKMod_OthersInBox(ent)) {
			if (ent->r.contents & CONTENTS_BODY) {
				ent->r.contents &= ~CONTENTS_BODY;
				JKMod_AntiStuckBox(ent);
			}
		} else {
			if (!(ent->r.contents & CONTENTS_BODY)) ent->r.contents = CONTENTS_BODY;
			ent->client->ps.stats[JK_PLAYER] &= ~JK_ANTI_STUCK;
		}
	}

	// Check chat off
	if (ent->client->ps.stats[JK_PLAYER] & JK_CHAT_IN)
	{
		if (!(ent->client->ps.eFlags & EF_TALK))
		{
			if (!ent->client->pers.jkmodPers.passThroughPerm) 
			{
				if (ent->client->ps.groundEntityNum != ENTITYNUM_WORLD && !ent->client->jkmodClient.chairModelUsed) { ent->client->ps.origin[2] += 2; ent->client->ps.eFlags ^= EF_TELEPORT_BIT; } // Workaround
				if (ent->client->ps.eFlags & JK_PASS_THROUGH) ent->client->ps.eFlags &= ~JK_PASS_THROUGH;
				if (JKMod_OthersInBox(ent)) JKMod_AntiStuckBox(ent);
			}
			if (ent->client->jkmodClient.chatTime != 0) ent->client->jkmodClient.chatTime = 0;
			if (ent->client->pers.jkmodPers.invulnerability) ent->client->pers.jkmodPers.invulnerability = qfalse;

			ent->client->ps.stats[JK_PLAYER] &= ~JK_CHAT_IN;
		}
	}

	// Private check for bots control
	if ((ent->client->pers.jkmodBots.actionFlags & JK_BOT_CONTROL) && (ent->r.svFlags & SVF_BOT))
	{
		ent->client->pers.cmd = g_entities[ent->client->pers.jkmodPers.botControl[BOT_INDEX]].client->pers.cmd;
		ent->client->ps.fd.saberAnimLevel = g_entities[ent->client->pers.jkmodPers.botControl[BOT_INDEX]].client->ps.fd.saberAnimLevel;
		if (!(g_entities[ent->client->pers.jkmodPers.botControl[BOT_INDEX]].client->ps.stats[JK_PLAYER] & JK_EMOTE_IN)) {
			ent->client->ps.saberHolstered = g_entities[ent->client->pers.jkmodPers.botControl[BOT_INDEX]].client->ps.saberHolstered;
		}
	}

	// Temp model check
	if (ent->client->pers.jkmodPers.tempModelNum)
	{
		static gentity_t *model;
		static vec3_t angles;
		static vec3_t origin;
		static int alignment, rotation, angle, glm;
 
		alignment = ent->client->pers.jkmodPers.tempModelSettings[0];
		rotation = ent->client->pers.jkmodPers.tempModelSettings[1];
		angle = ent->client->pers.jkmodPers.tempModelSettings[2];
		glm = ent->client->pers.jkmodPers.tempModelSettings[3];
		model = &g_entities[ent->client->pers.jkmodPers.tempModelNum];

		VectorCopy(ent->client->ps.origin, origin);
		if (alignment == 2) { 
			origin[2] += (glm ? ent->r.maxs[2] + abs(DEFAULT_MINS_2) : ent->r.maxs[2]);
		} else if (alignment == 1) { 
			origin[2] += (glm ? ent->r.maxs[2]/2 : ent->r.maxs[2]/2 - DEFAULT_MAXS_2/2);
		} else if (!alignment && !glm) { 
			origin[2] += DEFAULT_MINS_2;
		}
		G_SetOrigin(model, origin);
		
		VectorCopy(ent->client->ps.viewangles, angles);
		if (!glm) angles[YAW] -= 90;
		if (angle) angles[YAW] += angle;
		if (angle) angles[ROLL] = 0.0f;
		if (rotation) angles[PITCH] = 0.0f;
		if (!rotation) angles[PITCH] = (angle && !glm) || (!angle && glm) ? ent->client->ps.viewangles[PITCH] : 0.0f;
		if (!rotation && !angle && !glm) angles[ROLL] -= ent->client->ps.viewangles[PITCH];
		if (!rotation && angle && glm) angles[ROLL] = ent->client->ps.viewangles[PITCH];
		G_SetAngles(model, angles);
	}

	// Check race dimension saber toogle
	if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE && !ent->client->ps.saberHolstered)
	{
		ent->client->ps.saberHolstered = qtrue;
		ent->client->ps.weaponTime = 400;
	}

	// Check jetpack flaming
	if ((ent->client->ps.eFlags & JK_JETPACK_ACTIVE) && ent->client->ps.pm_type == PM_DEAD)
	{
		ent->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
		ent->client->ps.stats[JK_FUEL] = 0;
		ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
	}

	// Check jetpack effect
	if ((ent->client->ps.eFlags & JK_JETPACK_ACTIVE) && ent->client->pers.jkmodPers.jetpackFxDelay <= level.time && !ent->client->pers.jkmodPers.ghostPlayer)
	{
		static vec3_t temporigin, tempangles;

		VectorCopy(ent->client->ps.origin, temporigin);
		tempangles[PITCH] = -90;

		if (ent->client->ps.eFlags & JK_JETPACK_FLAMING) 
		{
			ent->client->pers.jkmodPers.jetpackFxDisplay = qtrue;
			JKMod_G_PlayEffect_ID(level.jkmodLocals.jetpackFxActive, temporigin, tempangles, ent->s.number, qtrue);
		} 
		else if (ent->client->pers.jkmodPers.jetpackFxDisplay && ent->client->ps.groundEntityNum != ENTITYNUM_NONE)
		{
			temporigin[2] -= 20;
			JKMod_G_PlayEffect_ID(level.jkmodLocals.jetpackFxIdle, temporigin, tempangles, ent->s.number, qtrue);
		}

		ent->client->pers.jkmodPers.jetpackFxDelay = level.time + 150;
	}

	// Check invulnerability
	if (ent->client->pers.jkmodPers.invulnerability)
	{
		if (ent->takedamage) ent->takedamage = qfalse;
	}
	else
	{
		if (!ent->takedamage && ent->health > 0) ent->takedamage = qtrue;
	}

	// Button use trigger animation
	if ((cmd->buttons & BUTTON_USE) 
		&& (ent->client->ps.stats[JK_TWEAKS] & JK_USE_STAND) 
		&& (ent->client->pers.jkmodPers.buttonUseAnimValid)
		&& !(ent->client->ps.eFlags & JK_JETPACK_FLAMING) 
		&& !((ent->client->ps.eFlags & JK_JETPACK_ACTIVE) && ent->client->ps.groundEntityNum == ENTITYNUM_NONE) 
		&& !JKMod_PlayerMoving(ent, qfalse, qtrue))
	{
		ent->client->pers.jkmodPers.buttonUseAnim = qtrue;
		ent->client->ps.saberMove = LS_NONE;
		ent->client->ps.saberBlocked = 0;
		ent->client->ps.saberBlocking = 0;
		ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
		ent->client->ps.forceDodgeAnim = BOTH_BUTTON_HOLD;
		ent->client->ps.forceHandExtendTime = level.time + INFINITE;
	}
	else if (ent->client->pers.jkmodPers.buttonUseAnim)
	{
		ent->client->pers.jkmodPers.buttonUseAnim = qfalse;
		ent->client->pers.jkmodPers.buttonUseAnimValid = qfalse;
		ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
		ent->client->ps.forceDodgeAnim = 0;
		ent->client->ps.forceHandExtendTime = 0;
	}

	// Fullbody push effect
	if (ent->client->jkmodClient.pushEffectTime > level.time)
	{
		ent->client->ps.eFlags |= JK_BODY_PUSH;
	}
	else if (ent->client->jkmodClient.pushEffectTime)
	{
		ent->client->jkmodClient.pushEffectTime = 0;
		ent->client->ps.eFlags &= ~JK_BODY_PUSH;
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
	qboolean GTconfigLoaded = jkcvar_gameTypeConfig.integer && level.newSession ? level.jkmodLocals.cvarTempUnlock == 2 : qtrue;

	if (jkcvar_antiWarp.integer)
	{
		gclient_t	*client = ent->client;
		usercmd_t	*cmd = &client->pers.cmd;

		if ((ent->r.svFlags & SVF_BOT) || g_synchronousClients.integer)
		{
			// Call ClientBegin for delayed bots now
			if (ent->client->pers.botDelayed && GTconfigLoaded)
			{ 
				ClientBegin(ent - g_entities, qtrue);
				ent->client->pers.botDelayed = qfalse;
			}

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
		if (!(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer) 
		{
			return;
		}

		if (ent->client->pers.botDelayed && GTconfigLoaded) // Call ClientBegin for delayed bots now
		{
			ClientBegin(ent-g_entities, qtrue);
			ent->client->pers.botDelayed = qfalse;
		}

		ent->client->pers.cmd.serverTime = level.time;
		ClientThink_real(ent);
	}
}

/*
=====================================================================
Pause client think function
=====================================================================
*/
void JKMod_PauseClientThink(gentity_t *ent) 
{
	gclient_t	*client = ent->client;
	usercmd_t	*cmd = &client->pers.cmd;

	// Stop command time
	client->ps.commandTime = cmd->serverTime;

	// Check chat flag
	if ( cmd->buttons & BUTTON_TALK ) {
		ent->s.eFlags |= EF_TALK;
		client->ps.eFlags |= EF_TALK;
	} else {
		ent->s.eFlags &= ~EF_TALK;
		client->ps.eFlags &= ~EF_TALK;
	}

	// Stop movement and prediction
	client->ps.pm_type = PM_SPINTERMISSION;

	// Force view angles
	SetClientViewAngle(ent, client->ps.viewangles);
}
