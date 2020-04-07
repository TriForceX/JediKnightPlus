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
	gentity_t	*ent;
	gclient_t	*client;
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

	// Connect message
	if (firstTime && !(ent->r.svFlags & SVF_BOT) && !isBot)
	{
		vmCvar_t	clientTemp;
		char		*clientIP;
		int			endTime, waitTime;

		endTime = 5;
		waitTime = endTime + 1;
		clientIP = Info_ValueForKey(userinfo, "ip");

		trap_Cvar_Register(&clientTemp, clientIP, "0", CVAR_ARCHIVE);
		trap_SendConsoleCommand(EXEC_APPEND, va("wait %i; %s %i\n", waitTime, clientIP, waitTime));

		client->JKPlusConnectTime = clientTemp.integer;

		if (client->JKPlusConnectTime < endTime) {
			return va("Server running " S_COLOR_CYAN "%s", GAMEVERSION);
		}

		client->JKPlusConnectTime = 0;
		trap_SendConsoleCommand(EXEC_APPEND, va("%s 0\n", clientIP));
		G_LogPrintf("ClientWelcome: %s is ready to join\n", clientIP);
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
		// Current mod version
		trap_SendServerCommand(ent - g_entities, va("print \""
			"This server is running " S_COLOR_CYAN "%s" S_COLOR_WHITE " (Version: %s.%s.%s - Build: %s)\n"
			"\"", JKPLUS_LONGNAME, JKPLUS_MAJOR, JKPLUS_MINOR, JKPLUS_PATCH, __DATE__));

		// Server motd time
		if(*jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
		{
			ent->client->JKPlusMotdTime = jkcvar_serverMotdTime.integer;
		}
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

/*
=====================================================================
Client clean name function
=====================================================================
*/
void JKPlus_ClientCleanName(gentity_t *ent, const char *in, char *out, int outSize) {
	int		len, colorlessLen;
	char	ch;
	char	*p;
	int		spaces;

	// Custom variables for name checks
	char	 outBuffer[MAX_STRING_CHARS];
	char	 tempBuffer[MAX_STRING_CHARS];
	char	 multiBuffer[MAX_NETNAME];
	int		 i, count, num;

	memset(outBuffer, 0, sizeof(outBuffer));
	memset(multiBuffer, 0, sizeof(multiBuffer));

	//save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while (1) {
		ch = *in++;
		if (!ch) {
			break;
		}

		// ASCII character filtering (Don't use in JKA)
		if ((int)ch < 0)
		{
			continue;
		}

		// don't allow leading spaces
		if (!*p && ch == ' ') {
			continue;
		}

		// check colors
		if (ch == Q_COLOR_ESCAPE) {
			// solo trailing carat is not a color prefix
			if (!*in) {
				break;
			}

			// don't allow black in a name, period (except the server allow it)
			if (ColorIndex(*in) == 0 && jkcvar_allowBlackNames.integer == 0) {
				in++;
				continue;
			}

			// make sure room in dest for both chars
			if (len > outSize - 2) {
				break;
			}

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// don't allow too many consecutive spaces
		if (ch == ' ') {
			spaces++;
			if (spaces > 3) {
				continue;
			}
		}
		else {
			spaces = 0;
		}

		if (len > outSize - 1) {
			break;
		}

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = 0;

	// Check the total name length
	if (strlen(p) > MAX_NAME_CHECK)
	{
		memset(multiBuffer, 0, sizeof(multiBuffer));
		for (i = 0; i < MAX_NAME_CHECK; i++)
		{
			multiBuffer[i] = p[i];
		}
		strcpy(p, multiBuffer);
	}

	// Check the name length without colours
	count = 0;
	for (i = 0; i < strlen(p); i++)
	{
		outBuffer[count] = p[i];
		count++;
		memset(tempBuffer, 0, sizeof(tempBuffer));
		JKPlus_sanitizeString(tempBuffer, outBuffer, sizeof(tempBuffer));

		if (strlen(tempBuffer) >= MAX_NAME_PRINT)
		{
			break;
		}
	}

	// Check duplicated player names
	if (jkcvar_noDuplicatedNames.integer)
	{
		num = JKPlus_duplicatedNameCheck(ent, outBuffer);

		if (num)
		{
			memset(multiBuffer, 0, sizeof(multiBuffer));
			strcpy(multiBuffer, va("%s" S_COLOR_WHITE "[%i]", outBuffer, num));
			Q_strncpyz(outBuffer, multiBuffer, sizeof(outBuffer));
		}
	}

	// don't allow empty names
	if (*p == 0 || colorlessLen == 0) {
		Q_strncpyz(p, "Padawan", outSize);
	}
	else // Give the player his current name
	{
		Q_strncpyz(p, outBuffer, outSize);
	}
}
