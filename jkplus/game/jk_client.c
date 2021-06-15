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

	// Get user info
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
			char		*clientPass;
			int			clientEnd, clientWait, num = 0;

			clientIP = Info_ValueForKey(userinfo, "ip");
			clientPass = Info_ValueForKey(userinfo, "password");

			while (++num < strlen(clientIP)) if (clientIP[num] == ':') clientIP[num] = 0;

			// Closed server
			if (jkcvar_serverClosed.string[0] && strcmp(jkcvar_serverClosed.string, "0") != 0 && Q_stricmp(jkcvar_serverClosed.string, clientPass))
			{
				G_Printf("Server closed for: %s using password: %s\n", clientIP, clientPass);
				return va("%s", jkcvar_serverClosedText.string);
			}

			// Connect message
			clientEnd = 5;
			clientWait = clientEnd + 1;
			
			trap_Cvar_Register(&clientTemp, clientIP, "0", CVAR_ARCHIVE);
			trap_SendConsoleCommand(EXEC_APPEND, va("wait %i; %s %i\n", clientWait, clientIP, clientWait));

			g_entities[clientNum].client->jkmodClient.ConnectTime = clientTemp.integer;

			if (g_entities[clientNum].client->jkmodClient.ConnectTime < clientEnd) {
				return va("Server running " S_COLOR_CYAN "%s", GAMEVERSION);
			}

			g_entities[clientNum].client->jkmodClient.ConnectTime = 0;
			trap_SendConsoleCommand(EXEC_APPEND, va("%s 0\n", clientIP));
			G_LogPrintf("ClientMessage: %s is ready to join\n", clientIP);

			// Show base client connect message
			trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", g_entities[clientNum].client->pers.netname, G_GetStripEdString("SVINGAME", "PLCONNECT")));

			// Set client IP
			Q_strncpyz(g_entities[clientNum].client->sess.jkmodSess.ClientIP, clientIP, sizeof(g_entities[clientNum].client->sess.jkmodSess.ClientIP));
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
	gentity_t	*ent;
	gclient_t	*client;
	char		userinfo[MAX_INFO_VALUE];
	qboolean	GTconfigLoaded = jkcvar_gameTypeConfig.integer && level.newSession ? level.jkmodLevel.cvarTempUnlock == 2 : qtrue;

	// Set user data
	ent	= &g_entities[clientNum];
	client	= ent->client;

	// Delay bots on custom gametype config load
	if ((ent->r.svFlags & SVF_BOT) && !ent->client->pers.botDelayed && !GTconfigLoaded) 
	{
		ent->client->pers.botDelayed = qtrue;
		return;
	}

	// Launch original client begin function
	BaseJK2_ClientBegin(clientNum, allowTeamReset);

	// Get user info
	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// Set default dimension and refresh
	if (client->ps.stats[JK_DIMENSION] != DIMENSION_FREE) JKMod_DimensionSet(ent, DIMENSION_FREE);

	// Set player movements
	if (jkcvar_playerMovement.integer) JKMod_PlayerMovementCheck(ent);

	// Check client plugin
	if (jkcvar_pluginRequired.integer && !client->pers.jkmodPers.ClientPlugin)
	{
		char	*serverVersion = JK_VERSION;
		char	*clientVersion;
		char	*pluginVersion;
		int		allowDownload;

		// Force to spectator mode
		if (jkcvar_pluginRequired.integer == 2 && client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			SetTeam(ent, "spectator");
			return;
		}

		// Get info
		clientVersion = Info_ValueForKey(userinfo, "JK2MV");
		pluginVersion = Info_ValueForKey(userinfo, "jkmod_clientversion");
		allowDownload = clientVersion[0] ? trap_Cvar_VariableIntegerValue("mv_httpdownloads") : trap_Cvar_VariableIntegerValue("sv_allowDownload");

		// Show center print message
		if (!pluginVersion[0])
		{
			trap_SendServerCommand(clientNum, va("cp \"Please download\n^5%s^7 client plugin\nCheck the console or enable downloads in main menu\"", serverVersion));
			G_LogPrintf("ClientPlugin: Player does not have any plugin\n", serverVersion);
		}
		else
		{
			trap_SendServerCommand(clientNum, va("cp \"Your client plugin is\n^3%s\nThe server version is ^5%s^7\nCheck the console or enable downloads in main menu\"", pluginVersion, serverVersion));
			G_LogPrintf("ClientPlugin: Player is using '%s' instead '%s'\n", pluginVersion, serverVersion);
		}

		// Show console print message
		if (allowDownload)
		{
			trap_SendServerCommand(clientNum, va("print \"Update your client plugin typing ^2/%s_allowdownload 1^7 in the console and reconnect\n\"", (clientVersion[0] ? "mv" : "cl")));
		}
		else
		{
			trap_SendServerCommand(clientNum, va("print \"Download ^5%s^7 client plugin from ^2https://jkmod.github.io\n\"", serverVersion));
		}
	}

	// Show a welcome message
	if (client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		// Show base client begin message
		if (g_gametype.integer != GT_TOURNAMENT)
		{
			// Current mod version
			trap_SendServerCommand(clientNum, va("print \"This server is running ^5%s ^7(Version: ^2%s.%s.%s ^7- Build: %s)\n\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH, __DATE__));

			// Random message
			if (jkcvar_randomBegin.integer && !Q_stricmp(level.jkmodLevel.RandomBegin[0], "") == 0)
			{
				int random = JKMod_Rand() % level.jkmodLevel.RandomBeginCount;
				trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, level.jkmodLevel.RandomBegin[random]));
			}
			else
			{
				trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLENTER")));
			}

			// Custom sound
			if (VALIDSTRINGCVAR(jkcvar_serverJoinSound.string))
			{
				G_Sound(ent, CHAN_VOICE, G_SoundIndex(jkcvar_serverJoinSound.string));
			}
		}

		// Server motd time
		if (!client->sess.jkmodSess.MotdSeen && *jkcvar_serverMotd.string && jkcvar_serverMotd.string[0] && !Q_stricmp(jkcvar_serverMotd.string, "0") == 0)
		{
			// Delay motd for non-plugin clients
			int motdDelayed = jkcvar_pluginRequired.integer && !client->pers.jkmodPers.ClientPlugin ? jkcvar_serverMotdTime.integer + 2 : 0;
			
			client->jkmodClient.MotdTime = motdDelayed ? motdDelayed : jkcvar_serverMotdTime.integer;
			client->sess.jkmodSess.MotdSeen = qtrue;
		}
	}
}

/*
=====================================================================
Client clean name function
=====================================================================
*/
void JKMod_ClientCleanName(const char *in, char *out, int outSize, gentity_t *ent) 
{
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
			strcpy(multiBuffer, va("%s" S_COLOR_WHITE "(%i)", outBuffer, num));
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
