/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Drop flag function
=====================================================================
*/
static void JKPlus_dropFlag(gentity_t *ent, int clientNum)
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
		trap_SendServerCommand(ent - g_entities, "cp \"You doesn't have any flag to drop\n\"");
		return;
	}

	if (ent->client->JKPlusDropFlagTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to take the flag again\n\"", ent->client->JKPlusDropFlagTime));
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

	ent->client->JKPlusDropFlagTime = jkcvar_dropFlagTime.integer;
	LaunchItem(item, org, velocity);
}

/*
=====================================================================
Instructs all chat to be ignored by the given
=====================================================================
*/
void JKPlus_IgnoreClient(char *option, int ignorer, int ignored, qboolean ignore)
{
	if (ignorer == ignored)
	{
		return;
	}
	if (g_entities[ignored].client->pers.connected != CON_CONNECTED)
	{
		return;
	}
	if (ignore)
	{
		if (!Q_stricmp(option, "chat")) {
			g_entities[ignored].client->sess.JKPlusIgnoredChats[ignorer / 32] |= (1 << (ignorer % 32));
		}
		else if (!Q_stricmp(option, "duel")) {
			g_entities[ignored].client->sess.JKPlusIgnoredDuels[ignorer / 32] |= (1 << (ignorer % 32));
		}
	}
	else
	{
		if (!Q_stricmp(option, "chat")) {
			g_entities[ignored].client->sess.JKPlusIgnoredChats[ignorer / 32] &= ~(1 << (ignorer % 32));
		}
		else if (!Q_stricmp(option, "duel")) {
			g_entities[ignored].client->sess.JKPlusIgnoredDuels[ignorer / 32] &= ~(1 << (ignorer % 32));
		}
	}
}

/*
=====================================================================
Checks to see if the given client is being ignored by a specific client
=====================================================================
*/
qboolean JKPlus_IsClientIgnored(char *option, int ignorer, int ignored)
{
	if (!Q_stricmp(option, "chat")) {
		if (g_entities[ignored].client->sess.JKPlusIgnoredChats[ignorer / 32] & (1 << (ignorer % 32)))
		{
			return qtrue;
		}
	}
	else if (!Q_stricmp(option, "duel")) {
		if (g_entities[ignored].client->sess.JKPlusIgnoredDuels[ignorer / 32] & (1 << (ignorer % 32)))
		{
			return qtrue;
		}
	}
	return qfalse;
}

/*
=====================================================================
Clears any possible ignore flags that were set and not reset
=====================================================================
*/
void JKPlus_RemoveFromAllIgnoreLists(char *option, int ignorer)
{
	int i;

	for (i = 0; i < level.maxclients; i++) {
		if (!Q_stricmp(option, "chat")) {
			g_entities[i].client->sess.JKPlusIgnoredChats[ignorer / 32] &= ~(1 << (ignorer % 32));
		}
		else if (!Q_stricmp(option, "duel")) {
			g_entities[i].client->sess.JKPlusIgnoredDuels[ignorer / 32] &= ~(1 << (ignorer % 32));
		}
	}
}

/*
=====================================================================
Custom callvote function
=====================================================================
*/
void JKPlus_CallVote(gentity_t *ent) 
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
	if (ent->client->JKPlusVoteWaitTime > 0) {
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to call a new vote again\n\"", ent->client->JKPlusVoteWaitTime));
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
				Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Poll: %s", JKPlus_ConcatArgs(2));
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
	ent->client->JKPlusVoteWaitTime = jkcvar_voteWaitTime.integer;

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
extern int saberOffSound;
extern int saberOnSound;

void JKPlus_EngageDuel(gentity_t *ent, int type)
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

		// Tr!Force: [Ignore] Apply duel ignore
		if (JKPlus_IsClientIgnored("duel", challenged->s.number, ent->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"This player doesn't want to be challenged\n\"");
			return;
		}
		if (JKPlus_IsClientIgnored("duel", ent->s.number, challenged->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"You have ignored this player challenges\n\"");
			return;
		}

		if (challenged->client->ps.duelIndex == ent->s.number && challenged->client->ps.duelTime >= level.time)
		{
			trap_SendServerCommand( /*challenged-g_entities*/-1, va("print \"%s" S_COLOR_WHITE " %s %s" S_COLOR_WHITE "!\n\"", challenged->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELACCEPT"), ent->client->pers.netname));

			ent->client->ps.duelInProgress = qtrue;
			challenged->client->ps.duelInProgress = qtrue;

			ent->client->ps.duelTime = level.time + 2000;
			challenged->client->ps.duelTime = level.time + 2000;

			G_AddEvent(ent, EV_PRIVATE_DUEL, 1);
			G_AddEvent(challenged, EV_PRIVATE_DUEL, 1);

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
					
					ent->client->pers.JKPlusCustomDuel = type;
					challenged->client->pers.JKPlusCustomDuel = type;
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
Client command function
=====================================================================
*/
void JKPlus_ClientCommand(int clientNum)
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
				"^7This server is running ^5%s ^7(Version: ^2%s.%s.%s^7 - Server Time: %02i:%02i%s)\n"
				"^7You can read the desired help topic using following command: ^2/help <topic>\n"
				"^5----------\n"
				"^7Topic list:\n"
				"^3Admin          Accounts       Ranking\n"
				"^3Emotes         Commands       Duels\n"
				"^3Build          About          Credits\n"
				"^7\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, serverTime.tm_hour, serverTime.tm_min, serverTimeType));
				return;
		}
		if (!Q_stricmp(arg1, "commands"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Command ^5]^7\n"
				"^7You can perform many console commands to do an action\n"
				"^7Some of this commands are restricted to be used with the ^2client plugin\n"
				"^5----------\n"
				"^7Command list:\n"
				"^3emote\n"
				"^3ignore\n"
				"^3dropflag\n"
				"^3callvote\n"
				"^3motd\n"
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
		else 
		{
			if (!JKPlus_emoteCheck(arg1, ent))
			{
				trap_SendServerCommand(ent - g_entities, va("print \"Invalid emote for ^3%s\n\"", arg1));
				return;
			}
		}
	}
	// Drop flag command
	else if (Q_stricmp(cmd, "dropflag") == 0)
	{
		JKPlus_dropFlag(ent, clientNum);
	}
	else if (Q_stricmp(cmd, "motd") == 0)
	{
		if (*jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
		{
			ent->client->JKPlusMotdTime = jkcvar_serverMotdTime.integer;
		}
	}
	// Ignore command
	else if (Q_stricmp(cmd, "ignore") == 0)
	{
		qboolean ignore;
		int		ignored = -1;
		char	*option;
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

		if (trap_Argc() < 3)
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Ignore ^5]^7\n"
				"^7Ignore a player chat or duel challenge\n"
				"^7You can use this feature using the following command: ^2/ignore <option> <player>\n"
				"^5----------\n"
				"^7Option list:\n"
				"^3chat\n"
				"^3duel\n"
				"^5----------\n"
				"^2Note 1: ^7No need to use full name or color name, you can use just a part of it\n"
				"^2Note 2: ^7Use this command again to undo the changes\n"
				"^7\""));
			return;
		}
		else 
		{
			if (!(!Q_stricmp(arg1, "chat") || !Q_stricmp(arg1, "duel")))
			{
				trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is not valid\n\"", arg1));
				return;
			}
			else 
			{
				ignored = JKPlus_ClientNumberFromArg(arg2);

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
				else if (!g_entities[ignored].inuse)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"The user ^3%s ^7is not active\n\"", arg2));
					return;
				}
				else
				{
					ignore = JKPlus_IsClientIgnored(arg1, ent->client->ps.clientNum, ignored) ? qfalse : qtrue;

					JKPlus_IgnoreClient(arg1, ent->client->ps.clientNum, ignored, ignore);

					if (ignore)
					{
						trap_SendServerCommand(ent - g_entities, va("print \"You are ignoring ^3%s ^7%ss now\n\"", g_entities[ignored].client->pers.netname, arg1));
					}
					else
					{
						trap_SendServerCommand(ent - g_entities, va("print \"You are no longer ignoring %ss from ^3%s\n\"", arg1, g_entities[ignored].client->pers.netname));
					}
				}
			}
		}
	}
	// Engage duel force command
	else if (Q_stricmp(cmd, "engage_duel_force") == 0)
	{
		if (!jkcvar_allowCustomDuel.integer)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"^3Force duel ^7is disabled by the server\n\""));
			JKPlus_EngageDuel(ent, 0);
		}
		else
		{
			JKPlus_EngageDuel(ent, 1);
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

		trap_SendServerCommand(ent - g_entities, va("print \"Cvar1: %s Cvar2: %i Arg1: %s Arg2: %i\n\"", jkcvar_test1.string, jkcvar_test2.integer, arg1, atoi(arg2)));
	}
	// Launch original client command function
	else
	{
		BaseJK2_ClientCommand(clientNum, cmd);
	}
}