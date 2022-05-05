/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2022
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
	if (baseMessage == NULL && firstTime && !isBot)
	{
		int		i = 0;
		int		clientTempID;
		char	clientIP[MAX_INFO_VALUE];
		char	clientPass[MAX_INFO_VALUE];
		char	clientChallenge[MAX_CHALLENGE];

		Q_strncpyz(clientIP, Info_ValueForKey(userinfo, "ip"), sizeof(clientIP));
		Q_strncpyz(clientPass, Info_ValueForKey(userinfo, "password"), sizeof(clientPass));
		Q_strncpyz(clientChallenge, Info_ValueForKey(userinfo, "challenge"), sizeof(clientChallenge));

		clientTempID = atoi(clientChallenge);
		
		while (++i < strlen(clientIP)) if (clientIP[i] == ':') clientIP[i] = 0;

		// Closed server check
		if (VALIDSTRINGCVAR(jkcvar_serverClosed.string) && Q_stricmp(jkcvar_serverClosed.string, clientPass))
		{
			char passError[MAX_INFO_VALUE];
			char passPrint[MAX_INFO_VALUE];
			char passCheck[MAX_INFO_VALUE];

			Q_strncpyz(passError, clientPass[0] ? "using password: " : "(no password set)", sizeof(passError));
			Q_strncpyz(passPrint, clientPass, sizeof(passPrint));

			// Reduce long pass
			if (strlen(clientPass) > 16) JKMod_TruncateString(passPrint, clientPass, 16);

			// Encode pass
			JKMod_DummyEncode(passCheck, clientPass[0] ? clientPass : clientIP);

			// Alert once
			if (level.jkmodLocals.closedCheck[clientTempID] != atoi(passCheck))
			{
				level.jkmodLocals.closedCheck[clientTempID] = atoi(passCheck);
				trap_SendServerCommand(-1, va("print \"Server closed for: ^3%s ^7%s^1%s\n\"", clientIP, passError, passPrint));
				G_Printf("Server closed for: %s %s%s\n", clientIP, passError, passPrint);
			}
			return jkcvar_serverClosedText.string;
		}

		// Connect message
		if (level.jkmodLocals.messageCheck[clientTempID] != clientTempID)
		{
			level.jkmodLocals.messageCheck[clientTempID] = clientTempID;
			return "Server running " GAMEVERSION;
		}
		G_LogPrintf("ClientMessage: %i connected with challenge response %i\n", clientNum, clientTempID);
			
		// Show client connect and reconnect message
		trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", g_entities[clientNum].client->pers.netname, (!Q_stricmp(level.jkmodLocals.reconnectedIP, clientIP) ? "reconnected" : G_GetStripEdString("SVINGAME", "PLCONNECT"))));

		// Set client IP
		Q_strncpyz(g_entities[clientNum].client->sess.jkmodSess.clientIP, clientIP, sizeof(g_entities[clientNum].client->sess.jkmodSess.clientIP));

		// Clear message and closed check
		level.jkmodLocals.messageCheck[clientTempID] = 0;
		level.jkmodLocals.closedCheck[clientTempID] = 0;

		// Clear reconnect check
		*level.jkmodLocals.reconnectedIP = '\0';
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
	qboolean	GTconfigLoaded = jkcvar_gameTypeConfig.integer && level.newSession ? level.jkmodLocals.cvarTempUnlock == 2 : qtrue;

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
	JKMod_DimensionSet(ent, level.jkmodLocals.dimensionBase);

	// Set player movements
	if (jkcvar_playerMovement.integer) JKMod_PlayerMovementCheck(ent);

	// Check dual saber
	if (client->sess.sessionTeam == TEAM_SPECTATOR && client->pers.jkmodPers.dualSaber) client->pers.jkmodPers.dualSaber = qfalse;

	// Check client plugin
	if (jkcvar_pluginRequired.integer && !client->pers.jkmodPers.clientPlugin)
	{
		char	clientVersion[MAX_INFO_VALUE];
		char	pluginVersion[MAX_INFO_VALUE];
		char	clientEternal[MAX_INFO_VALUE];
		int		allowDownload;

		// Force to spectator mode
		if (jkcvar_pluginRequired.integer == 2 && client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			SetTeam(ent, "spectator");
			return;
		}

		// Get info
		Q_strncpyz(clientVersion, Info_ValueForKey(userinfo, "JK2MV"), sizeof(clientVersion));
		Q_strncpyz(pluginVersion, Info_ValueForKey(userinfo, "jkmod_client"), sizeof(pluginVersion));
		Q_strncpyz(clientEternal, Info_ValueForKey(userinfo, "cjp_client"), sizeof(clientEternal));

		allowDownload = clientVersion[0] ? trap_Cvar_VariableIntegerValue("mv_httpdownloads") : trap_Cvar_VariableIntegerValue("sv_allowDownload");

		// Check for EternalJK2
		if (!strcmp(clientEternal, "1.4JAPRO")) 
		{
			trap_SendServerCommand(clientNum, "cp \"You are using ^1EternalJK2\nSome features may be ^3disabled^7\nPlease use JK2MV ^5https://jk2mv.org\"");
			trap_SendServerCommand(clientNum, "print \"You are running in ^3Server Side^7 mode only due ^1EternalJK2^7 was detected\n\"");
			G_LogPrintf("ClientPlugin: Player does not have any plugin (Using EternalJK2)\n");
		}
		else
		{
			// Show center print message
			if (!VALIDSTRINGCVAR(pluginVersion))
			{
				trap_SendServerCommand(clientNum, va("cp \"Please download\n^5%s^7 client plugin\nCheck the console or enable downloads in main menu\"", JK_VERSION));
				G_LogPrintf("ClientPlugin: Player does not have any plugin\n");
			}
			else
			{
				trap_SendServerCommand(clientNum, va("cp \"Your client plugin is\n^3%s\nThe server version is ^5%s^7\nCheck the console or enable downloads in main menu\"", pluginVersion, JK_VERSION));
				G_LogPrintf("ClientPlugin: Player is using '%s' instead '%s'\n", pluginVersion, JK_VERSION);
			}

			// Show console print message
			if (allowDownload)
			{
				trap_SendServerCommand(clientNum, va("print \"Update your client plugin typing ^2/%s_allowdownload 1^7 in the console and reconnect\n\"", (clientVersion[0] ? "mv" : "cl")));
			}
			else
			{
				trap_SendServerCommand(clientNum, va("print \"Download ^5%s^7 client plugin from ^2%s\n\"", JK_VERSION, JK_URL));
			}
		}
	}

	// Show a welcome message
	if (client->sess.sessionTeam != TEAM_SPECTATOR)
	{
		// Show base client begin message
		if (g_gametype.integer != GT_TOURNAMENT)
		{
			// Current mod version
			trap_SendServerCommand(clientNum, va("print \"This server is running ^5%s ^7- Version: ^2%s.%s.%s\n\"", JK_LONGNAME, JK_MAJOR, JK_MINOR, JK_PATCH));

			// Random message
			if (jkcvar_randomBegin.integer && VALIDSTRING(level.jkmodLocals.randomBegin[0]))
			{
				int random = JKMod_Rand() % level.jkmodLocals.randomBeginCount;
				trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, level.jkmodLocals.randomBegin[random]));
			}
			else
			{
				trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLENTER")));
			}
			
			// Custom sound
			if (VALIDSTRINGCVAR(jkcvar_serverJoinSound.string) && !level.jkmodLocals.mapRestarted)
			{
				G_Sound(ent, CHAN_AUTO, G_SoundIndex(jkcvar_serverJoinSound.string));
			}
		}

		// Server motd time
		if (!client->sess.jkmodSess.motdSeen && VALIDSTRINGCVAR(jkcvar_serverMotd.string))
		{
			// Delay motd for non-plugin clients
			int motdDelayed = jkcvar_pluginRequired.integer && !client->pers.jkmodPers.clientPlugin ? jkcvar_serverMotdTime.integer + 2 : 0;
			
			client->jkmodClient.motdTime = motdDelayed ? motdDelayed : jkcvar_serverMotdTime.integer;
			client->sess.jkmodSess.motdSeen = qtrue;
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
		JKMod_SanitizeString(tempBuffer, outBuffer, sizeof(tempBuffer));

		if (strlen(tempBuffer) >= MAX_NAME_PRINT)
		{
			break;
		}
	}

	// Check duplicated player names
	if (jkcvar_noDuplicatedNames.integer)
	{
		num = JKMod_DuplicatedNameCheck(ent, outBuffer);

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
