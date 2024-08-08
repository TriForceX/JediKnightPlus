/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Common utilities functions
=====================================================================
*/

#include "../../code/game/g_local.h"	// Original header

/*
=====================================================================
Only print when "developer 1"
=====================================================================
*/
void QDECL JKMod_Printf(const char *fmt, ...) 
{
	if (developer.integer)
	{
		va_list		argptr;
		char		text[1024];

		va_start (argptr, fmt);
		Q_vsnprintf (text, sizeof(text), fmt, argptr);
		va_end (argptr);

		trap_Printf(text);
	}
}

/*
=====================================================================
Convert double dot number version string into a valid float number
=====================================================================
*/
float JKMod_GetVersion(const char *s)
{
	int major, minor, patch;
	float version = 0.0;

	if (JKMod_CheckVersion(s)) {
		sscanf(s, "%d.%d.%d", &major, &minor, &patch);
		version = atof(va("%d.%02d%02d", major, minor, patch));
	}

	return version;
}

/*
=====================================================================
Check double dot number version string format
=====================================================================
*/
qboolean JKMod_CheckVersion(const char *s) 
{
	int	i;
	int len = strlen(s);
	int dots = 0;

	for (i = 0; i < len; i++) {
		if (!VALIDDIGIT(s[i]) && s[i] != '.') return qfalse;
		if (s[i] == '.') dots++;
	}

	if (dots != 2) return qfalse;
	if (!VALIDDIGIT(s[0]) || !VALIDDIGIT(s[len - 1])) return qfalse;

	return qtrue;
}

/*
=====================================================================
Check the given string as number
=====================================================================
*/
qboolean JKMod_ValidNumber(const char *s)
{
	int i;

	if (s == NULL || *s == '\0') return qfalse;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') return qfalse;
    }

    return qtrue;
}

/*
=====================================================================
Truncate buffer with custom length limit
=====================================================================
*/
void JKMod_TruncateString(char *buffer, const char *s, int limit) 
{
	int length = strlen(s);

	if (length <= limit) {
		Q_strncpyz(buffer, s, limit);
	} else {
		Q_strncpyz(buffer, s, limit - 2);
		Q_strcat(buffer, limit + 1, "...");
	}
}

/*
=====================================================================
Encode characters of a string into its ASCII numbers total sum
=====================================================================
*/
void JKMod_DummyEncode(char *buffer, const char *s)
{
	int i;
	int temp = 0;
	int length = strlen(s);

	for (i = 0; i < strlen(s); i++) temp += s[i];
	Q_strncpyz(buffer, va("%i", temp), length);
}

/*
=====================================================================
Replace multiple spaces by single space
=====================================================================
*/
char *JKMod_TrimWhiteSpace(char *s)
{
    char *from, *to;
    int spc = 0;
    to = from = s;
    while (1) {
        if (spc && *from == ' ' && to[-1] == ' ')
            ++from;
        else {
            spc = (*from==' ')? 1 : 0;
            *to++ = *from++;
            if (!to[-1]) break;
        }
    }
    return s;
}

/*
=====================================================================
String token with long delimiter
=====================================================================
*/
char *JKMod_StrTok(char *str, const char *delim)
{
    static char *tok;
    static char *next;
    char *m;

    if (delim == NULL) return NULL;
    tok = (str) ? str : next;
    if (tok == NULL) return NULL;

    m = strstr(tok, delim);

    if (m) {
        next = m + strlen(delim);
        *m = '\0';
    } else {
        next = NULL;
    }
    return tok;
}

/*
=====================================================================
Count string words as arguments
=====================================================================
*/
int JKMod_Str_Argc(char *str)
{
	if (str[0])
	{
		int i, count = 0;
		int len = strlen(str);
		int inWord = 0;

		for (i = 0; i < len; i++) {
			if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && !inWord) {
				count++;
				inWord = 1;
			}
			else if ((str[i] == ' ' || str[i] == '\n' || str[i] == '\t') && inWord) {
				inWord = 0;
			}
		}

		return count;
	}
	else
	{
		return trap_Argc();
	}
}

/*
=====================================================================
Split string into arguments
=====================================================================
*/
void JKMod_Str_Argv(int n, char *buffer, int bufferLength, char *src)
{
	if (src[0])
	{
		int i = 0;
		char tempBuffer[MAX_TOKEN_CHARS];
		char *token;
    
		Q_strncpyz(tempBuffer, src, MAX_TOKEN_CHARS);
		token = JKMod_StrTok(tempBuffer, " ");
    
		while (token != NULL) {
			if (i++ == n) {
				Q_strncpyz(buffer, token, bufferLength);
				return;
			}
			token = JKMod_StrTok(NULL, " ");
		}
    
		Q_strncpyz(buffer, "", bufferLength);
	}
	else
	{
		trap_Argv(n, buffer, bufferLength);
	}
}

/*
=====================================================================
Simple string clear
=====================================================================
*/
void JKMod_StringClear(char* str, int len)
{
	int	i;
	for (i = 0; i < len; i += 1)
	{
		str[i] = '\0';
	}
}

/*
=====================================================================
Cleans the given string from newlines and such
=====================================================================
*/
void JKMod_StringEscape(char *in, char *out, int outSize)
{
	char	ch, ch1;
	int len = 0;
	outSize--;

	while (1)
	{
		ch = *in++;
		ch1 = *in;
		if (ch == '\\' && ch1 == 'n') {
			in++;
			*out++ = '\n';
		} else {
			*out++ = ch;
		}
		if (len > outSize - 1) break;
		len++;
	}
	return;
}

/*
=====================================================================
Sanitize strings with color codes
=====================================================================
*/
char *JKMod_SanitizeString(char *dest, char *source, int destSize)
{
	char	string[MAX_TOKEN_CHARS];
	char	clean[MAX_TOKEN_CHARS];
	int		i, n, length;

	memset(string, 0, sizeof(string));
	memset(clean, 0, sizeof(clean));

	Q_strncpyz(string, source, sizeof(string));

	length = strlen(string);

	n = 0;

	for (i = 0; i < length; i++)
	{
		if (string[i] != '^') {
			clean[n] = tolower(string[i]);
			n++;
		} else if (string[i] == '^') {
			i++;
		}
	}

	Q_strncpyz(dest, clean, destSize);
	return dest;
}

/*
=====================================================================
Sanitize strings with color codes (alternative method)
=====================================================================
*/
void JKMod_SanitizeStringRev2(char *in, char *out)
{
	int i = 0;
	int r = 0;

	while (in[i])
	{
		if (i >= MAX_NAME_LENGTH - 1) break;
		if (in[i] == '^')
		{
			if (in[i + 1] >= 48 &&	//'0'
				in[i + 1] <= 57)	//'9'
			{
				i += 2;
				continue;
			}
			else
			{
				i++;
				continue;
			}
		}
		if (in[i] < 32)
		{
			i++;
			continue;
		}
		out[r] = tolower(in[i]);
		r++;
		i++;
	}
	out[r] = 0;
}

/*
=====================================================================
Check for duplicated player names
=====================================================================
*/
int JKMod_DuplicatedNameCheck(gentity_t *ent, char *clientName)
{
	gentity_t	*other;
	int			i, j, num;
	char		cleanEnt[MAX_NETNAME];
	char		cleanOther[MAX_NETNAME];
	char		newName[MAX_NETNAME];

	Q_strncpyz(newName, clientName, sizeof(newName));

	num = 0;
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		for (j = 0; j < MAX_CLIENTS; j++)
		{
			other = &g_entities[j];
			JKMod_SanitizeString(cleanEnt, newName, sizeof(cleanEnt));

			if (other && other->client && other->inuse && other->client->pers.connected == CON_CONNECTED)
			{
				JKMod_SanitizeString(cleanOther, other->client->pers.netname, sizeof(cleanOther));

				if (other - g_entities != ent - g_entities)
				{
					if (!Q_stricmp(cleanOther, cleanEnt))
					{
						num++;
						Q_strncpyz(newName, va("%s(%i)", clientName, num), sizeof(newName));
					}
				}
			}
		}
	}
	return num;
}

/*
=====================================================================
Drops a player from the server with the given message
=====================================================================
*/
void JKMod_DropPlayer(gentity_t *ent, char *reason)
{
	int clientNum = ent - g_entities;

	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		ent->client->sess.spectatorState = SPECTATOR_FREE;
		ent->client->sess.spectatorClient = ent - g_entities;
	}

	if (!VALIDSTRING(reason)) 
	{
		reason = "Unknown";
	}
	
	G_LogPrintf("ClientDrop: %s Reason: %s\n", ent->client->sess.jkmodSess.clientIP, reason);
	trap_DropClient(clientNum, va("%s (Reason: ^3%s^7)", G_GetStripEdString("SVINGAME", "WAS_KICKED"), reason));
}

/*
=====================================================================
Team name list in different formats
=====================================================================
*/
const char *JKMod_TeamName(team_t team, int letterCase) 
{
	const char *teamName[CASE_MAX][TEAM_NUM_TEAMS] = {
		{	// Normal
			"Free",
			"Red",
			"Blue",
			"Spectator",
		},
		{	// Upper
			"FREE",
			"RED",
			"BLUE",
			"SPECTATOR",
		},
		{	// Lower
			"free",
			"red",
			"blue",
			"spectator",
		}
	};
	
	return teamName[letterCase][team];
}

/*
=====================================================================
Convert milliseconds to string
=====================================================================
*/
const char *JKMod_MsToString(const int ms)
{
	int	timeSec, timeMin, timeMsec;

	timeMsec = ms;
	timeSec = timeMsec / 1000;
	timeMsec -= timeSec * 1000;
	timeMin = timeSec / 60;
	timeSec -= timeMin * 60;

	return !ms ? "00:00:000" : va("%02i:%02i:%03i", timeMin, timeSec, timeMsec);
}

/*
=====================================================================
Convert milliseconds to word
=====================================================================
*/
const char *JKMod_MsToWord(const int ms, qboolean abbr) 
{
	int	   		fsecs = ms / 1000;
	int			wholemins = fsecs / 60;
	float		fremainsecs;
	char		*strseconds = abbr ? "sec" : "seconds";
	char		*strminutes = abbr ? "min" : "minutes";
	char		*strhours = abbr ? "hrs" : "hours";

	if (wholemins < 1)
	{
		return va("%d %s", fsecs, strseconds);
	}
	else if (wholemins >= 60) 
	{
		const int hrs = wholemins / 60;

		wholemins -= hrs * 60;

		if (wholemins == 0)
		{
			return va("%d %s", hrs, strhours);
		}

		return va("%d %s %d %s", hrs, strhours, wholemins, strminutes);
	}

	fremainsecs = (ms - wholemins * 60000) * 0.001f;

	if (wholemins == 1 && !abbr) strminutes = "minute";

	if (fremainsecs == 0) {
		return va("%d %s", wholemins, strminutes);
	} else {
		return va("%d %s %d %s", wholemins, strminutes, (int)fremainsecs, strseconds);
	}
}

/*
=====================================================================
Get client number from stripped substring
=====================================================================
*/
int JKMod_GetClientNumberStripped(const char* name)
{
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];
	int			i, match = -1;
	gclient_t	*cl;

	JKMod_SanitizeStringRev2((char*)name, s2);

	for (i = 0; i < level.numConnectedClients; i++)
	{
		cl = &level.clients[level.sortedClients[i]];
		JKMod_SanitizeStringRev2(cl->pers.netname, n2);

		if (strstr(n2, s2))
		{
			if (match != -1)
			{ 
				return -2;
			}
			match = level.sortedClients[i];
		}
	}

	return match;
}

/*
=====================================================================
Get client number from argument
=====================================================================
*/
int JKMod_GetClientNumber(char* name)
{
	int client_id = 0;
	char *cp;

	cp = name;

	while (*cp)
	{
		if (*cp >= '0' && *cp <= '9') {
			cp++;
		} else {
			client_id = -1; 
			break;
		}
	}

	if (client_id == 0) { 
		client_id = atoi(name);
	} else if (client_id == -1) {
		client_id = JKMod_GetClientNumberStripped(name);
	}

	return client_id;
}

/*
=====================================================================
Check valid client from name
=====================================================================
*/
int JKMod_CheckValidClient(gentity_t *ent, char *name)
{
	int target = JKMod_GetClientNumber(name);

	if (target == -1)
	{
		if (ent) trap_SendServerCommand(ent - g_entities, va("print \"Can't find the name ^3%s\n\"", name));
		else G_Printf("Can't find the name ^3%s\n", name);
		return -1;
	}
	else if (target == -2)
	{
		if (ent) trap_SendServerCommand(ent - g_entities, va("print \"There are other names that contains ^3%s\n\"", name));
		else G_Printf("There are other names that contains ^3%s\n", name);
		return -1;
	}
	else if (target >= MAX_CLIENTS || target < 0)
	{
		if (ent) trap_SendServerCommand(ent - g_entities, va("print \"Invalid name for ^3%s\n\"", name));
		else G_Printf("Invalid name for ^3%s\n", name);
		return -1;
	}
	else if (!g_entities[target].inuse || g_entities[target].client->pers.connected != CON_CONNECTED)
	{
		if (ent) trap_SendServerCommand(ent - g_entities, va("print \"The client ^3%s ^7is not active\n\"", name));
		else G_Printf("The client ^3%s ^7is not active\n", name);
		return -1;
	}
	else if (ent && target == ent->client->ps.clientNum)
	{
		trap_SendServerCommand(ent - g_entities, "print \"You can't do it to yourself\n\"");
		return -1;
	}

	return target;
}

/*
=====================================================================
Read file
=====================================================================
*/
char *JKMod_ReadFile(char *filename)
{
	fileHandle_t	f;
	char			*buf = (char *)BG_TempAlloc(MAX_FILE_LENGTH);
	int				len, rlen;

	// File check
	len = trap_FS_FOpenFile(filename, &f, FS_READ);

	if (!f) {
		BG_TempFree(MAX_FILE_LENGTH);
		return NULL;
	}
	if (len == 0) {
		BG_TempFree(MAX_FILE_LENGTH);
		return NULL;
	}
	if (len >= MAX_FILE_LENGTH) {
		G_Printf("File too large: %s is %i, max allowed is %i", filename, len, MAX_FILE_LENGTH);
		BG_TempFree(MAX_FILE_LENGTH);
		trap_FS_FCloseFile(f);
		return NULL;
	}
	
	trap_FS_Read(buf, len, f);
	rlen = len;

	// Kill all characters after the file length, since sometimes FS_Read doesn't do that entirely (or so it seems)
	while (len < MAX_FILE_LENGTH) { 
		buf[len] = '\0';
		len++;
	}

	len = rlen;
	buf[len] = 0;
	BG_TempFree(MAX_FILE_LENGTH);
	trap_FS_FCloseFile(f);
	return buf;
}

/*
=====================================================================
Comparator function used to sort an array of strings
=====================================================================
*/
int JKMod_CompcStr(const void * a, const void * b) {
	const char * aa = * (const char * *) a;
	const char * bb = * (const char * *) b;
	return strcmp(aa, bb);
}

/*
=====================================================================
Rand function (Linux rand() behaves different than on Windows or qvm)
=====================================================================
*/
static int JKModRandSeed = 0;

void JKMod_RandSeed(unsigned seed) 
{
	JKModRandSeed = seed;
}

int	JKMod_Rand(void) 
{
	JKModRandSeed = (69069 * JKModRandSeed + 1);
	return JKModRandSeed & 0x7fff;
}

float JKMod_RandFloat(float min, float max) {
	return ((JKMod_Rand() * (max - min)) / 32768.0F) + min;
}
