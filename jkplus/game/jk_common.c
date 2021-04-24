/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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
	if (trap_Cvar_VariableIntegerValue("developer"))
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
Compare the given command with another
=====================================================================
*/
int JKMod_compareCmd(char *cmd, char *required)
{
	if (Q_stricmp(cmd, required) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
=====================================================================
Cleans the given string from newlines and such
=====================================================================
*/
void JKMod_stringEscape(char *in, char *out, int outSize)
{
	char	ch, ch1;
	int len = 0;
	outSize--;

	while (1)
	{
		ch = *in++;
		ch1 = *in;
		if (ch == '\\' && ch1 == 'n')
		{
			in++;
			*out++ = '\n';
		}
		else *out++ = ch;
		if (len > outSize - 1) {
			break;
		}
		len++;
	}
	return;
}

/*
=====================================================================
Removes color codes and converts everything to lower case
=====================================================================
*/
void JKMod_cleanString(char *in, char *out)
{
	int	i, count = 0;
	int	strLen = strlen(in);

	for (i = 0; i < strLen; i++)
	{
		if ((strLen > i + 1) && in[i] == '^' && in[i + 1] >= '0' && in[i + 1] <= '9')
		{
			i++;
			continue;
		}

		if ((int)in < 0)
		{
			continue;
		}

		out[count] = tolower(in[i]);
		count++;
	}
}

/*
=====================================================================
Sanitize strings with color codes
=====================================================================
*/
char *JKMod_sanitizeString(char *dest, char *source, int destSize)
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
		if (string[i] != '^')
		{
			clean[n] = tolower(string[i]);
			n++;
		}
		else if (string[i] == '^')
		{
			i++;
		}
	}

	Q_strncpyz(dest, clean, destSize);
	return dest;
}

/*
=====================================================================
Sanitize strings with color codes Rev2
=====================================================================
*/
void JKMod_sanitizeStringRev2(char *in, char *out)
{
	int i = 0;
	int r = 0;

	while (in[i])
	{
		if (i >= MAX_NAME_LENGTH - 1)
		{
			break;
		}
		if (in[i] == '^')
		{
			if (in[i + 1] >= 48 && //'0'
				in[i + 1] <= 57) //'9'
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
int JKMod_duplicatedNameCheck(gentity_t *ent, char *clientName)
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
			JKMod_sanitizeString(cleanEnt, newName, sizeof(cleanEnt));

			if (other && other->client && other->inuse && other->client->pers.connected == CON_CONNECTED)
			{
				JKMod_sanitizeString(cleanOther, other->client->pers.netname, sizeof(cleanOther));

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
void JKMod_dropPlayer(gentity_t *ent, char *msg)
{
	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		ent->client->sess.spectatorState = SPECTATOR_FREE;
		ent->client->sess.spectatorClient = ent - g_entities;
	}

	trap_DropClient(ent - g_entities, msg);
}

/*
=====================================================================
Sends a message/command to the client target
=====================================================================
*/
void JKMod_sendCommand(int target, char *cmd, char *string)
{
	trap_SendServerCommand(target, va("%s \"%s\"", cmd, string));
}

/*
=====================================================================
Convert milliseconds to string
=====================================================================
*/
const char *JKMod_msToString(const int ms, qboolean abbr) 
{
	int	   		fsecs = ms / 1000;
	int			wholemins = fsecs / 60;
	float		fremainsecs;
	char		*strseconds = abbr ? "sec" : "seconds";
	char		*strminutes = abbr ? "min" : "minutes";
	char		*strhours = abbr ? "hrs" : "minutes";

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

	return va("%d %s %d %s", wholemins, strminutes, (int)fremainsecs, strseconds);
}

/*
=====================================================================
Get client number from stripped substring
=====================================================================
*/
int JKMod_ClientNumberFromStrippedSubstring(const char* name)
{
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];
	int			i, match = -1;
	gclient_t	*cl;

	JKMod_sanitizeStringRev2((char*)name, s2);

	for (i = 0; i < level.numConnectedClients; i++)
	{
		cl = &level.clients[level.sortedClients[i]];
		JKMod_sanitizeStringRev2(cl->pers.netname, n2);
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
int JKMod_ClientNumberFromArg(char* name)
{
	int client_id = 0;
	char *cp;

	cp = name;
	while (*cp)
	{
		if (*cp >= '0' && *cp <= '9') cp++;
		else
		{
			client_id = -1; 
			break;
		}
	}
	if (client_id == 0)
	{ 
		client_id = atoi(name);
	}
	else
	{ 
		if (client_id == -1)
		{
			client_id = JKMod_ClientNumberFromStrippedSubstring(name);
		}
	}
	return client_id;
}

/*
=====================================================================
Get client number from stripped name
=====================================================================
*/
int JKMod_ClientNumberFromStrippedName(const char* name)
{
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];
	int			i;
	gclient_t*	cl;

	// check for a name match
	JKMod_sanitizeStringRev2((char*)name, s2);

	for (i = 0, cl = level.clients; i < level.numConnectedClients; i++, cl++)
	{
		JKMod_sanitizeStringRev2(cl->pers.netname, n2);
		if (!strcmp(n2, s2))
		{
			return i;
		}
	}
	return -1;
}

/*
=====================================================================
Concatenate arguments
=====================================================================
*/
char *JKMod_ConcatArgs(int start) 
{
	int			i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int			len;
	char		arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();
	for (i = start; i < c; i++) {
		trap_Argv(i, arg, sizeof(arg));
		tlen = strlen(arg);
		// Added this line below for extra anti msgboom protection
		if (len + tlen >= 850) {
			break;
		}
		memcpy(line + len, arg, tlen);
		len += tlen;
		if (i != c - 1) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
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
		return 0;
	}
	if (len == 0) {
		BG_TempFree(MAX_FILE_LENGTH);
		return 0;
	}
	if (len >= MAX_FILE_LENGTH) {
		G_Printf("File too large: %s is %i, max allowed is %i", filename, len, MAX_FILE_LENGTH);
		BG_TempFree(MAX_FILE_LENGTH);
		trap_FS_FCloseFile(f);
		return 0;
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
