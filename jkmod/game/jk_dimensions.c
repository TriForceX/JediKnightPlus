/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Main dimension stuff
=====================================================================
*/

#include "../../code/game/g_local.h" // Main header

// Undo re-routing for calls made from here
#undef trap_Trace

// Re-declare trace function
void trap_Trace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

// Extern stuff
extern int G_ParseInfos(char *buf, int max, char *infos[]);

/*
=====================================================================
Dimension data table
=====================================================================
*/
jkmod_dimension_data_t JKModDimensionData[] =
{
	// dimension		weapondisable	forcedisable	forcelevel		holdables	jetpack		invulnerability	passthrough	speed	 gravity	command		name
    { DIMENSION_FREE,	DEFAULT,		DEFAULT,		DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		DEFAULT, DEFAULT,	"normal",	"Normal" },
    { DIMENSION_GUNS,	6,				262141,			FORCE_LEVEL_1,	qtrue,		qtrue,		qfalse,			qfalse,		250,	 800,		"guns",		"Guns Arena" },
	{ DIMENSION_RACE,	65531,			229373,			FORCE_LEVEL_1,	qfalse,		qfalse,		qtrue,			qtrue,		250,	 800,		"race",		"Race Defrag" },
	{ DIMENSION_SABER,	65531,			163837,			FORCE_LEVEL_1,	qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"saber",	"Saber Only" },
	{ DIMENSION_FORCE,	65531,			6144,			FORCE_LEVEL_3,	qfalse,		qfalse,		qfalse,			qfalse,		250,	 800,		"force",	"Full Force" },
	{ DIMENSION_INSTA,	65503,			262141,			FORCE_LEVEL_3,	qfalse,		qfalse,		qfalse,			qfalse,		415,	 800,		"insta",	"Insta Kill" },
	{ DIMENSION_CHEAT,	DEFAULT,		DEFAULT,		DEFAULT,		qfalse,		qfalse,		qfalse,			qfalse,		DEFAULT, DEFAULT,	"cheats",	"Cheats Mode" },
};

int JKModDimensionDataSize = ARRAY_LEN(JKModDimensionData);

/*
=====================================================================
Dimension index from table
=====================================================================
*/
int JKMod_DimensionIndex(unsigned dimension)
{
	int	i;

	for (i = 0; i < JKModDimensionDataSize; i++)
	{
		if (JKModDimensionData[i].dimension == dimension) return i;
	}
	
	return 0;
}

/*
=====================================================================
Dimension settings function
=====================================================================
*/
void JKMod_DimensionSettings(gentity_t *ent, unsigned dimension)
{
	if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] && dimension >= (1 << DIMENSION_MAX))
	{
		int dim = JKMod_DimensionToPrivate(dimension);
		JKMod_SettingsPrivate(ent, dim);
	}
	else 
	{
		int dim = JKMod_DimensionIndex(dimension);
		JKMod_CustomGameSettings(ent, 
			JKModDimensionData[dim].weapondisable,		// Weapons
			JKModDimensionData[dim].forcedisable,		// Force disable
			JKModDimensionData[dim].forcelevel,			// Force level
			JKModDimensionData[dim].holdables,			// Holdable items
			JKModDimensionData[dim].jetpack,			// Jetpack
			JKModDimensionData[dim].invulnerability,	// Invulnerability
			JKModDimensionData[dim].passthrough,		// Pass-through
			JKModDimensionData[dim].speed,				// Speed
			JKModDimensionData[dim].gravity				// Gravity
		);
	}
}

/*
=====================================================================
Load custom dimension settings function
=====================================================================
*/
static char *JKModDimensionFile[MAX_TOKEN_CHARS];
static int JKModDimensionFileCount;

void JKMod_DimensionLoad(void)
{
	if (g_gametype.integer == GT_FFA)
	{
		char *lineStart = JKMod_ReadFile("config/dimension_data.cfg");

		JKModDimensionFileCount = 0;

		if (lineStart[0]) JKModDimensionFileCount += G_ParseInfos(lineStart, MAX_TOKEN_CHARS - JKModDimensionFileCount, &JKModDimensionFile[JKModDimensionFileCount]);

		if (JKModDimensionFile[0]) 
		{
			int			i = 0;
			char		*data = NULL;
			char		name[MAX_TOKEN_CHARS] = { 0 };
			int			weapondisable = 0;
			int			forcedisable = 0;
			int			forcelevel = 0;
			qboolean	holdables = 0;
			qboolean	jetpack = 0;
			qboolean	invulnerability = 0;
			qboolean	passthrough = 0;
			int			speed = 0;
			int			gravity = 0;

			// Parse data
			for (i = 0; i < JKModDimensionFileCount; i++)
			{
				int dim = 0;
				int num = i+1;

				strcpy(name, Info_ValueForKey(JKModDimensionFile[i], "name"));

				data = Info_ValueForKey(JKModDimensionFile[i], "weapondisable");
				weapondisable = !Q_stricmp(data, "DEFAULT") ? DEFAULT : atoi(data);

				data = Info_ValueForKey(JKModDimensionFile[i], "forcedisable");
				forcedisable = !Q_stricmp(data, "DEFAULT") ? DEFAULT : atoi(data);

				data = Info_ValueForKey(JKModDimensionFile[i], "forcelevel");
				forcelevel = strstr(data, "3") ? FORCE_LEVEL_3 : (strstr(data, "2") ? FORCE_LEVEL_2 : (strstr(data, "1") ? FORCE_LEVEL_1 : (strstr(data, "0") ? FORCE_LEVEL_0 : DEFAULT)));

				data = Info_ValueForKey(JKModDimensionFile[i], "holdables");
				holdables = !Q_stricmp(data, "true") ? qtrue : qfalse;

				data = Info_ValueForKey(JKModDimensionFile[i], "jetpack");
				jetpack = !Q_stricmp(data, "true") ? qtrue : qfalse;

				data = Info_ValueForKey(JKModDimensionFile[i], "invulnerability");
				invulnerability = !Q_stricmp(data, "true") ? qtrue : qfalse;
				
				data = Info_ValueForKey(JKModDimensionFile[i], "passthrough");
				passthrough = !Q_stricmp(data, "true") ? qtrue : qfalse;

				data = Info_ValueForKey(JKModDimensionFile[i], "speed");
				speed = !Q_stricmp(data, "DEFAULT") ? DEFAULT : atoi(data);

				data = Info_ValueForKey(JKModDimensionFile[i], "gravity");
				gravity = !Q_stricmp(data, "DEFAULT") ? DEFAULT : atoi(data);

				for (dim = 1; dim < JKModDimensionDataSize; dim++)
				{
					if (!Q_stricmp(JKModDimensionData[dim].name, name)) 
					{
						JKModDimensionData[dim].weapondisable = weapondisable;
						JKModDimensionData[dim].forcedisable = forcedisable;
						JKModDimensionData[dim].forcelevel = forcelevel;
						JKModDimensionData[dim].holdables = holdables;
						JKModDimensionData[dim].jetpack = jetpack;
						JKModDimensionData[dim].invulnerability = invulnerability;
						JKModDimensionData[dim].passthrough = passthrough;
						JKModDimensionData[dim].speed = speed;
						JKModDimensionData[dim].gravity = gravity;
						break;
					}
				}
			}

			G_Printf("%i dimension settings loaded\n", JKModDimensionFileCount);

			// Update clients
			if (level.numConnectedClients)
			{
				int i = 0;
				gentity_t *ent;

				for (i = 0, ent = g_entities; i < MAX_CLIENTS; ++i, ++ent)
				{
					if (ent && ent->client && ent->client->pers.connected != CON_DISCONNECTED && ent->client->ps.stats[JK_DIMENSION] != DIMENSION_FREE)
					{
						int dim = JKMod_DimensionIndex(ent->client->ps.stats[JK_DIMENSION]);

						JKMod_CustomGameSettings(ent, 
							JKModDimensionData[dim].weapondisable,
							JKModDimensionData[dim].forcedisable,
							JKModDimensionData[dim].forcelevel,
							JKModDimensionData[dim].holdables,
							JKModDimensionData[dim].jetpack,
							JKModDimensionData[dim].invulnerability,
							JKModDimensionData[dim].passthrough,
							JKModDimensionData[dim].speed,
							JKModDimensionData[dim].gravity
						);
					}
				}
			}
		}
	}
}

/*
=====================================================================
Set dimension private function
=====================================================================
*/
static void JKMod_DimensionPrivate(gentity_t *ent, char *option, char *args)
{
	qboolean say = args[0];
	char *print = say ? "cp" : "print";

	// Default command
	if (!Q_stricmp(option, "private"))
	{
		JKMod_EngagePrivate(ent, say);
	}

	// Modify settings
	if (!Q_stricmp(option, "settings")) 
	{
		char arg2[MAX_TOKEN_CHARS];
		JKMod_Str_Argv(2, arg2, sizeof(arg2), args);

		if (!ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
		{
			trap_SendServerCommand(ent - g_entities, "print \"You are not in any private room\n\"");
		}
		else if (JKMod_Str_Argc(args) < 3)
		{
			trap_SendServerCommand(ent - g_entities, "print \"Usage: /dimension settings <modify|view>\n\"");
		}
		else
		{
			unsigned privateNum = ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM];
			unsigned dimensionCustom = JKMod_DimensionToPrivate(privateNum);

			if (!Q_stricmp(arg2, "modify")) 
			{
				char arg3[MAX_TOKEN_CHARS];
				char arg4[MAX_TOKEN_CHARS];
				qboolean modified = qfalse;

				JKMod_Str_Argv(3, arg3, sizeof(arg3), args);
				JKMod_Str_Argv(4, arg4, sizeof(arg4), args);

				if (!Q_stricmp(arg3, "weapondisable") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_WEAPONDISABLE] = !Q_stricmp(arg4, "DEFAULT") ? DEFAULT : atoi(arg4);
				}
				else if (!Q_stricmp(arg3, "forcedisable") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCEDISABLE] = !Q_stricmp(arg4, "DEFAULT") ? DEFAULT : atoi(arg4);
				}
				else if (!Q_stricmp(arg3, "forcelevel") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL] = strstr(arg4, "3") ? FORCE_LEVEL_3 : (strstr(arg4, "2") ? FORCE_LEVEL_2 : (strstr(arg4, "1") ? FORCE_LEVEL_1 : (strstr(arg4, "0") ? FORCE_LEVEL_0 : DEFAULT)));
				}
				else if (!Q_stricmp(arg3, "holdables") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_HOLDABLES] = !Q_stricmp(arg4, "true") ? qtrue : qfalse;
				}
				else if (!Q_stricmp(arg3, "jetpack") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_JETPACK] = !Q_stricmp(arg4, "true") ? qtrue : qfalse;
				}
				else if (!Q_stricmp(arg3, "invulnerability") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_INVULNERABILITY] = !Q_stricmp(arg4, "true") ? qtrue : qfalse;
				}
				else if (!Q_stricmp(arg3, "passthrough") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_PASSTHROUGH] = !Q_stricmp(arg4, "true") ? qtrue : qfalse;
				}
				else if (!Q_stricmp(arg3, "speed") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_SPEED] = !Q_stricmp(arg4, "DEFAULT") ? DEFAULT : atoi(arg4);
				}
				else if (!Q_stricmp(arg3, "gravity") && arg4[0]) {
					modified = qtrue; level.jkmodLocals.privateRoom[privateNum][PRIVATE_GRAVITY] = !Q_stricmp(arg4, "DEFAULT") ? DEFAULT : atoi(arg4);
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, va("%s \""
						"Usage: %sdimension settings modify ^2<option> <value>%s"
						"^3weapondisable\n"
						"^3forcedisable\n"
						"^3forcelevel\n"
						"^3holdables\n"
						"^3jetpack\n"
						"^3invulnerability\n"
						"^3passthrough\n"
						"^3speed\n"
						"^3gravity%s\"", print, (say ? "\n!" : "/"), (say ? "\n" : "\n^7Options:\n"), (say ? "" : "\n")));

					trap_SendServerCommand(ent - g_entities, "print \"Use ^5/dimension settings view ^7to see current values\n\"");
				}

				// Update clients
				if (level.numConnectedClients && modified)
				{
					int i = 0;
					gentity_t *challenged;

					for (i = 0, challenged = g_entities; i < MAX_CLIENTS; ++i, ++challenged)
					{
						if (challenged && challenged->client && challenged->client->pers.connected != CON_DISCONNECTED && challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == privateNum)
						{
							JKMod_SettingsPrivate(challenged, privateNum);
							trap_SendServerCommand(challenged - g_entities, va("print \"Private room settings updated! ^5(%s changed to %s)\n\"", arg3, arg4));
							if (!Q_stricmp(arg3, "forcedisable")) trap_SendServerCommand(challenged - g_entities, "print \"^2Remember to change your force side from player menu.\n\"");
						}
					}
					
					if (level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL] == DEFAULT && !Q_stricmp(arg3, "forcedisable")) 
					{
						trap_SendServerCommand(ent - g_entities, "print \"Note: Remember to set a ^3forcelevel ^7to apply changes on ^3forcedisable\n\"");
					}
				}
			}
			else
			{
				char weapondisable[MAX_STRING_CHARS] = { 0 };
				char forcedisable[MAX_STRING_CHARS] = { 0 };
				char forcelevel[MAX_STRING_CHARS] = { 0 };
				char speed[MAX_STRING_CHARS] = { 0 };
				char gravity[MAX_STRING_CHARS] = { 0 };

				Com_sprintf(weapondisable, sizeof(weapondisable), "^5%i", level.jkmodLocals.privateRoom[privateNum][PRIVATE_WEAPONDISABLE]);
				Com_sprintf(forcedisable, sizeof(forcedisable), "^5%i", level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCEDISABLE]);
				Com_sprintf(forcelevel, sizeof(forcelevel), "^5%i", level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL]);
				Com_sprintf(speed, sizeof(speed), "^5%i", level.jkmodLocals.privateRoom[privateNum][PRIVATE_SPEED]);
				Com_sprintf(gravity, sizeof(gravity), "^5%i", level.jkmodLocals.privateRoom[privateNum][PRIVATE_GRAVITY]);

				trap_SendServerCommand(ent - g_entities, va("print \""
					"^5[^7 Private Room %i ^5]^7\n"
					"^7There are currently ^2%i ^7players in this room\n"
					"^7Next current room settings values can be numbers, true, false or default\n"
					"^5----------\n"
					"^3weapondisable:   %s\n"
					"^3forcedisable:    %s\n"
					"^3forcelevel:      %s\n"
					"^3holdables:       %s\n"
					"^3jetpack:         %s\n"
					"^3invulnerability: %s\n"
					"^3passthrough:     %s\n"
					"^3speed:           %s\n"
					"^3gravity:         %s\n"
					"^5----------\n"
					"^2Note 1: ^7Force powers and weapons requires to calculate points in bit values\n"
					"^2Note 2: ^7From holdables to passthrough the values must be ^2true ^7or ^1false\n\"", 
					privateNum,
					JKMod_PlayersPrivate(privateNum, qfalse),
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_WEAPONDISABLE] == DEFAULT ? "^6default" : weapondisable,
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCEDISABLE] == DEFAULT ? "^6default" : forcedisable,
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_FORCELEVEL] == DEFAULT ? "^6default" : forcelevel,
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_HOLDABLES] ? "^2true" : "^1false",
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_JETPACK] ? "^2true" : "^1false",
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_INVULNERABILITY] ? "^2true" : "^1false",
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_PASSTHROUGH] ? "^2true" : "^1false",
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_SPEED] == DEFAULT ? "^6default" : speed,
					level.jkmodLocals.privateRoom[privateNum][PRIVATE_GRAVITY] == DEFAULT ? "^6default" : gravity
				));
			}
		}
	}

	// Invite & kick dimension
	if (!Q_stricmp(option, "invite") || !Q_stricmp(option, "kick"))
	{
		int target;
		char arg2[MAX_TOKEN_CHARS];
		
		if (JKMod_Str_Argc(args) < 3)
		{
			trap_SendServerCommand(ent - g_entities, va("print \"Usage: /dimension %s <number|name>\nNote: ^7You can use the command ^3/whois ^7to check the dimension number\n\"", option));
		}
		else if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
		{
			JKMod_Str_Argv(2, arg2, sizeof(arg2), args);
			target = JKMod_CheckValidClient(ent, arg2);

			if (target != -1)
			{
				gentity_t *challenged = &g_entities[target];

				if (!Q_stricmp(option, "invite"))
				{
					if (!challenged || !challenged->client || !challenged->inuse ||
					challenged->health < 1 || challenged->client->ps.stats[STAT_HEALTH] < 1 ||
					challenged->client->ps.duelInProgress || challenged->client->ps.saberInFlight)
					{
						trap_SendServerCommand(ent - g_entities, "print \"You can't invite this player yet\n\"");
						return;
					}

					if (!jkcvar_privateInviteBots.integer && (challenged->r.svFlags & SVF_BOT))
					{
						trap_SendServerCommand(ent - g_entities, "cp \"Bots invites are disabled by the server\"");
						return;
					}

					if (challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] != ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]) {
						JKMod_InvitePrivate(ent, challenged);
					} else {
						trap_SendServerCommand(ent - g_entities, "print \"This player is already in your private room\n\"");
					}
				}
				else if (challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
				{
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " has kicked by %s" S_COLOR_WHITE " from ^3private room (%i)\n\"", challenged->client->pers.netname, ent->client->pers.netname, ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]));
					ARRAY_CLEAR(challenged->client->pers.jkmodPers.privateRoom);
					JKMod_DimensionSet(challenged, level.jkmodLocals.dimensionBase);
				}
			}
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, "print \"You are not in any private room\n\"");
		}
	}

	// Join dimension
	if (!Q_stricmp(option, "join")) 
	{
		char arg2[MAX_TOKEN_CHARS];
		JKMod_Str_Argv(2, arg2, sizeof(arg2), args);
		
		if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] && ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == atoi(arg2))
		{
			trap_SendServerCommand(ent - g_entities, va("print \"You are already in ^3private room (%i)\n\"", ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]));
		}
		else if (JKMod_Str_Argc(args) < 3)
		{
			trap_SendServerCommand(ent - g_entities, "print \"Usage: /dimension join <number>\nNote: ^7You can use the command ^3/whois ^7to check the dimension number\n\"");
		}
		else if (!ent->client->pers.jkmodPers.privateRoom[PRIVATE_INVITE])
		{
			int i;
			int privateNum = atoi(arg2);
			int foundBot = -1;
			qboolean foundPlayer = qfalse;
			gentity_t *challenged;

			for (i = 0, challenged = g_entities; i < MAX_CLIENTS; ++i, ++challenged)
			{
				if (challenged && challenged->client && 
					challenged->client->pers.connected != CON_DISCONNECTED && 
					challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] && 
					challenged->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] == privateNum)
				{
					trap_SendServerCommand(challenged - g_entities, va("print \"%s" S_COLOR_WHITE " wants to join your private room, allow him by using ^2/dimension invite %i^7 or say ^2!accept %i\n\"", ent->client->pers.netname, ent->s.number, ent->s.number));
					trap_SendServerCommand(challenged - g_entities, va("cp \"Theres an invite request by\n%s" S_COLOR_WHITE "\nto join your private room\nsay ^2!accept %i^7 to let him in\"", ent->client->pers.netname, ent->s.number));
					if (!foundPlayer) foundPlayer = qtrue;
					if (foundBot == -1 && (challenged->r.svFlags & SVF_BOT)) foundBot = challenged->s.number;
				}
			}

			if (foundPlayer) {
				ent->client->pers.jkmodPers.privateRoom[PRIVATE_REQUEST] = privateNum;
				if (foundBot != -1 && !JKMod_PlayersPrivate(privateNum, qtrue)) {
					trap_EA_Say(foundBot, va("!accept %i", ent->s.number));
					trap_EA_Say(foundBot, va("You can configure me by saying !bot %i help", foundBot));
				} else {
					trap_SendServerCommand(ent - g_entities, "print \"Request to join has been sent, wait for someone to accept you\n\"");
				}
			} else {
				trap_SendServerCommand(ent - g_entities, "print \"This private room doesn't exist\n\"");
			}
		}
	}

	// Leave dimension
	if (!Q_stricmp(option, "leave"))
	{
		if (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM])
		{
			trap_SendServerCommand(-1, va("print \"%s ^7left the ^3private room (%i)\n\"", ent->client->pers.netname, ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]));
			ARRAY_CLEAR(ent->client->pers.jkmodPers.privateRoom);
			JKMod_DimensionSet(ent, level.jkmodLocals.dimensionBase);
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, "print \"You are not in any private room\n\"");
		}
	}
}

/*
=====================================================================
Set dimension change function
=====================================================================
*/
qboolean JKMod_DimensionChange(gentity_t *ent, char *dimension, char *say)
{
	int			i;
	char		*print = say[0] ? "cp" : "print";
	char		message[MAX_STRING_CHARS] = { 0 };
	qboolean	baseCmd = qfalse;

	if (!jkcvar_altDimension.integer)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Dimensions are disabled by the server\n\"");
		return qfalse;
	}
	else if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, "cp \"Join the game first before switch dimension\"");
		return qfalse;
	}
	else if (ent->client->ps.pm_type == PM_DEAD)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension while dead\n\"");
		return qfalse;
	}
	else if (ent->client->ps.duelInProgress)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in a private duel\n\"");
		return qfalse;
	}
	else if (g_gametype.integer != GT_FFA)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension in this game type\n\"");
		return qfalse;
	}
	else if (level.jkmodLocals.pauseTime > level.time)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't change dimension during pause mode\n\"");
		return qfalse;
	}
	else if (ent->client->jkmodClient.dimensionTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds before change dimension\n\"", ent->client->jkmodClient.dimensionTime));
		return qfalse;
	}
	else
	{
		// Private?
		if (!Q_stricmp(dimension, "private") ||
			!Q_stricmp(dimension, "settings") ||
			!Q_stricmp(dimension, "invite") ||
			!Q_stricmp(dimension, "kick") ||
			!Q_stricmp(dimension, "join") ||
			!Q_stricmp(dimension, "leave"))
		{
			if (!(jkcvar_altDimension.integer & DIMENSION_PRIVATE)) 
			{
				trap_SendServerCommand(ent - g_entities, "print \"Private dimensions are ^1disabled ^7by the server\n\"");
				return qfalse;
			}
			else
			{
				JKMod_DimensionPrivate(ent, dimension, say);
				return qfalse;
			}
		}

		// Base?
		if (!Q_stricmp(dimension, "base")) baseCmd = qtrue;
		
		// Message
		Q_strcat(message, sizeof(message), "^7Usage: ^2!dimension <option>\n^7Option list:");

		// Start
		for (i = 0; i < JKModDimensionDataSize; i++)
		{
			// Add to message
			Q_strcat(message, sizeof(message), va("\n%s%s", (jkcvar_altDimension.integer & JKModDimensionData[i].dimension ? S_COLOR_YELLOW : S_COLOR_RED), JKModDimensionData[i].command));

			// Command
			if (!Q_stricmp(dimension, JKModDimensionData[i].command) || baseCmd)
			{
				int checkSide = (1 << FP_ABSORB) | (1 << FP_HEAL) | (1 << FP_PROTECT) | (1 << FP_TELEPATHY) | (1 << FP_GRIP) | (1 << FP_DRAIN) | (1 << FP_LIGHTNING) | (1 << FP_RAGE);
				int	dimensionBase = level.jkmodLocals.dimensionBase;
				int	dimensionBaseIndex = JKMod_DimensionIndex(dimensionBase);
				int	dimensionIndex = JKModDimensionData[baseCmd ? dimensionBaseIndex : i].dimension;
				char *dimensionName = JKModDimensionData[baseCmd ? dimensionBaseIndex : i].name;

				// Delay
				ent->client->jkmodClient.dimensionTime = jkcvar_altDimensionTime.integer;

				// Check
				if (!(jkcvar_altDimension.integer & dimensionIndex))
				{
					trap_SendServerCommand(ent - g_entities, "print \"This dimension is disabled by server\n\"");
					return qfalse;
				}
				// Enable
				else if (ent->client->ps.stats[JK_DIMENSION] != dimensionIndex)
				{
					JKMod_DimensionSet(ent, dimensionIndex);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\"", dimensionName));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s ^3%s ^7dimension\n\"", ent->client->pers.netname, (dimensionIndex != dimensionBase ? "joined the" : "back to"), dimensionName));
					if (dimensionIndex == DIMENSION_FORCE && (g_forcePowerDisable.integer & checkSide)) trap_SendServerCommand(ent - g_entities, "print \"^2Remember to change your force side from player menu.\n\"");
					return qtrue;
				}
				// Disable
				else if (dimensionIndex != dimensionBase)
				{
					JKMod_DimensionSet(ent, dimensionBase);
					trap_SendServerCommand(ent - g_entities, va("cp \"%s\nDimension\"", JKModDimensionData[dimensionBaseIndex].name));
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " back to ^3%s ^7dimension\n\"", ent->client->pers.netname, JKModDimensionData[dimensionBaseIndex].name));
					if (dimensionBase == DIMENSION_FORCE && (g_forcePowerDisable.integer & checkSide)) trap_SendServerCommand(ent - g_entities, "print \"^2Remember to change your force side from player menu.\n\"");
					return qtrue;
				}
				else
				{
					trap_SendServerCommand(ent - g_entities, "print \"You are already in this dimension\n\"");
					return qfalse;
				}
			}
		}

		// Check private
		Q_strcat(message, sizeof(message), va("\n%s%s", (jkcvar_altDimension.integer & DIMENSION_PRIVATE ? S_COLOR_YELLOW : S_COLOR_RED), "private"));

		// Not found
		trap_SendServerCommand(ent - g_entities, va("%s \"%s\"", print, (say[0] ? message : "Usage: dimension <option>\nSee ^3/help dimensions ^7for more information\n")));
		return qfalse;
	}
}

/*
=====================================================================
Dimension basic player/entity check
=====================================================================
*/
qboolean JKMod_DimensionCheck(int ent1, int ent2)
{
	int num1 = g_entities[ent1].jkmodEnt.dimensionNumber;
	int num2 = g_entities[ent2].jkmodEnt.dimensionNumber;
	
	if (ent1 == ENTITYNUM_WORLD) return qtrue;
	if (ent2 == ENTITYNUM_WORLD) return qtrue;
	if (ent1 == ent2) return qtrue;

	return (num1 == num2);
}

/*
=====================================================================
Dimension snapshot ignore and owner check
=====================================================================
*/
void JKMod_DimensionOwnerCheck(int owner, gentity_t *ent)
{
	assert(owner >= 0 && owner < MAX_GENTITIES);

	if (owner >= MAX_CLIENTS && owner < ENTITYNUM_MAX_NORMAL) {
		owner = g_entities[owner].jkmodEnt.dimensionOwner;
		assert(owner < MAX_CLIENTS || owner == ENTITYNUM_NONE || owner == ENTITYNUM_WORLD);
	}

	ent->jkmodEnt.dimensionOwner = owner;

	if (ent - g_entities < MAX_CLIENTS) {
		ent->jkmodEnt.dimensionNumber = level.jkmodLocals.dimensionBase;
	} else {
		ent->jkmodEnt.dimensionNumber = DIMENSION_ALL;
	}

	if (mvapi)
	{
		uint8_t *snapshotIgnore = mv_entities[ent->s.number].snapshotIgnore;
		int	i;

		if (owner == ENTITYNUM_WORLD) {
			memset(snapshotIgnore, 0, sizeof(snapshotIgnore[0]) * MAX_CLIENTS);
		} else {
			for (i = 0; i < level.maxclients; i++) {
				if (g_entities[i].inuse) {
					snapshotIgnore[i] = !JKMod_DimensionCheck(owner, i);
				}
			}
		}
	}
}

/*
=====================================================================
Dimension get custom number
=====================================================================
*/
unsigned JKMod_DimensionGetCustom(void)
{
	unsigned dimension;
	qboolean available;
	int i;

	for (dimension = (1 << DIMENSION_MAX); dimension != 0; dimension++) 
	{
		available = qtrue;
		for (i = 0; i < level.maxclients; i++) 
		{
			if (!g_entities[i].inuse) continue;
			if (g_entities[i].jkmodEnt.dimensionNumber == dimension) 
			{
				available = qfalse;
				break;
			}
		}
		if (available) return dimension;
	}

	assert(0);
	return level.jkmodLocals.dimensionBase;
}

/*
=====================================================================
Check dimension and private numbers
=====================================================================
*/
unsigned JKMod_DimensionToPrivate(int number)
{
	if (number >= (1 << DIMENSION_MAX)) {
		return (number - (1 << DIMENSION_MAX) + 1);
	} else {
		return (number + (1 << DIMENSION_MAX) - 1);
	}
}

/*
=====================================================================
Dimension snapshot ignore apply
=====================================================================
*/
void JKMod_DimensionSet(gentity_t *ent, unsigned dimension)
{
	int	i;
	int	clientNum = ent->s.number;
	vec3_t spawnOrigin, spawnAngles;
	qboolean spawnActivated = qfalse;

	if (ent->client->ps.stats[JK_DIMENSION] != dimension) 
	{
		// Clear private room
		if (dimension < (1 << DIMENSION_MAX)) ARRAY_CLEAR(ent->client->pers.jkmodPers.privateRoom);

		// Clear chair model
		if (ent->client->jkmodClient.chairModelUsed) JKMod_ChairModelDisable(ent);

		// Set new dimension
		ent->client->ps.stats[JK_DIMENSION] = ent->client->ps.duelInProgress ? DIMENSION_DUEL : (ent->client->pers.jkmodPers.privateRoom[PRIVATE_NUM] ? DIMENSION_PRIVATE : dimension);
		ent->jkmodEnt.dimensionNumber = dimension;

		// Check saber
		if (ent->client->ps.saberEntityNum != ENTITYNUM_NONE) g_entities[ent->client->ps.saberEntityNum].jkmodEnt.dimensionNumber = dimension;

		// Remove bot control
		if (ent->client->pers.jkmodPers.botControl[BOT_ENABLED]) JKMod_botControl(ent->client->pers.jkmodPers.botControl[BOT_INDEX], clientNum, "remove");

		// Remove controlled bot
		if (ent->client->pers.jkmodBots.actionFlags & JK_BOT_CONTROL) JKMod_botControl(clientNum, ent->client->pers.jkmodPers.botControl[BOT_INDEX], "remove");

		// Clear bots stuff
		ARRAY_CLEAR(ent->client->pers.jkmodPers.botControl);
		ent->client->pers.jkmodBots.actionFlags = 0;

		// Clear saved teleport
		ARRAY_CLEAR(ent->client->pers.jkmodPers.teleportChat);
		ent->client->pers.jkmodPers.teleportChatCheck = 0;
		
		// Clear custom spawn
		ARRAY_CLEAR(ent->client->pers.jkmodPers.customSpawn);
		ent->client->pers.jkmodPers.customSpawnCheck = 0;

		// Update settings
		ent->client->pers.jkmodPers.customSettingsCount++;

		// Set settings
		JKMod_DimensionSettings(ent, dimension);

		// Check emotes
		if (!ent->client->ps.duelInProgress) {
			ent->client->ps.forceHandExtend = HANDEXTEND_NONE;
			ent->client->ps.forceDodgeAnim = 0;
			ent->client->ps.forceHandExtendTime = 0;
		}
		
		// Set random spawn point
		if ((jkcvar_altDimensionSpawn.integer & ent->client->ps.stats[JK_DIMENSION]) &&
			ent->client->sess.sessionTeam != TEAM_SPECTATOR &&
			ent->client->ps.pm_type != PM_DEAD &&
			!(ent->client->ps.stats[JK_DIMENSION] & (DIMENSION_DUEL | DIMENSION_PRIVATE)) &&
			ent->jkmodEnt.dimensionNumberOld &&
			ent->jkmodEnt.dimensionNumberOld != ent->jkmodEnt.dimensionNumber &&
			(((ent->jkmodEnt.dimensionPrevious & DIMENSION_DUEL | DIMENSION_PRIVATE) && !(jkcvar_altDimensionSpawn.integer & (DIMENSION_DUEL | DIMENSION_PRIVATE))) ? qfalse : qtrue)) 
		{
			spawnActivated = qtrue;
			SelectSpawnPoint(ent->client->ps.origin, spawnOrigin, spawnAngles);
			JKMod_TeleportPlayer(ent, spawnOrigin, spawnAngles, qfalse, 0, NULL, "sound/interface/secret_area");
		}
		else if (ent->jkmodEnt.dimensionNumberOld && ent->client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			JKMod_G_SoundAtLoc(ent->client->ps.origin, CHAN_VOICE, G_SoundIndex("sound/interface/secret_area"), ent->s.number);
		}

		// Check stuck anyways
		if (((ent->client->ps.duelInProgress && !jkcvar_duelPassThrough.integer) || !spawnActivated) && JKMod_OthersInBox(ent))
		{
			JKMod_AntiStuckBox(ent);
		}

		// Update previous dimension
		ent->jkmodEnt.dimensionNumberOld = dimension;
		ent->jkmodEnt.dimensionPrevious = ent->client->ps.stats[JK_DIMENSION];

		if (mvapi)
		{
			for (i = 0; i < level.num_entities; i++) {
				if (g_entities[i].inuse) {
					int owner = g_entities[i].jkmodEnt.dimensionOwner;

					if (JKMod_DimensionCheck(owner, clientNum)) {
						mv_entities[i].snapshotIgnore[clientNum] = 0;
					} else {
						mv_entities[i].snapshotIgnore[clientNum] = 1;
					}
				}
			}

			for (i = 0; i < level.maxclients; i++) {
				if (g_entities[i].inuse) {
					int owner = g_entities[i].jkmodEnt.dimensionOwner;

					if (JKMod_DimensionCheck(owner, clientNum)) {
						mv_entities[clientNum].snapshotIgnore[i] = 0;
					} else {
						mv_entities[clientNum].snapshotIgnore[i] = 1;
					}
				}
			}
		}

		JKMod_Printf(S_COLOR_YELLOW "Client %i changed dimension to %i\n", clientNum, dimension);
	}
	else
	{
		JKMod_Printf(S_COLOR_YELLOW "Client %i already in dimension %i\n", clientNum, dimension);
	}
}

/*
=====================================================================
Dimension check collide
=====================================================================
*/
qboolean JKMod_DimensionCollide(gentity_t *ent1, gentity_t *ent2)
{
	int owner1 = ent1->jkmodEnt.dimensionOwner;
	int owner2 = ent2->jkmodEnt.dimensionOwner;
	
	if ((JKModDimensionData[JKMod_DimensionIndex(ent1->jkmodEnt.dimensionNumber)].passthrough && 
		JKModDimensionData[JKMod_DimensionIndex(ent2->jkmodEnt.dimensionNumber)].passthrough) || 
		(level.jkmodLocals.privateRoom[ent1->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]][PRIVATE_PASSTHROUGH] && 
		level.jkmodLocals.privateRoom[ent2->client->pers.jkmodPers.privateRoom[PRIVATE_NUM]][PRIVATE_PASSTHROUGH]))
	{
		return !JKMod_DimensionCheck(owner1, owner2);
	}

	return JKMod_DimensionCheck(owner1, owner2);
}

/*
=====================================================================
Dimensions main trace function
=====================================================================
*/
void JKMod_DimensionTrace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask)
{
	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	if (results->entityNum < ENTITYNUM_MAX_NORMAL) 
	{
		gentity_t	*passEnt = g_entities + passEntityNum;
		gentity_t	*ent = g_entities + results->entityNum;

		if (!JKMod_DimensionCollide(ent, passEnt) || !JKMod_DuelIsolationCheck(ent, passEnt)) 
		{
			int contents;

			contents = ent->r.contents;
			ent->r.contents = 0;
			JKMod_DimensionTrace(results, start, mins, maxs, end, passEntityNum, contentmask);
			ent->r.contents = contents;

			return;
		}
	}

	if (results->startsolid && start != end) 
	{
		trace_t tw;

		JKMod_DimensionTrace(&tw, start, mins, maxs, start, passEntityNum, contentmask);
		results->startsolid = tw.startsolid;
	}
}

/*
=====================================================================
Dimensions check entities in box
=====================================================================
*/
int JKMod_DimensionEntitiesInBox(const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount, int entityNum)
{
	gentity_t	*passEnt = g_entities + entityNum;
	int			fullCount;
	int			count;
	int			i;

	fullCount = trap_EntitiesInBox(mins, maxs, entityList, maxcount);

	for (i = 0, count = 0; i < fullCount; i++) 
	{
		if (JKMod_DimensionCollide(g_entities + entityList[i], passEnt)) 
		{
			entityList[count] = entityList[i];
			count++;
		}
	}

	return count;
}
