/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2018
=====================================================================
[Description]: Main user interface module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/ui/ui_local.h"	// Original header

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

vmCvar_t	jkplus_ui_test1;
vmCvar_t	jkplus_ui_test2;

static cvarTable_t	JKPlusUICvarTable[] = {

	{ &jkplus_ui_test1, "jkplus_ui_test1", "0", CVAR_ARCHIVE },
	{ &jkplus_ui_test2, "jkplus_ui_test2", "0", CVAR_ARCHIVE },

};

static int JKPlusUICvarTableSize = sizeof(JKPlusUICvarTable) / sizeof(JKPlusUICvarTable[0]);

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/

void JKPlus_UI_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for (i = 0, cv = JKPlusUICvarTable; i < JKPlusUICvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}

	// Launch original register cvars function
	BaseJK2_UI_RegisterCvars();
}

void JKPlus_UI_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKPlusUICvarTable; i < JKPlusUICvarTableSize; i++, cv++) {
		trap_Cvar_Update(cv->vmCvar);
	}

	// Launch original update cvars function
	BaseJK2_UI_UpdateCvars();
}
