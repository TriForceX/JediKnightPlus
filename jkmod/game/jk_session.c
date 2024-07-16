/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2024
=====================================================================
[Description]: Main session data
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Initialize session data
=====================================================================
*/
void JKMod_InitSessionData(gclient_t *client)
{
	if (level.newSession)
	{
		JKMod_ReadSessionData(client);
		return;
	}

	memset(&client->sess.jkmodSess, 0, sizeof(client->sess.jkmodSess));
	Q_strncpyz(client->sess.jkmodSess.clientIP, "none", sizeof(client->sess.jkmodSess.clientIP));
}

/*
=====================================================================
Read session data
=====================================================================
*/
void JKMod_ReadSessionData(gclient_t *client)
{
	char	s[MAX_STRING_CHARS] = "";	// String
	char	buf[MAX_STRING_CHARS] = "";	// String section
	int     sessionDataLen = 0;

	int	i = 0;		// Location in string
	int	j = 0;		// Location of last space
	int	num = 0;	// String section number

	trap_Cvar_VariableStringBuffer(va("session%i", (int)(client - level.clients)), s, sizeof(s));
	sessionDataLen = strlen(s);

	for (i = 0; i < sessionDataLen; i++)
	{
		if (s[i] == ' ')
		{
			num++;

			memset(buf, 0, sizeof(buf));
			memcpy(buf, &s[j + 1], i - j - 1);

			j = i;

			// Speed it up
			if (num <= 10) continue;

			switch (num)
			{
				case 11: client->sess.jkmodSess.motdSeen = atoi(buf); break;
				case 12: client->sess.jkmodSess.privateDuel = atoi(buf); break;
				case 13: client->sess.jkmodSess.autoDuel = atoi(buf); break;
				case 14: client->sess.jkmodSess.playerStatus = atoi(buf); break;
				case 15: client->sess.jkmodSess.ignoredPlayer[0] = atoi(buf); break;
				case 16: client->sess.jkmodSess.ignoredPlayer[1] = atoi(buf); break;
				case 17: client->sess.jkmodSess.ignoredPlayer[2] = atoi(buf); break;
				case 18: client->sess.jkmodSess.ignoredAll[0] = atoi(buf); break;
				case 19: client->sess.jkmodSess.ignoredAll[1] = atoi(buf); break;
				case 20: client->sess.jkmodSess.ignoredAll[2] = atoi(buf); break;
				case 21: client->sess.jkmodSess.chatColor = atoi(buf); break;
				case 22: Q_strncpyz(client->sess.jkmodSess.clientIP, buf, sizeof(client->sess.jkmodSess.clientIP)); break;
				default: break;
			}
		}
	}
}

/*
=====================================================================
Write session data
=====================================================================
*/
void JKMod_WriteSessionData(gclient_t *client)
{
	char	original[MAX_STRING_CHARS] = "";

	trap_Cvar_VariableStringBuffer(va("session%i", (int)(client - level.clients)), original, sizeof(original));

	Q_strcat(original, sizeof(original), va(" %i %i %i %i %i %i %i %i %i %i %i %s ", // Fix me
		client->sess.jkmodSess.motdSeen,
		client->sess.jkmodSess.privateDuel,
		client->sess.jkmodSess.autoDuel,
		client->sess.jkmodSess.playerStatus,
		client->sess.jkmodSess.ignoredPlayer[0],
		client->sess.jkmodSess.ignoredPlayer[1],
		client->sess.jkmodSess.ignoredPlayer[2],
		client->sess.jkmodSess.ignoredAll[0],
		client->sess.jkmodSess.ignoredAll[1],
		client->sess.jkmodSess.ignoredAll[2],
		client->sess.jkmodSess.chatColor,
		client->sess.jkmodSess.clientIP
	));

	trap_Cvar_Set(va("session%i", (int)(client - level.clients)), va("%s", original));
}
