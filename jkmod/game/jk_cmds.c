/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
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
extern jkmod_emotes_data_t JKModEmotesData[];
extern int JKModEmotesDataSize;
extern int G_ClientNumberFromName(const char* name);
extern int G_ClientNumberFromStrippedName(const char* name);
extern qboolean G_OtherPlayersDueling(void);

/*
=====================================================================
Help command function
=====================================================================
*/
static void JKMod_Cmd_HelpInfo(gentity_t *ent)
{
	char arg1[MAX_TOKEN_CHARS];
	trap_Argv(1, arg1, sizeof(arg1));

	// Main help
	if(trap_Argc() < 2)
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Help ^5]^7\n"
			"^7This server is running ^5%s ^7- Version: ^2%i.%i.%i ^7(Build: %s)\n"
			"^7You can read the desired help topic using following command: ^2/help <topic>\n"
			"^5----------\n"
			"^7Topic list:\n"
			"^3Admin          ^1Accounts       ^3Dimensions\n"
			"^3Emotes         Commands       Duels\n"
			"^1Build          ^3About          Credits\n"
			"^5----------\n"
			"^2Note 1: ^7Commands marked in ^1red ^7are not available to use yet\n\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, g_gamedate.string));

		if (!ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("print \"^2Note 2: ^7Plugin not detected or invalid, download from ^5%s\n\"", JK_URL));
		} else {
			trap_SendServerCommand(ent - g_entities, va("print \"^2Note 2: ^7More information at the mod website ^5%s\n\"", JK_URL));
		}
		return;
	}
	// Help admin
	else if (!Q_stricmp(arg1, "admin"))
	{
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Admin ^5]^7\n"
			"^7You can perform additional console commands if you are logged as admin or by remote console\n"
			"^7For ^5rcon ^7commands you have to use the following sintax: ^2/rcon <command>\n"
			"^5----------\n"
			"^7Rcon commands:\n"
			"^3gameplay\n"
			"^3pause\n"
			"^3reload\n"
			"^3remapshader\n"
			"^3changemusic\n"
			"^3togglemod\n"
			"^3teleport\n"
			"^3forcedimension\n"
			"^3forceteam\n"
			"^3lockteam\n"
			"^3whois\n"
			"^3listdir\n"
			"^3control\n"
			"^5----------\n"
			"^2Note: ^7To use ^5rcon ^7commands you must be logged with ^3/rconpassword <password>\n\"");
		return;
	}
	// Help commands
	else if (!Q_stricmp(arg1, "commands"))
	{
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Commands ^5]^7\n"
			"^7You can perform some console commands to perform a special action\n"
			"^7Chat commands are meant to be used on ^2chat ^7while typing\n"
			"^5----------\n"
			"^7Console & Binds:   Chat / Say:    Plugin Only:\n"
			"^3motd               !motd          strafehelper\n"
			"^3dimension          !dimension     speedometer\n"
			"^3dualsaber          !status\n"
			"^3emote              !savepos\n"
			"^3ignore             !loadpos\n"
			"^3dropflag           !savespawn\n"
			"^3callvote           !resetspawn\n"
			"^3whois              !whois\n"
			"^3taunt2             !where\n"
			"^3savepos            !racetime\n"
			"^3loadpos            !teleports\n"
			"^3savespawn          !bot\n"
			"^3resetspawn\n"
			"^3jetpack\n"
			"^3maplist\n"
			"^3chatcolor\n"
			"^3engage_force\n"
			"^3engage_private\n"
			"^3toggle_auto\n"
			"^3toggle_private\n\"");
		return;
	}
	// Help emotes
	else if(!Q_stricmp(arg1, "emotes"))
	{
		int			i;
		char		*emoteStatus;
		qboolean	emoteLine = qfalse;

		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Emotes ^5]^7\n"
			"^7Emotes are visual animations that allows you to sit down, greet someone, etc...\n"
			"^7There are over %i emotes to choose from. (Some may be disabled by the server)\n"
			"^7Play an emote animation by using the following command: ^2/emote <animation>\n"
			"^5----------\n\"", JKModEmotesDataSize));
			
		for (i = 0; i < JKModEmotesDataSize; i++) 
		{
			emoteLine = (i % 6 == 5 || i == (JKModEmotesDataSize-1));
			emoteStatus = S_COLOR_YELLOW;

			if (!(jkcvar_emotesEnabled.integer & (1 << JKModEmotesData[i].emoteIndex))) emoteStatus = S_COLOR_RED;
			if (!JKModEmotesData[i].compatible && jk2startversion == VERSION_1_02) emoteStatus = S_COLOR_RED;

			trap_SendServerCommand(ent - g_entities, va("print \"%s%-15s%s\"", emoteStatus, JKModEmotesData[i].cmd, (emoteLine ? "\n" : "")));
		}

		trap_SendServerCommand(ent - g_entities, va("print \""
			"%s^5----------\n"
			"^2Note 1: ^7You can also use this command by using ^5/am<animation>\n"
			"^2Note 2: ^7Emotes marked in ^1red ^7are not available to use\n\"", (emoteLine ? "" : "\n")));
		return;
	}
	// Help duels
	else if (!Q_stricmp(arg1, "duels"))
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"^5[^7 Duels ^5]^7\n"
			"^7Choose between different dueling challenges actions\n"
			"^7Your private duels are %s ^7and your auto duels accept is %s\n"
			"^5----------\n"
			"^7Command list:\n"
			"^3engage_duel\n"
			"%sengage_force\n"
			"%stoggle_auto\n"
			"%stoggle_private\n"
			"^5----------\n"
			"^2Note 1: ^7Commands marked in ^1red ^7are disabled by the server\n"
			"^2Note 2: ^7Toggle private command applies to ^5duel ^7and ^5force ^7commands\n\"", 
			(ent->client->sess.jkmodSess.privateDuel ? "^2enabled" : "^1disabled"), 
			(ent->client->sess.jkmodSess.autoDuel ? "^2enabled" : "^1disabled"),
			(jkcvar_allowCustomDuel.integer ? S_COLOR_YELLOW : S_COLOR_RED),
			(jkcvar_duelAutoAccept.integer ? S_COLOR_YELLOW : S_COLOR_RED),
			(jkcvar_altDimension.integer & DIMENSION_DUEL ? S_COLOR_YELLOW : S_COLOR_RED)));
		return;
	}
	// Help dimensions
	else if (!Q_stricmp(arg1, "dimensions"))
	{
		int i;

		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Dimensions ^5]^7\n"
			"^7Change between the available server dimensions\n"
			"^7You can use this feature using the following command: ^2/dimension <option>\n"
			"^7There are also private duel dimensions that you can check by using: ^2/help duels\n"
			"^5----------\n"
			"^7Option list:\n\"");

		for (i = 0; i < JKModDimensionDataSize; i++) {
			trap_SendServerCommand(ent - g_entities, va("print \"%s%s\n\"", (jkcvar_altDimension.integer & JKModDimensionData[i].dimension ? S_COLOR_YELLOW : S_COLOR_RED), JKModDimensionData[i].command));
		}

		trap_SendServerCommand(ent - g_entities, va("print \"%s%s\n\"", (jkcvar_altDimension.integer & DIMENSION_PRIVATE ? S_COLOR_YELLOW : S_COLOR_RED), "private"));
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5----------\n"
			"^2Note 1: ^7You can also use this command on chat saying ^5!dimension <option>\n"
			"^2Note 2: ^7For private dimension you can use the next options ^3/dimension <settings|invite|kick|join|leave>\n"
			"^2Note 3: ^7Dimensions marked in ^1red ^7are not available to use\n\"");
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
			"^7Mod Website: ^5%s\n\"", JK_LONGNAME, JK_URL));

		if (!Q_stricmp(arg1, "credits"))
		{
			trap_SendServerCommand(ent - g_entities, "print \""
				"^5----------\n"
				"^7The following people supported and helped me directly on this project:\n"
				"^3DeathSpike       Lee Oates        Nerevar        Kaiser/TVpath\n"
				"^3Rich Whitehouse  NTxC/Reality     Tox Laximus    McMonkey\n"
				"^3JediDog~         NewAge           Mr.Wonko       Xycaleth\n"
				"^3Daggolin         Ouned/Twitch     Kameleon       Fau\n"
				"^3Bucky            EternalCodes     Abrum          BarryHit\n"
				"^3Player           Flendo           Padaget        VideoP/Loda\n"
				"^3Noodle           Smoo             Tayst          QK-Lite/Effect.Ve\n"
				"^3Tom Arrow        MVSDK/JK2MV      x[Dsk]x Clan   =AJC= Clan\n\"");
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
		trap_SendServerCommand(ent - g_entities, "cp \"You can't drop a flag in this game type\"");
		return;
	}

	if ((ent->client->sess.sessionTeam == TEAM_RED && ent->client->ps.powerups[PW_BLUEFLAG] == 0) || 
		(ent->client->sess.sessionTeam == TEAM_BLUE && ent->client->ps.powerups[PW_REDFLAG] == 0))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You don't have any flag to drop\"");
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
	if (VALIDCVAR(jkcvar_serverMotd.string))
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
		g_entities[i].client->sess.jkmodSess.ignoredPlayer[2] &= ~(1 << ignored); // Emote
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
	int		option;
	char	arg1[MAX_TOKEN_CHARS];
	char	arg2[MAX_TOKEN_CHARS];

	trap_Argv(1, arg1, sizeof(arg1));
	trap_Argv(2, arg2, sizeof(arg2));

	if (!jkcvar_playerIgnore.integer) 
	{
		trap_SendServerCommand(ent - g_entities, "print \"This command is disabled by the server\n\"");
		return;
	}
	else if (trap_Argc() < 3)
	{
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Ignore ^5]^7\n"
			"^7Ignore a player chat or duel challenge\n"
			"^7You can use this feature using the following command: ^2/ignore <player|number|all> <option>\n"
			"^7To undo the changes use the same command again\n"
			"^5----------\n"
			"^7Option list:\n"
			"^3chat\n"
			"^3duel\n"
			"^3emote\n"
			"^5----------\n"
			"^2Note 1: ^7No need to use full name or color name, you can use just a part of it\n"
			"^2Note 2: ^7You can use the command ^3/whois ^7to check the player number and ignore status\n\"");
		return;
	}
	else if (!(!Q_stricmp(arg2, "chat") || !Q_stricmp(arg2, "duel") || !Q_stricmp(arg2, "emote")))
	{
		trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is not valid\n\"", arg2));
		return;
	}
	else 
	{
		option = !Q_stricmp(arg2, "emote") ? 2 : (!Q_stricmp(arg2, "duel") ? 1 : 0);

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

	if (ent->client->jkmodClient.chairModelUsed) return;

	// Check duel challenge
	if (jkcvar_altDimension.integer && ent->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Private duels are not allowed in this dimension\n\"");
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
			trap_SendServerCommand(ent - g_entities, "cp \"You can't challenge this player yet\"");
			return;
		}

		if (g_gametype.integer >= GT_TEAM && OnSameTeam(ent, challenged)) return;

		// Tr!Force: [Dimensions] Check duel challenge
		if (jkcvar_altDimension.integer && challenged->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE) return;

		// Tr!Force: [Ignore] Apply duel ignore
		if (JKMod_IgnoreClientCheck(1, challenged->s.number, ent->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"He doesn't want to be challenged\"");
			return;
		}
		if (JKMod_IgnoreClientCheck(1, ent->s.number, challenged->s.number))
		{
			trap_SendServerCommand(ent - g_entities, "cp \"You have ignored his challenges\"");
			return;
		}

		if (challenged->client->ps.duelIndex == ent->s.number && challenged->client->ps.duelTime >= level.time)
		{
			char *duelmessage;

			if (challenged->client->pers.jkmodPers.customDuel == DUEL_FORCE) {
				duelmessage = "Full force";
			} else {
				duelmessage = "Saber only";
			}

			trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s %s" S_COLOR_WHITE "! (%s%s)\n\"", challenged->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELACCEPT"), ent->client->pers.netname, duelmessage,  (challenged->client->sess.jkmodSess.privateDuel ? " private" : "")));

			// Enable duel
			ent->client->ps.duelInProgress = qtrue;
			challenged->client->ps.duelInProgress = qtrue;

			// Set dimension
			if ((jkcvar_altDimension.integer & DIMENSION_DUEL) && challenged->client->sess.jkmodSess.privateDuel)
			{
				unsigned dimensionCustom = JKMod_DimensionGetCustom();

				JKMod_DimensionSet(ent, dimensionCustom);
				JKMod_DimensionSet(challenged, dimensionCustom);
			}

			// Set custom settings
			if (challenged->client->pers.jkmodPers.customDuel == DUEL_FORCE) 
			{
				int checkSide = (1 << FP_ABSORB) | (1 << FP_HEAL) | (1 << FP_PROTECT) | (1 << FP_TELEPATHY) | (1 << FP_GRIP) | (1 << FP_DRAIN) | (1 << FP_LIGHTNING) | (1 << FP_RAGE);

				if (g_forcePowerDisable.integer & checkSide)
				{
					trap_SendServerCommand(ent - g_entities, "print \"^2Remember to change your force side from player menu.\n\"");
					trap_SendServerCommand(challenged - g_entities, "print \"^2Remember to change your force side from player menu.\n\"");
				}

				JKMod_DimensionSettings(ent, DIMENSION_FORCE);
				JKMod_DimensionSettings(challenged, DIMENSION_FORCE);
			}

			ent->client->ps.duelTime = level.time + 2000;
			challenged->client->ps.duelTime = level.time + 2000;

			// Reset hit count
			ent->client->pers.jkmodPers.duelHitCount = 0;
			challenged->client->pers.jkmodPers.duelHitCount = 0;

			G_AddEvent(ent, EV_PRIVATE_DUEL, 1);
			G_AddEvent(challenged, EV_PRIVATE_DUEL, 1);

			// Disable jetpack
			if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE) {
				ent->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
				ent->client->ps.stats[JK_FUEL] = 0;
				ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
			}
			if (challenged->client->ps.eFlags & JK_JETPACK_ACTIVE) {
				challenged->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
				challenged->client->ps.stats[JK_FUEL] = 0;
				challenged->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
			}

			// Custom start emote
			if (VALIDCVAR(jkcvar_duelStartEmote.string) && 
				JKMod_EmoteCheck(jkcvar_duelStartEmote.string, ent) && 
				JKMod_EmoteCheck(jkcvar_duelStartEmote.string, challenged))
			{
				duelEmote = qtrue;
			}

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
			qboolean privateDuel = ((jkcvar_altDimension.integer & DIMENSION_DUEL) && ent->client->sess.jkmodSess.privateDuel);

			if (jkcvar_allowCustomDuel.integer) 
			{
				if (type == DUEL_FORCE) 
				{
					// Print full force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (Full force%s)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE"), privateDuel ? " private" : ""));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (Full force%s)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname, privateDuel ? " private" : ""));
					
					ent->client->pers.jkmodPers.customDuel = DUEL_FORCE;
					challenged->client->pers.jkmodPers.customDuel = DUEL_FORCE;
				}
				else 
				{
					// Print full no-force duel initiation in private
					G_CenterPrint(challenged - g_entities, 3, va("%s" S_COLOR_WHITE " %s (Saber only%s)\n", ent->client->pers.netname, G_GetStripEdString("SVINGAME", "PLDUELCHALLENGE"), privateDuel ? " private" : ""));
					G_CenterPrint(ent - g_entities, 3, va("%s %s" S_COLOR_WHITE " (Saber only%s)\n", G_GetStripEdString("SVINGAME", "PLDUELCHALLENGED"), challenged->client->pers.netname, privateDuel ? " private" : ""));
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
	if (!jkcvar_allowCustomDuel.integer) {
		trap_SendServerCommand(ent - g_entities, "print \"Custom duels are ^3disabled ^7by the server. Applying normal duel...\n\"");
		JKMod_EngageDuel(ent, 0);
	} else {
		JKMod_EngageDuel(ent, 1);
	}
	return;
}

/*
=====================================================================
Check private room challenge
=====================================================================
*/
void JKMod_EngagePrivate(gentity_t* ent, qboolean say)
{
	char *print = say ? "cp" : "print";

	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Join the game first before switch dimension\"");
		return;
	}
	else if (ent->client->ps.pm_type == PM_DEAD)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension while dead\n\"");
		return;
	}
	else if (ent->client->ps.duelInProgress)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in a private duel\n\"");
		return;
	}
	else if (g_gametype.integer != GT_FFA)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in this game type\n\"");
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension during pause mode\n\"");
		return;
	}
	else if (jkcvar_altDimension.integer & DIMENSION_PRIVATE)
	{
		if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] >= level.time && !ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE]) return;
		
		if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] && !ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE]) 
		{
			trap_SendServerCommand(ent - g_entities, va("print \"You have left ^3private room (%i)^7\n\"", ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]));
			ARRAY_CLEAR(ent->client->pers.jkmodPers.privateRoom);
			JKMod_DimensionSet(ent, level.jkmodLocals.dimensionBase);
		}
		else 
		{
			int target = -1;

			if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] && ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] >= level.time) 
			{
				target = ent->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX];
			} 
			else 
			{
				trace_t tr;
				vec3_t forward, fwdOrg;
			
				AngleVectors(ent->client->ps.viewangles, forward, NULL, NULL);

				fwdOrg[0] = ent->client->ps.origin[0] + forward[0] * 256;
				fwdOrg[1] = ent->client->ps.origin[1] + forward[1] * 256;
				fwdOrg[2] = (ent->client->ps.origin[2] + ent->client->ps.viewheight) + forward[2] * 256;

				trap_Trace(&tr, ent->client->ps.origin, NULL, NULL, fwdOrg, ent->s.number, MASK_PLAYERSOLID);

				if (tr.fraction != 1 && tr.entityNum < MAX_CLIENTS) target = tr.entityNum;
			}

			if (target != -1)
			{
				gentity_t *challenged = &g_entities[target];

				if (!challenged || !challenged->client || !challenged->inuse ||
					challenged->health < 1 || challenged->client->ps.stats[STAT_HEALTH] < 1 ||
					challenged->client->ps.duelInProgress || challenged->client->ps.saberInFlight)
				{
					trap_SendServerCommand(ent - g_entities, va("%s \"You can't invite this player yet%s\"", print, (say ? "" : "\n")));
					return;
				}
				
				// Invite from private
				if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] && challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
				{
					unsigned privateNum = challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM];
					unsigned dimensionCustom = JKMod_DimensionToPrivate(privateNum);

					JKMod_JoinPrivate(ent, challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM], dimensionCustom);

					ent->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
					ent->client->ps.forceHandExtendTime = level.time + 1000;

					challenged->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = 0;
					challenged->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = 0;
					challenged->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = 0;

					trap_SendServerCommand(ent - g_entities, va("cp \"Private Room (%i)\nDimension\"", privateNum));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " has been accepted by %s" S_COLOR_WHITE " to join ^3private room (%i)\n\"", ent->client->pers.netname, challenged->client->pers.netname, privateNum));
				}
				// Accept new private
				else if (challenged->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] == ent->s.number && challenged->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] >= level.time)
				{
					unsigned dimensionCustom = JKMod_DimensionGetCustom();
					unsigned privateNum = JKMod_DimensionToPrivate(dimensionCustom);

					JKMod_SettingsPrivate(NULL, privateNum);
					JKMod_JoinPrivate(ent, privateNum, dimensionCustom);
					JKMod_JoinPrivate(challenged, privateNum, dimensionCustom);

					ent->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
					ent->client->ps.forceHandExtendTime = level.time + 1000;

					trap_SendServerCommand(ent - g_entities, va("cp \"Private Room (%i)\nDimension\"", privateNum));
					trap_SendServerCommand(challenged - g_entities, va("cp \"Private Room (%i)\nDimension\"", privateNum));
					trap_SendServerCommand(-1, va("print \"Private room (%i) was created by %s " S_COLOR_WHITE "and %s\n\"", privateNum, challenged->client->pers.netname, ent->client->pers.netname));
				}
				// Check private bots
				else if (!jkcvar_privateInviteBots.integer && (challenged->r.svFlags & SVF_BOT))
				{
					trap_SendServerCommand(ent - g_entities, va("%s \"Bots invites are disabled by the server%s\"", print, (say ? "" : "\n")));
					return;
				}
				// Engage new private
				else 
				{
					JKMod_InvitePrivate(ent, challenged);
				}
			}
			else
			{
				ent->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = 0;
				ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = 0;
				ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = 0;

				trap_SendServerCommand(ent - g_entities, va("%s \"You need someone in front of you to invite%s\"", print, (say ? "" : "\n")));
			}
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, "print \"Private dimensions are ^1disabled ^7by the server\n\"");
	}
}

/*
=====================================================================
Invite private room challenge
=====================================================================
*/
void JKMod_InvitePrivate(gentity_t *ent, gentity_t *challenged)
{
	int remainingTime = jkcvar_privateInviteTime.integer >= MIN_PRIVATE_TIME ? jkcvar_privateInviteTime.integer : MIN_PRIVATE_TIME;

	ent->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = challenged->s.number;
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = level.time + (remainingTime*1000);

	challenged->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = ent->s.number;
	challenged->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = level.time + (remainingTime*1000);
	challenged->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = remainingTime;

	if (!ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
	{
		ent->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
		ent->client->ps.forceHandExtendTime = level.time + 1000;
	}

	trap_SendServerCommand(ent - g_entities, va("cp \"You have invited\n%s" S_COLOR_WHITE "\nto create a private room\"", challenged->client->pers.netname));
	trap_SendServerCommand(challenged - g_entities, va("cp \"You have been invited by\n%s" S_COLOR_WHITE "\nto create a private room\nsay ^2!accept^7 in chat to join\"", ent->client->pers.netname));
}

/*
=====================================================================
Join private room challenge
=====================================================================
*/
void JKMod_JoinPrivate(gentity_t* ent, int privateNum, unsigned dimension)
{
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = 0;
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = 0;
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = 0;
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_REQUEST] = 0;
	ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] = privateNum;

	JKMod_DimensionSet(ent, dimension);
}

/*
=====================================================================
Settings for private room challenge
=====================================================================
*/
void JKMod_SettingsPrivate(gentity_t* ent, int privateNum)
{
	if (ent)
	{
		JKMod_CustomGameSettings(ent, 
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_WEAPONDISABLE],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCEDISABLE],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_HOLDABLES],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_JETPACK],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_INVULNERABILITY],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_PASSTHROUGH],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_SPEED],
			level.jkmodLocals.privateRoom[privateNum][PRIVATE_GRAVITY]
		);
	}
	else
	{
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_WEAPONDISABLE] = DEFAULT;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCEDISABLE] = DEFAULT;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL] = DEFAULT;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_HOLDABLES] = qfalse;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_JETPACK] = qfalse;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_INVULNERABILITY] = qfalse;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_PASSTHROUGH] = qfalse;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_SPEED] = DEFAULT;
		level.jkmodLocals.privateRoom[privateNum][PRIVATE_GRAVITY] = DEFAULT;
	}
}

/*
=====================================================================
Check private room players
=====================================================================
*/
int JKMod_PlayersPrivate(int privateRoom, qboolean skipBots)
{
	int i = 0;
	int pl = 0;

	while (i < MAX_CLIENTS)
	{
		if (g_entities[i].client && 
			g_entities[i].client->pers.connected == CON_CONNECTED && 
			g_entities[i].client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == privateRoom && (skipBots ? !(g_entities[i].r.svFlags & SVF_BOT) : qtrue))
		{
			pl++;
		}
		i++;
	}

	return pl;
}

/*
=====================================================================
Private challenge command
=====================================================================
*/
static void JKMod_Cmd_EngagePrivate(gentity_t* ent)
{
	JKMod_EngagePrivate(ent, qfalse);
	return;
}

/*
=====================================================================
Check private duel challenge
=====================================================================
*/
static void JKMod_Cmd_TogglePrivate(gentity_t* ent)
{
	if (jkcvar_altDimension.integer & DIMENSION_DUEL)
	{
		if (ent->client->sess.jkmodSess.privateDuel) {
			ent->client->sess.jkmodSess.privateDuel = qfalse;
			trap_SendServerCommand(ent - g_entities, "print \"Private duels are now ^1disabled\n\"");
		} else {
			ent->client->sess.jkmodSess.privateDuel = qtrue;
			trap_SendServerCommand(ent - g_entities, "print \"Private duels are now ^2enabled\n\"");
		}

		// Update clientside
		if (ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("jk_cg_privateDuel %i", (int)ent->client->sess.jkmodSess.privateDuel));
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, "print \"Private duels are ^1disabled ^7by the server\n\"");
	}
	return;
}

/*
=====================================================================
Check private duel challenge
=====================================================================
*/
static void JKMod_Cmd_ToggleAuto(gentity_t* ent)
{
	if (jkcvar_duelAutoAccept.integer)
	{
		if (ent->client->sess.jkmodSess.autoDuel) {
			ent->client->sess.jkmodSess.autoDuel = qfalse;
			trap_SendServerCommand(ent - g_entities, "print \"Auto duel challenges are now ^1disabled\n\"");
		} else {
			ent->client->sess.jkmodSess.autoDuel = qtrue;
			trap_SendServerCommand(ent - g_entities, "print \"Auto duel challenges are now ^2enabled\n\"");
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
Send toggle console function
=====================================================================
*/
void JKMod_Cmd_ToggleConsole(gentity_t *ent)
{
	if (ent->client->pers.jkmodPers.clientPlugin) {
		trap_SendServerCommand(ent - g_entities, "jk_cg_toggleConsole");
	}
}

/*
=====================================================================
Who is / status function
=====================================================================
*/
void JKMod_Cmd_WhoIs(gentity_t *ent)
{
	int			num;
	const char	*mapname = jkcvar_mapName.string;
	team_t		team;
	char		locked[MAX_STRING_CHARS] = { 0 };

	for (team = 0; team < TEAM_NUM_TEAMS; team++)
	{
		if (level.jkmodLocals.lockedTeam[team]) Q_strcat(locked, sizeof(locked), va(" %s,", JKMod_TeamName(team, CASE_NORMAL)));
	}

	if (locked[0]) locked[strlen(locked) - 1] = '\0';

	if (ent) 
	{
		trap_SendServerCommand(ent - g_entities, va("cp \""
			"You are in the ^3%s ^7dimension\n"
			"Open console for player list\"",
			(ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] ? va("Private (%i)", ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]) : JKModDimensionData[JKMod_DimensionIndex(ent->client->ps.stats[JK_DIMENSION])].name)));
		
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Who is ^5]^7\n"
			"^7List of all players connected on the server\n"
			"^7Client plugin status: ^2Valid^7, ^5Newer, ^3Older^7, ^1Invalid\n\"");

		if (locked[0]) trap_SendServerCommand(ent - g_entities, va("print \"Currently locked teams:%s\n\"", locked));
		
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5----------------------------------------------------------------------------------------------------------\n"
			"^7Num ^5|^7 Name                         ^5|^7 Type  ^5|^7 Ignored         ^5|^7 Dimension    ^5|^7 Plugin          ^5|^7 Game\n"
			"^5----------------------------------------------------------------------------------------------------------\n\"");
	}
	else
	{
		G_Printf("Map: %s\n", mapname);
		if (locked[0]) G_Printf("Locked Teams:%s\n", locked);
		G_Printf("Num Name                         Type  Dimension    Plugin          Game\n");
		G_Printf("--- ---------------------------- ----- ------------ --------------- ----------\n");
	}

	for (num = 0; num < level.maxclients; num++)
	{
		gentity_t 	*user = &g_entities[num];
		char		userinfo[MAX_INFO_VALUE] = { 0 };
		char        name[MAX_STRING_CHARS] = { 0 };
		char		ignored[MAX_STRING_CHARS] = { 0 };
		char		*value, *etjk2;
		char		*type;
		char		*dimension;
		char		*status;
		char		*plugin;
		char		*gameversion;

		// Check
		if (!user || !user->client || !user->inuse) continue;
		if (user->client->pers.connected == CON_DISCONNECTED) continue;

		// Get info
		trap_GetUserinfo(num, userinfo, sizeof(userinfo));

		// Find info
		etjk2 = Info_ValueForKey(userinfo, "cjp_client");
		value = Info_ValueForKey(userinfo, "JK2MV");

		if (strstr(value, "ETJK2") || strstr(etjk2, "JAPRO")) {
			gameversion = "EternalJK2";
;		} 
		else if (value[0] || (user->r.svFlags & SVF_BOT && mvapi)) {
			gameversion = "JK2MV";
		} 
		else {
			gameversion = "BaseJK2";
		}

		// Check plugin
		value = Info_ValueForKey(userinfo, "jkmod_client");

		if (value[0]) {
			plugin = value;
		} else {
			plugin = "No plugin";
		}

		// Check ignore
		if (ent)
		{
			if (JKMod_IgnoreClientCheck(2, ent->s.number, user->s.number)) {
				Q_strcat(ignored, sizeof(ignored), "Emote/");
			}
			if (JKMod_IgnoreClientCheck(1, ent->s.number, user->s.number)) {
				Q_strcat(ignored, sizeof(ignored), "Duel/");
			}
			if (JKMod_IgnoreClientCheck(0, ent->s.number, user->s.number)) {
				Q_strcat(ignored, sizeof(ignored), "Chat/");
			}
			if (ignored[0] == '\0') strcpy(ignored, "None/");
			
			ignored[strlen(ignored) - 1] = '\0';
		}

		// Set info
		strcpy(name, user->client->pers.netname);
		dimension = user->client->sess.sessionTeam == TEAM_SPECTATOR ? "Spectator" : (user->client->ps.duelInProgress ? "Private Duel" : (user->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] ? va("Private (%i)", user->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]) : va("%s", JKModDimensionData[JKMod_DimensionIndex(user->client->ps.stats[JK_DIMENSION])].name)));
		type = user->r.svFlags & SVF_BOT ? "Bot" : "Human";
		status = user->client->pers.jkmodPers.clientVersion ? (user->client->pers.jkmodPers.clientVersion == level.jkmodLocals.serverVersion ? S_COLOR_GREEN : (user->client->pers.jkmodPers.clientVersion > level.jkmodLocals.serverVersion ? S_COLOR_CYAN : S_COLOR_YELLOW)) : S_COLOR_RED;

		// Player print
		if (ent) {
			trap_SendServerCommand(ent - g_entities, va("print \"^7%-3i ^5|^7 %-28s ^5|^7 %-5s ^5|^7 %-15s ^5|^7 %-12s ^5|^7 %s%-15s ^5|^7 %-9s\n\"",
				num,
				Q_CleanStr(name, (qboolean)(jk2startversion == VERSION_1_02)),
				type,
				ignored,
				dimension,
				status,
				plugin,
				gameversion
			));
		// Server print
		} else {
			G_Printf("%-3i %-28s %-5s %-12s %-15s %-9s\n",
				num,
				Q_CleanStr(name, (qboolean)(jk2startversion == VERSION_1_02)),
				type,
				dimension,
				plugin,
				gameversion
			);
		}
	}

	if (ent) {
		trap_SendServerCommand(ent - g_entities, "print \"^5----------------------------------------------------------------------------------------------------------\n\"");
		trap_SendServerCommand(ent - g_entities, va("print \"Your position in ^3%s ^7is: ^2(^7%i^2) (^7%i^2) (^7%i^2) : (^7%i^2)\n\"",
			mapname,
			(int)ent->client->ps.origin[0],
			(int)ent->client->ps.origin[1],
			(int)ent->client->ps.origin[2],
			(int)ent->client->ps.viewangles[YAW]));
	} else if (level.numConnectedClients) {
		G_Printf("--- ---------------------------- ----- ------------ --------------- ----------\n");
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
	char arg1[MAX_TOKEN_CHARS];
	trap_Argv(1, arg1, sizeof(arg1));

	JKMod_DimensionChange(ent, arg1, NULL);
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
		trap_SendServerCommand(ent - g_entities, va("%s \"This teleport is not available%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport in spectator%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport during pause mode%s\"", print, (say ? "" : "\n")));
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

		trap_SendServerCommand(ent - g_entities, va("%s \"Saved position!%s\"", print, (say ? "" : "\n")));

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
		trap_SendServerCommand(ent - g_entities, va("%s \"This teleport is not available%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (!ent->client->pers.jkmodPers.teleportChatCheck)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You don't have any saved position%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport in spectator%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't teleport during pause mode%s\"", print, (say ? "" : "\n")));
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
	char cmd[MAX_TOKEN_CHARS];
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

	if (jkcvar_teleportChat.integer != 2 && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_RACE | DIMENSION_CHEAT | DIMENSION_PRIVATE)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"Save spawn is not available%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't save spawn in spectator%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't save spawn during pause mode%s\"", print, (say ? "" : "\n")));
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

		trap_SendServerCommand(ent - g_entities, va("%s \"Saved spawn point!%s\"", print, (say ? "" : "\n")));

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
		trap_SendServerCommand(ent - g_entities, va("%s \"Reset spawn is not available%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (!ent->client->pers.jkmodPers.customSpawnCheck)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You don't have any saved spawn%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, va("%s \"You can't reset spawn during pause mode%s\"", print, (say ? "" : "\n")));
		return qfalse;
	}
	else
	{
		ARRAY_CLEAR(ent->client->pers.jkmodPers.customSpawn);
		trap_SendServerCommand(ent - g_entities, va("%s \"Spawn point reset!%s\"", print, (say ? "" : "\n")));

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
	char cmd[MAX_TOKEN_CHARS];
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
	if (!jkcvar_jetPack.integer && !(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_GUNS | DIMENSION_CHEAT)) && !g_cheats.integer)
	{
		trap_SendServerCommand(ent - g_entities, "print \"This command is disabled by the server\n\"");
		return;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use jetpack in spectator\n\"");
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use jetpack during pause mode\n\"");
		return;
	}
	else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use jetpack in this dimension\n\"");
		return;
	}
	else if (ent->client->ps.duelInProgress)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use jetpack in a private duel\n\"");
		return;
	}
	else
	{
		// Disable
		if (ent->client->ps.eFlags & JK_JETPACK_ACTIVE)
		{
			ent->client->ps.eFlags &= ~(JK_JETPACK_ACTIVE | JK_JETPACK_FLAMING);
			ent->client->ps.stats[JK_FUEL] = 0;
			ent->client->pers.jkmodPers.jetpackFxDisplay = qfalse;
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, "print \"Jetpack disabled\n\"");
			return;
		}
		// Enable
		else
		{
			ent->client->ps.eFlags |= JK_JETPACK_ACTIVE;
			ent->client->ps.stats[JK_FUEL] = 100;
			ent->client->pers.jkmodPers.jetpackFxDisplay = qtrue;
			if (!ent->client->pers.jkmodPers.clientPlugin) trap_SendServerCommand(ent - g_entities, "print \"Jetpack equiped\n\"");
			trap_SendServerCommand(ent - g_entities, "cp \"Press USE button on air to enable\"");
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
	char arg1[MAX_TOKEN_CHARS];
	trap_Argv(1, arg1, sizeof(arg1));

	if (trap_Argc() < 2)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Usage: emote <animation>\nSee ^3/help emotes ^7for more information\n\"");
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
		trap_SendServerCommand(ent - g_entities, "print \"This command is disabled by the server\n\"");
		return;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use dual saber in spectator\n\"");
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use dual saber during pause mode\n\"");
		return;
	}
	else if (ent->client->ps.stats[JK_DIMENSION] == DIMENSION_RACE)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't use dual saber in this dimension\n\"");
		return;
	}
	else if (ent->client->ps.weapon != WP_SABER)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't enable it using this weapon\n\"");
		return;
	}
	else
	{
		// Checks
		if (!saberOffSound || !saberOnSound)
		{
			saberOffSound = G_SoundIndex("sound/weapons/saber/saberoffquick.wav");
			saberOnSound = G_SoundIndex("sound/weapons/saber/saberon.wav");
		}
		if (ent->client->ps.saberInFlight)
		{
			return;
		}
		if (ent->client->ps.forceHandExtend != HANDEXTEND_NONE)
		{
			return;
		}
		if (ent->client->ps.duelTime >= level.time)
		{
			return;
		}
		if (ent->client->ps.saberLockTime >= level.time)
		{
			return;
		}
		if (ent->client && ent->client->ps.weaponTime < 1)
		{
			// Disable
			if (ent->client->ps.dualBlade)
			{
				ent->client->ps.dualBlade = qfalse;
				ent->client->pers.jkmodPers.dualSaber = qfalse;
				ent->client->ps.saberHolstered = qtrue;
				ent->client->ps.weaponTime = 400;
				G_Sound(ent, CHAN_AUTO, saberOffSound);
				return;
			}
			// Enable
			else
			{
				ent->client->ps.dualBlade = qtrue;
				ent->client->pers.jkmodPers.dualSaber = qtrue;
				ent->client->ps.saberHolstered = qfalse;
				G_Sound(ent, CHAN_AUTO, saberOnSound);
				return;
			}
		}
	}
}

/*
=====================================================================
Taunt2 command function
=====================================================================
*/
static void JKMod_Cmd_Taunt2(gentity_t* ent)
{
	char sound[MAX_TOKEN_CHARS];

	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		return;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		return;
	}
	else if (ent->client->ps.torsoTimer < 1 && ent->client->ps.forceHandExtend == HANDEXTEND_NONE && ent->client->ps.legsTimer < 1 && ent->client->ps.weaponTime < 1 && (ent->client->ps.saberLockTime < ent->client->pers.cmd.serverTime || jk2gameplay == VERSION_1_02)) 
	{
		// Animation
		ent->client->ps.forceHandExtend = HANDEXTEND_TAUNT;
		ent->client->ps.forceDodgeAnim = BOTH_ENGAGETAUNT;
		ent->client->ps.forceHandExtendTime = ent->client->pers.cmd.serverTime + 1000;
		ent->client->ps.weaponTime = 100;

		if (jkcvar_allowTaunt2.integer) 
		{
			trap_Argv(1, sound, sizeof(sound));
			if (trap_Argc() < 2) Com_sprintf(sound, sizeof(sound), "taunt");

			G_MuteSound(ent->s.number, CHAN_VOICE);
			G_Sound(ent, CHAN_VOICE, G_SoundIndex(va("%s/%s", ent->client->pers.jkmodPers.modelSounds, sound)));
		}
		else
		{
			G_AddEvent(ent, EV_TAUNT, 0);
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
	orig[2] += ent->r.maxs[2] / 2;
	VectorMA(orig, distance, fwd, dest);

	trap_Trace(&tr, orig, mins, maxs, dest, ent->s.number, MASK_ALL);

	if (tr.allsolid || tr.startsolid || tr.fraction != 1.0f)
	{
		gentity_t	*found = &g_entities[tr.entityNum];
		char		 found_mins[20];
		char		 found_maxs[20];
		char		 found_angles[20];
		char		 found_origin[40];

		JKMod_G_TestLine(orig, dest, 255, linedelay, ent->s.number);

		if (!found->inuse) return;
				
		Q_strncpyz(found_mins, va("%.0f %.0f %.0f", found->r.mins[0], found->r.mins[1], found->r.mins[2]), sizeof(found_mins));
		Q_strncpyz(found_maxs, va("%.0f %.0f %.0f", found->r.maxs[0], found->r.maxs[1], found->r.maxs[2]), sizeof(found_maxs));
		Q_strncpyz(found_angles, va("%.0f %.0f %.0f", found->s.angles[0], found->s.angles[1], found->s.angles[2]), sizeof(found_angles));
		Q_strncpyz(found_origin, va("%.0f %.0f %.0f", found->s.origin[0], found->s.origin[1], found->s.origin[2]), sizeof(found_origin));

		trap_SendServerCommand(ent - g_entities, va("print \"\n"
			"^7Classname: ^3%-40.40s ^7Target:     ^3%-25.25s ^7Angles: ^3%-20.20s\n"
			"^7Model:     ^3%-40.40s ^7Targetname: ^3%-25.25s ^7Mins:   ^3%-20.20s\n"
			"^7Origin:    ^3%-40.40s ^7Spawnflags: ^3%-25.25i ^7Maxs:   ^3%-20.20s\n\"", 
			found->classname,
			found->target,
			found_angles,
			found->model,
			found->targetname,
			found_mins,
			found_origin,
			found->spawnflags,
			found_maxs));

		JKMod_DrawBoxLines(found->s.origin, found->r.mins,  found->r.maxs, 255, boxdelay, ent->s.number);
		return;
	}

	JKMod_G_TestLine(orig, dest, 0, linedelay, ent->s.number);
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

	Q_strcat(message, sizeof(message), "^7Teleports available for this map:");

	for (i = 0; i < level.jkmodLocals.teleportChatsCount; i++)
	{
		strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));

		if (Q_stricmp(map, jkcvar_mapName.string) == 0)
		{
			strcpy(command, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "command"));

			Q_strcat(message, sizeof(message), va("\n^3%s", command));

			found = qtrue;
		}
	}
	
	trap_SendServerCommand(ent - g_entities, va("cp \"%s\"", (found ? message : "This map doesn't have chat teleports")));
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

		if (Q_stricmp(text, command) == 0)
		{
			strcpy(map, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "map"));

			if (Q_stricmp(map, jkcvar_mapName.string) == 0)
			{
				if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
				{
					trap_SendServerCommand(ent - g_entities, "cp \"You can't teleport in spectator\"");
					return qfalse;
				}
				else if (level.jkmodLocals.pauseTime > level.time)
				{
					trap_SendServerCommand(ent - g_entities, "cp \"You can't teleport during pause mode\"");
					return qfalse;
				}
				else if (JKMod_PlayerMoving(ent, qtrue, qtrue))
				{
					trap_SendServerCommand(ent - g_entities, "cp \"You can't teleport while moving\"");
					return qfalse;
				}
				else if (ent->client->ps.pm_type == PM_DEAD)
				{
					trap_SendServerCommand(ent - g_entities, "print \"You can't teleport while dead\n\"");
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

					// Parse
					strcpy(origin, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "origin"));
					strcpy(rotation, Info_ValueForKey(level.jkmodLocals.teleportChats[i], "rotation"));
					sscanf(origin, "%f %f %f %i", &realorigin[0], &realorigin[1], &realorigin[2], &realrotation);

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

					JKMod_TeleportPlayer(ent, temporigin, tempangles, qtrue, 300, jkcvar_teleportChatFx.string, jkcvar_teleportChatSound.string);
					ent->client->jkmodClient.teleportChatUsed = jkcvar_teleportChatTime.integer ? qfalse : qtrue;
					return qtrue;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, "cp \"You need to respawn to teleport again\"");
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
	qboolean	argValid = qfalse;
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
	jkmod_callvote_t callVote[] =
	{
		// bitmask				option				hint
		{ VOTE_MAP_RESTART,		"map_restart",		NULL },				
		{ VOTE_NEXTMAP,			"nextmap",			"<map name>" },		
		{ VOTE_MAP,				"map",				"<map name>" },		
		{ VOTE_G_GAMETYPE,		"g_gametype",		"<number>" },			
		{ VOTE_KICK,			"kick",				"<player name>" },		
		{ VOTE_CLIENTKICK,		"clientkick",		"<client number>" },	
		{ VOTE_G_DOWARMUP,		"g_doWarmup",		NULL },				
		{ VOTE_TIMELIMIT,		"timelimit",		"<seconds>" },		
		{ VOTE_FRAGLIMIT,		"fraglimit",		"<kills>" },			
		{ VOTE_POLL,			"poll",				"<poll question>" },			
		{ VOTE_SCORE_RESTART,	"score_restart",	NULL },				
		{ VOTE_PAUSE,			"pause",			"<seconds>" },
		{ VOTE_MAP_NUMBER,		"mapnum",			"<map number>" },		
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

	if (trap_Argc() < 2)
	{
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Call Vote ^5]^7\n"
			"^7You can perform a call vote within the other players to change some server options\n"
			"^7You can do it using the following command: ^2/callvote <option> <value>\n"
			"^5----------\n"
			"^7Option:        Value:\n\"");

		for (i = 0; i < ARRAY_LEN(callVote); i++)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"%s%-15s%s\n\"", ((jkcvar_voteControl.integer & (1 << callVote[i].bitmask)) ? S_COLOR_YELLOW : S_COLOR_RED), callVote[i].option, (callVote[i].hint ? callVote[i].hint : "")));
		}

		trap_SendServerCommand(ent - g_entities, "print \""
			"^5----------\n"
			"^2Note: ^7Options marked in ^1red ^7are not available to use\n\"");
		return;
	}
	
	if (level.numVotingClients < jkcvar_voteMinPlayers.integer) {
		trap_SendServerCommand(ent - g_entities, va("print \"You need atleast %i players to call a vote\n\"", jkcvar_voteMinPlayers.integer));
		return;
	}

	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR) {
		trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "NOSPECVOTE")));
		return;
	}

	trap_Argv(1, arg1, sizeof(arg1));
	trap_Argv(2, arg2, sizeof(arg2));

	if( strchr( arg1, ';' ) || strchr( arg2, ';' ) || strchr( arg1, '\n' ) || strchr( arg2, '\r' ) ) {
		trap_SendServerCommand(ent - g_entities, "print \"Invalid vote string\n\"");
		return;
	}

	for (i = 0; i < ARRAY_LEN(callVote); i++)
	{
		if (!Q_stricmp(arg1, callVote[i].option)) {
			argValid = qtrue;
			break;
		} 
	}

	// check valid option
	if (!argValid) {
		trap_SendServerCommand(ent - g_entities, va("print \"Voting not allowed for %s\n\"", arg1));
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
			trap_SendServerCommand(ent - g_entities, va("print \"%s\"", G_GetStripEdString("SVINGAME", "NOVOTE_MAPNOTSUPPORTEDBYGAME")));
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
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Kick %s from server", Q_CleanStr(g_entities[n].client->pers.netname, (qboolean)(jk2startversion == VERSION_1_02)));
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
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Kick %s from server", Q_CleanStr(g_entities[clientid].client->pers.netname, (qboolean)(jk2startversion == VERSION_1_02)));
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
	// Score restart
	else if (!Q_stricmp(arg1, "score_restart"))
	{
		int		val;
		char	*action;

		if (!(jkcvar_voteControl.integer & (1 << VOTE_SCORE_RESTART))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		Com_sprintf(level.voteString, sizeof(level.voteString), "resetscores");
		Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Restart all players scores");
	}
	// Pause game
	else if (!Q_stricmp(arg1, "pause"))
	{
		if (!(jkcvar_voteControl.integer & (1 << VOTE_PAUSE))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (arg2[0] == '\0') {
			Com_sprintf(level.voteString, sizeof(level.voteString), "pause 300");
			Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Pause game for 5 minutes");
		} else {
			if (JKMod_ValidNumber(arg2)) {
				Com_sprintf(level.voteString, sizeof(level.voteString), "pause %s", arg2);
				Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Pause game for %s seconds", arg2);
			} else {
				trap_SendServerCommand(ent - g_entities, "print \"The specified value is not a number\n");
				return;
			}
		}
	}
	// Map number
	else if (!Q_stricmp(arg1, "mapnum"))
	{
		int					i, len, numMaps;
		char				unsortedMaps[4096];
		char				*sortedMaps[512];
		char				*mapName;
		unsigned int		mapCount = 0;
		unsigned int		mapSinglePlayer = 0;
		const unsigned int	maxMaps = 512;
		qboolean			mapValid = qfalse;

		if (!(jkcvar_voteControl.integer & (1 << VOTE_MAP_NUMBER))) {
			trap_SendServerCommand(ent - g_entities, "print \"This vote option is not allowed on this server\n\"");
			return;
		}

		if (!JKMod_ValidNumber(arg2)) {
			trap_SendServerCommand(ent - g_entities, "print \"Invalid map number\n\"");
			return;
		}

		numMaps = trap_FS_GetFileList("maps", ".bsp", unsortedMaps, sizeof(unsortedMaps));

		if (numMaps) 
		{
			if (numMaps > maxMaps) numMaps = maxMaps;

			mapName = unsortedMaps;

			for (i = 0; i < numMaps; i++) 
			{
				len = strlen(mapName);
				if (!Q_stricmp(mapName+len-4, ".bsp")) mapName[len-4] = '\0';
				sortedMaps[i] = mapName;
				mapName += len+1;
			}
	
			qsort(sortedMaps, numMaps, sizeof(sortedMaps[0]), JKMod_CompcStr);
		
			for (i = 0; i < numMaps; i++) 
			{
				mapSinglePlayer = JKMod_SPMapCheck(sortedMaps[i]);
			
				if (mapSinglePlayer && !(jkcvar_mapFixes.integer & JK_MAP_SPVOTE)) {
					mapCount = mapCount > 0 ? mapCount-- : 0;
					continue;
				}

				mapCount++;
				
				if (atoi(arg2) == mapCount) {
					mapValid = qtrue;
					break;
				}
			}

			if (mapValid) {
				if (!G_DoesMapSupportGametype(sortedMaps[i], trap_Cvar_VariableIntegerValue("g_gametype"))) {
					trap_SendServerCommand(ent - g_entities, va("print \"%s\"", G_GetStripEdString("SVINGAME", "NOVOTE_MAPNOTSUPPORTEDBYGAME")));
					return;
				} else {
					Com_sprintf(level.voteString, sizeof(level.voteString), "map %s", sortedMaps[i]);
					Com_sprintf(level.voteDisplayString, sizeof(level.voteDisplayString), "Change map to %s", sortedMaps[i]);
				}
			} else {
				trap_SendServerCommand(ent - g_entities, "print \"Map number not found\n\"");
				return;
			}
		}
	}
	// Others
	else
	{
		if ((!Q_stricmp(arg1, "g_dowarmup") && !(jkcvar_voteControl.integer & (1 << VOTE_G_DOWARMUP))) ||
			(!Q_stricmp(arg1, "timelimit") && !(jkcvar_voteControl.integer & (1 << VOTE_TIMELIMIT))) ||
			(!Q_stricmp(arg1, "map_restart") && !(jkcvar_voteControl.integer & (1 << VOTE_MAP_RESTART))) ||
			(!Q_stricmp(arg1, "fraglimit") && !(jkcvar_voteControl.integer & (1 << VOTE_FRAGLIMIT))))
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Voting for ^3%s ^7is disabled by the server\n\"", arg1));
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
	int chatColor = ent->client->sess.jkmodSess.chatColor;
	int scoreColor = 5, lostColor = 3, healthColor = 1, shieldColor = 2;
	qboolean isCTF = g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY;
	qboolean isDuel = g_gametype.integer == GT_TOURNAMENT;

	if (ent->client->ps.pm_type == PM_DEAD) return qfalse;
	if (!(jkcvar_chatColors.integer && (chatColor && chatColor <= 7) && !(g_gametype.integer >= GT_TEAM && chatColor == 5))) chatColor = 2;
	if (chatColor == 1) healthColor = 5;
	if (chatColor == 3) lostColor = 1;
	if (chatColor == 5) scoreColor = 2;

	if (announce)
	{
		if (ent->client->pers.jkmodPers.playerStatusDelay <= level.time) 
		{
			ent->client->pers.jkmodPers.playerStatusDelay = level.time + (jkcvar_chatAutoStatusTime.integer*1000);

			G_Say(ent, NULL, SAY_ALL, va("I %s ^%i%i ^%iand %s ^%i%i ^%itimes! My health %s ^%i%i^7/^%i%i",
				(isCTF || isDuel ? "scored" : "killed"), 
				scoreColor,
				(isDuel ? ent->client->sess.wins : ent->client->ps.persistant[PERS_SCORE]),
				chatColor,
				(isCTF ? "captured" : (isDuel ? "lost" : "died")), 
				lostColor,
				(isCTF ? ent->client->ps.persistant[PERS_CAPTURES] : ent->client->ps.persistant[PERS_KILLED]),
				chatColor,
				(jkcvar_chatAutoStatus.integer || isDuel ? "was" : "is"),
				healthColor,
				ent->client->ps.stats[STAT_HEALTH],
				shieldColor,
				ent->client->ps.stats[STAT_ARMOR])
			);
		}

		if (!jkcvar_chatAutoStatus.integer && !ent->client->sess.jkmodSess.playerStatusSeen)
		{
			ent->client->sess.jkmodSess.playerStatusSeen = qtrue;
			trap_SendServerCommand(ent - g_entities, "print \"Auto status tracking is ^1disabled ^7by the server\n\"");
		}

		return !announce;
	}
	else
	{
		if (ent->client->sess.jkmodSess.playerStatus) {
			ent->client->sess.jkmodSess.playerStatus = qfalse;
			trap_SendServerCommand(ent - g_entities, "cp \"Auto status tracking is now ^1disabled\"");
		} else {
			ent->client->sess.jkmodSess.playerStatus = qtrue;
			trap_SendServerCommand(ent - g_entities, "cp \"Auto status tracking is now ^2enabled\"");
		}

		if (ent->client->sess.jkmodSess.playerStatusSeen) ent->client->sess.jkmodSess.playerStatusSeen = qfalse;

		// Update clientside
		if (ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("jk_cg_chatAutoStatus %i", (int)ent->client->sess.jkmodSess.playerStatus));
		}

		return !announce && ent->client->sess.jkmodSess.playerStatus;
	}
}

/*
=====================================================================
Show current server maps
=====================================================================
*/
static void JKMod_Cmd_MapList(gentity_t *ent)
{
	int					i, len, numMaps;
	char				unsortedMaps[4096];
	char				*sortedMaps[512];
	char				*mapName, *mapNameClean;
	char				*mapType;
	unsigned int		mapCount = 0;
	unsigned int		mapSinglePlayer = 0;
	const unsigned int	mapNameLimit = 24, maxMaps = 512;
	qboolean			mapLine = qfalse;

	numMaps = trap_FS_GetFileList("maps", ".bsp", unsortedMaps, sizeof(unsortedMaps));

	if (numMaps) 
	{
		trap_SendServerCommand(ent - g_entities, "print \""
			"^5[^7 Map List ^5]^7\n"
			"^7List of all existing maps on the server\n"
			"^7Map type: ^2Custom^7, ^3Multiplayer, ^6Single Player^7, ^1Single Player (Special)\n"
			"^5----------\n\"");

		if (numMaps > maxMaps) numMaps = maxMaps;

		mapName = unsortedMaps;

		for (i = 0; i < numMaps; i++) 
		{
			len = strlen(mapName);
			if (!Q_stricmp(mapName+len-4, ".bsp")) mapName[len-4] = '\0';
			sortedMaps[i] = mapName;
			mapName += len+1;
		}
	
		qsort(sortedMaps, numMaps, sizeof(sortedMaps[0]), JKMod_CompcStr);
		
		for (i = 0; i < numMaps; i++) 
		{
			mapType = S_COLOR_GREEN;
			mapSinglePlayer = JKMod_SPMapCheck(sortedMaps[i]);
			
			if (mapSinglePlayer) {
				if (jkcvar_mapFixes.integer & JK_MAP_SPVOTE) {
					mapType = mapSinglePlayer == 2 ? S_COLOR_RED : S_COLOR_MAGENTA;
				} else {
					mapCount = mapCount > 0 ? mapCount-- : 0;
					continue;
				}
			}
			else if (JKMod_MPMapCheck(sortedMaps[i])) {
				mapType = S_COLOR_YELLOW;
			}

			mapNameClean = Q_CleanStr(sortedMaps[i], qfalse);
			if (strlen(mapNameClean) > mapNameLimit) mapNameClean[mapNameLimit] = '\0';
			mapLine = (mapCount % 4 == 3 || mapCount == (numMaps-1));
			mapCount++;
			trap_SendServerCommand(ent - g_entities, va("print \"^7[%s%03d^7] %-25s%s\"", mapType, mapCount, mapNameClean, (mapLine ? "\n" : "")));
		}

		trap_SendServerCommand(ent - g_entities, va("print \""
			"%s^5----------\n"
			"^7%i available maps listed (Out of a total of %i)\n"
			"^2Note 1: ^7Use ^3/callvote mapnum <number> ^7for voting by number\n"
			"^2Note 2: ^7Availability of ^1Single Player ^7maps depends of server settings\n\"", (mapLine ? "" : "\n"), mapCount, numMaps));
	}
}

/*
=====================================================================
Custom chat color
=====================================================================
*/
static void JKMod_Cmd_ChatColor(gentity_t* ent)
{
	if (jkcvar_chatColors.integer)
	{
		int i, num;
		int selectedColor = 0;
		const char *chatColor[] = {
			"Red",
			"Green",
			"Yellow",
			"Blue",
			"Cyan",
			"Magenta",
			"White",
		};
		char arg1[MAX_TOKEN_CHARS];
		trap_Argv(1, arg1, sizeof(arg1));

		if (trap_Argc() < 2)
		{
			trap_SendServerCommand(ent - g_entities, "print \""
				"^5[^7 Chat Color ^5]^7\n"
				"^7Change the color of your chat messages\n"
				"^7Use it with the following command: ^2/changecolor <color>\n"
				"^7List of available colors:\n"
				"^5----------\n"
				"^11 - Red\n"
				"^22 - Green\n"
				"^33 - Yellow\n"
				"^44 - Blue\n"
				"^55 - Cyan\n"
				"^66 - Magenta\n"
				"^77 - White\n"
				"^5----------\n"
				"^2Note 1: ^5Cyan ^7color is disabled in team game types (CTF & CTY)\n"
				"^2Note 2: ^7To disable this use ^3/chatcolor disable ^7to reset to default\n\"");
			return;
		}

		if (!Q_stricmp(arg1, "disable")) 
		{
			ent->client->sess.jkmodSess.chatColor = 0;
			trap_SendServerCommand(ent - g_entities, "print \"Your chat color has reset to default\n\"");
			
			// Update clientside
			if (ent->client->pers.jkmodPers.clientPlugin) {
				trap_SendServerCommand(ent - g_entities, "jk_cg_chatColor 0");
			}
			return;
		}

		for (i = 0; i < ARRAY_LEN(chatColor); i++) 
		{
			num = i+1;
			if (atoi(arg1) == num || !Q_stricmp(arg1, chatColor[i])) {
				selectedColor = num;
				break;
			}
		}

		if (!selectedColor) {
			trap_SendServerCommand(ent - g_entities, "print \"This option is not valid\n");
			return;
		}

		ent->client->sess.jkmodSess.chatColor = selectedColor;
		trap_SendServerCommand(ent - g_entities, va("print \"Your chat color has changed to ^%i%s\n\"", selectedColor, chatColor[selectedColor-1]));

		// Update clientside
		if (ent->client->pers.jkmodPers.clientPlugin) {
			trap_SendServerCommand(ent - g_entities, va("jk_cg_chatColor %i", selectedColor));
		}
	}
	else
	{
		trap_SendServerCommand(ent - g_entities, va("print \"Chat colors are ^1disabled ^7by the server\n\""));
	}
	return;
}

/*
=====================================================================
Bot control function
=====================================================================
*/
void JKMod_botControl(int botIndex, int ownerIndex, char *action)
{
	gentity_t *bot = &g_entities[botIndex];
	gentity_t *owner = &g_entities[ownerIndex];

	if ((bot->client->pers.jkmodBots.actionFlags & JK_BOT_CONTROL) || !Q_stricmp(action, "remove"))
	{
		bot->client->pers.jkmodBots.actionFlags &= ~JK_BOT_CONTROL;
		ARRAY_CLEAR(bot->client->pers.jkmodPers.botControl);
		ARRAY_CLEAR(owner->client->pers.jkmodPers.botControl);

		JKMod_Printf(S_COLOR_MAGENTA "Removing bot %i control from owner %i\n", botIndex, ownerIndex);
	}
	else if (!Q_stricmp(action, "apply") || !Q_stricmp(action, "toggle"))
	{
		// Bot
		bot->client->pers.jkmodBots.actionFlags |= JK_BOT_CONTROL;
		bot->client->pers.jkmodPers.botControl[BOT_ENABLED] = qfalse;	
		bot->client->pers.jkmodPers.botControl[BOT_INDEX] = owner->s.number;	
		// Owner
		owner->client->pers.jkmodPers.botControl[BOT_ENABLED] = qtrue;
		owner->client->pers.jkmodPers.botControl[BOT_INDEX] = bot->s.number;	
		// Message
		if (owner->client->sess.sessionTeam == TEAM_SPECTATOR) 
		{
			trap_SendServerCommand(ownerIndex, va("print \"Now controlling bot %i (%s)...\n\"", bot->s.number, bot->client->pers.netname));
			trap_SendServerCommand(ownerIndex, "cp \"" NEWLINES "Press USE button + JUMP to stop controlling\"");
		}
		
		JKMod_Printf(S_COLOR_MAGENTA "Applying bot %i control from owner %i\n", botIndex, ownerIndex);
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
	// Show whois
	else if (Q_stricmp(p, "!where") == 0 || Q_stricmp(p, "!whois") == 0)
	{
		JKMod_Cmd_WhoIs(ent);
		JKMod_Cmd_ToggleConsole(ent);
	}
	// Private check
	else if (Q_stricmpn(p, "!accept", 7) == 0)
	{
		int argNum = JKMod_Str_Argc(p);
			
		if (argNum < 2 && ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] && ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] >= level.time)
		{
			JKMod_EngagePrivate(ent, qtrue);
		} 
		else if (argNum > 1 && !ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] && ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
		{
			int target;
			char arg1[MAX_TOKEN_CHARS];
			JKMod_Str_Argv(1, arg1, sizeof(arg1), p);
			target = JKMod_CheckValidClient(ent, arg1);

			if (target != -1)
			{
				gentity_t *challenged = &g_entities[target];

				if (challenged->client->pers.jkmodPers.privateRoom[PRIVATE_REQUEST] == ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]) 
				{
					unsigned privateNum = ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM];
					unsigned dimensionCustom = JKMod_DimensionToPrivate(privateNum);

					JKMod_JoinPrivate(challenged, privateNum, dimensionCustom);

					challenged->client->ps.forceHandExtend = HANDEXTEND_DUELCHALLENGE;
					challenged->client->ps.forceHandExtendTime = level.time + 1000;
					
					trap_SendServerCommand(challenged - g_entities, va("cp \"Private Room (%i)\nDimension\"", privateNum));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " has been accepted by %s" S_COLOR_WHITE " to join ^3private room (%i)\n\"", challenged->client->pers.netname, ent->client->pers.netname, privateNum));
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("print \"There are no requests by %s\n\"", challenged->client->pers.netname));
				}
			}
		}
		else 
		{
			ent->client->pers.jkmodPers.privateRoom[PRIVATE_INDEX] = 0;
			ent->client->pers.jkmodPers.privateRoom[PRIVATE_TIME] = 0;
			ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE] = 0;

			trap_SendServerCommand(ent - g_entities, "cp \"You have nothing to accept\"");
		}
		return;
	}
	// Bot check
	else if (Q_stricmp(p, "!bot") == 0 || Q_stricmpn(p, "!bot ", 5) == 0)
	{
		if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
		{
			int argNum = JKMod_Str_Argc(p);
			char argHelp[MAX_STRING_CHARS] = ""
				"Action list:\n"
				"^3static\n"
				"^3attack\n"
				"^3altattack\n"
				"^3crouch\n"
				"^3jump\n"
				"^3taunt\n"
				"^3talk\n"
				"^3god\n"
				"^3control\n"
				"^3togglesaber\n"
				"^3teleport\n"
				"^3reset";
	
			if (argNum < 2)
			{
				trap_SendServerCommand(ent - g_entities, va("cp \"Usage: !bot ^2<number> <action>\n%s\"", argHelp));
				trap_SendServerCommand(ent - g_entities, "print \"Note: ^7You can use the command ^3/whois ^7to check the bot number\n\"");
				return;
			} 
			else if (argNum > 1)
			{
				int target;
				char arg1[MAX_TOKEN_CHARS];
				JKMod_Str_Argv(1, arg1, sizeof(arg1), p);
				target = JKMod_CheckValidClient(ent, arg1);

				if (target != -1)
				{
					gentity_t *challenged = &g_entities[target];

					if (!(challenged->r.svFlags & SVF_BOT)) 
					{
						trap_SendServerCommand(ent - g_entities, "cp \"This player is not a bot!\"");
						return;
					}
					if (challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]) 
					{
						if (argNum < 3)
						{
							trap_SendServerCommand(ent - g_entities, va("cp \"Usage: !bot %i ^2<action>\n%s\"", challenged->s.number, argHelp));
							return;
						}
						else
						{
							char arg2[MAX_TOKEN_CHARS];
							JKMod_Str_Argv(2, arg2, sizeof(arg2), p);

							if (!Q_stricmp(arg2, "static")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_STATIC) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_STATIC;
							}
							else if (!Q_stricmp(arg2, "attack")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_ATTACK) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_ATTACK;
							}
							else if (!Q_stricmp(arg2, "altattack")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_ATTACK_ALT) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_ATTACK_ALT;
							}
							else if (!Q_stricmp(arg2, "crouch")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_CROUCH) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_CROUCH;
							}
							else if (!Q_stricmp(arg2, "jump")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_JUMP) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_JUMP;
							}
							else if (!Q_stricmp(arg2, "taunt")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_TAUNT) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_TAUNT;
							}
							else if (!Q_stricmp(arg2, "talk")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_TALK) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_TALK;
							}
							else if (!Q_stricmp(arg2, "god")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_GOD) ? "Removing" : "Applying"), arg2, challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags ^= JK_BOT_GOD;
							}
							else if (!Q_stricmp(arg2, "control")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"%s %s on bot %i\"", ((challenged->client->pers.jkmodBots.actionFlags & JK_BOT_CONTROL) ? "Removing" : "Applying"), arg2, challenged->s.number));
								JKMod_botControl(challenged->s.number, ent->s.number, "toggle");
							}
							else if (!Q_stricmp(arg2, "togglesaber")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"Turning %s saber on bot %i\"", (!challenged->client->ps.saberHolstered ? "off" : "on"), challenged->s.number));
								Cmd_ToggleSaber_f(challenged);
							}
							else if (!Q_stricmp(arg2, "teleport")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"Teleporting bot %i\"", challenged->s.number));
								trap_SendConsoleCommand(EXEC_APPEND, va("teleport %i %i\n", challenged->s.number, ent->s.number));
							}
							else if (!Q_stricmp(arg2, "reset")) {
								trap_SendServerCommand(ent - g_entities, va("cp \"Resetting bot %i\"", challenged->s.number));
								challenged->client->pers.jkmodBots.actionFlags = 0;
								JKMod_botControl(challenged->s.number, ent->s.number, "remove");
							}
							else {
								trap_SendServerCommand(ent - g_entities, va("cp \"Usage: !bot %i ^2<action>\n%s\"", challenged->s.number, argHelp));
								return;
							}
						}
					}
					else if (jkcvar_privateInviteBots.integer)
					{
						trap_SendServerCommand(ent - g_entities, va("cp \"This bot is not in your private room\nUse: ^2!dimension invite %i\"", challenged->s.number));
						return;
					}
					else
					{
						trap_SendServerCommand(ent - g_entities, "cp \"Bots invites are disabled by the server\"");
						return;
					}
				}
			}
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, "cp \"You can't use this outside a private room\"");
			return;
		}
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
			trap_SendServerCommand(ent - g_entities, "cp \"You must be in race mode\"");
			return;
		}

		Q_strncpyz(timeLastStr, JKMod_MsToString(timeLast), sizeof(timeLastStr));
		Q_strncpyz(timeBestStr, JKMod_MsToString(timeBest), sizeof(timeBestStr));

		if (!timeLast && !timeBest)
			trap_SendServerCommand(ent - g_entities, "cp \"You don't have any record\"");
		else if (timeLast <= timeBest)
			G_Say(ent, NULL, SAY_ALL, va("^7Last Time: ^2%s", timeLastStr));
		else
			G_Say(ent, NULL, SAY_ALL, va("^7Last Time: ^2%s ^7Record: ^5%s", timeLastStr, timeBestStr));

		return;
	}
	// Dimension trigger
	else if (Q_stricmpn(p, "!dim", 4) == 0)
	{
		char arg1[MAX_TOKEN_CHARS];
		JKMod_Str_Argv(1, arg1, sizeof(arg1), p);

		if (!JKMod_DimensionChange(ent, arg1, p)) return;
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
	{ "admin",					JKMod_Cmd_HelpInfo },
	{ "dropflag",				JKMod_Cmd_DropFlag },
	{ "motd",					JKMod_Cmd_ShowMotd },
	{ "ignore",					JKMod_Cmd_IgnoreClient },
	
	{ "engage_force",			JKMod_Cmd_EngageDuel },
	{ "engage_forceduel",		JKMod_Cmd_EngageDuel },
	{ "engage_fullforceduel",	JKMod_Cmd_EngageDuel },
	{ "engage_duel_force",		JKMod_Cmd_EngageDuel },
	{ "engage_ff",				JKMod_Cmd_EngageDuel },
	{ "engage_private",			JKMod_Cmd_EngagePrivate },
	{ "toggle_auto",			JKMod_Cmd_ToggleAuto },
	{ "toggle_private",			JKMod_Cmd_TogglePrivate },

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
	{ "taunt2",					JKMod_Cmd_Taunt2 },
	{ "maplist",				JKMod_Cmd_MapList },
	{ "chatcolor",				JKMod_Cmd_ChatColor },
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
		if (!Q_stricmp(cmd, command->name) || !Q_stricmp(cmd, va("am%s", command->name)))
		{
			command->func(ent);
			return;
		}
	}

	// Special case for emotes
	for (i = 0; i < JKModEmotesDataSize; i++)
	{
		if (!Q_stricmp(cmd, va("am%s", JKModEmotesData[i].cmd)))
		{
			JKMod_EmotePlay(ent, JKModEmotesData[i].emoteIndex);
			return;
		}
	}

	// Test command
	if (!Q_stricmp(cmd, "test") && developer.integer)
	{
		int		i;
		int		args = trap_Argc();
		char	argcmd[MAX_TOKEN_CHARS];
		char	arglist[MAX_STRING_CHARS] = { 0 };

		trap_Argv(1, argcmd, sizeof(argcmd));

		// Entity scan
		if (!Q_stricmp(argcmd, "scan")) 
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

		// Effect play
		if (!Q_stricmp(argcmd, "effect")) 
		{
			char arg2[MAX_TOKEN_CHARS];
			char arg3[MAX_TOKEN_CHARS];

			trap_Argv(2, arg2, sizeof(arg2));
			trap_Argv(3, arg3, sizeof(arg3));

			JKMod_G_PlayEffect_ID(G_EffectIndex(arg2), ent->client->ps.origin, ent->client->ps.viewangles, ent->s.number, atoi(arg3));
			return;
		}

		// Check args
		for (i = 0; i < args; i++) 
		{
			if (i != 0) {
				trap_Argv(i, argcmd, sizeof(argcmd));
				Q_strcat(arglist, sizeof(arglist), va("Arg%i: %s\n", i, argcmd));
			}
		}

		trap_SendServerCommand(ent - g_entities, va("print \"Developer: %i\nCvarTest1: %s\nCvarTest2: %i\n%s\"", developer.integer, jkcvar_test1.string, jkcvar_test2.integer, arglist));
		return;
	}

	// Launch original client command function
	BaseJK2_ClientCommand(clientNum);
}
