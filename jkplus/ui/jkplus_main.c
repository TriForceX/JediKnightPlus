/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main user interface module, cvar definitions, etc...
=====================================================================
*/

#include "../../code/ui/ui_local.h"	// Original header

/*
==================================================
Store the structs
==================================================
*/

JKPlusUiInfo_t JKPlusUiInfo;

emoteData_t emotesList[] =
{
	// cmd				title
	{ "bar",			"Bartender" },
	{ "beg",			"Beg" },
	{ "buried",			"Buried in Ground" },
	{ "cocky",			"Cocky" },
	{ "comeon",			"Come on!" },
	{ "comtalk",		"Communicator Talk" },
	{ "crossarms",		"Cross Arms" },
	{ "dontkillme",		"Don't Kill Me!" },
	{ "dontknow",		"Don't Know" },
	{ "dontknow2",		"Don't Know 2" },
	{ "explain",		"Explain" },
	{ "explain2",		"Explain 2" },
	{ "fakedead",		"Fake Dead" },
	{ "flip",			"Flip Saber" },
	{ "handhips",		"Hand Hips" },
	{ "hi",				"Say Hi!" },
	{ "hug",			"Hug a friend" },
	{ "kiss",			"Kiss Someone" },
	{ "kneel",			"Kneel" },
	{ "laugh",			"Laugh!" },
	{ "look",			"Look Arround" },
	{ "look2",			"Look Arround 2" },
	{ "nod",			"Nod" },
	{ "point",			"Point" },
	{ "point2",			"Point 2" },
	{ "punch",			"Punch Him!" },
	{ "sit",			"Sit Down" },
	{ "sit2",			"Sit Down 2" },
	{ "sit3",			"Sit Down 3" },
	{ "shake",			"Head Shake" },
	{ "super",			"I'm a Hero" },
	{ "super2",			"I'm a Hero 2" },
	{ "surrender",		"Surrender" },
	{ "spin",			"Spin" },
	{ "spin2",			"Spin 2" },
	{ "spin3",			"Spin 3" },
	{ "spin4",			"Spin 4" },
	{ "taunt2",			"Taunt 2" },
	{ "taunt3",			"Taunt 3" },
	{ "think",			"Thinking" },
	{ "threaten",		"Threaten" },
	{ "thumbsup",		"Thumbs Up" },
	{ "thumbsdown",		"Thumbs Down" },
	{ "tossback",		"Toss Back" },
	{ "tossover",		"Toss Over" },
	{ "tossup",			"Toss Up" },
	{ "type",			"Typing" },
	{ "type2",			"Typing 2" },
	{ "victory",		"Victory" },
	{ "victory2",		"Victory 2" },
	{ "waiting",		"Waiting" },
	{ "watchout",		"Watch Out!" },
	{ "writing",		"Writing" },
	{ "writing2",		"Writing 2" },
};

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

	{ &jkplus_ui_test1, "jk_ui_test1", "0", CVAR_ARCHIVE },
	{ &jkplus_ui_test2, "jk_ui_test2", "0", CVAR_ARCHIVE },

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

/*
=====================================================================
Emotes functions
=====================================================================
*/

static char *JKPlus_UI_EmotesList(int index, int *actual) 
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < JKPlusUiInfo.emotesCount; i++)
	{
		if (c == index)
		{
			*actual = i;
			return emotesList[i].emotesTitle;
		}
		else
		{
			c++;
		}
	}
	return "";
}

/*
=====================================================================
Feeder item text function
=====================================================================
*/
const char *JKPlus_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6)
{
	static char info[MAX_STRING_CHARS];
	static char hostname[1024];
	static char clientBuff[32];
	static char needPass[32];
	static int lastColumn = -1;
	static int lastTime = 0;
	*handle1 = *handle2 = *handle3 = *handle4 = -1;

	if(feederID == FEEDER_EMOTES)
	{
		int actual;
		return JKPlus_UI_EmotesList(index, &actual);
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederItemText(feederID, index, column, handle1, handle2, handle3, handle4, handle5, handle6);
}

/*
=====================================================================
Feeder count function
=====================================================================
*/

int JKPlus_UI_FeederCount(float feederID)
{
	switch ((int)feederID)
	{
		case FEEDER_EMOTES:
			JKPlusUiInfo.emotesCount = (sizeof(emotesList) / sizeof(emotesList[0]));
			return JKPlusUiInfo.emotesCount;
	}

	// Launch original feeder count function
	return BaseJK2_UI_FeederCount(feederID);
}

/*
=====================================================================
Feeder selection function
=====================================================================
*/

qboolean JKPlus_UI_FeederSelection(float feederID, int index)
{
	static char info[MAX_STRING_CHARS];

	if (feederID == FEEDER_EMOTES)
	{
		trap_Cmd_ExecuteText(EXEC_APPEND, va("%s\n", emotesList[index].emotesCmd));
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederSelection(feederID, index);
}
