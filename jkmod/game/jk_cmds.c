/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
=====================================================================
[Description]: Main commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

// Extern stuff
extern int saberOffSound;
extern int saberOnSound;
extern int trap_RealTime(qtime_t *qtime);
extern char *ConcatArgs(int start);
extern void G_Say(gentity_t *ent, gentity_t *target, int mode, const char *chatText);
extern jkmod_dimension_data_t JKModDimensionData[];
extern int JKModDimensionDataSize;
extern int G_ClientNumberFromName(const char* name);
extern int G_ClientNumberFromStrippedName(const char* name);
extern qboolean G_OtherPlayersDueling(void);
extern void G_TestLine(vec3_t start, vec3_t end, int color, int time);

/*
=====================================================================
Help command function
=====================================================================
*/
static void JKMod_Cmd_HelpInfo(gentity_t *ent)
{
	char    arg1[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	// Main help
	if(trap_Argc() < 2)
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Help ^5]^7\n"
			"^7This server is running ^5%s ^7- Version: ^2%s.%s.%s ^7(Build: %s)\n"
			"^7You can read the desired help topic using following command: ^2/help <topic>\n"
			"^5----------\n"
			"^7Topic list:\n"
			"^3Admin          Accounts       Dimensions\n"
			"^3Emotes         Commands       Duels\n"
			"^3Build          About          Credits\n"
			"^5----------\n"
			"^2Note: ^7For more information please visit ^5%s\n"
			"^7\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, JKMod_TrimWhiteSpace(__DATE__), JK_URL));
		return;
	}
	// Help admin
	else if (!Q_stricmp(arg1, "admin"))
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Admin ^5]^7\n"
			"^7You can perform some console commands if you are logged as admin or by remote console\n"
			"^7For ^5rcon ^7commands you have to use the following sintax: ^2/rcon <command>\n"
			"^5----------\n"
			"^7Rcon commands:\n"
			"^3gameplay\n"
			"^3pause\n"
			"^3remapshader\n"
			"^3changemusic\n"
			"^3togglemod\n"
			"^3forcedimension\n"
			"^3whois\n"
			"^3listdir\n"
			"^5----------\n"
			"^2Note: ^7To use ^5rcon ^7commands you must be logged with ^3/rconpassword <password>\n"
			"^7\""));
		return;
	}
	// Help commands
	else if (!Q_stricmp(arg1, "commands"))
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Command ^5]^7\n"
			"^7You can perform some console commands to perform a special action\n"
			"^7Chat commands are meant to be used on ^2chat ^7while typing\n"
			"^5----------\n"
			"^7Commands:      Chat commands:      Plugin commands:\n"
			"^3motd           !motd               strafehelper\n"
			"^3dimension      !dimension          speedometer\n"
			"^3dualsaber      !status\n"
			"^3emote          !savepos\n"
			"^3ignore         !loadpos\n"
			"^3dropflag       !savespawn\n"
			"^3callvote       !resetspawn\n"
			"^3whois          !racetime\n"
			"^3savepos        !teleports\n"
			"^3loadpos\n"
			"^3savespawn\n"
			"^3resetspawn\n"
			"^7\""));
		return;
	}
	// Help emotes
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
	// Help duels
	else if (!Q_stricmp(arg1, "duels"))
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Duels ^5]^7\n"
			"^7Choose between different dueling challenges actions\n"
			"^7Your private duels are %s ^7and auto duel accept is %s\n"
			"^5----------\n"
			"^7Command list:\n"
			"^3engage_duel\n"
			"^3engage_duel_force\n"
			"^3engage_private\n"
			"^3engage_auto\n"
			"^5----------\n"
			"^2Note 1: ^7Force duel will be ^1disabled ^7if the server doesn't allow force powers\n"
			"^2Note 2: ^7Private and auto duels will work only if the server ^2allows ^7them\n"
			"^7\"", 
			(ent->client->sess.jkmodSess.privateDuel ? "^2enabled" : "^1disabled"), 
			(ent->client->sess.jkmodSess.autoDuel ? "^2enabled" : "^1disabled")));
		return;
	}
	// Help dimensions
	else if (!Q_stricmp(arg1, "dimensions"))
	{
		int i;

		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Dimension ^5]^7\n"
			"^7Change between server dimensions\n"
			"^7You can use this feature using the following command: ^2/dimension <option>\n"
			"^5----------\n"
			"^7Option list:\n\""));

		for (i = 0; i < JKModDimensionDataSize; i++) {
			trap_SendServerCommand(ent - g_entities, va("print \"%s%s\n\"", (jkcvar_altDimension.integer & JKModDimensionData[i].dimension ? S_COLOR_YELLOW : S_COLOR_RED), JKModDimensionData[i].command));
		}
			
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5----------\n"
			"^2Note 1: ^7You can also use this command on chat saying ^5!dimension <option>\n"
			"^2Note 2: ^7Dimensions marked in ^3yellow ^7are available to use\n"
			"^7\""));
		return;
	}
	// Help about and credits
	else if (!Q_stricmp(arg1, "about") || !Q_stricmp(arg1, "credits"))
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 About ^5]^7\n"
			"^5%s ^7is a game modification for Jedi Knight II: Jedi Outcast Multiplayer. The main goal is\n"
			"^7to reunite useful stuff and add custom features to the game environment, specially making players ideas\n"
			"^7come true adding new features and porting some from other mods and games.\n\n"
			"^7I started to code the first version of this mod in 2005. Back then it was a private mod and only few\n"
			"^7people had access. So as a personal task and due to some people request i decided to re-code the mod\n"
			"^7into a public version, just as a legacy.\n"
			"^5----------\n"
			"^7Mod Author: ^2Tr!^4For^1ce\n"
			"^7Mod Website: ^5%s\n"
			"^7\"", JK_LONGNAME, JK_URL));

		if (!Q_stricmp(arg1, "credits"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5----------\n"
				"^7The following people supported and helped me directly on this project:\n"
				"^3DeathSpike       Lee Oates          Nerevar\n"
				"^3Kaiser/TVpath    Rich Whitehouse    NTxC/Reality\n"
				"^3Tox Laximus      McMonkey           JediDog~\n"
				"^3NewAge           Mr.Wonko           Xycaleth\n"
				"^3Daggolin         Ouned/Twitch       Kameleon\n"
				"^3Fau              Bucky              EternalCodes\n"
				"^3Abrum            BarryHit           Player\n"
				"^3Flendo           Padaget            VideoP/Loda\n"
				"^3Noodle           Smoo               QK-Lite/Effect.Ve\n"
				"^3MVSDK/JK2MV      x[Dsk]x Clan       =AJC= Clan\n"
				"^7\""));
		}
		return;
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is not available\n\"", arg1));
		return;
	}
}

/*
=====================================================================
Drop flag function
=====================================================================
*/
static void JKMod_Cmd_DropFlag(gentity_t *ent)
{
	gitem_t		*item = ent->client->sess.sessionTeam == TEAM_RED ? BG_FindItem("team_CTF_blueflag") : BG_FindItem("team_CTF_redflag");
	vec3_t		angles, velocity, org, offset, mins, maxs;
	trace_t		tr;

	if (!jkcvar_dropFlag.integer)
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

	if (ent->client->jkmodClient.dropFlagTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to take the flag again\n\"", ent->client->jkmodClient.dropFlagTime));
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

	ent->client->jkmodClient.dropFlagTime = jkcvar_dropFlagTime.integer;
	JKMod_LaunchItem(item, org, velocity, ent->s.number);
}

/*
=====================================================================
Motd function
=====================================================================
*/
static void JKMod_Cmd_ShowMotd(gentity_t *ent)
{
	if (VALIDSTRINGCVAR(jkcvar_serverMotd.string))
	{
		ent->client->jkmodClient.motdTime = jkcvar_serverMotdTime.integer;
	}
}

/*
=====================================================================
Instructs all chat & duels to be ignored by the given
=====================================================================
*/
void JKMod_IgnoreClient(int option, int ignorer, int ignored, qboolean ignore)
{
	if (ignore) {
		g_entities[ignorer].client->sess.jkmodSess.ignoredPlayer[option] |= (1 << ignored);
	} else {
		g_entities[ignorer].client->sess.jkmodSess.ignoredPlayer[option] &= ~(1 << ignored);
	}
}

/*
=====================================================================
Checks if the given client is being ignored by a specific client
=====================================================================
*/
qboolean JKMod_IgnoreClientCheck(int option, int ignorer, int ignored)
{
	return (g_entities[ignorer].client->sess.jkmodSess.ignoredPlayer[option] & (1 << ignored)) || g_entities[ignorer].client->sess.jkmodSess.ignoredAll[option];
}

/*
=====================================================================
Clear disconnected ignored clients
=====================================================================
*/
void JKMod_IgnoreClientClear(int ignored)
{
	int i;

	for (i = 0; i < level.maxclients; i++) 
	{
		g_entities[i].client->sess.jkmodSess.ignoredPlayer[0] &= ~(1 << ignored); // Chat
		g_entities[i].client->sess.jkmodSess.ignoredPlayer[1] &= ~(1 << ignored); // Duel
	}

	JKMod_Printf(S_COLOR_MAGENTA "Client %i removed from ingore lists\n", ignored);
}

/*
=====================================================================
Ignore client cmd
=====================================================================
*/
static void JKMod_Cmd_IgnoreClient(gentity_t *ent)
{
	int			option;
	char		arg1[MAX_TOKEN_CHARS];
	char		arg2[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));
	trap_Argv(2, arg2, sizeof(arg2));

	if (!jkcvar_playerIgnore.integer) 
	{
		trap_SendServerCommand(ent - g_entities, va("print \"This command is disabled by the server\n\""));
		return;
	}
	else if (trap_Argc() < 3)
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Ignore ^5]^7\n"
			"^7Ignore a player chat or duel challenge\n"
			"^7You can use this feature using the following command: ^2/ignore <player|number|all> <option>\n"
			"^7To undo the changes use the same command again\n"
			"^5----------\n"
			"^7Option list:\n"
			"^3chat\n"
			"^3duel\n"
			"^5----------\n"
			"^2Note 1: ^7No need to use full name or color name, you can use just a part of it\n"
			"^2Note 2: ^7You can use the command ^3/whois ^7to check the player number and ignore status\n"
			"^7\""));
		return;
	}
	else if (!(!Q_stricmp(arg2, "chat") || !Q_stricmp(arg2, "duel")))
	{
		trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is not valid\n\"", arg2));
		return;
	}
	else 
	{
		option = !Q_stricmp(arg2, "chat") ? 0 : 1;

		if (!Q_stricmp(arg1, "all"))
		{
			ent->client->sess.jkmodSess.ignoredAll[option] = ent->client->sess.jkmodSess.ignoredAll[option] ? qfalse : qtrue;

			if (ent->client->sess.jkmodSess.ignoredAll[option])
			{
				trap_SendServerCommand(ent - g_entities, va("print \"You are ignoring all ^3%ss ^7now\n\"", arg2));
				return;
			}
			else
			{
				int i;
				for (i = 0; i < level.maxclients; i++) g_entities[ent - g_entities].client->sess.jkmodSess.ignoredPlayer[option] &= ~(1 << i);

				trap_SendServerCommand(ent - g_entities, va("print \"You are no longer ignoring ^3%ss\n\"", arg2));
				return;
			}
		}
		else
		{
			int target = JKMod_CheckValidClient(ent, arg1);

			if (target != -1)
			{
				qboolean ignore = JKMod_IgnoreClientCheck(option, ent->client->ps.clientNum, target) ? qfalse : qtrue;

				JKMod_IgnoreClient(option, ent->client->ps.clientNum, target, ignore);

				if (ignore)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You are ignoring ^3%s ^7%ss now\n\"", g_entities[target].client->pers.netname, arg2));
					return;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You are no longer ignoring %ss from ^3%s\n\"", arg2, g_entities[target].client->pers.netname));
					return;
				}
			}
		}
	}
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
	qboolean duelEmote = qfalse;

	if (!g_privateDuel.integer) return;

	if (g_gametype.integer == GT_TOURNAMENT)
	{ 
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NODUEL_GAMETYPE")));
		return;
	}

	if (g_gametype.integer >= GT_TEAM)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NODUEL_GAMETYPE")));
		return;
	}

	if (ent->client->ps.duelTime >= level.time) return;

	if (ent->client->ps.weapon != WP_SABER) return;

	if (ent->client->ps.saberInFlight) return;

	if (ent->client->ps.duelInProgress) return;

	// Check duel challenge
	if (jkcvar_altDimension.integer && ent->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Private duels are not allowed in this dimension\n\""));
		return;
	}

	// Allow multiple duels
	if (jkcvar_allowMultiDuel.integer != 1)
	{
		// Don't let a player duel if he just did and hasn't waited 10 seconds yet (note: If someone challenges him, his duel timer will reset so he can accept)
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

		if (g_gametype.integer >= GT_TEAM && OnSameTeam(ent, challenged)) return;

		// Tr!Force: [Dimensions] Check duel challenge
		if (jkcvar_altDimension.integer && challenged->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE) return;

		// Tr!Force: [Ignore] Apply duel ignore
		if (JKMod_IgnoreClientCheck(1, challenged->s.number, ent->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"This player doesn't want to be challenged\n\"");
			return;
		}
		if (JKMod_IgnoreClientCheck(1, ent->s.number, challenged->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"You have ignored this player challenges\n\"");
			return;
		}

		if (challenged->client->ps.duelIndex == ent->s.number && challenged->client->ps.duelTime >= level.time)
		{
			trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s %s" S_COLOR_WHITE "!\n\"", challenged->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELACCEPT"), ent->client->pers.netname));

			// Custom start emote
			if (VALIDSTRINGCVAR(jkcvar_duelStartEmote.string) && 
				JKMod_EmoteCheck(jkcvar_duelStartEmote.string, ent) && 
				JKMod_EmoteCheck(jkcvar_duelStartEmote.string, challenged))
			{
				duelEmote = qtrue;
			}

			// Enable duel
			ent->client->ps.duelInProgress = qtrue;
			challenged->client->ps.duelInProgress = qtrue;

			// Set dimension
			if ((jkcvar_altDimension.integer & DIMENSION_DUEL) && challenged->client->sess.jkmodSess.privateDuel)
			{
				unsigned DIMENSION_DUEL_FREE = JKMod_DimensionGetFree();

				JKMod_DimensionSet(ent, DIMENSION_DUEL_FREE);
				JKMod_DimensionSet(challenged, DIMENSION_DUEL_FREE);
			}

			ent->client->ps.duelTime = level.time + 2000;
			challenged->client->ps.duelTime = level.time + 2000;

			G_AddEvent(ent, EV_PRIVATE_DUEL, 1);
			G_AddEvent(challenged, EV_PRIVATE_DUEL, 1);

			if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) ent->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;
			if (challenged->client->ps.eFlags & JK_JETPACK_ACTIVE) challenged->client->ps.eFlags &= ~JK_JETPACK_ACTIVE;

			// Default start health and shield
			if (jkcvar_duelStartHealth.integer != 0 && jkcvar_duelStartArmor.integer != 0)
			{
				ent->client->ps.stats[STAT_HEALTH] = ent->health = jkcvar_duelStartHealth.integer;
				ent->client->ps.stats[STAT_ARMOR] = jkcvar_duelStartArmor.integer;

				challenged->client->ps.stats[STAT_HEALTH] = challenged->health = jkcvar_duelStartHealth.integer;
				challenged->client->ps.stats[STAT_ARMOR] = jkcvar_duelStartArmor.integer;
			}

			// Holster their sabers now, until the duel starts (then they'll get auto-turned on to look cool)
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
			qboolean privateDuel = (jkcvar_altDimension.integer & DIMENSION_DUEL) && ent->client->sess.jkmodSess.privateDuel;

			if (jkcvar_allowCustomDuel.integer) 
			{
				if (type == 1) 
				{
					// Print full force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (Full force%s)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE"), privateDuel ? " private" : ""));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (Full force%s)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname, privateDuel ? " private" : ""));
					
					ent->client->pers.jkmodPers.customDuel = type;
					challenged->client->pers.jkmodPers.customDuel = type;
				}
				else 
				{
					// Print full no-force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (No force%s)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE"), privateDuel ? " private" : ""));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (No force%s)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname, privateDuel ? " private" : ""));
				}
			}
			else 
			{
				// Print the message that a player has been challenged in private, only announce the actual duel initiation in private
				G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s %s\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE"), privateDuel ? "(Private)" : ""));
				G_CenterPrint(ent - g_entities, 3, va("%s %s %s\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname, privateDuel ? "(Private)" : ""));
			}
		}

		// Reset the timer in case this player just got out of a duel. He should still be able to accept the challenge
		challenged->client->ps.fd.privateDuelTime = 0; 

		if (!duelEmote)
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
			ent->client->ps.forceHandExtendTime = level.time + 1000;
		}

		ent->client->ps.duelIndex = challenged->s.number;
		ent->client->ps.duelTime = level.time + 5000;

		// Auto duel view and challenge enemy
		if (challenged->client->sess.jkmodSess.autoDuel && jkcvar_duelAutoAccept.integer)
		{
			vec3_t entDir, otherAngles, lookAng;

			VectorSubtract(ent->client->ps.origin, challenged->client->ps.origin, entDir);
			VectorCopy(challenged->client->ps.viewangles, otherAngles);
			vectoangles(entDir, lookAng);

			otherAngles[YAW] = vectoyaw(entDir);
			otherAngles[PITCH] = lookAng[PITCH];

			SetClientViewAngle(challenged, otherAngles);
			JKMod_EngageDuel(challenged, type);
		}
	}
}

// Re-route old function
void Cmd_EngageDuel_f(gentity_t *ent) { JKMod_EngageDuel(ent, 0); }

/*
=====================================================================
Custom engage duel function
=====================================================================
*/
static void JKMod_Cmd_EngageDuel(gentity_t *ent)
{
	if (!jkcvar_allowCustomDuel.integer || g_forcePowerDisable.integer >= 163837 || g_forcePowerDisable.integer == -1) {
		trap_SendServerCommand(ent - g_entities, va("print \"Force powers ^3disabled ^7by the server. Applying normal duel...\n\""));
		JKMod_EngageDuel(ent, 0);
	} else {
		JKMod_EngageDuel(ent, 1);
	}
	return;
}

/*
=====================================================================
Check private duel challenge
=====================================================================
*/
static void JKMod_Cmd_EngagePrivate(gentity_t* ent)
{
	if (jkcvar_altDimension.integer & DIMENSION_DUEL)
	{
		if (ent->client->sess.jkmodSess.privateDuel) {
			ent->client->sess.jkmodSess.privateDuel = qfalse;
			trap_SendServerCommand(ent - g_entities, va("print \"Private duels are now ^1disabled\n\""));
		} else {
			ent->client->sess.jkmodSess.privateDuel = qtrue;
			trap_SendServerCommand(ent - g_entities, va("print \"Private duels are now ^2enabled\n\""));
		}

		// Update clientside
		if (ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("jk_cg_privateDuel %i", (int)ent->client->sess.jkmodSess.privateDuel));
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Private duels are ^1disabled ^7by the server\n\""));
	}
	return;
}

/*
=====================================================================
Check private duel challenge
=====================================================================
*/
static void JKMod_Cmd_EngageAuto(gentity_t* ent)
{
	if (jkcvar_duelAutoAccept.integer)
	{
		if (ent->client->sess.jkmodSess.autoDuel) {
			ent->client->sess.jkmodSess.autoDuel = qfalse;
			trap_SendServerCommand(ent - g_entities, va("print \"Auto duel challenges are now ^1disabled\n\""));
		} else {
			ent->client->sess.jkmodSess.autoDuel = qtrue;
			trap_SendServerCommand(ent - g_entities, va("print \"Auto duel challenges are now ^2enabled\n\""));
		}

		// Update clientside
		if (ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("jk_cg_autoDuel %i", (int)ent->client->sess.jkmodSess.autoDuel));
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Auto duels are ^1disabled ^7by the server\n\""));
	}
	return;
}

/*
=====================================================================
Who is / status function
=====================================================================
*/
void JKMod_Cmd_WhoIs(gentity_t *ent)
{
	int	num;

	if (ent) 
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Who is ^5]^7\n"
			"^7List of all players connected on the server\n"
			"^7Client plugin status: ^2Valid^7, ^3Invalid^7, ^1No plugin\n"
			"^7\""));
		
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5--- ---------------------------- ----- --------- ------------ ---------------\n"
			"^7Num Name                         Type  Ignore    Dimension    Plugin\n"
			"^5--- ---------------------------- ----- --------- ------------ ---------------\n"
			"^7\""));
	}
	else
	{
		G_Printf("Map: %s\n", JKMod_GetCurrentMap());
		G_Printf("Num Name                         Type  Dimension    Plugin\n");
		G_Printf("--- ---------------------------- ----- ------------ ---------------\n");
	}

	for (num = 0; num < level.maxclients; num++)
	{
		gentity_t 	*user = &g_entities[num];
		char		userinfo[MAX_INFO_VALUE] = { 0 };
		char		name[MAX_STRING_CHARS] = "";
		char		ignored[MAX_STRING_CHARS] = "";
		char		*value;
		char		*type;
		char		*dimension;
		char		*status;
		char		*plugin;

		// Check
		if (!user || !user->client || !user->inuse) continue;
		if (user->client->pers.connected == CON_DISCONNECTED) continue;

		// Get info
		trap_GetUserinfo(num, userinfo, sizeof(userinfo));

		// Find info
		value = Info_ValueForKey(userinfo, "jkmod_client");
		if (value[0]) {
			plugin = value;
		} else {
			plugin = "No plugin";
		}

		// Check ignore
		if (ent)
		{
			if (JKMod_IgnoreClientCheck(1, ent->s.number, user->s.number)) {
				Q_strcat(ignored, sizeof(ignored), va("Duel/", ignored));
			}
			if (JKMod_IgnoreClientCheck(0, ent->s.number, user->s.number)) {
				Q_strcat(ignored, sizeof(ignored), va("Chat/", ignored));
			}
			if (ignored[0] == '\0') strcpy(ignored, "No/");
			
			ignored[strlen(ignored) - 1] = '\0';
		}

		// Set info
		strcpy(name, user->client->pers.netname);
		dimension = user->client->sess.sessionTeam == TEAM_SPECTATOR ? "Spectator" : (user->client->ps.duelInProgress ? "Private Duel" : va("%s", JKModDimensionData[JKMod_DimensionIndex(user->client->ps.stats[JK_DIMENSION])].name));
		type = user->r.svFlags & SVF_BOT ? "Bot" : "Human";
		status = strcmp(plugin, "No plugin") ? (user->client->pers.jkmodPers.clientPlugin ? S_COLOR_GREEN : S_COLOR_YELLOW) : S_COLOR_RED;

		// Player print
		if (ent) {
			trap_SendServerCommand(ent - g_entities, va("print \"^7%-3i %-28s %-5s %-9s %-12s %s%-15s\n\"",
				num,
				Q_CleanStr(name, qfalse),
				type,
				ignored,
				dimension,
				status,
				plugin
			));
		// Server print
		} else {
			G_Printf("%-3i %-28s %-5s %-12s %-15s\n",
				num,
				Q_CleanStr(name, qfalse),
				type,
				dimension,
				plugin
			);
		}
	}

	if (ent) {
		trap_SendServerCommand(ent - g_entities, va("print \"^5--- ---------------------------- ----- --------- ------------ ---------------\n\""));
	} else {
		G_Printf("--- ---------------------------- ----- ------------ ---------------\n");
	}
}

/*
=====================================================================
Macro alert command function
=====================================================================
*/
static void JKMod_Cmd_MacroAlert(gentity_t* ent)
{
	char    arg1[MAX_TOKEN_CHARS];
	char	bind[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));

	if (trap_Argc() < 2) {
		strcpy(bind, "Unknown");
	} else  {
		JKMod_TrimWhiteSpace(arg1);
		JKMod_TruncateString(bind, arg1, 25);
	}

	if (jkcvar_macroScan.integer)
	{
		G_LogPrintf("Illegal macro detected! (IP: %s Bind: %s)\n", ent->client->sess.jkmodSess.clientIP, bind);

		if (jkcvar_macroScanAlert.integer)
		{
			G_Say(ent, NULL, SAY_ALL, va("^5Illegal macro detected^1! ^7(Bind: ^3%s^7)", bind));
		}
		
		if (jkcvar_macroScan.integer == 2)
		{
			SetTeam(ent, "spectator");
			return;
		}
		else if (jkcvar_macroScan.integer == 3)
		{
			JKMod_DropPlayer(ent, "Illegal macro detected");
			return;
		}
	}
}

/*
=====================================================================
Dimension command function
=====================================================================
*/
void JKMod_Cmd_Dimension(gentity_t* ent)
{
	char    arg1[MAX_TOKEN_CHARS];
	trap_Argv(1, arg1, sizeof(arg1));

	JKMod_DimensionChange(ent, arg1, qfalse);
	return;
}

/*
=====================================================================
Save position for teleport
=====================================================================
*/
static qboolean JKMod_savePosition(gentity_t *ent, qboolean say)
{
	char *print = say ? "cp" : "print";

	if (jkcvar_teleportChat.integer != 2 && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_CHEAT)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"This teleport is not available\n\"", print));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport in spectator\n\"", print));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport during pause mode\n\"", print));
		return qfalse;
	}
	else
	{
		ent->client->ps.viewangles[2] = 0.0f;
		ent->client->pers.jkmodPers.teleportChat[0] = ent->client->ps.origin[0];
		ent->client->pers.jkmodPers.teleportChat[1] = ent->client->ps.origin[1];
		ent->client->pers.jkmodPers.teleportChat[2] = ent->client->ps.origin[2];
		ent->client->pers.jkmodPers.teleportChat[3] = ent->client->ps.viewangles[PITCH];
		ent->client->pers.jkmodPers.teleportChat[4] = ent->client->ps.viewangles[YAW];

		trap_SendServerCommand(ent - g_entities, va("%s \"Saved position!\n\"", print));

		// Teleport check
		if (ent->client->pers.jkmodPers.teleportChatCheck != 1) {
			ent->client->pers.jkmodPers.teleportChatCheck = 1; 
			return qtrue;
		} else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Load position for teleport
=====================================================================
*/
static qboolean JKMod_loadPosition(gentity_t *ent, qboolean say)
{
	char *print = say ? "cp" : "print";

	if (jkcvar_teleportChat.integer != 2 && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_CHEAT)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"This teleport is not available\n\"", print));
		return qfalse;
	}
	else if (!ent->client->pers.jkmodPers.teleportChatCheck)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You don't have any saved position\n\"", print));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport in spectator\n\"", print));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport during pause mode\n\"", print));
		return qfalse;
	}
	else
	{
		vec3_t temporigin, tempangles;

		temporigin[0] = ent->client->pers.jkmodPers.teleportChat[0];
		temporigin[1] = ent->client->pers.jkmodPers.teleportChat[1];
		temporigin[2] = ent->client->pers.jkmodPers.teleportChat[2];
		tempangles[PITCH] = ent->client->pers.jkmodPers.teleportChat[3];
		tempangles[YAW] = ent->client->pers.jkmodPers.teleportChat[4];
		tempangles[ROLL] = 0.0f;

		JKMod_TeleportPlayer(ent, temporigin, tempangles, qfalse, 0, "thermal/shockwave", "sound/interface/secret_area");

		// Reset timer
		if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
		{
			ent->client->ps.duelTime = 0;
			ent->client->pers.jkmodPers.raceStartTime = 0;
		}

		// Teleport check
		if (ent->client->pers.jkmodPers.teleportChatCheck == 1) {
			ent->client->pers.jkmodPers.teleportChatCheck = 2;
			return qtrue;
		} else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Teleport save/load position command function
=====================================================================
*/
static void JKMod_Cmd_TelePos(gentity_t* ent)
{
	char    cmd[MAX_TOKEN_CHARS];
	trap_Argv(0, cmd, sizeof(cmd));

	if (!Q_stricmp(cmd, "savepos") || !Q_stricmp(cmd, "amsavepos")) {
		JKMod_savePosition(ent, qfalse);
		return;
	} else {
		JKMod_loadPosition(ent, qfalse);
		return;
	}
}

/*
=====================================================================
Save position for teleport
=====================================================================
*/
static qboolean JKMod_saveSpawn(gentity_t *ent, qboolean say)
{
	char *print = say ? "cp" : "print";

	if (jkcvar_teleportChat.integer != 2 && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_CHEAT)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"Save spawn is not available\n\"", print));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't save spawn in spectator\n\"", print));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't save spawn during pause mode\n\"", print));
		return qfalse;
	}
	else
	{
		ent->client->ps.viewangles[2] = 0.0f;
		ent->client->pers.jkmodPers.customSpawn[0] = ent->client->ps.origin[0];
		ent->client->pers.jkmodPers.customSpawn[1] = ent->client->ps.origin[1];
		ent->client->pers.jkmodPers.customSpawn[2] = ent->client->ps.origin[2];
		ent->client->pers.jkmodPers.customSpawn[3] = ent->client->ps.viewangles[PITCH];
		ent->client->pers.jkmodPers.customSpawn[4] = ent->client->ps.viewangles[YAW];

		trap_SendServerCommand(ent - g_entities, va("%s \"Saved spawn point!\n\"", print));

		// Teleport check
		if (!ent->client->pers.jkmodPers.customSpawnCheck) {
			ent->client->pers.jkmodPers.customSpawnCheck = 1; 
			return qtrue;
		} else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Load position for teleport
=====================================================================
*/
static qboolean JKMod_resetSpawn(gentity_t *ent, qboolean say)
{
	char *print = say ? "cp" : "print";

	if (jkcvar_teleportChat.integer != 2 && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_CHEAT)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"Reset spawn is not available\n\"", print));
		return qfalse;
	}
	else if (!ent->client->pers.jkmodPers.customSpawnCheck)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You don't have any saved spawn\n\"", print));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't reset spawn during pause mode\n\"", print));
		return qfalse;
	}
	else
	{
		ARRAY_CLEAR(ent->client->pers.jkmodPers.customSpawn);
		trap_SendServerCommand(ent - g_entities, va("%s \"Spawn point reset!\n\"", print));

		// Teleport check
		if (ent->client->pers.jkmodPers.customSpawnCheck) {
			ent->client->pers.jkmodPers.customSpawnCheck = 0;
			return qtrue;
		} else {
			return qfalse;
		}
	}
}

/*
=====================================================================
Teleport save/reset spawn position command function
=====================================================================
*/
static void JKMod_Cmd_SpawnPos(gentity_t* ent)
{
	char    cmd[MAX_TOKEN_CHARS];
	trap_Argv(0, cmd, sizeof(cmd));

	if (!Q_stricmp(cmd, "savespawn") || !Q_stricmp(cmd, "amsavespawn")) {
		JKMod_saveSpawn(ent, qfalse);
		return;
	} else {
		JKMod_resetSpawn(ent, qfalse);
		return;
	}
}

/*
=====================================================================
JetPack command function
=====================================================================
*/
static void JKMod_Cmd_JetPack(gentity_t* ent)
{
	if (!jkcvar_jetPack.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"This command is disabled by the server\n\""));
		return;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't use jetpack in spectator\n\""));
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't use jetpack during pause mode\n\""));
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
			ent->client->ps.stats[JK_FUEL] = 0;
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, va("print \"Jetpack disabled\n\""));
			return;
		}
		// Enable
		else
		{
			ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
			ent->client->ps.stats[JK_FUEL] = 100;
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, va("print \"Jetpack equiped\n\""));
			trap_SendServerCommand(ent - g_entities, va("cp \"Press USE button on air to enable\""));
			return;
		}
	}
}

/*
=====================================================================
Emote command function
=====================================================================
*/
static void JKMod_Cmd_Emote(gentity_t* ent)
{
	char    arg1[MAX_TOKEN_CHARS];
	trap_Argv(1, arg1, sizeof(arg1));

	if (trap_Argc() < 2)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Usage: emote <animation>\nSee ^3/help emotes ^7for more information\n\""));
		return;
	}
	else if (!Q_stricmp(arg1, "stop")) 
	{
		if (JKMod_EmoteIn(ent, -1))
		{
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		return;
	}
	else if (!JKMod_EmoteCheck(arg1, ent))
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Invalid emote ^3%s\n\"", arg1));
		return;
	}
}

/*
=====================================================================
Dual saber command function
=====================================================================
*/
static void JKMod_Cmd_DualSaber(gentity_t* ent)
{
	if (!jkcvar_dualSaber.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"This command is disabled by the server\n\""));
		return;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't use dual saber in spectator\n\""));
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't use dual saber during pause mode\n\""));
		return;
	}
	else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't use dual saber in this dimension\n\""));
		return;
	}
	else if (ent->client->ps.weapon != WP_SABER)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You can't enable it using this weapon\n\""));
		return;
	}
	else
	{
		if (ent->client->ps.saberHolstered) Cmd_ToggleSaber_f(ent);

		// Disable
		if (ent->client->ps.dualBlade)
		{
			ent->client->ps.dualBlade = qfalse;
			ent->client->pers.jkmodPers.dualSaber = qfalse;
			return;
		}
		// Enable
		else
		{
			ent->client->ps.dualBlade = qtrue;
			ent->client->pers.jkmodPers.dualSaber = qtrue;
			return;
		}
	}
}

/*
=====================================================================
Entity scan function
=====================================================================
*/
static void JKMod_Cmd_EntityScan(gentity_t *ent, int distance, int boxdelay, int linedelay)
{
	trace_t		tr;
	vec3_t		fwd, dest, orig;
	vec3_t		mins = { -5, -5, -5 }, maxs = { 5, 5, 5 };

	if (!distance) distance = 200;
	if (!boxdelay) boxdelay = 500;
	if (!linedelay) linedelay = 500;

	AngleVectors(ent->client->ps.viewangles, fwd, NULL, NULL);

	VectorCopy(ent->client->ps.origin, orig);
	VectorMA(orig, distance, fwd, dest);

	trap_Trace(&tr, orig, mins, maxs, dest, ent->s.number, MASK_ALL);

	if (tr.allsolid || tr.startsolid || tr.fraction != 1.0f)
	{
		gentity_t	*found = &g_entities[tr.entityNum];
		char		 found_mins[20];
		char		 found_maxs[20];
		char		 found_angles[20];
		char		 found_origin[40];

		G_TestLine(orig, dest, 255, linedelay);

		if (!found->inuse) return;
				
		Q_strncpyz(found_mins, va("%.0f %.0f %.0f", found->r.mins[0], found->r.mins[1], found->r.mins[2]), sizeof(found_mins));
		Q_strncpyz(found_maxs, va("%.0f %.0f %.0f", found->r.maxs[0], found->r.maxs[1], found->r.maxs[2]), sizeof(found_maxs));
		Q_strncpyz(found_angles, va("%.0f %.0f %.0f", found->s.angles[0], found->s.angles[1], found->s.angles[2]), sizeof(found_angles));
		Q_strncpyz(found_origin, va("%.0f %.0f %.0f", found->s.origin[0], found->s.origin[1], found->s.origin[2]), sizeof(found_origin));

		trap_SendServerCommand(ent - g_entities, va("print \"\n"
			"^7Classname: ^3%-40.40s ^7Target:     ^3%-25.25s ^7Angles: ^3%-20.20s\n"
			"^7Model:     ^3%-40.40s ^7Targetname: ^3%-25.25s ^7Mins:   ^3%-20.20s\n"
			"^7Origin:    ^3%-40.40s ^7Spawnflags: ^3%-25.25i ^7Maxs:   ^3%-20.20s\n"
			"^7\"", 
			found->classname,
			found->target,
			found_angles,
			found->model,
			found->targetname,
			found_mins,
			found_origin,
			found->spawnflags,
			found_maxs));

		JKMod_DrawBoxLines(found->s.origin, found->r.mins,  found->r.maxs, 255, boxdelay);
		return;
	}

	G_TestLine(orig, dest, 0, linedelay);
	return;
}

/*
=====================================================================
Check teleport say commands
=====================================================================
*/
static qboolean JKMod_teleportCheck(gentity_t *ent)
{
	int			i = 0;
	char		command[MAX_TOKEN_CHARS];
	char		map[MAX_TOKEN_CHARS];
	char		message[MAX_STRING_CHARS] = { 0 };
	qboolean	found = qfalse;

	Q_strcat(message, sizeof(message), "^7Teleports available for this map:\n");

	for (i = 0; i < level.jkmodLocals.teleportChatsCount; i++)
	{
		strcpy(command, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "command"));
		strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));

		if (Q_stricmp(map, JKMod_GetCurrentMap()) == 0)
		{
			Q_strcat(message, sizeof(message), va("^3%s\n", command));
			found = qtrue;
		}
	}
	
	if (found) message[strlen(message) - 1] = '\0';
	trap_SendServerCommand(ent - g_entities, va("cp \"%s\n\"", (found ? message : "This map doesn't have chat teleports")));
	return qfalse;
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

	for (i = 0; i < level.jkmodLocals.teleportChatsCount; i++)
	{
		strcpy(command, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "command"));
		strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));
		strcpy(origin, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "origin"));
		strcpy(rotation, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "rotation"));

		sscanf(origin, "%f %f %f %i", &realorigin[0], &realorigin[1], &realorigin[2], &realrotation);

		if (Q_stricmp(text, command) == 0)
		{
			if (Q_stricmp(map, JKMod_GetCurrentMap()) == 0)
			{
				if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport in spectator\n\""));
					return qfalse;
				}
				else if (level.jkmodLocals.pauseTime > level.time)
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport during pause mode\n\""));
					return qfalse;
				}
				else if (JKMod_PlayerMoving(ent, qtrue, qtrue))
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You can't teleport while moving\n\""));
					return qfalse;
				}
				else if (ent->client->ps.pm_type == PM_DEAD)
				{
					trap_SendServerCommand(ent - g_entities, va("print \"You can't teleport while dead\n\""));
					return qfalse;
				}
				else if (ent->client->jkmodClient.teleportChatTime > 0)
				{
					trap_SendServerCommand(ent - g_entities,va("print \"You have to wait %d seconds before teleport again\n\"", ent->client->jkmodClient.teleportChatTime));
					return qfalse;
				}
				else if (!ent->client->jkmodClient.teleportChatUsed || jkcvar_teleportChatTime.integer)
				{
					vec3_t		temporigin, tempangles;

					// Delay
					ent->client->jkmodClient.teleportChatTime = jkcvar_teleportChatTime.integer;

					VectorClear(temporigin);
					VectorClear(tempangles);

					temporigin[0] = realorigin[0];
					temporigin[1] = realorigin[1];
					temporigin[2] = realorigin[2];
					tempangles[PITCH] = 0.0f;
					tempangles[YAW] = realrotation;
					tempangles[ROLL] = 0.0f;

					JKMod_TeleportPlayer(ent, temporigin, tempangles, qtrue, 300, "cinematics/hugesparks", "default");
					ent->client->jkmodClient.teleportChatUsed = jkcvar_teleportChatTime.integer ? qfalse : qtrue;
					return qtrue;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("cp \"You need to respawn to teleport again\n\""));
					return qfalse;
				}
			}
		}
	}
	return qtrue;
}

/*
=====================================================================
Custom callvote function
=====================================================================
*/
void JKMod_CallVote(gentity_t *ent) 
{
	int			i;
	char		arg1[MAX_STRING_TOKENS];
	char		arg2[MAX_STRING_TOKENS];
	const char	*gameNames[] = {
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
	if (ent->client->jkmodClient.voteWaitTime > 0) {
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to call a new vote again\n\"", ent->client->jkmodClient.voteWaitTime));
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

	if (Q_stricmp(arg1, "map_restart") 
		&& Q_stricmp(arg1, "nextmap") 
		&& Q_stricmp(arg1, "map") 
		&& Q_stricmp(arg1, "g_gametype") 
		&& Q_stricmp(arg1, "kick") 
		&& Q_stricmp(arg1, "clientkick") 
		&& Q_stricmp(arg1, "g_doWarmup") 
		&& Q_stricmp(arg1, "timelimit") 
		&& Q_stricmp(arg1, "fraglimit") 
		&& Q_stricmp(arg1, "poll") 
		&& Q_stricmp(arg1, "itemphysics") 
		&& Q_stricmp(arg1, "pausegame")) 
	{
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
			"^3poll <question>\n"
			"^3itemphysics\n"
			"^3pausegame\n"
			"^5----------\n"
			"^2Note: ^7Some options may be disabled by the server\n"
			"^7\""));
		return;
	}

	// if there is still a vote to be executed
	if (level.voteExecuteTime) 
	{
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand(EXEC_APPEND, va("%s\n", level.voteString));
	}

	// Game type vote
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
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Change game type to %s", gameNames[i]);
	}
	// Map vote
	else if (!Q_stricmp(arg1, "map"))
	{
		char	s[MAX_STRING_CHARS];
		char	*nextmap;

		if (!(jkcvar_voteControl.integer & (1 << VOTE_MAP))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (!G_DoesMapSupportGametype(arg2, trap_Cvar_VariableIntegerValue("g_gametype"))) {
			trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NOVOTE_MAPNOTSUPPORTEDBYGAME")));
			return;
		}

		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));

		if (*s) {
			nextmap = strchr(s, ' ');
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s; set nextmap \"%s\"", arg1, arg2, s);
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Change map to %s, then %s", arg2, nextmap+1);
		} else {
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s", arg1, arg2);
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Change map to %s", arg2);
		}

	}
	// Client kick vote
	else if (!Q_stricmp(arg1, "clientkick"))
	{
		int n = atoi(arg2);

		if (!(jkcvar_voteControl.integer & (1 << VOTE_CLIENTKICK))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (n < 0 || n >= MAX_CLIENTS) {
			trap_SendServerCommand(ent - g_entities, va("print \"invalid client number %d\n\"", n));
			return;
		}

		if (g_entities[n].client->pers.connected == CON_DISCONNECTED) {
			trap_SendServerCommand(ent - g_entities, va("print \"there is no client with the client number %d\n\"", n));
			return;
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "%s %s", arg1, arg2);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Kick %s from server", Q_CleanStr(g_entities[n].client->pers.netname, qfalse));
	}
	// Kick vote
	else if (!Q_stricmp(arg1, "kick"))
	{
		int clientid = G_ClientNumberFromName(arg2);

		if (!(jkcvar_voteControl.integer & (1 << VOTE_KICK))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (clientid == -1) 
		{
			// Re-check
			clientid = G_ClientNumberFromStrippedName(arg2);

			if (clientid == -1) {
				trap_SendServerCommand(ent - g_entities, va("print \"there is no client named '%s' currently on the server\n\"", arg2));
				return;
			}
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "clientkick %d", clientid);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Kick %s from server", Q_CleanStr(g_entities[clientid].client->pers.netname, qfalse));
	}
	// Next map vote
	else if (!Q_stricmp(arg1, "nextmap"))
	{
		char	s[MAX_STRING_CHARS];
		char	*nextmap;

		if (!(jkcvar_voteControl.integer & (1 << VOTE_NEXTMAP))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		trap_Cvar_VariableStringBuffer("nextmap", s, sizeof(s));

		if (!*s) {
			trap_SendServerCommand(ent - g_entities, "print \"nextmap not set\n\"");
			return;
		}

		nextmap = strchr(s, ' ');

		Com_sprintf(level.voteString, sizeof(level.voteString), "vstr nextmap");
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Skip to next map %s", nextmap+1);
	}
	// Poll vote
	else if (!Q_stricmp(arg1, "poll"))
	{
		if (!(jkcvar_voteControl.integer & (1 << VOTE_POLL))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}
		
		if (arg2[0] == '\0') {
			trap_SendServerCommand(ent - g_entities, "print \"Usage: callvote poll <question>\n\"");
			return;
		}
			
		Com_sprintf(level.voteString, sizeof(level.voteString), "");
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Poll: %s", Q_CleanStr(ConcatArgs(2), qfalse));
	}
	// Item physics
	else if (!Q_stricmp(arg1, "itemphysics"))
	{
		int		val;
		char	*action;

		if (!(jkcvar_voteControl.integer & (1 << VOTE_ITEMPHYSICS))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		val = trap_Cvar_VariableIntegerValue("jk_itemForcePhysics") == 0 ? 1 : 0;
		action = val ? "Enable" : "Disable";

		Com_sprintf(level.voteString, sizeof(level.voteString), "jk_itemForcePhysics %i", val);
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "%s map item force physics", action);
	}
	// Item physics
	else if (!Q_stricmp(arg1, "pausegame"))
	{
		if (!(jkcvar_voteControl.integer & (1 << VOTE_PAUSE))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "pause 60");
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Pause game for 60 seconds");
	}
	// Others
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
				trap_SendServerCommand(ent - g_entities, "print \"The specified value is too long\n");
				return;
			}
		}

		if (arg2[0] == '\0') {
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s", arg1);
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Apply %s", arg1);
		} else {
			Com_sprintf(level.voteString, sizeof(level.voteString), "%s \"%s\"", arg1, arg2);
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Change %s to %s", arg1, arg2);
		}
	}

	trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLCALLEDVOTE")));

	// Start the voting, the caller autoamtically votes yes
	level.voteTime = level.time;
	level.voteYes = 1;
	level.voteNo = 0;

	for (i = 0; i < level.maxclients; i++) level.clients[i].ps.eFlags &= ~EF_VOTED;

	ent->client->ps.eFlags |= EF_VOTED;

	// Call vote timer
	ent->client->jkmodClient.voteWaitTime = jkcvar_voteWaitTime.integer;

	// Append white colorcode at the end of the display string as workaround for cgame leaking colors
	Q_strcat(level.voteDisplayString, sizeof(level.voteDisplayString), S_COLOR_WHITE);

	trap_SetConfigstring(CS_VOTE_TIME, va("%i", level.voteTime));
	trap_SetConfigstring(CS_VOTE_STRING, level.voteDisplayString);
	trap_SetConfigstring(CS_VOTE_YES, va("%i", level.voteYes));
	trap_SetConfigstring(CS_VOTE_NO, va("%i", level.voteNo));
}

/*
=====================================================================
Show current player status
=====================================================================
*/
qboolean JKMod_playerStatus(gentity_t *ent, qboolean announce)
{
	qboolean isCTF = g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY;
	qboolean isDuel = g_gametype.integer == GT_TOURNAMENT;

	if (ent->client->ps.pm_type == PM_DEAD) return qfalse;

	if (announce)
	{
		if (ent->client->pers.jkmodPers.playerStatusDelay <= level.time) 
		{
			ent->client->pers.jkmodPers.playerStatusDelay = level.time + (jkcvar_chatAutoStatusTime.integer*1000);

			G_Say(ent, NULL, SAY_ALL, va("I have %s ^5%i ^2and %s ^3%i ^2times! My health %s ^1%i^7/^2%i",
				(isCTF || isDuel ? "scored" : "killed"), 
				(isDuel ? ent->client->sess.wins : ent->client->ps.persistant[PERS_SCORE]),
				(isCTF ? "captured" : (isDuel ? "lost" : "died")), 
				(isCTF ? ent->client->ps.persistant[PERS_CAPTURES] : ent->client->sess.losses),
				(jkcvar_chatAutoStatus.integer || isDuel ? "was" : "is"),
				ent->client->ps.stats[STAT_HEALTH],
				ent->client->ps.stats[STAT_ARMOR])
			);
		}

		if (!jkcvar_chatAutoStatus.integer && !ent->client->sess.jkmodSess.playerStatusSeen)
		{
			ent->client->sess.jkmodSess.playerStatus = qfalse;
			ent->client->sess.jkmodSess.playerStatusSeen = qtrue;

			trap_SendServerCommand(ent - g_entities, va("print \"Auto status tracking is ^1disabled ^7by the server\n\""));
		}

		return !announce;
	}
	else
	{
		if (ent->client->sess.jkmodSess.playerStatus) {
			ent->client->sess.jkmodSess.playerStatus = qfalse;
			trap_SendServerCommand(ent - g_entities, va("cp \"Auto status tracking is now ^1disabled\n\""));
		} else {
			ent->client->sess.jkmodSess.playerStatus = qtrue;
			trap_SendServerCommand(ent - g_entities, va("cp \"Auto status tracking is now ^2enabled\n\""));
		}

		if (ent->client->sess.jkmodSess.playerStatusSeen) ent->client->sess.jkmodSess.playerStatusSeen = qfalse;

		return !announce && ent->client->sess.jkmodSess.playerStatus;
	}
}

/*
=====================================================================
Custom say function
=====================================================================
*/
void JKMod_Say(gentity_t *ent, int mode, qboolean arg0) 
{ 
	char	*p;
	int		clientNum = ent - g_entities;

	if (trap_Argc() < 2 && !arg0) return;

	p = arg0 ? ConcatArgs(0) : ConcatArgs(1);

	// Show motd
	if (Q_stricmp(p, "!motd") == 0)
	{
		JKMod_Cmd_ShowMotd(ent);
	}
	// Player stats
	else if (Q_stricmp(p, "!status") == 0)
	{
		if (!JKMod_playerStatus(ent, (jkcvar_chatAutoStatus.integer ? qfalse : qtrue))) return;
	}
	// Race time
	else if (Q_stricmp(p, "!racetime") == 0)
	{
		int timeLast = ent->client->pers.jkmodPers.raceLastTime;
		int timeBest = ent->client->pers.jkmodPers.raceBestTime;
		char timeLastStr[32];
		char timeBestStr[32];

		if (ent->client->ps.stats[JK_DIMENSION] != DIMENSION_RACE) {
			trap_SendServerCommand(ent - g_entities, "cp \"You must be in race mode\n\"");
			return;
		}

		Q_strncpyz(timeLastStr, JKMod_MsToString(timeLast), sizeof(timeLastStr));
		Q_strncpyz(timeBestStr, JKMod_MsToString(timeBest), sizeof(timeBestStr));

		if (!timeLast && !timeBest)
			trap_SendServerCommand(ent - g_entities, "cp \"You don't have any record\n\"");
		else if (timeLast <= timeBest)
			G_Say(ent, NULL, SAY_ALL, va("^7Last Time: ^2%s", timeLastStr));
		else
			G_Say(ent, NULL, SAY_ALL, va("^7Last Time: ^2%s ^7Record: ^5%s", timeLastStr, timeBestStr));

		return;
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

		if (!JKMod_DimensionChange(ent, dimension, qtrue)) return;
	}
	// Teleport chat (Save position)
	else if (Q_stricmp(p, "!savepos") == 0)
	{
		if (!JKMod_savePosition(ent, qtrue)) return;
	}
	// Teleport chat (Load position)
	else if (Q_stricmp(p, "!loadpos") == 0)
	{
		if (!JKMod_loadPosition(ent, qtrue)) return;
	}
	// Teleport chat (Save spawn point)
	else if (Q_stricmp(p, "!savespawn") == 0)
	{
		if (!JKMod_saveSpawn(ent, qtrue)) return;
	}
	// Teleport chat (Reset spawn point)
	else if (Q_stricmp(p, "!resetspawn") == 0)
	{
		if (!JKMod_resetSpawn(ent, qtrue)) return;
	}
	// Teleport chat check
	else if (Q_stricmp(p, "!teleports") == 0)
	{
		if (!JKMod_teleportCheck(ent)) return;
	}
	// Teleport chat (Load from file)
	else if (jkcvar_teleportChat.integer && level.jkmodLocals.teleportChats[0] && (p[0] == '!') && p[1] && (p[1] != '!'))
	{
		if (!JKMod_teleportChat(ent, p)) return;
	}

	G_Say(ent, NULL, mode, p);
}

/*
=====================================================================
Custom client commands
=====================================================================
*/

// Client commands list
jkmod_commands_t JKModCommandsTable[] = 
{
	// cmd						// function
	{ "help",					JKMod_Cmd_HelpInfo },
	{ "info",					JKMod_Cmd_HelpInfo },
	{ "dropflag",				JKMod_Cmd_DropFlag },
	{ "motd",					JKMod_Cmd_ShowMotd },
	{ "ignore",					JKMod_Cmd_IgnoreClient },

	{ "engage_duel_force",		JKMod_Cmd_EngageDuel },
	{ "engage_forceduel",		JKMod_Cmd_EngageDuel },
	{ "engage_fullforceduel",	JKMod_Cmd_EngageDuel },
	{ "engage_ff",				JKMod_Cmd_EngageDuel },
	{ "engage_private",			JKMod_Cmd_EngagePrivate },
	{ "engage_auto",			JKMod_Cmd_EngageAuto },

	{ "whois",					JKMod_Cmd_WhoIs },
	{ "macroalert",				JKMod_Cmd_MacroAlert },
	{ "dimension",				JKMod_Cmd_Dimension },

	{ "savepos",				JKMod_Cmd_TelePos },
	{ "loadpos",				JKMod_Cmd_TelePos },
	{ "savespawn",				JKMod_Cmd_SpawnPos },
	{ "resetspawn",				JKMod_Cmd_SpawnPos },

	{ "jetpack",				JKMod_Cmd_JetPack },
	{ "emote",					JKMod_Cmd_Emote },
	{ "dualsaber",				JKMod_Cmd_DualSaber },
};

static const size_t JKModCommandsTableSize = ARRAY_LEN(JKModCommandsTable);

// Client command function
void JKMod_ClientCommand(int clientNum)
{
	gentity_t			*ent;
	char				cmd[MAX_TOKEN_CHARS];
	char				token[BIG_INFO_STRING]; // As the engine uses Cmd_TokenizeString2 a single parameter is theoretically not limited by MAX_TOKEN_CHARS, but by BIG_INFO_STRING
	int					i, argc;
	jkmod_commands_t	*command;

	ent = g_entities + clientNum;

	// Not fully in game yet
	if (!ent->client || ent->client->pers.connected < CON_CONNECTED) return;

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

	// Check commands list
	for (i = 0, command = JKModCommandsTable; i < JKModCommandsTableSize; i++, command++)
	{
		if (!Q_stricmp(cmd, command->name) || (!Q_stricmpn(cmd, "am", 2) && !Q_stricmp((cmd + 2), command->name)))
		{
			command->func(ent);
			return;
		}
	}

	// Special case for emotes
	if (!Q_stricmpn(cmd, "am", 2) && JKMod_EmoteCheck(cmd, ent)) return;

	// Test command
	if (!Q_stricmp(cmd, "test") && developer.integer)
	{
		int		i;
		int		args = trap_Argc();
		char	argcmd[MAX_TOKEN_CHARS];
		char	arglist[MAX_STRING_CHARS] = { 0 };

		for (i = 0; i < args; i++) 
		{
			if (i != 0) {
				trap_Argv(i, argcmd, sizeof(argcmd));
				Q_strcat(arglist, sizeof(arglist), va("Arg%i: %s\n", i, argcmd));
			}
		}

		trap_Argv(1, argcmd, sizeof(argcmd));

		// Entity scan
		if (!Q_stricmp(argcmd, "entity")) 
		{
			char arg2[MAX_TOKEN_CHARS];
			char arg3[MAX_TOKEN_CHARS];
			char arg4[MAX_TOKEN_CHARS];

			trap_Argv(2, arg2, sizeof(arg2));
			trap_Argv(3, arg3, sizeof(arg3));
			trap_Argv(4, arg4, sizeof(arg4));

			JKMod_Cmd_EntityScan(ent, atoi(arg2), atoi(arg3), atoi(arg4));
			return;
		}

		trap_SendServerCommand(ent - g_entities, va("print \"CvarTest1: %s\nCvarTest2: %i\n%s\"", jkcvar_test1.string, jkcvar_test2.integer, arglist));
		return;
	}

	// Launch original client command function
	BaseJK2_ClientCommand(clientNum);
}
