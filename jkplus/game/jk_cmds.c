/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main commands functions
=====================================================================
*/

#include "../../code/game/g_local.h" // Original header

/*
=====================================================================
Drop flag function
=====================================================================
*/

static void JKPlus_dropFlag(gentity_t *ent, int clientNum)
{
	gitem_t		*item = ent->client->sess.sessionTeam == TEAM_RED ? BG_FindItem("team_CTF_blueflag") : BG_FindItem("team_CTF_redflag");
	vec3_t		angles, velocity, org, offset, mins, maxs;
	trace_t		tr;

	if (jkcvar_dropFlag.integer != 1)
	{
		trap_SendServerCommand(ent - g_entities, "print \"Drop flag is disabled by the server\n\"");
		return;
	}

	if (!(g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You can't drop a flag in this game type\n\"");
		return;
	}

	if ((ent->client->sess.sessionTeam == TEAM_RED && ent->client->ps.powerups[PW_BLUEFLAG] == 0) || 
		(ent->client->sess.sessionTeam == TEAM_BLUE && ent->client->ps.powerups[PW_REDFLAG] == 0))
	{
		trap_SendServerCommand(ent - g_entities, "cp \"You doesn't have any flag to drop\n\"");
		return;
	}

	if (ent->client->JKPlusDropFlagTime > 0)
	{
		trap_SendServerCommand(ent - g_entities, va("print \"You have to wait %d seconds to take the flag again\n\"", ent->client->JKPlusDropFlagTime));
		return;
	}
	
	ent->client->ps.powerups[PW_REDFLAG] = 0;
	ent->client->ps.powerups[PW_BLUEFLAG] = 0;
	ent->client->ps.persistant[PERS_SCORE] -= CTF_FLAG_BONUS;

	VectorCopy(ent->client->ps.viewangles, angles);

	angles[PITCH] = 0;

	AngleVectors(angles, velocity, NULL, NULL);
	VectorScale(velocity, 64, offset);
	offset[2] += ent->client->ps.viewheight / 2.f;
	VectorScale(velocity, 75, velocity);
	velocity[2] += 50 + random() * 35;

	VectorAdd(ent->client->ps.origin, offset, org);

	VectorSet(mins, -ITEM_RADIUS, -ITEM_RADIUS, 0);
	VectorSet(maxs, ITEM_RADIUS, ITEM_RADIUS, 2 * ITEM_RADIUS);

	trap_Trace(&tr, ent->client->ps.origin, mins, maxs, org, ent->s.number, MASK_SOLID);
	VectorCopy(tr.endpos, org);

	ent->client->JKPlusDropFlagTime = jkcvar_dropFlagTime.integer;
	LaunchItem(item, org, velocity);
}

/*
=====================================================================
Client command function
=====================================================================
*/

void JKPlus_ClientCommand(int clientNum)
{
	gentity_t *ent = &g_entities[clientNum];

	char cmd[MAX_TOKEN_CHARS];

	if (!ent->inuse || !ent->client)
	{
		return;
	}

	trap_Argv(0, cmd, sizeof(cmd));

	// Start commands
	if (JKPlus_emoteCheck(cmd, ent))
	{
		return;
	}
	else if (Q_stricmp(cmd, "help") == 0)
	{
		char    arg1[MAX_TOKEN_CHARS];

		trap_Argv(1, arg1, sizeof(arg1));

		if(trap_Argc() < 2)
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Help ^5]^7\n"
				"^7This server is running ^5%s %s.%s.%s\n"
				"^7You can read the desired help topic using following command: ^2/help <topic>\n"
				"^5----------\n"
				"^7Topic list:\n"
				"^3Admin          Accounts       Ranking\n"
				"^3Emotes         Commands       Build\n"
				"^3GamePlay       GameModes      CallVote\n"
				"^3About          Credits        \n"
				"^7\"", JKPLUS_LONGNAME, JKPLUS_MAJOR, JKPLUS_MINOR, JKPLUS_PATCH));
				return;
		}
		if(!Q_stricmp(arg1, "emotes"))
		{
			trap_SendServerCommand(ent - g_entities, va("print \""
				"^5[^7 Emotes ^5]^7\n"
				"^7Emotes are visual animations that allows you to sit down, greet someone, etc...\n"
				"^7There are over 50 emotes to choose from. (Some might be disabled by the server).\n"
				"^7You can assign an emote to a key using the following command: ^2/bind <key> <emote>\n"
				"^5----------\n"
				"^7Emote list:\n"
				"^3Bar            Beg            Buried        Cocky       ComeOn      ComTalk\n"
				"^3CrossArms      DontKillMe     DontKnow      DontKnow2   Explain     Explain2\n"
				"^3FakeDead       Flip           HandHips      Hi          Hug         Kiss\n"
				"^3Kneel          Laugh          Look          Look2       Nod         Point\n"
				"^3Point2         Punch          Sit           Sit2        Sit3        Shake\n"
				"^3Super          Super2         Surrender     Spin        Spin2       Spin3\n"
				"^3Spin4          Taunt2         Taunt3        Think       Threaten    ThumbsUp\n"
				"^3ThumbsDown     TossBack       TossOver      TossUp      Type        Type2\n"
				"^3Victory        Victory2       Waiting       WatchOut    Writing     Writing2\n"
				"^7\""));
				return;
		}
		else
		{
			trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is disabled at the moment\n\"", arg1));
			return;
		}
	}
	else if (Q_stricmp(cmd, "dropflag") == 0)
	{
		JKPlus_dropFlag(ent, clientNum);
	}
	else if (Q_stricmp(cmd, "testcmd") == 0)
	{
		char *message = jkcvar_test1.integer == 1 ? "Test 1 is enabled" : "Test 1 is disabled";
		
		trap_SendServerCommand(ent - g_entities, va("print \"%s and the value is %i. Test 2 is %i\n\"", message, jkcvar_test1.integer, jkcvar_test2.integer));
	}

	// Launch original client command function
	else
	{
		BaseJK2_ClientCommand(clientNum, cmd);
	}
}
