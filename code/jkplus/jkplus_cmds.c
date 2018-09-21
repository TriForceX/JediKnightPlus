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
				"^3Bar            Beg            Cocky         ComeOn      ComTalk     CrossArms\n"
				"^3DontKillMe     DontKnow       DontKnow2     Explain     Explain2    FakeDead\n"
				"^3Flip           HandHips       Hug           Kiss        Kneel       Laugh\n"
				"^3LookAround     LookAround2    Nod           Point       Point2      Punch\n"
				"^3Punch2         Sit            Sit2          Sit3        Shake       Super\n"
				"^3Super2         Surrender      Spin          SpinR       Spin2       Spin2R\n"
				"^3Taunt2         Taunt3         Think         Threaten    ThumbsUp    ThumbsDown\n"
				"^3TossBack       TossOver       TossUp        Type        Type2       Victory\n"
				"^3Waiting        WatchOut       Wave          Wave2       Write       Write2\n"
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
