/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2019
=====================================================================
[Description]: Main client module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/cgame/cg_local.h"	// Original header

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
} cvarTable_t;

vmCvar_t	jkplus_cg_test1;
vmCvar_t	jkplus_cg_test2;

static cvarTable_t	JKPlusCGCvarTable[] = {

	{ &jkplus_cg_test1, "jk_cg_test1", "0", CVAR_ARCHIVE },
	{ &jkplus_cg_test2, "jk_cg_test2", "0", CVAR_ARCHIVE },

};

static int JKPlusCGCvarTableSize = sizeof(JKPlusCGCvarTable) / sizeof(JKPlusCGCvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

void JKPlus_CG_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for (i = 0, cv = JKPlusCGCvarTable; i < JKPlusCGCvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags);
	}

	// Launch original register cvars function
	BaseJK2_CG_RegisterCvars();
}

void JKPlus_CG_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKPlusCGCvarTable; i < JKPlusCGCvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Launch original update cvars function
	BaseJK2_CG_UpdateCvars();
}
