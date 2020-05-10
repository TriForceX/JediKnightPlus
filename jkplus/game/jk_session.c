/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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
	Q_strncpyz(client->sess.jkmodSess.ClientIP, "none", sizeof(client->sess.jkmodSess.ClientIP));
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
				case 11:
					client->sess.jkmodSess.MotdSeen = atoi(buf);
					break;
				case 12:
					client->sess.jkmodSess.IgnoredPlayer[0] = atoi(buf);
					break;
				case 13:
					client->sess.jkmodSess.IgnoredPlayer[1] = atoi(buf);
					break;
				case 14:
					client->sess.jkmodSess.IgnoredAll[0] = atoi(buf);
					break;
				case 15:
					client->sess.jkmodSess.IgnoredAll[1] = atoi(buf);
					break;
				case 16:
					Q_strncpyz(client->sess.jkmodSess.ClientIP, buf, sizeof(client->sess.jkmodSess.ClientIP));
				default:
					break;
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

	Q_strcat(original, sizeof(original), va(" %i %i %i %i %i %s ", // Fix me
		client->sess.jkmodSess.MotdSeen,
		client->sess.jkmodSess.IgnoredPlayer[0],
		client->sess.jkmodSess.IgnoredPlayer[1],
		client->sess.jkmodSess.IgnoredAll[0],
		client->sess.jkmodSess.IgnoredAll[1],
		client->sess.jkmodSess.ClientIP
	));

	trap_Cvar_Set(va("session%i", (int)(client - level.clients)), va("%s", original));
}
