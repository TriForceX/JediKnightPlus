/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern int saberOffSound;
extern int saberOnSound;
extern char *ConcatArgs(int start);
extern void G_Say(gentity_t *ent, gentity_t *target, int mode, const char *chatText);

/*
=====================================================================
Drop flag function
=====================================================================
*/
static void JKMod_dropFlag(gentity_t *ent)
{
	gitem_t		*item = ent->client->sess.sessionTeam == TEAM_RED ? BG_FindItem("team_CTF_blueflag") : BG_FindItem("team_CTF_redflag");
	vec3_t		angles, velocity, org, offset, mins, maxs;
	trace_t		tr;

	if (jkcvar_dropFlag.integer != 1)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Drop flag is disabled by the server\n\"");
		return;
	}

	if (!(g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You can't drop a flag in this game type\n\"");
		return;
	}

	if ((ent->client->sess.sessionTeam == TEAM_RED && ent->client->ps.powerups[PW_BLUEFLAG] == 0) || 
		(ent->client->sess.sessionTeam == TEAM_BLUE && ent->client->ps.powerups[PW_REDFLAG] == 0))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You don't have any flag to drop\n\"");
		return;
	}

	if (ent->client->jkmodClient.DropFlagTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to take the flag again\n\"", ent->client->jkmodClient.DropFlagTime));
		return;
	}
	
	ent->client->ps.powerups[PW_REDFLAG] = 0;
	ent->client->ps.powerups[PW_BLUEFLAG] = 0;
	ent->client->ps.persistant[PERS_SCORE] -= CTF_FLAG_BONUS;

	VectorCopy(ent->client->ps.viewangles, angles);

	angles[PITCH] = 0;

	AngleVectors(angles, velocity, NULL, NULL);
	VectorScale(velocity, 64, offset);
	offset[2] += ent->client->ps.viewheight / 2.f;
	VectorScale(velocity, 75, velocity);
	velocity[2] += 50 + random() * 35;

	VectorAdd(ent->client->ps.origin, offset, org);

	VectorSet(mins, -ITEM_RADIUS, -ITEM_RADIUS, 0);
	VectorSet(maxs, ITEM_RADIUS, ITEM_RADIUS, 2 * ITEM_RADIUS);

	trap_Trace(&tr, ent->client->ps.origin, mins, maxs, org, ent->s.number, MASK_SOLID);
	VectorCopy(tr.endpos, org);

	ent->client->jkmodClient.DropFlagTime = jkcvar_dropFlagTime.integer;
	LaunchItem(item, org, velocity);
}

/*
=====================================================================
Motd function
=====================================================================
*/
static void JKMod_showMotd(gentity_t *ent)
{
	if (*jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
	{
		ent->client->jkmodClient.MotdTime = jkcvar_serverMotdTime.integer;
	}
}

/*
=====================================================================
Save position for teleport
=====================================================================
*/
static qboolean JKMod_savePosition(gentity_t *ent)
{
	if (jkcvar_teleportChat.integer != 2)
	{
		trap_SendServerCommand(ent - g_entities, va("cp \"This teleport is disabled by the server\n\""));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport in spectator\n\""));
		return qfalse;
	}
	else
	{
		ent->client->ps.viewangles[0] = 0.0f;
		ent->client->ps.viewangles[2] = 0.0f;
		ent->client->pers.jkmodPers.TeleportChatOrigin[0] = ent->client->ps.origin[0];
		ent->client->pers.jkmodPers.TeleportChatOrigin[1] = ent->client->ps.origin[1];
		ent->client->pers.jkmodPers.TeleportChatOrigin[2] = ent->client->ps.origin[2];
		ent->client->pers.jkmodPers.TeleportChatOrigin[3] = ent->client->ps.viewangles[1];

		trap_SendServerCommand(ent - g_entities, va("cp \"Saved position!\n\""));

		if (ent->client->pers.jkmodPers.TeleportChatSaved != "true") {
			ent->client->pers.jkmodPers.TeleportChatSaved = "true"; 
			return qtrue;
		}
		else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Load position for teleport
=====================================================================
*/
static qboolean JKMod_loadPosition(gentity_t *ent)
{
	if (jkcvar_teleportChat.integer != 2)
	{
		trap_SendServerCommand(ent - g_entities, va("cp \"This teleport is disabled by the server\n\""));
		return qfalse;
	}
	else if (!ent->client->pers.jkmodPers.TeleportChatSaved)
	{
		trap_SendServerCommand(ent - g_entities, va("cp \"You don't have any saved position\n\""));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport in spectator\n\""));
		return qfalse;
	}
	else
	{
		vec3_t temporigin, tempangles;

		temporigin[0] = ent->client->pers.jkmodPers.TeleportChatOrigin[0];
		temporigin[1] = ent->client->pers.jkmodPers.TeleportChatOrigin[1];
		temporigin[2] = ent->client->pers.jkmodPers.TeleportChatOrigin[2];
		tempangles[PITCH] = 0.0f;
		tempangles[YAW] = ent->client->pers.jkmodPers.TeleportChatOrigin[3];
		tempangles[ROLL] = 0.0f;

		JKMod_TeleportPlayer(ent, temporigin, tempangles, qfalse, 0, "thermal/shockwave", "sound/interface/secret_area");

		if (ent->client->pers.jkmodPers.TeleportChatSaved == "true") {
			ent->client->pers.jkmodPers.TeleportChatSaved = va("%i %i %i", temporigin[0], temporigin[1], temporigin[2]);
			return qtrue;
		}
		else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Custom teleport say commands
=====================================================================
*/
static qboolean JKMod_teleportChat(gentity_t *ent, char *text)
{
	int			i = 0;
	char		command[MAX_TOKEN_CHARS];
	char		map[MAX_TOKEN_CHARS];
	char		origin[MAX_TOKEN_CHARS];
	char		rotation[MAX_TOKEN_CHARS];
	int			realrotation;
	vec3_t		realorigin;
	vmCvar_t	currentmap;

	trap_Cvar_Register(&currentmap, "mapname", "", CVAR_SERVERINFO | CVAR_ROM);

	for (i = 0; i < level.jkmodLevel.TeleportChatsCount; i++)
	{
		strcpy(command, Info_ValueForKey(level.jkmodLevel.TeleportChats[i], "command"));
		strcpy(map, Info_ValueForKey(level.jkmodLevel.TeleportChats[i], "map"));
		strcpy(origin, Info_ValueForKey(level.jkmodLevel.TeleportChats[i], "origin"));
		strcpy(rotation, Info_ValueForKey(level.jkmodLevel.TeleportChats[i], "rotation"));

		sscanf(origin, "%f %f %f %i", &realorigin[0], &realorigin[1], &realorigin[2], &realrotation);

		if (Q_stricmp(text, command) == 0)
		{
			if (Q_stricmp(map, currentmap.string) == 0)
			{
				if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport in spectator\n\""));
					return qfalse;
				}
				else if (JKMod_PlayerMoving(ent, qtrue, qtrue))
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport while moving\n\""));
					return qfalse;
				}
				else if (!ent->client->jkmodClient.TeleportChatUsed)
				{
					vec3_t		temporigin, tempangles;

					VectorClear(temporigin);
					VectorClear(tempangles);

					temporigin[0] = realorigin[0];
					temporigin[1] = realorigin[1];
					temporigin[2] = realorigin[2];
					tempangles[PITCH] = 0.0f;
					tempangles[YAW] = realrotation;
					tempangles[ROLL] = 0.0f;

					JKMod_TeleportPlayer(ent, temporigin, tempangles, qtrue, 300, "cinematics/hugesparks", NULL);
					ent->client->jkmodClient.TeleportChatUsed = qtrue;
					return qtrue;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You need to respawn to teleport again\n\""));
					return qfalse;
				}
			}
			else
			{
				trap_SendServerCommand(ent - g_entities, va("cp \"Teleport not available for this map\n\""));
				return qfalse;
			}
		}
	}
	return qtrue;
}

/*
=====================================================================
Instructs all chat & duels to be ignored by the given
=====================================================================
*/
void JKMod_IgnoreClient(int option, int ignorer, int ignored, qboolean ignore)
{
	if (ignore)
	{
		g_entities[ignorer].client->sess.jkmodSess.IgnoredPlayer[option] |= (1 << ignored);
	}
	else
	{
		g_entities[ignorer].client->sess.jkmodSess.IgnoredPlayer[option] &= ~(1 << ignored);
	}
}

/*
=====================================================================
Checks if the given client is being ignored by a specific client
=====================================================================
*/
qboolean JKMod_IsClientIgnored(int option, int ignorer, int ignored)
{
	if (g_entities[ignorer].client->sess.jkmodSess.IgnoredPlayer[option] & (1 << ignored))
	{
		return qtrue;
	}
	return qfalse;
}

/*
=====================================================================
Custom callvote function
=====================================================================
*/
void JKMod_CallVote(gentity_t *ent) 
{
	int		i;
	char	arg1[MAX_STRING_TOKENS];
	char	arg2[MAX_STRING_TOKENS];
	const	char *gameNames[] = {
		"Free For All",
		"Holocron FFA",
		"Jedi Master",
		"Duel",
		"Single Player",
		"Team FFA",
		"N/A",
		"Capture the Flag",
		"Capture the Ysalamiri"
	};

	if (!g_allowVote.integer) {
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NOVOTE")));
		return;
	}

	if (level.voteTime) {
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "VOTEINPROGRESS")));
		return;
	}
	if (ent->client->jkmodClient.VoteWaitTime > 0) {
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to call a new vote again\n\"", ent->client->jkmodClient.VoteWaitTime));
		return;
	}
	if (ent->client->pers.voteCount >= MAX_VOTE_COUNT) {
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "MAXVOTES")));
		return;
	}
	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR) {
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NOSPECVOTE")));
		return;
	}
	if (level.numVotingClients < jkcvar_voteMinPlayers.integer) {
		trap_SendServerCommand(ent - g_entities, va("print \"You need atleast %i players to call a vote\n\"", jkcvar_voteMinPlayers.integer));
		return;
	}

	// make sure it is a valid command to vote on
	trap_Argv(1, arg1, sizeof(arg1));
	trap_Argv(2, arg2, sizeof(arg2));

	if( strchr( arg1, ';' ) || strchr( arg2, ';' ) || strchr( arg1, '\n' ) || strchr( arg2, '\r' ) ) {
		trap_SendServerCommand(ent - g_entities, "print \"Invalid vote string\n\"");
		return;
	}

	if ( !Q_stricmp( arg1, "map_restart" ) ) {
	} else if ( !Q_stricmp( arg1, "nextmap" ) ) {
	} else if ( !Q_stricmp( arg1, "map" ) ) {
	} else if ( !Q_stricmp( arg1, "g_gametype" ) ) {
	} else if ( !Q_stricmp( arg1, "kick" ) ) {
	} else if ( !Q_stricmp( arg1, "clientkick" ) ) {
	} else if ( !Q_stricmp( arg1, "g_doWarmup" ) ) {
	} else if ( !Q_stricmp( arg1, "timelimit" ) ) {
	} else if ( !Q_stricmp( arg1, "fraglimit" ) ) {
	} else if ( !Q_stricmp( arg1, "gameplay" ) ) {
	} else if ( !Q_stricmp( arg1, "poll" ) ) {
	} else {
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Call Vote ^5]^7\n"
			"^7You can perform a call vote within the other players to change some server options\n"
			"^7You can do it using the following command: ^2/callvote <option> <value>\n"
			"^5----------\n"
			"^7Option list:\n"
			"^3map_restart\n"
			"^3nextmap\n"
			"^3map <mapname>\n"
			"^3g_gametype <number>\n"
			"^3kick <player name>\n"
			"^3clientkick <client number>\n"
			"^3g_doWarmup\n"
			"^3timelimit <time>\n"
			"^3fraglimit <frags>\n"
			"^3gameplay <version>\n"
			"^3poll <question>\n"
			"^5----------\n"
			"^2Note: ^7Some options may be disabled by the server\n"
			"^7\""));
		return;
	}

	// if there is still a vote to be executed
	if (level.voteExecuteTime) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand(EXEC_APPEND, va("%s\n", level.voteString));
	}

	// special case for g_gametype, check for bad values
	if (!Q_stricmp(arg1, "g_gametype"))
	{
		i = atoi(arg2);

		if (!(jkcvar_voteControl.integer & (1 << VOTE_G_GAMETYPE))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (i == GT_SINGLE_PLAYER || i < GT_FFA || i >= GT_MAX_GAME_TYPE) {
			trap_SendServerCommand(ent - g_entities, "print \"Invalid gametype\n\"");
			return;
		}

		level.votingGametype = qtrue;
		level.votingGametypeTo = i;

		Com_sprintf(level.voteString, sizeof(level.voteString), "%s %d", arg1, i);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s %s", arg1, gameNames[i]);
	}
	else if (!Q_stricmp(arg1, "map"))
	{
		// special case for map changes, we want to reset the nextmap setting
		// this allows a player to change maps, but not upset the map rotation
		char	s[MAX_STRING_CHARS];

		if (!(jkcvar_voteControl.integer & (1 << VOTE_MAP))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (!G_DoesMapSupportGametype(arg2, trap_Cvar_VariableIntegerValue("g_gametype")))
		{
			//trap_SendServerCommand( ent-g_entities, "print \"You can't vote for this map, it isn't supported by the current gametype.\n\"" );
			trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NOVOTE_MAPNOTSUPPORTEDBYGAME")));
			return;
		}

		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));
		if (*s) {
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s; set nextmap \"%s\"", arg1, arg2, s);
		}
		else {
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s", arg1, arg2);
		}
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s", level.voteString);
	}
	else if (!Q_stricmp(arg1, "clientkick"))
	{
		int n = atoi(arg2);

		if (!(jkcvar_voteControl.integer & (1 << VOTE_CLIENTKICK))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (n < 0 || n >= MAX_CLIENTS)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"invalid client number %d\n\"", n));
			return;
		}

		if (g_entities[n].client->pers.connected == CON_DISCONNECTED)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"there is no client with the client number %d\n\"", n));
			return;
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s", arg1, arg2);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "kick %s", g_entities[n].client->pers.netname);
	}
	else if (!Q_stricmp(arg1, "kick"))
	{
		int clientid = G_ClientNumberFromName(arg2);

		if (!(jkcvar_voteControl.integer & (1 << VOTE_KICK))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (clientid == -1)
		{
			clientid = G_ClientNumberFromStrippedName(arg2);

			if (clientid == -1)
			{
				trap_SendServerCommand(ent - g_entities, va("print \"there is no client named '%s' currently on the server\n\"", arg2));
				return;
			}
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "clientkick %d", clientid);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "kick %s", g_entities[clientid].client->pers.netname);
	}
	else if (!Q_stricmp(arg1, "nextmap"))
	{
		char	s[MAX_STRING_CHARS];

		if (!(jkcvar_voteControl.integer & (1 << VOTE_NEXTMAP))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));
		if (!*s) {
			trap_SendServerCommand(ent - g_entities, "print \"nextmap not set\n\"");
			return;
		}
		Com_sprintf(level.voteString, sizeof(level.voteString), "vstr nextmap");
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s", level.voteString);
	}
	else if (!Q_stricmp(arg1, "gameplay"))
	{
		if (!(jkcvar_voteControl.integer & (1 << VOTE_GAMEPLAY))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}
		else 
		{
			if (!(!Q_stricmp(arg2, "1.02") || !Q_stricmp(arg2, "1.03") || !Q_stricmp(arg2, "1.04"))) {
				trap_SendServerCommand(ent - g_entities, "print \"Invalid gameplay version, use: ^31.02^7, ^31.03^7 or ^31.04\n\"");
				return;
			}
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s \"%s\"", arg1, arg2);
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s", level.voteString);
		}
	}
	else if (!Q_stricmp(arg1, "poll"))
	{
		if (!(jkcvar_voteControl.integer & (1 << VOTE_POLL))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}
		else
		{
			if (arg2[0] == '\0') {
				trap_SendServerCommand(ent - g_entities, "print \"Usage: callvote poll <question>\n\"");
				return;
			}
			else {
				Com_sprintf(level.voteString, sizeof(level.voteString), "");
				Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Poll: %s", JKMod_ConcatArgs(2));
			}
		}
	}
	else
	{
		if ((!Q_stricmp(arg1, "g_dowarmup") && !(jkcvar_voteControl.integer & (1 << VOTE_G_DOWARMUP))) ||
			(!Q_stricmp(arg1, "timelimit") && !(jkcvar_voteControl.integer & (1 << VOTE_TIMELIMIT))) ||
			(!Q_stricmp(arg1, "map_restart") && !(jkcvar_voteControl.integer & (1 << VOTE_MAP_RESTART))) ||
			(!Q_stricmp(arg1, "fraglimit") && !(jkcvar_voteControl.integer & (1 << VOTE_FRAGLIMIT))))
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Voting not allowed for %s\n\"", arg1));
			return;
		}
		if (!Q_stricmp(arg1, "g_doWarmup") || !Q_stricmp(arg1, "timelimit") || !Q_stricmp(arg1, "fraglimit"))
		{
			if (strlen(arg2) >= MAX_CVAR_VALUE_STRING)
			{
				trap_SendServerCommand(ent - g_entities, "print \"The specified value is too long.\n");
				return;
			}
		}
		Com_sprintf(level.voteString, sizeof(level.voteString), "%s \"%s\"", arg1, arg2);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s", level.voteString);
	}

	trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLCALLEDVOTE")));

	// start the voting, the caller autoamtically votes yes
	level.voteTime = level.time;
	level.voteYes = 1;
	level.voteNo = 0;

	for (i = 0; i < level.maxclients; i++) {
		level.clients[i].ps.eFlags &= ~EF_VOTED;
	}
	ent->client->ps.eFlags |= EF_VOTED;

	// Call vote timer
	ent->client->jkmodClient.VoteWaitTime = jkcvar_voteWaitTime.integer;

	// Append white colorcode at the end of the display string as workaround for cgame leaking colors
	Q_strcat(level.voteDisplayString, sizeof(level.voteDisplayString), S_COLOR_WHITE);

	trap_SetConfigstring(CS_VOTE_TIME, va("%i", level.voteTime));
	trap_SetConfigstring(CS_VOTE_STRING, level.voteDisplayString);
	trap_SetConfigstring(CS_VOTE_YES, va("%i", level.voteYes));
	trap_SetConfigstring(CS_VOTE_NO, va("%i", level.voteNo));
}

/*
=====================================================================
Custom engage duel function
=====================================================================
*/
void JKMod_EngageDuel(gentity_t *ent, int type)
{
	trace_t tr;
	vec3_t forward, fwdOrg;

	if (!g_privateDuel.integer)
	{
		return;
	}

	if (g_gametype.integer == GT_TOURNAMENT)
	{ //rather pointless in this mode..
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NODUEL_GAMETYPE")));
		return;
	}

	if (g_gametype.integer >= GT_TEAM)
	{ //no private dueling in team modes
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NODUEL_GAMETYPE")));
		return;
	}

	if (ent->client->ps.duelTime >= level.time)
	{
		return;
	}

	if (ent->client->ps.weapon != WP_SABER)
	{
		return;
	}

	/*
	if (!ent->client->ps.saberHolstered)
	{ //must have saber holstered at the start of the duel
		return;
	}
	*/
	//NOTE: No longer doing this..

	if (ent->client->ps.saberInFlight)
	{
		return;
	}

	if (ent->client->ps.duelInProgress)
	{
		return;
	}

	// Tr!Force: [Duel] Allow multiple duels
	if (jkcvar_allowMultiDuel.integer != 1)
	{
		//New: Don't let a player duel if he just did and hasn't waited 10 seconds yet (note: If someone challenges him, his duel timer will reset so he can accept)
		if (ent->client->ps.fd.privateDuelTime > level.time)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "CANTDUEL_JUSTDID")));
			return;
		}

		if (G_OtherPlayersDueling())
		{
			trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "CANTDUEL_BUSY")));
			return;
		}
	}

	// Tr!Force: [Dimensions] Check duel challenge
	if (ent->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE)
	{
		return;
	}

	AngleVectors(ent->client->ps.viewangles, forward, NULL, NULL);

	fwdOrg[0] = ent->client->ps.origin[0] + forward[0] * 256;
	fwdOrg[1] = ent->client->ps.origin[1] + forward[1] * 256;
	fwdOrg[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + forward[2] * 256;

	trap_Trace(&tr, ent->client->ps.origin, NULL, NULL, fwdOrg, ent->s.number, MASK_PLAYERSOLID);

	if (tr.fraction != 1 && tr.entityNum < MAX_CLIENTS)
	{
		gentity_t *challenged = &g_entities[tr.entityNum];

		if (!challenged || !challenged->client || !challenged->inuse ||
			challenged->health < 1 || challenged->client->ps.stats[STAT_HEALTH] < 1 ||
			challenged->client->ps.weapon != WP_SABER || challenged->client->ps.duelInProgress ||
			challenged->client->ps.saberInFlight)
		{
			return;
		}

		if (g_gametype.integer >= GT_TEAM && OnSameTeam(ent, challenged))
		{
			return;
		}

		// Tr!Force: [Dimensions] Check duel challenge
		if (challenged->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE)
		{
			return;
		}

		// Tr!Force: [Ignore] Apply duel ignore
		if (JKMod_IsClientIgnored(1, challenged->s.number, ent->s.number) || challenged->client->sess.jkmodSess.IgnoredAll[1])
		{
			trap_SendServerCommand(ent - g_entities, "cp \"This player doesn't want to be challenged\n\"");
			return;
		}
		if (JKMod_IsClientIgnored(1, ent->s.number, challenged->s.number) || ent->client->sess.jkmodSess.IgnoredAll[1])
		{
			trap_SendServerCommand(ent - g_entities, "cp \"You have ignored this player challenges\n\"");
			return;
		}

		if (challenged->client->ps.duelIndex == ent->s.number && challenged->client->ps.duelTime >= level.time)
		{
			unsigned DIMENSION_DUEL_FREE = JKMod_DimensionGetFree();

			trap_SendServerCommand( /*challenged-g_entities*/-1, va("print \"%s" S_COLOR_WHITE " %s %s" S_COLOR_WHITE "!\n\"", challenged->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELACCEPT"), ent->client->pers.netname));

			ent->client->ps.duelInProgress = qtrue;
			challenged->client->ps.duelInProgress = qtrue;

			ent->client->ps.duelTime = level.time + 2000;
			challenged->client->ps.duelTime = level.time + 2000;

			G_AddEvent(ent, EV_PRIVATE_DUEL, 1);
			G_AddEvent(challenged, EV_PRIVATE_DUEL, 1);

			JKMod_DimensionSet(ent, DIMENSION_DUEL_FREE);
			JKMod_DimensionSet(challenged, DIMENSION_DUEL_FREE);

			if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;
			if (challenged->client->ps.eFlags & JK_JETPACK_ACTIVE) challenged->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;

			// Tr!Force: [Duel] Default start health and shield
			if (jkcvar_duelStartHealth.integer != 0 && jkcvar_duelStartArmor.integer != 0)
			{
				ent->client->ps.stats[STAT_HEALTH] = ent->health = jkcvar_duelStartHealth.integer;
				ent->client->ps.stats[STAT_ARMOR] = jkcvar_duelStartArmor.integer;

				challenged->client->ps.stats[STAT_HEALTH] = challenged->health = jkcvar_duelStartHealth.integer;
				challenged->client->ps.stats[STAT_ARMOR] = jkcvar_duelStartArmor.integer;
			}

			//Holster their sabers now, until the duel starts (then they'll get auto-turned on to look cool)

			if (!ent->client->ps.saberHolstered)
			{
				G_Sound(ent, CHAN_AUTO, saberOffSound);
				ent->client->ps.weaponTime = 400;
				ent->client->ps.saberHolstered = qtrue;
			}
			if (!challenged->client->ps.saberHolstered)
			{
				G_Sound(challenged, CHAN_AUTO, saberOffSound);
				challenged->client->ps.weaponTime = 400;
				challenged->client->ps.saberHolstered = qtrue;
			}
		}
		else
		{
			if (jkcvar_allowCustomDuel.integer) 
			{
				if (type == 1) 
				{
					// Print full force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (Full force)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE")));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (Full force)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname));
					
					ent->client->pers.jkmodPers.CustomDuel = type;
					challenged->client->pers.jkmodPers.CustomDuel = type;
				}
				else 
				{
					// Print full no-force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (No force)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE")));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (No force)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname));
				}
			}
			else 
			{
				//Print the message that a player has been challenged in private, only announce the actual duel initiation in private
				G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE")));
				G_CenterPrint(ent - g_entities, 3, va("%s %s\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname));
			}
		}

		challenged->client->ps.fd.privateDuelTime = 0; //reset the timer in case this player just got out of a duel. He should still be able to accept the challenge.

		ent->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
		ent->client->ps.forceHandExtendTime = level.time + 1000;

		ent->client->ps.duelIndex = challenged->s.number;
		ent->client->ps.duelTime = level.time + 5000;
	}
}

/*
=====================================================================
Custom say function
=====================================================================
*/
void JKMod_Say(gentity_t *ent, int mode, qboolean arg0) 
{ 
	char		*p;
	int			clientNum;

	// Get client number
	clientNum = ent - g_entities;

	if (trap_Argc() < 2 && !arg0) {
		return;
	}

	if (arg0)
	{
		p = ConcatArgs(0);
	}
	else
	{
		p = ConcatArgs(1);
	}

	// Show motd
	if (Q_stricmp(p, "!motd") == 0)
	{
		JKMod_showMotd(ent);
	}
	// Dimension trigger
	else if (Q_stricmpn(p, "!dimension", strlen("!dimension")) == 0)
	{
		char dimension[MAX_STRING_CHARS] = "";
		int i = 0, pos = 0, part = 0;

		for (i = 0; i < strlen(p); i++)
		{
			if (p[i] == ' ') part = 1;
			else if (part == 1) {
				dimension[pos] = p[i];
				pos++;
			}
		}

		if (!JKMod_DimensionCmd(ent, dimension)) return;
	}
	// Teleport chat (Save position)
	else if (Q_stricmp(p, "!savepos") == 0)
	{
		if (!JKMod_savePosition(ent)) return;
	}
	// Teleport chat (Load position)
	else if (Q_stricmp(p, "!loadpos") == 0)
	{
		if (!JKMod_loadPosition(ent)) return;
	}
	// Teleport chat (Load from file)
	else if (jkcvar_teleportChat.integer && level.jkmodLevel.TeleportChats[0] && (p[0] == '!') && p[1] && (p[1] != '!'))
	{
		if (!JKMod_teleportChat(ent, p)) return;
	}

	G_Say(ent, NULL, mode, p);
}

/*
=====================================================================
Client command function
=====================================================================
*/
void JKMod_ClientCommand(int clientNum)
{
	gentity_t *ent;
	char	cmd[MAX_TOKEN_CHARS];
	char token[BIG_INFO_STRING]; // As the engine uses Cmd_TokenizeString2 a single parameter is theoretically not limited by MAX_TOKEN_CHARS, but by BIG_INFO_STRING
	int i, argc;

	ent = g_entities + clientNum;
	if (!ent->client || ent->client->pers.connected < CON_CONNECTED) {
		return;		// not fully in game yet
	}

	// Filter '\n' and '\r'
	argc = trap_Argc();
	for (i = 0; i < argc; i++)
	{
		trap_Argv(i, token, sizeof(token));
		if (strchr(token, '\n') || strchr(token, '\r'))
		{
			trap_SendServerCommand(clientNum, "print \"Invalid input - command blocked.\n\"");
			G_Printf("ClientCommand: client '%i' (%s) tried to use an invalid command - command blocked.\n", clientNum, ent->client->pers.netname);
			return;
		}
	}

	trap_Argv(0, cmd, sizeof(cmd));

	// Help command
	if (Q_stricmp(cmd, "help") == 0)
	{
		char    arg1[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));

		if(trap_Argc() < 2)
		{
			qtime_t		serverTime;
			char		*serverTimeType;

			trap_RealTime(&serverTime);
			serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Help ^5]^7\n"
				"^7This server is running ^5%s ^7- Version: ^2%s.%s.%s\n"
				"^7The current server gameplay is ^21.0%i ^7and the server time is %02i:%02i%s\n"
				"^7You can read the desired help topic using following command: ^2/help <topic>\n"
				"^5----------\n"
				"^7Topic list:\n"
				"^3Admin          Accounts       Ranking\n"
				"^3Emotes         Commands       Duels\n"
				"^3Build          About          Credits\n"
				"^7\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, jk2gameplay, serverTime.tm_hour, serverTime.tm_min, serverTimeType));
			return;
		}
		else if (!Q_stricmp(arg1, "commands"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Command ^5]^7\n"
				"^7You can perform many console commands to do an action\n"
				"^7Some of this commands are restricted to be used with the ^2client plugin\n"
				"^5----------\n"
				"^7Command list:\n"
				"^3emote\n"
				"^3dimension\n"
				"^3ignore\n"
				"^3dropflag\n"
				"^3callvote\n"
				"^3motd\n"
				"^3whois\n"
				"^7\""));
			return;
		}
		else if(!Q_stricmp(arg1, "emotes"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Emotes ^5]^7\n"
				"^7Emotes are visual animations that allows you to sit down, greet someone, etc...\n"
				"^7There are over 50 emotes to choose from. (Some might be disabled by the server)\n"
				"^7You can play an emote animation by using the following command: ^2/emote <animation>\n"
				"^5----------\n"
				"^7Animation list:\n"
				"^3Bar            Beg            Buried        Cocky       ComeOn      ComTalk\n"
				"^3CrossArms      DontKillMe     DontKnow      DontKnow2   Explain     Explain2\n"
				"^3FakeDead       Flip           HandHips      Hi          Hug         Kiss\n"
				"^3Kneel          Laugh          Look          Look2       Nod         Point\n"
				"^3Point2         Punch          Sit           Sit2        Sit3        Shake\n"
				"^3Super          Super2         Surrender     Spin        Spin2       Spin3\n"
				"^3Spin4          Taunt2         Taunt3        Think       Threaten    ThumbsUp\n"
				"^3ThumbsDown     TossBack       TossOver      TossUp      Type        Type2\n"
				"^3Victory        Victory2       Waiting       WatchOut    Writing     Writing2\n"
				"^7\""));
			return;
		}
		else if (!Q_stricmp(arg1, "duels"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Duels ^5]^7\n"
				"^7Engage different duel challenge to another players\n"
				"^7You can assign to a key using the following command: ^2/bind engage_duel_<type>\n"
				"^5----------\n"
				"^7Command list:\n"
				"^3engage_duel\n"
				"^3engage_duel_force\n"
				"^5----------\n"
				"^2Note: ^7You also can use ^5engage_ff ^7for force duel challenge\n"
				"^7\""));
			return;
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is disabled at the moment\n\"", arg1));
			return;
		}
	}
	// Emote command
	else if (Q_stricmp(cmd, "emote") == 0)
	{
		char    arg1[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));

		if (trap_Argc() < 2)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Usage: emote <animation>\nSee ^3/help emotes ^7for more information\n\""));
			return;
		}
		else if (!JKMod_emoteCheck(arg1, ent))
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Invalid emote ^3%s\n\"", arg1));
			return;
		}
	}
	// Drop flag command
	else if (Q_stricmp(cmd, "dropflag") == 0)
	{
		JKMod_dropFlag(ent);
		return;
	}
	// Show motd command
	else if (Q_stricmp(cmd, "motd") == 0)
	{
		JKMod_showMotd(ent);
		return;
	}
	// Ignore command
	else if (Q_stricmp(cmd, "ignore") == 0)
	{
		qboolean ignore;
		int		ignored = -1;
		int		option;
		char	name[MAX_STRING_CHARS];
		char    arg1[MAX_TOKEN_CHARS];
		char    arg2[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		Com_sprintf(arg1, sizeof(arg1), "%s", arg1);
		Com_sprintf(arg2, sizeof(arg2), "%s", arg2);

		if (!jkcvar_playerIgnore.integer) 
		{
			trap_SendServerCommand(ent - g_entities, va("print \"This command is ^1disabled^7 by the server\n\""));
			return;
		}
		else if (trap_Argc() < 3)
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Ignore ^5]^7\n"
				"^7Ignore a player chat or duel challenge\n"
				"^7You can use this feature using the following command: ^2/ignore <option> <player|number|all>\n"
				"^5----------\n"
				"^7Option list:\n"
				"^3chat\n"
				"^3duel\n"
				"^5----------\n"
				"^2Note 1: ^7No need to use full name or color name, you can use just a part of it\n"
				"^2Note 2: ^7You can use the command ^3/whois ^7to check the player number\n"
				"^2Note 3: ^7Use this command again to undo the changes\n"
				"^7\""));
			return;
		}
		else if (!(!Q_stricmp(arg1, "chat") || !Q_stricmp(arg1, "duel")))
		{
			trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is not valid\n\"", arg1));
			return;
		}
		else 
		{
			option = !Q_stricmp(arg1, "chat") ? 0 : 1;

			if (!Q_stricmp(arg2, "all"))
			{
				ent->client->sess.jkmodSess.IgnoredAll[option] = ent->client->sess.jkmodSess.IgnoredAll[option] ? qfalse : qtrue;

				if (ent->client->sess.jkmodSess.IgnoredAll[option])
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You are ignoring all ^3%ss ^7now\n\"", arg1));
					return;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You are no longer ignoring ^3%ss\n\"", arg1));
					return;
				}
			}
			else
			{
				ignored = JKMod_ClientNumberFromArg(arg2);

				if (ignored == -1)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"Can't find the name ^3%s\n\"", arg2));
					return;
				}
				else if (ignored == -2)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"There are more names that contains ^3%s\n\"", arg2));
					return;
				}
				else if (ignored >= MAX_CLIENTS || ignored < 0)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"Invalid name for ^3%s\n\"", arg2));
					return;
				}
				else if (ignored == ent->client->ps.clientNum)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You can't do it to yourself\n\""));
					return;
				}
				else if (!g_entities[ignored].inuse || g_entities[ignored].client->pers.connected != CON_CONNECTED)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"The user ^3%s ^7is not active\n\"", arg2));
					return;
				}
				else
				{
					ignore = JKMod_IsClientIgnored(option, ent->client->ps.clientNum, ignored) ? qfalse : qtrue;

					JKMod_IgnoreClient(option, ent->client->ps.clientNum, ignored, ignore);

					if (ignore)
					{
						trap_SendServerCommand(ent - g_entities, va("print \"You are ignoring ^3%s ^7%ss now\n\"", g_entities[ignored].client->pers.netname, arg1));
						return;
					}
					else
					{
						trap_SendServerCommand(ent - g_entities, va("print \"You are no longer ignoring %ss from ^3%s\n\"", arg1, g_entities[ignored].client->pers.netname));
						return;
					}
				}
			}
		}
	}
	// Engage force duel command
	else if (Q_stricmp(cmd, "engage_duel_force") == 0 || Q_stricmp(cmd, "engage_ff") == 0)
	{
		if (!jkcvar_allowCustomDuel.integer)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"^3Force duel ^7is disabled by the server\n\""));
			JKMod_EngageDuel(ent, 0);
			return;
		}
		else
		{
			JKMod_EngageDuel(ent, 1);
			return;
		}
	}
	// Who is command
	else if (Q_stricmp(cmd, "whois") == 0)
	{
		char	status[MAX_STRING_CHARS] = "";
		int		i;

		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Who is ^5]^7\n"
			"^7List of all players connected on the server\n"
			"^7Client plugin status: ^2Updated^7, ^3Outdated^7, ^1No plugin\n"
			"^7\""));

		Q_strcat(status, sizeof(status), "^5--- ---------------------------- ----- --------- ---------------\n");
		Q_strcat(status, sizeof(status), "^7Num Name                         Type  Dimension Plugin\n");
		Q_strcat(status, sizeof(status), "^5--- ---------------------------- ----- --------- ---------------\n");

		for (i = 0; i < level.maxclients; i++)
		{
			gentity_t 	*ent = &g_entities[i];
			char		name[MAX_STRING_CHARS] = "";
			char		userinfo[MAX_INFO_STRING];
			char		*type;
			char		*dimension;
			char		*valid;
			char		*plugin;

			if (!ent || !ent->client || !ent->inuse) continue;
			if (ent->client->pers.connected == CON_DISCONNECTED) continue;

			strcpy(name, ent->client->pers.netname);
			trap_GetUserinfo(i, userinfo, sizeof(userinfo));

			if (ent->r.svFlags & SVF_BOT) {
				type = "Bot";
				plugin = JK_VERSION;
				valid = S_COLOR_GREEN;
			}
			else {
				type = "Human";
				plugin = Info_ValueForKey(userinfo, "jkmod_clientversion");
				valid = plugin[0] != '\0' ? (strcmp(plugin, JK_VERSION) == 0 ? S_COLOR_GREEN : S_COLOR_YELLOW) : S_COLOR_RED;
			}

			if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_DUEL) dimension = "Duel";
			else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_GUNS) dimension = "Guns";
			else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE) dimension = "Race";
			else dimension = "Normal";
			
			Q_strcat(status, sizeof(status), va(S_COLOR_WHITE "%3i %28s %5s %9s %s%9s\n",
				i,
				Q_CleanStr(name, qfalse),
				type,
				dimension,
				valid,
				plugin[0] == '\0' ? "No plugin" : plugin
			));
		}

		Q_strcat(status, sizeof(status), "^5--- ---------------------------- ----- --------- ---------------\n");
		trap_SendServerCommand(clientNum, va("print \"%s\"", status));
		return;
	}
	// Illegal macro announce
	else if (Q_stricmp(cmd, "macroused") == 0)
	{
		if (jkcvar_macroScan.integer)
		{
			G_Say(ent, NULL, SAY_ALL, va("^5Illegal script/bind detected^1! ^7(IP: ^3%s^7)", ent->client->sess.jkmodSess.ClientIP));

			if (jkcvar_macroScan.integer == 2)
			{
				SetTeam(ent, "spectator");
				return;
			}
			else if (jkcvar_macroScan.integer == 3)
			{
				trap_DropClient(clientNum, va("%s", G_GetStripEdString("SVINGAME", "WAS_KICKED")));
				return;
			}
		}
	}
	// Dimension change command
	else if (Q_stricmp(cmd, "dimension") == 0)
	{
		char    arg1[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));

		if (trap_Argc() < 2)
		{
			qtime_t		serverTime;
			char		*serverTimeType;

			trap_RealTime(&serverTime);
			serverTimeType = (serverTime.tm_hour > 11 && serverTime.tm_hour < 24) ? "pm" : "am";

			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Dimension ^5]^7\n"
				"^7Change between server dimensions\n"
				"^7You can use this feature using the following command: ^2/dimension <option>\n"
				"^5----------\n"
				"^7Option list:\n"
				"^3guns\n"
				"^3race\n"
				"^5----------\n"
				"^2Note: ^7You will join in ^5duel ^7dimension automatically if is available in the server\n"
				"^7\""));
			return;
		}
		else
		{
			JKMod_DimensionCmd(ent, arg1);
			return;
		}
	}
	// Save position command
	else if (Q_stricmp(cmd, "savepos") == 0)
	{
		JKMod_savePosition(ent);
		return;
	}
	// Load position command
	else if (Q_stricmp(cmd, "loadpos") == 0)
	{
		JKMod_loadPosition(ent);
		return;
	}
	// Jetpack command
	else if (Q_stricmp(cmd, "jetpack") == 0)
	{
		if (!jkcvar_jetPack.integer)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"This command is ^1disabled^7 by the server\n\""));
			return;
		}
		else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"You can't use jetpack in spectator\n\""));
			return;
		}
		else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"You can't use jetpack in this dimension\n\""));
			return;
		}
		else if (ent->client->ps.duelInProgress)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"You can't use jetpack in a private duel\n\""));
			return;
		}
		else
		{
			// Disable
			if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE)
			{
				ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;
				trap_SendServerCommand(ent - g_entities, va("print \"Jetpack disabled\n\""));
				return;
			}
			// Enable
			else
			{
				ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
				if (!ent->client->ps.stats[JK_FUEL]) ent->client->ps.stats[JK_FUEL] = 100;
				trap_SendServerCommand(ent - g_entities, va("print \"Jetpack equiped\n\""));
				trap_SendServerCommand(ent - g_entities, va("cp \"Press USE button on air to enable\""));
				return;
			}
		}
	}
	// Test command
	else if (Q_stricmp(cmd, "testcmd") == 0)
	{
		char    arg1[MAX_TOKEN_CHARS];
		char    arg2[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));
		trap_Argv(2, arg2, sizeof(arg2));

		Com_sprintf(arg1, sizeof(arg1), "%s", arg1);
		Com_sprintf(arg2, sizeof(arg2), "%s", arg2);

		if (arg1[0] == '\0') strcpy(arg1, "empty");

		trap_SendServerCommand(ent - g_entities, va("print \"Cvar1: %s Cvar2: %i Arg1: %s Arg2: %i\n\"", jkcvar_test1.string, jkcvar_test2.integer, arg1, atoi(arg2)));
		return;
	}
	// Launch original client command function
	else
	{
		BaseJK2_ClientCommand(clientNum);
	}
}
