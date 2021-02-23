/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
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

vmCvar_t	jkcvar_ui_test1;
vmCvar_t	jkcvar_ui_test2;

static cvarTable_t	JKModUICvarTable[] = {

	{ &jkcvar_ui_test1, "jk_ui_test1", "0", CVAR_ARCHIVE },
	{ &jkcvar_ui_test2, "jk_ui_test2", "0", CVAR_ARCHIVE },

};

static int JKModUICvarTableSize = sizeof(JKModUICvarTable) / sizeof(JKModUICvarTable[0]);

/*
=====================================================================
Emotes table list
=====================================================================
*/
jkmod_emotes_table_t jkmodEmotesTable[] =
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

jkmod_emotes_list_t jkmodEmotes;

/*
=====================================================================
Dimensions table list
=====================================================================
*/
jkmod_dimensions_table_t jkmodDimensionsTable[] =
{
	// cmd				title
	{ "guns",			"Guns Arena" },
	{ "race",			"Race Challenge" },
	{ "saber",			"Saber Only" },
	{ "insta",			"Instant Kill" },
	{ "cheats",			"Cheats Mode" },
};

jkmod_dimensions_list_t jkmodDimensions;

/*
=====================================================================
Register / update cvars functions
=====================================================================
*/
void JKMod_UI_RegisterCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Register all the cvars
	for (i = 0, cv = JKModUICvarTable; i < JKModUICvarTableSize; i++, cv++) {
		trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags);
	}

	// Set default temp cvars
	trap_Cvar_Set("jk_ui_cmd_emoteToggle", va("%s", jkmodEmotesTable[0].cmd));
	trap_Cvar_Set("jk_ui_cmd_dimensionToggle", va("%s", jkmodDimensionsTable[0].cmd));

	// Launch original register cvars function
	BaseJK2_UI_RegisterCvars();
}

void JKMod_UI_UpdateCvars(void)
{
	int			i;
	cvarTable_t	*cv;

	// Update all the cvars
	for (i = 0, cv = JKModUICvarTable; i < JKModUICvarTableSize; i++, cv++) {
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
static char *JKMod_UI_EmotesList(int index, int *actual) 
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < jkmodEmotes.count; i++)
	{
		if (c == index)
		{
			*actual = i;
			return jkmodEmotesTable[i].title;
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
Dimensions functions
=====================================================================
*/
static char *JKMod_UI_DimensionsList(int index, int *actual)
{
	int i, c = 0;
	*actual = 0;

	for (i = 0; i < jkmodDimensions.count; i++)
	{
		if (c == index)
		{
			*actual = i;
			return jkmodDimensionsTable[i].title;
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
const char *JKMod_UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle1, qhandle_t *handle2, qhandle_t *handle3, qhandle_t *handle4, qhandle_t *handle5, qhandle_t *handle6)
{
	static char info[MAX_STRING_CHARS];
	static char hostname[1024];
	static char clientBuff[32];
	static char needPass[32];
	static int lastColumn = -1;
	static int lastTime = 0;
	*handle1 = *handle2 = *handle3 = *handle4 = -1;

	if(feederID == FEEDER_JK_EMOTES)
	{
		int actual;
		return JKMod_UI_EmotesList(index, &actual);
	}
	else if (feederID == FEEDER_JK_DIMENSIONS)
	{
		int actual;
		return JKMod_UI_DimensionsList(index, &actual);
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederItemText(feederID, index, column, handle1, handle2, handle3, handle4, handle5, handle6);
}

/*
=====================================================================
Feeder count function
=====================================================================
*/
int JKMod_UI_FeederCount(float feederID)
{
	switch ((int)feederID)
	{
		case FEEDER_JK_EMOTES:
			jkmodEmotes.count = (sizeof(jkmodEmotesTable) / sizeof(jkmodEmotesTable[0]));
			return jkmodEmotes.count;
		case FEEDER_JK_DIMENSIONS:
			jkmodDimensions.count = (sizeof(jkmodDimensionsTable) / sizeof(jkmodDimensionsTable[0]));
			return jkmodDimensions.count;
	}

	// Launch original feeder count function
	return BaseJK2_UI_FeederCount(feederID);
}

/*
=====================================================================
Feeder selection function
=====================================================================
*/
qboolean JKMod_UI_FeederSelection(float feederID, int index)
{
	static char info[MAX_STRING_CHARS];

	if (feederID == FEEDER_JK_EMOTES)
	{
		trap_Cvar_Set("jk_ui_cmd_emoteToggle", va("%s", jkmodEmotesTable[index].cmd));
	}
	else if (feederID == FEEDER_JK_DIMENSIONS)
	{
		trap_Cvar_Set("jk_ui_cmd_dimensionToggle", va("%s", jkmodDimensionsTable[index].cmd));
	}

	// Final return, probably NULL
	return BaseJK2_UI_FeederSelection(feederID, index);
}
