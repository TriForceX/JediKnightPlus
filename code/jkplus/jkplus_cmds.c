/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main commands functions
=====================================================================
*/

#include "../game/g_local.h" // Original header

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
				"^7This server is running ^5%s %s.%s %s.\n"
				"^7You can read the desired help topic using following command: ^2/help <topic>\n"
				"^5----------\n"
				"^7Topic list:\n"
				"^3Admin          Accounts       Ranking\n"
				"^3Emotes         Commands       MapEditing\n"
				"^3GamePlay       GameModes      CallVote\n"
				"^3About          Credits        Source\n"
				"^7\"", JKPLUS_LONGNAME, JKPLUS_RELEASE, JKPLUS_DEVELOP, JKPLUS_PHASE));
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
			trap_SendServerCommand(ent - g_entities, va("print \"The option ^3%s ^7is disabled at the moment.\n\"", arg1));
			return;
		}
	}
	else if (Q_stricmp(cmd, "testcmd") == 0)
	{
		char *message = jkplus_test1.integer == 1 ? "Test 1 is enabled" : "Test 1 is disabled";
		
		trap_SendServerCommand(ent - g_entities, va("print \"%s and the value is %i. Test 2 is %i\n\"", message, jkplus_test1.integer, jkplus_test2.integer));
	}

	// Launch original client command function
	else
	{
		BaseJK2_ClientCommand(clientNum, cmd);
	}
}
