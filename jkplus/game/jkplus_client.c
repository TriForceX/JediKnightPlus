/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main client module
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Client connect function
=====================================================================
*/

char *JKPlus_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot)
{
	char		userinfo[MAX_INFO_STRING];

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// Closed server
	if (strcmp(jkcvar_serverClosed.string, "0") != 0 && !isBot && firstTime)
	{
		char		IPonly[MAX_IP];
		int			num = 0;

		Q_strncpyz(IPonly, Info_ValueForKey(userinfo, "ip"), sizeof(IPonly));
		while (++num < strlen(IPonly)) if (IPonly[num] == ':') IPonly[num] = 0;

		if (Q_stricmp(jkcvar_serverClosedIP.string, IPonly))
		{
			G_Printf("Server closed for: %s\n", IPonly);
			if (jkcvar_serverClosedBroadcast.integer) trap_SendServerCommand(-1, va("print \"Server closed for: %s\n\"", IPonly));
			return va("%s", jkcvar_serverClosed.string);
		}
	}

	// Launch original client connect function
	return BaseJK2_ClientConnect(clientNum, firstTime, isBot);
}

/*
=====================================================================
Client begin function
=====================================================================
*/

void JKPlus_ClientBegin(int clientNum, qboolean allowTeamReset)
{
	gentity_t *ent = &g_entities[clientNum];

	// Check and modify allowed teams
	if (g_gametype.integer < GT_TEAM && (ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_BLUE))
	{
		ent->client->sess.sessionTeam = TEAM_FREE;
	}
	else if (g_gametype.integer >= GT_TEAM
		&& ent->client->sess.sessionTeam != TEAM_RED
		&& ent->client->sess.sessionTeam != TEAM_BLUE
		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
	}

	// Launch original client begin function
	BaseJK2_ClientBegin(clientNum, allowTeamReset);

	// Show a welcome message
	if (ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		trap_SendServerCommand(ent - g_entities, va("print \""
			"This server is running " S_COLOR_CYAN "%s" S_COLOR_WHITE " (Version: %s.%s.%s - Build: %s)\n"
			"\"", JKPLUS_LONGNAME, JKPLUS_MAJOR, JKPLUS_MINOR, JKPLUS_PATCH, __DATE__));
	}
}

/*
=====================================================================
Client spawn function
=====================================================================
*/

void JKPlus_ClientSpawn(gentity_t *ent)
{
	// Launch original client spawn function
	BaseJK2_ClientSpawn(ent);
	
	// Stuff here...
}
