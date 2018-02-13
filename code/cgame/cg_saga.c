#include "cg_local.h"
#include "../game/bg_saga.h"

#define DEFAULT_WIN_OBJECTIVE		"sound/chars/kyle/prototype/personal.mp3"
#define DEFAULT_LOSE_OBJECTIVE		"sound/chars/kyle/prototype/isntworking.mp3"

#define DEFAULT_WIN_ROUND			"sound/chars/kyle/prototype/personal.mp3"
#define DEFAULT_LOSE_ROUND			"sound/chars/kyle/prototype/isntworking.mp3"

char		cg_saga_info[MAX_SAGA_INFO_SIZE];
int			cg_saga_valid;

//The following two functions are very cheap parsing functions
int CGSagaGetValueGroup(char *buf, char *group, char *outbuf)
{
	char *place, *placesecond;
	int failure;
	int i;
	int startpoint, startletter;
	int subg = 0;

	i = 0;

	place = strstr(buf, group);

	if (!place)
	{
		return 0;
	}

	startpoint = place - buf + strlen(group) + 1;
	startletter = (place - buf) - 1;

	failure = 0;

	while (buf[startpoint+1] != '{' || buf[startletter] != '\n')
	{
		placesecond = strstr(place+1, group);

		if (placesecond)
		{
			startpoint += (placesecond - place);
			startletter += (placesecond - place);
			place = placesecond;
		}
		else
		{
			failure = 1;
			break;
		}
	}

	if (failure)
	{
		return 0;
	}

	//we have found the proper group name if we made it here, so find the opening brace and read into the outbuf
	//until hitting the end brace

	while (buf[startpoint] != '{')
	{
		startpoint++;
	}

	startpoint++;

	while (buf[startpoint] != '}' || subg)
	{
		if (buf[startpoint] == '{')
		{
			subg++;
		}
		else if (buf[startpoint] == '}')
		{
			subg--;
		}
		outbuf[i] = buf[startpoint];
		i++;
		startpoint++;
	}
	outbuf[i] = '\0';

	return 1;
}

int CGSagaGetPairedValue(char *buf, char *key, char *outbuf, int inquotes)
{
	char *place, *placesecond;
	int startpoint, startletter;
	int i, found;

	if (!buf || !key || !outbuf)
	{
		return 0;
	}

	i = 0;

	while (buf[i] && buf[i] != '\0')
	{
		if (buf[i] == '/')
		{
			if (buf[i+1] && buf[i+1] != '\0' && buf[i+1] == '/')
			{
				while (buf[i] != '\n')
				{
					buf[i] = '/';
					i++;
				}
			}
		}
		i++;
	}

	place = strstr(buf, key);

	if (!place)
	{
		return 0;
	}
	//tab == 9
	startpoint = place - buf + strlen(key);
	startletter = (place - buf) - 1;

	found = 0;

	while (!found)
	{
		if (startletter == 0 || !buf[startletter] || buf[startletter] == '\0' || buf[startletter] == 9 || buf[startletter] == ' ' || buf[startletter] == '\n')
		{
			if (buf[startpoint] == '\0' || buf[startpoint] == 9 || buf[startpoint] == ' ' || buf[startpoint] == '\n')
			{
				found = 1;
				break;
			}
		}

		placesecond = strstr(place+1, key);

		if (placesecond)
		{
			startpoint += placesecond - place;
			startletter += placesecond - place;
			place = placesecond;
		}
		else
		{
			place = NULL;
			break;
		}

	}

	if (!found || !place || !buf[startpoint] || buf[startpoint] == '\0')
	{
		return 0;
	}

	while (buf[startpoint] == ' ' || buf[startpoint] == 9 || buf[startpoint] == '\n')
	{
		startpoint++;
	}

	i = 0;

	if (inquotes)
	{
		while (buf[startpoint] && buf[startpoint] != '\"')
		{
			startpoint++;
		}

		startpoint++;
	}

	while (buf[startpoint] && buf[startpoint] != '\0' && buf[startpoint] != '\n')
	{
		outbuf[i] = buf[startpoint];
		i++;
		startpoint++;

		if (inquotes && buf[startpoint] == '\"')
		{
			break;
		}
	}

	outbuf[i] = '\0';

	return 1;
}

void CG_InitSagaMode(void)
{
	char			mapname[512];
	char			levelname[512];
	int				len = 0;
	int				i = 0;
	fileHandle_t	f;

	if (cgs.gametype != GT_SAGA)
	{
		goto failure;
	}

	Q_strncpyz(mapname, cgs.mapname, sizeof(mapname));

	i = strlen(mapname)-1;

	while (i > 0 && mapname[i] && mapname[i] != '.')
	{
		i--;
	}

	if (!i)
	{
		goto failure;
	}

	mapname[i] = '\0'; //kill the ".bsp"

	Com_sprintf(levelname, sizeof(levelname), "%s.saga", mapname);

	if (!levelname[0])
	{
		goto failure;
	}

	len = trap_FS_FOpenFile(levelname, &f, FS_READ);

	if (!f || len >= MAX_SAGA_INFO_SIZE)
	{
		goto failure;
	}

	trap_FS_Read(cg_saga_info, len, f);

	cg_saga_valid = 1;

	return;

failure:
	cg_saga_valid = 0;
}

void CG_SagaRoundOver(centity_t *ent, int won)
{
	int				myTeam;
	char			teamstr[64];
	char			objectives[MAX_SAGA_INFO_SIZE];
	char			appstring[1024];
	char			soundstr[1024];
	int				success = 0;

	if (!cg_saga_valid)
	{
		Com_Printf("ERROR: Saga data does not exist on client!\n");
		return;
	}

	if (!cg.snap)
	{
		return; //yikes
	}

	myTeam = cg.snap->ps.persistant[PERS_TEAM];

	if (myTeam == TEAM_SPECTATOR)
	{
		return;
	}

	if (myTeam == SAGATEAM_IMPERIAL)
	{
		Q_strncpyz(teamstr, "Imperial", sizeof(teamstr));
	}
	else
	{
		Q_strncpyz(teamstr, "Rebel", sizeof(teamstr));
	}

	if (CGSagaGetValueGroup(cg_saga_info, teamstr, objectives))
	{
		if (won == myTeam)
		{
			success = CGSagaGetPairedValue(objectives, "wonround", appstring, 1);
		}
		else
		{
			success = CGSagaGetPairedValue(objectives, "lostround", appstring, 1);
		}

		if (success)
		{
			Com_Printf("%s\n", appstring);
		}

		if (myTeam == won)
		{
			Q_strncpyz(teamstr, "roundover_sound_wewon", sizeof(teamstr));
		}
		else
		{
			Q_strncpyz(teamstr, "roundover_sound_welost", sizeof(teamstr));
		}

		if (CGSagaGetPairedValue(objectives, teamstr, appstring, 1))
		{
			Q_strncpyz(soundstr, appstring, sizeof(soundstr));
		}
		else
		{
			if (myTeam != won)
			{
				Q_strncpyz(soundstr, DEFAULT_LOSE_ROUND, sizeof(soundstr));
			}
			else
			{
				Q_strncpyz(soundstr, DEFAULT_WIN_ROUND, sizeof(soundstr));
			}
		}

		trap_S_StartLocalSound(trap_S_RegisterSound(soundstr), CHAN_ANNOUNCER);
		//Com_Printf("Play sound: %s\n", soundstr);
	}
}

void CG_SagaObjectiveCompleted(centity_t *ent, int won, int objectivenum)
{
	int				myTeam;
	char			teamstr[64];
	char			objstr[256];
	char			objectives[MAX_SAGA_INFO_SIZE];
	char			foundobjective[MAX_SAGA_INFO_SIZE];
	char			appstring[1024];
	char			soundstr[1024];
	int				success = 0;
	int				i = 0;
	int				i_r = 0;

	if (!cg_saga_valid)
	{
		Com_Printf("ERROR: Saga data does not exist on client!\n");
		return;
	}

	if (!cg.snap)
	{
		return; //yikes
	}

	myTeam = cg.snap->ps.persistant[PERS_TEAM];

	if (myTeam == TEAM_SPECTATOR)
	{
		return;
	}

	if (won == SAGATEAM_IMPERIAL)
	{
		Q_strncpyz(teamstr, "Imperial", sizeof(teamstr));
	}
	else
	{
		Q_strncpyz(teamstr, "Rebel", sizeof(teamstr));
	}

	if (CGSagaGetValueGroup(cg_saga_info, teamstr, objectives))
	{
		Com_sprintf(objstr, sizeof(objstr), "Objective%i", objectivenum);

		while (objectives[i])
		{
			if (objectives[i] != 9)
			{
				objectives[i_r] = objectives[i];
				i_r++;
			}

			i++;
		}
		objectives[i_r] = '\0';

		if (CGSagaGetValueGroup(objectives, objstr, foundobjective))
		{
			if (myTeam == SAGATEAM_IMPERIAL)
			{
				success = CGSagaGetPairedValue(foundobjective, "message_imperial", appstring, 1);
			}
			else
			{
				success = CGSagaGetPairedValue(foundobjective, "message_rebel", appstring, 1);
			}

			if (success)
			{
				Com_Printf("%s\n", appstring);
			}

			if (myTeam == SAGATEAM_IMPERIAL)
			{
				Q_strncpyz(teamstr, "sound_imperial", sizeof(teamstr));
			}
			else
			{
				Q_strncpyz(teamstr, "sound_rebel", sizeof(teamstr));
			}

			if (CGSagaGetPairedValue(foundobjective, teamstr, appstring, 1))
			{
				Q_strncpyz(soundstr, appstring, sizeof(soundstr));
			}
			else
			{
				if (myTeam != won)
				{
					Q_strncpyz(soundstr, DEFAULT_LOSE_OBJECTIVE, sizeof(soundstr));
				}
				else
				{
					Q_strncpyz(soundstr, DEFAULT_WIN_OBJECTIVE, sizeof(soundstr));
				}
			}

			trap_S_StartLocalSound(trap_S_RegisterSound(soundstr), CHAN_ANNOUNCER);
			//Com_Printf("Play sound: %s\n", soundstr);
		}
	}
}
