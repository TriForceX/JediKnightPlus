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

void JKPlus_stringEscape(char *in, char *out)
{
	char	ch, ch1;
	int		len = 0;
	int		Size = 1023;

	while (1)
	{
		ch = *in++;
		ch1 = *in;

		if (ch == '\\' && ch1 == 'n')
		{
			in++;
			*out++ = '\n';
		}
		else
		{
			*out++ = ch;
		}

		if (len > Size - 1)
		{
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

const char *JKPlus_msToString(const int ms) 
{
	int	   			fsecs = ms / 1000;
	int				wholemins = fsecs / 60;
	float			fremainsecs;

	if (wholemins < 1)
	{
		return va("%d seconds", fsecs);
	}
	else if (wholemins >= 60) 
	{
		const int hrs = wholemins / 60;

		wholemins -= hrs * 60;

		if (wholemins == 0)
		{
			return va("%d hours", hrs);
		}

		return va("%d hours %d minutes", hrs, wholemins);
	}

	fremainsecs = (ms - wholemins * 60000) * 0.001f;

	return va("%d minutes %d seconds", wholemins, (int)fremainsecs);
}
