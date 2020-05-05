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
char *JKMod_ClientConnect(int clientNum, qboolean firstTime, qboolean isBot)
{
	char		userinfo[MAX_INFO_STRING];
	char		*baseMessage;

	// Set userinfo
	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// Launch original client connect function
	baseMessage = BaseJK2_ClientConnect(clientNum, firstTime, isBot);

	// First time connect and allowed connect
	if (baseMessage == NULL && firstTime)
	{
		if (!isBot)
		{
			vmCvar_t	clientTemp;
			char		*clientIP;
			int			clientEnd, clientWait, num = 0;

			clientIP = Info_ValueForKey(userinfo, "ip");
			while (++num < strlen(clientIP)) if (clientIP[num] == ':') clientIP[num] = 0;

			// Closed server
			if (strcmp(jkcvar_serverClosed.string, "0") != 0)
			{
				if (Q_stricmp(jkcvar_serverClosedIP.string, clientIP))
				{
					G_Printf("Server closed for: %s\n", clientIP);
					return va("%s", jkcvar_serverClosed.string);
				}
			}

			// Connect message
			clientEnd = 5;
			clientWait = clientEnd + 1;
			
			trap_Cvar_Register(&clientTemp, clientIP, "0", CVAR_ARCHIVE);
			trap_SendConsoleCommand(EXEC_APPEND, va("wait %i; %s %i\n", clientWait, clientIP, clientWait));

			g_entities[clientNum].client->JKModConnectTime = clientTemp.integer;

			if (g_entities[clientNum].client->JKModConnectTime < clientEnd) {
				return va("Server running " S_COLOR_CYAN "%s", GAMEVERSION);
			}

			g_entities[clientNum].client->JKModConnectTime = 0;
			trap_SendConsoleCommand(EXEC_APPEND, va("%s 0\n", clientIP));
			G_LogPrintf("ClientMessage: %s is ready to join\n", clientIP);

			// Show base client connect message
			trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", g_entities[clientNum].client->pers.netname, G_GetStripEdString("SVINGAME", "PLCONNECT")));
		}
	}

	return baseMessage;
}

/*
=====================================================================
Client begin function
=====================================================================
*/
void JKMod_ClientBegin(int clientNum, qboolean allowTeamReset)
{
	gentity_t	*ent = &g_entities[clientNum];
	gclient_t	*client;
	char		userinfo[MAX_INFO_VALUE];
	const char	*serverVersion;
	const char	*clientVersion;

	// Check and modify allowed teams
	if (g_gametype.integer < GT_TEAM && (ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_BLUE))
	{
		ent->client->sess.sessionTeam = TEAM_FREE;
	}
	else if (g_gametype.integer >= GT_TEAM && 
		ent->client->sess.sessionTeam != TEAM_RED && 
		ent->client->sess.sessionTeam != TEAM_BLUE && 
		ent->client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
	}

	// Launch original client begin function
	BaseJK2_ClientBegin(clientNum, allowTeamReset);

	// Set and get user info
	client = level.clients + clientNum;
	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// Set and check client/server version
	serverVersion = JK_VERSION;
	clientVersion = Info_ValueForKey(userinfo, "JKMod_ClientVersion");

	client->pers.JKModClientPlugin = strcmp(clientVersion, serverVersion) == 0 || ent->r.svFlags & SVF_BOT ? qtrue : qfalse;

	// Check client plugin
	if (client->pers.JKModClientPlugin || !jkcvar_forcePlugin.integer)
	{
		// Show a welcome message
		if (ent->client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			// Show base client begin message
			if (g_gametype.integer != GT_TOURNAMENT)
			{
				// Current mod version
				trap_SendServerCommand(clientNum, va("print \""
					"This server is running ^5%s ^7(Version: ^2%s.%s.%s ^7- Build: %s)\n"
					"\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, __DATE__));

				// Random message
				if (jkcvar_randomBegin.integer && !Q_stricmp(level.jkmodData.RandomBegin[0], "") == 0)
				{
					int random = JKMod_Rand() % level.jkmodData.RandomBeginCount;
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, level.jkmodData.RandomBegin[random]));
				}
				else
				{
					trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLENTER")));
				}
			}

			// Server motd time
			if (*jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0 && !ent->client->sess.JKModMotdSeen)
			{
				client->JKModMotdTime = jkcvar_serverMotdTime.integer;
				client->sess.JKModMotdSeen = qtrue;
			}
		}
	}
	else if (jkcvar_forcePlugin.integer)
	{
		// Force to spectator mode
		if (client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			SetTeam(ent, "spectator");
			return;
		}

		// Show center print message
		if (clientVersion[0] == '\0')
		{
			trap_SendServerCommand(clientNum, va("cp \"Please download\n" S_COLOR_CYAN "%s" S_COLOR_WHITE " client plugin\"", serverVersion));
			G_LogPrintf("ClientPlugin: Player does not have any plugin\n", serverVersion);

		}
		else
		{
			trap_SendServerCommand(clientNum, va("cp \"Your client plugin is\n" S_COLOR_YELLOW "%s\nThe server version is " S_COLOR_CYAN "%s" S_COLOR_WHITE "\nPlease download again\"", clientVersion, serverVersion));
			G_LogPrintf("ClientPlugin: Player is using '%s' instead '%s'\n", clientVersion, serverVersion);
		}

		// Show console print message
		if (trap_Cvar_VariableIntegerValue("mv_httpdownloads") && Info_ValueForKey(userinfo, "JK2MV")[0])
		{
			trap_SendServerCommand(clientNum, va("print \""
				"To update your client plugin type " S_COLOR_GREEN "/mv_allowdownload 1" S_COLOR_WHITE " in the console and reconnect\n"
				"\""));
		}
		else if (trap_Cvar_VariableIntegerValue("sv_allowDownload"))
		{
			trap_SendServerCommand(clientNum, va("print \""
				"To update your client plugin type " S_COLOR_GREEN "/cl_allowdownload 1" S_COLOR_WHITE " in the console and reconnect\n"
				"\""));
		}
		else
		{
			trap_SendServerCommand(clientNum, va("print \""
				"Download " S_COLOR_CYAN "%s" S_COLOR_WHITE " client plugin from " S_COLOR_GREEN "https://jkmod.github.io\n"
				"\"", serverVersion));
		}
	}
}

/*
=====================================================================
Client spawn function
=====================================================================
*/
void JKMod_ClientSpawn(gentity_t *ent)
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
void JKMod_ClientCleanName(gentity_t *ent, const char *in, char *out, int outSize) {
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
		JKMod_sanitizeString(tempBuffer, outBuffer, sizeof(tempBuffer));

		if (strlen(tempBuffer) >= MAX_NAME_PRINT)
		{
			break;
		}
	}

	// Check duplicated player names
	if (jkcvar_noDuplicatedNames.integer)
	{
		num = JKMod_duplicatedNameCheck(ent, outBuffer);

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
