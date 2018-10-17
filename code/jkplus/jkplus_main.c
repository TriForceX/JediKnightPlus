/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main game module, cvar definitions, etc...
=====================================================================
*/

#include "../game/g_local.h" // Original header

/*
=====================================================================
Cvar table list
=====================================================================
*/

typedef struct { // Cvar table struct

	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;
	qboolean	trackChange;

} cvarTable_t;

vmCvar_t	jkplus_test1;
vmCvar_t	jkplus_test2;

vmCvar_t	jkplus_emotesBreak;
vmCvar_t	jkplus_emotesFreeze;
vmCvar_t	jkplus_emotesEnabled;
vmCvar_t	jkplus_emotesPunchDMG;

static cvarTable_t	JKPlusCvarTable[] = {

	{ &jkplus_test1,				"jkplus_test1",					"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_test2,				"jkplus_test2",					"0",					CVAR_ARCHIVE,		0, qtrue },

	{ &jkplus_emotesBreak,			"jkplus_emotesBreak",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesFreeze,			"jkplus_emotesFreeze",			"1",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesEnabled,		"jkplus_emotesEnabled",			"0",					CVAR_ARCHIVE,		0, qtrue },
	{ &jkplus_emotesPunchDMG,		"jkplus_emotesPunchDMG",		"0",					CVAR_ARCHIVE,		0, qtrue },

};

static int JKPlusCvarTableSize = sizeof(JKPlusCvarTable) / sizeof(JKPlusCvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

void JKPlus_G_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for(i = 0, cv = JKPlusCvarTable; i < JKPlusCvarTableSize; i++, cv++)
	{
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);

		if(cv->vmCvar)
		{
			cv->modificationCount = cv->vmCvar->modificationCount;
		}
	}

	// Launch original register cvars function
	BaseJK2_G_RegisterCvars();
}

void JKPlus_G_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	for(i = 0, cv = JKPlusCvarTable; i < JKPlusCvarTableSize; i++, cv++)
	{
		if(cv->vmCvar)
		{
			trap_Cvar_Update(cv->vmCvar);

			if(cv->modificationCount != cv->vmCvar->modificationCount)
			{
				cv->modificationCount = cv->vmCvar->modificationCount;

				if(cv->trackChange)
				{
					trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string));
				}
			}
		}
	}

	// Launch original update cvars function
	BaseJK2_G_UpdateCvars();
}
