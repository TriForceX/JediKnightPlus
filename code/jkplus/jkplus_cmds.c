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
	if (Q_stricmp(cmd, "testcmd") == 0)
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
