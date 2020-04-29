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
Compare the given command with another
=====================================================================
*/
int JKPlus_compareCmd(char *cmd, char *required)
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
void JKPlus_stringEscape(char *in, char *out, int outSize)
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
void JKPlus_cleanString(char *in, char *out)
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
char *JKPlus_sanitizeString(char *dest, char *source, int destSize)
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
void JKPlus_sanitizeStringRev2(char *in, char *out)
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
int JKPlus_duplicatedNameCheck(gentity_t *ent, char *clientName)
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
			JKPlus_sanitizeString(cleanEnt, newName, sizeof(cleanEnt));

			if (other && other->client && other->inuse && other->client->pers.connected == CON_CONNECTED)
			{
				JKPlus_sanitizeString(cleanOther, other->client->pers.netname, sizeof(cleanOther));

				if (other - g_entities != ent - g_entities)
				{
					if (!Q_stricmp(cleanOther, cleanEnt))
					{
						num++;
						Q_strncpyz(newName, va("%s[%i]", clientName, num), sizeof(newName));
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
void JKPlus_dropPlayer(gentity_t *ent, char *msg)
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
void JKPlus_sendCommand(int target, char *cmd, char *string)
{
	trap_SendServerCommand(target, va("%s \"%s\"", cmd, string));
}

/*
=====================================================================
Convert milliseconds to string
=====================================================================
*/
const char *JKPlus_msToString(const int ms, qboolean abbr) 
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
int JKPlus_ClientNumberFromStrippedSubstring(const char* name)
{
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];
	int			i, match = -1;
	gclient_t	*cl;

	JKPlus_sanitizeStringRev2((char*)name, s2);

	for (i = 0; i < level.numConnectedClients; i++)
	{
		cl = &level.clients[level.sortedClients[i]];
		JKPlus_sanitizeStringRev2(cl->pers.netname, n2);
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
int JKPlus_ClientNumberFromArg(char* name)
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
			client_id = JKPlus_ClientNumberFromStrippedSubstring(name);
		}
	}
	return client_id;
}

/*
=====================================================================
Get client number from stripped name
=====================================================================
*/
int JKPlus_ClientNumberFromStrippedName(const char* name)
{
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];
	int			i;
	gclient_t*	cl;

	// check for a name match
	JKPlus_sanitizeStringRev2((char*)name, s2);

	for (i = 0, cl = level.clients; i < level.numConnectedClients; i++, cl++)
	{
		JKPlus_sanitizeStringRev2(cl->pers.netname, n2);
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
char *JKPlus_ConcatArgs(int start) 
{
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

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
Gameplay changer
=====================================================================
*/
void JKPlus_gamePlay(char *gameplay)
{
	if (!Q_stricmp(gameplay, "2") || !Q_stricmp(gameplay, "1.02"))
	{
		MV_SetGamePlay(VERSION_1_02);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.02\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.02\n\"");
	}
	else if (!Q_stricmp(gameplay, "3") || !Q_stricmp(gameplay, "1.03"))
	{
		MV_SetGamePlay(VERSION_1_03);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.03\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.03\n\"");
	}
	else if (!Q_stricmp(gameplay, "4") || !Q_stricmp(gameplay, "1.04"))
	{
		MV_SetGamePlay(VERSION_1_04);
		trap_SendServerCommand(-1, "print \"Server gameplay changed to 1.04\n\"");
		trap_SendServerCommand(-1, "cp \"Gameplay changed to 1.04\n\"");
	}
	else 
	{
		trap_Cvar_Set("jk_gamePlay", va("%i", jk2gameplay));
	}
}

/*
=====================================================================
Rand alternative (Linux rand() behaves different than on Winodws or qvm
=====================================================================
*/
static int JKPlusRandSeed = 0;

void JKPlus_sRand(unsigned seed) 
{
	JKPlusRandSeed = seed;
}

int	JKPlus_Rand(void) 
{
	JKPlusRandSeed = (69069 * JKPlusRandSeed + 1);
	return JKPlusRandSeed & 0x7fff;
}
