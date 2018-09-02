/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main client module
=====================================================================
*/

#include "../game/g_local.h" // Original header

/*
=====================================================================
Client connect function
=====================================================================
*/

char *JKPlus_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot)
{
	char	*returnMessage;
	char	userinfo[MAX_INFO_STRING];

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// Launch original client connect function
	returnMessage = BaseJK2_ClientConnect(clientNum, firstTime, isBot);

	// Stuff here...
	
	// Final return, probably NULL
	return returnMessage;
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
			"^7Welcome to ^5%s\n"
			"^7The version is ^2%s.%s %s\n"
			"^7This build was created in %s\n"
			"\"", JKPLUS_LONGNAME, JKPLUS_RELEASE, JKPLUS_DEVELOP, JKPLUS_PHASE, __DATE__));
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
