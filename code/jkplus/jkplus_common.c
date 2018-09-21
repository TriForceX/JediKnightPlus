/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Common utilities functions
=====================================================================
*/

#include "../game/g_local.h"	// Original header

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
Removes color codes and converts everything to lower case.
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
